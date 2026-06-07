/**
 ******************************************************************************
 * @file     main.c
 * @author   智能小夜灯项目
 * @version  V1.0
 * @date     2026-01-15
 * @brief    智能小夜灯 — 光敏自动调光 / 多模式 RGB 氛围灯
 * @license  Copyright (c) 2020-2032, 广州市星翼电子科技有限公司
 ******************************************************************************
 * @attention
 *
 * 实验平台: 正点原子 M48Z-M3最小系统板 STM32F103版
 * 硬件模块: ATK-MB001 光敏传感器 + ATK-MB011 RGB LED + ATK-MB022 OLED
 *
 * 引脚分配:
 *   PA0 — 光敏传感器 AO (ADC1_CH0)
 *   PA2 — OLED SCL (软件I2C)
 *   PA3 — OLED SDA (软件I2C)
 *   PA6 — RGB LED R (TIM3_CH1)
 *   PA7 — RGB LED G (TIM3_CH2)
 *   PB0 — RGB LED B (TIM3_CH3)
 *   KEY0 — 板载按键 (模式切换)
 *
 * 工作模式:
 *   0=自动模式: 根据环境光自动调节亮度和色温
 *   1=常亮模式: 固定暖光 50%
 *   2=呼吸模式: 独立呼吸灯效果
 ******************************************************************************
 */

#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/LED/led.h"
#include "./BSP/KEY/key.h"
#include "./BSP/ADC/adc.h"
#include "./BSP/ATK_LIGHT/atk_light.h"
#include "./BSP/ATK_OLED/atk_oled.h"
#include "./BSP/TIM/gtim.h"
#include <stdio.h>

/* ========================== 宏定义 ========================== */

/* 光敏阈值 */
#define LIGHT_DARK      25      /* 低于此值=很暗, 灯全亮 */
#define LIGHT_BRIGHT    80      /* 高于此值=够亮, 灯关闭 */
#define LIGHT_HYST      5       /* 迟滞窗口, 防抖 */

/* 色温预设 */
#define WARM_R          999     /* 暖光 R */
#define WARM_G          500     /* 暖光 G */
#define WARM_B          250     /* 暖光 B */
#define COOL_R          400     /* 冷光 R */
#define COOL_G          600     /* 冷光 G */
#define COOL_B          800     /* 冷光 B */

/* ========================== 全局变量 ========================== */

typedef enum {
    MODE_AUTO = 0,      /* 自动调光模式 */
    MODE_CONSTANT,      /* 常亮模式 */
    MODE_BREATH,        /* 呼吸灯模式 */
    MODE_MAX
} night_light_mode_t;

static night_light_mode_t g_mode = MODE_AUTO;
static uint16_t g_r = 0, g_g = 0, g_b = 0;     /* 当前RGB值 */
static uint8_t  g_auto_brightness = 0;          /* 自动模式当前亮度(0-100) */

/* ========================== 工具函数 ========================== */

/**
 * @brief       将 0-100 亮度值映射为暖色调 RGB PWM 值
 * @param       brightness: 亮度百分比 0-100
 * @param       r/g/b: 输出PWM值指针
 * @retval      无
 */
static void brightness_to_warm(uint8_t brightness, uint16_t *r, uint16_t *g, uint16_t *b)
{
    uint16_t scale;

    if (brightness == 0)
    {
        *r = 0; *g = 0; *b = 0;
        return;
    }

    /* 1% → PWM 10, 100% → PWM 999 */
    scale = (uint16_t)brightness * 999 / 100;

    *r = scale;
    *g = scale * WARM_G / WARM_R;   /* 按暖光比例缩放 */
    *b = scale * WARM_B / WARM_R;
}

/**
 * @brief       平滑亮度过渡 (渐亮渐暗, 消除突兀跳动)
 * @param       target: 目标亮度 0-100
 * @param       current: 当前亮度指针 (原地更新)
 * @retval      无
 */
static void smooth_to(uint8_t target, uint8_t *current)
{
    if (target > *current)
    {
        (*current)++;           /* 渐亮: 每帧+1 */
    }
    else if (target < *current)
    {
        (*current)--;           /* 渐暗: 每帧-1 */
    }
}

/**
 * @brief       根据光照值计算目标亮度 (带死区和迟滞)
 * @param       light_val: atk_light_get_val() 返回值 (0最亮→100最暗)
 * @retval      目标亮度 0-100
 */
static uint8_t calc_target_brightness(uint8_t light_val)
{
    static uint8_t last_target = 0;

    if (light_val >= LIGHT_BRIGHT)
    {
        /* 环境够亮 → 关灯 */
        last_target = 0;
        return 0;
    }
    else if (light_val <= LIGHT_DARK)
    {
        /* 环境很暗 → 全亮 */
        last_target = 100;
        return 100;
    }
    else
    {
        /* 中间区域: 线性映射 + 迟滞 */
        uint8_t raw;
        raw = (uint8_t)((uint32_t)(light_val - LIGHT_DARK) * 100 / (LIGHT_BRIGHT - LIGHT_DARK));

        /* 迟滞: 只有超过 LIGHT_HYST 才更新目标 */
        if (raw > last_target + LIGHT_HYST || raw < last_target - LIGHT_HYST)
        {
            last_target = raw;
        }
        return last_target;
    }
}

/* ========================== 模式处理 ========================== */

/**
 * @brief       模式0: 自动调光
 * @param       无
 * @retval      无
 */
static void mode_auto(void)
{
    uint8_t light_val;
    uint8_t target;

    light_val = atk_light_get_val();            /* 0=亮环境, 100=暗环境 */
    target = calc_target_brightness(light_val); /* 计算目标亮度 */
    smooth_to(target, &g_auto_brightness);      /* 平滑过渡 */

    brightness_to_warm(g_auto_brightness, &g_r, &g_g, &g_b);
}

/**
 * @brief       模式1: 常亮暖光
 * @param       无
 * @retval      无
 */
static void mode_constant(void)
{
    g_r = WARM_R / 2;   /* 50% 暖光 */
    g_g = WARM_G / 2;
    g_b = WARM_B / 2;
}

/**
 * @brief       模式2: 呼吸灯
 * @param       无
 * @retval      无
 */
static void mode_breath(void)
{
    static uint16_t val = 0;
    static int16_t dir = 5;
    uint16_t r, g, b;

    val += dir;

    /* 到达端点时反转方向 */
    if (val >= 999)
    {
        val = 999;
        dir = -5;
    }
    else if (val == 0)
    {
        dir = 5;
    }

    /* 呼吸时保持暖色调 */
    r = val;
    g = val * WARM_G / WARM_R;
    b = val * WARM_B / WARM_R;

    g_r = r; g_g = g; g_b = b;
}

/* ========================== OLED 显示 ========================== */

/**
 * @brief       更新OLED显示
 * @param       无
 * @retval      无
 */
static void oled_update(void)
{
    char buf[20];
    const char *mode_names[] = {"Auto", "Const", "Breath"};

    atk_oled_clear();

    /* 第1行: 模式 */
    atk_oled_show_string(0, 0, "Mode:", 16);
    atk_oled_show_string(50, 0, mode_names[g_mode], 16);

    /* 第2行: 光照值 */
    sprintf(buf, "Light:%3d", atk_light_get_val());
    atk_oled_show_string(0, 16, buf, 16);

    /* 第3行: RGB亮度 */
    sprintf(buf, "R:%-4d", g_r);
    atk_oled_show_string(0, 32, buf, 12);

    sprintf(buf, "G:%-4d", g_g);
    atk_oled_show_string(42, 32, buf, 12);

    sprintf(buf, "B:%-4d", g_b);
    atk_oled_show_string(84, 32, buf, 12);

    /* 第4行: 亮度百分比 */
    {
        uint8_t percent;
        if (g_mode == MODE_AUTO)
            percent = g_auto_brightness;
        else
            percent = (uint8_t)(g_r * 100 / 999);

        sprintf(buf, "Bright: %3d%%", percent);
        atk_oled_show_string(0, 48, buf, 16);
    }

    atk_oled_refresh_gram();
}

/* ========================== 主函数 ========================== */

int main(void)
{
    uint8_t key;
    uint32_t frame_count = 0;

    HAL_Init();                                     /* 初始化HAL库 */
    sys_stm32_clock_init(RCC_PLL_MUL9);             /* 设置时钟 72MHz */
    delay_init(72);                                 /* 延时初始化 */
    usart_init(115200);                             /* 串口初始化 */
    led_init();                                     /* 板载LED初始化 */
    key_init();                                     /* 按键初始化 */

    atk_light_init();                               /* 光敏传感器初始化 (含ADC) */
    gtim_rgb_init(999, 0);                          /* TIM3 PWM初始化, 72kHz */
    atk_oled_init();                                /* OLED初始化 */

    /* 启动信息 */
    printf("\r\n");
    printf("========================================\r\n");
    printf("   Smart Night Light  V1.0\r\n");
    printf("   STM32F103C8T6 + BEE BLOCK\r\n");
    printf("========================================\r\n");
    printf("\r\n");
    printf("  KEY0: 切换模式 (自动/常亮/呼吸)\r\n");
    printf("  PA0:  光敏传感器\r\n");
    printf("  PA6/PA7/PB0: RGB LED (R/G/B)\r\n");
    printf("\r\n");

    /* OLED 欢迎画面 */
    atk_oled_clear();
    atk_oled_show_string(0, 0, "Smart NightLight", 16);
    atk_oled_show_string(0, 24, "STM32F103C8T6", 16);
    atk_oled_show_string(0, 48, "BEE BLOCK V1.0", 16);
    atk_oled_refresh_gram();
    delay_ms(1500);

    /* ===================== 主循环 ===================== */
    while (1)
    {
        /* --- 按键处理 --- */
        key = key_scan(0);                          /* 非阻塞扫描 */
        if (key == KEY0_PRES)
        {
            g_mode = (night_light_mode_t)((g_mode + 1) % MODE_MAX);
            printf("[KEY] 模式切换: %d\r\n", g_mode);

            /* 切换模式时重置状态 */
            if (g_mode == MODE_BREATH)
            {
                g_r = g_g = g_b = 0;               /* 呼吸模式从头开始 */
            }
        }

        /* --- 模式运行 --- */
        switch (g_mode)
        {
            case MODE_AUTO:
                mode_auto();
                break;

            case MODE_CONSTANT:
                mode_constant();
                break;

            case MODE_BREATH:
                mode_breath();
                break;

            default:
                break;
        }

        /* --- 输出到RGB LED --- */
        gtim_rgb_set_all(g_r, g_g, g_b);

        /* --- OLED刷新 (每 200ms) --- */
        if (frame_count % 8 == 0)
        {
            oled_update();
        }

        /* --- 串口调试输出 (每 500ms) --- */
        if (frame_count % 20 == 0)
        {
            printf("[%d] Mode:%d Light:%3d R:%03d G:%03d B:%03d\r\n",
                   (int)frame_count / 40,
                   g_mode,
                   atk_light_get_val(),
                   g_r, g_g, g_b);
        }

        frame_count++;
        delay_ms(25);                               /* ~40fps 主循环 */
    }
}
