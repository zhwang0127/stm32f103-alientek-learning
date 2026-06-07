智能小夜灯 — Smart Night Light
================================

基于 STM32F103C8T6 (M48Z-M3 最小系统板) + BEE BLOCK 积木模块

硬件模块:
  ATK-MB001 光敏传感器    — 检测环境光照
  ATK-MB011 RGB LED       — 全彩氛围灯
  ATK-MB022 OLED 显示屏   — 状态显示

引脚分配:
  PA0  — 光敏传感器 AO (ADC1_CH0)
  PA2  — OLED SCL (软件I2C)
  PA3  — OLED SDA (软件I2C)
  PA6  — RGB LED R (TIM3_CH1)
  PA7  — RGB LED G (TIM3_CH2)
  PB0  — RGB LED B (TIM3_CH3)
  PC13 — KEY0 按键 (模式切换)
  PA9  — USART1 TX (串口调试)
  PA10 — USART1 RX (串口调试)

工作模式:
  MODE 0: 自动模式 — 根据环境光自动调节亮度和色温
  MODE 1: 常亮模式 — 固定暖光 50%
  MODE 2: 呼吸模式 — 独立呼吸灯效果

开发环境:
  Keil MDK-ARM V5
  STM32F1xx HAL 库
  编译优化: -O0 (调试) / -Os (发布)

如需创建Keil工程:
  1. 新建工程 → 选择STM32F103C8
  2. 添加源文件组:
     - Users/main.c, stm32f1xx_it.c
     - Drivers/BSP/ADC/adc.c
     - Drivers/BSP/ATK_LIGHT/atk_light.c
     - Drivers/BSP/ATK_OLED/atk_oled.c
     - Drivers/BSP/IIC/myiic.c
     - Drivers/BSP/KEY/key.c
     - Drivers/BSP/LED/led.c
     - Drivers/BSP/TIM/gtim.c
     - Drivers/SYSTEM/sys/sys.c, delay/delay.c, usart/usart.c
     - Drivers/STM32F1xx_HAL_Driver/Src/*.c (按需)
     - Drivers/CMSIS/Device/.../startup_stm32f103xb.s
  3. Include路径: Drivers/, Drivers/BSP/, Drivers/SYSTEM/, Drivers/CMSIS/Include/
  4. 定义宏: STM32F103xB, USE_HAL_DRIVER
