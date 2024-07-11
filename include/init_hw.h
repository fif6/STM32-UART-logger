#ifndef HW_INIT_H_
#define HW_INIT_H_

#include "stm32f0xx_hal.h"

// ------------- SPI Gyroscope on board ------------------
// Interrupt 1 from gyroscope
#define MEMS_Int1_Pin GPIO_PIN_1
#define MEMS_Int1_GPIO_Port GPIOC
// Interrupt 2 from gyroscope
#define MEMS_Int2_Pin GPIO_PIN_2
#define MEMS_Int2_GPIO_Port GPIOC

// ------------- LEDs on board ---------------------------
#define LED_Red_Pin GPIO_PIN_6
#define LED_Red_GPIO_Port GPIOC

#define LED_Blue_Pin GPIO_PIN_7
#define LED_Blue_GPIO_Port GPIOC

#define LED_Orange_Pin GPIO_PIN_8
#define LED_Orange_GPIO_Port GPIOC

#define LED_Green_Pin GPIO_PIN_9
#define LED_Green_GPIO_Port GPIOC

// ------------- UART3 pins ------------------------------
#define UART3_TX_Pin GPIO_PIN_4 // TX (out)
#define UART3_TX_GPIO_Port GPIOC

#define UART3_RX_Pin GPIO_PIN_5 // RX (in)
#define UART3_RX_GPIO_Port GPIOC

//---------------------------------------------------------

void HAL_MspInit(void);

void SystemClock_Config(void);

void GPIO_Init(void);

void I2C1_Init(void);

void UART3_Init(void);

void DMA_Init(void);

void DMA_UART3_Init(void);

void RTC_Init(void);

#endif // HW_INIT_H_
