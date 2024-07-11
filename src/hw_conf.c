#include "stm32f0xx_hal.h"
#include "hw_conf.h"


extern UART_HandleTypeDef huart3;
extern DMA_HandleTypeDef hdma_usart3_tx;

void GPIO_Init(void) {
    //--- GPIO Ports Clock Enable -------------------------
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct = {0};

    //--- Configure GPIO pins: MEMS_Int1_Pin MEMS_Int2_Pin ---
    GPIO_InitStruct.Pin = MEMS_Int1_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(MEMS_Int1_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = MEMS_Int2_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(MEMS_Int2_GPIO_Port, &GPIO_InitStruct);

    //--- Configure onboard LEDs PINs ------------------------
    GPIO_InitStruct.Pin = LED_Orange_Pin; // Orange LED
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(LED_Orange_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = LED_Green_Pin; // Green LED
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(LED_Green_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = LED_Blue_Pin; // Blue LED
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(LED_Blue_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = LED_Red_Pin; // Red LED
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(LED_Red_GPIO_Port, &GPIO_InitStruct);

    //--- Configure LEDs GPIO pin Output Level
    HAL_GPIO_WritePin(LED_Orange_GPIO_Port, LED_Orange_Pin, GPIO_PIN_RESET); // set off
    HAL_GPIO_WritePin(LED_Green_GPIO_Port, LED_Green_Pin, GPIO_PIN_RESET); // set off
    HAL_GPIO_WritePin(LED_Blue_GPIO_Port, LED_Blue_Pin, GPIO_PIN_RESET); // set off
    HAL_GPIO_WritePin(LED_Red_GPIO_Port, LED_Red_Pin, GPIO_PIN_RESET); // set off

    //--- Configure UART3 GPIO pins ------------------------------------------
    GPIO_InitStruct.Pin = UART3_TX_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF1_USART3;
    HAL_GPIO_Init(UART3_TX_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = UART3_RX_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF1_USART3;
    HAL_GPIO_Init(UART3_RX_GPIO_Port, &GPIO_InitStruct);
}


void UART3_Init(void) {
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = USART_OVERSAMPLING_16;
  //huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;

  if ( HAL_UART_Init(&huart3) != HAL_OK ) {
    Error_Handler();
  }

  // USART3 interrupt Init
  HAL_NVIC_SetPriority(USART3_4_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(USART3_4_IRQn);
}


void DMA_UART3_Init(void) {
    // USART3 DMA Init, USART3_TX Init
    hdma_usart3_tx.Instance = DMA1_Channel2;
    hdma_usart3_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_usart3_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart3_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart3_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart3_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart3_tx.Init.Mode = DMA_NORMAL;
    hdma_usart3_tx.Init.Priority = DMA_PRIORITY_LOW;
    
    if ( HAL_DMA_Init(&hdma_usart3_tx) != HAL_OK ) {
        Error_Handler();
    }

    __HAL_DMA_REMAP_CHANNEL_ENABLE(DMA_REMAP_USART3_DMA_CH32); // aka SYSCFG->CFGR1 |= (1UL<<26);
    __HAL_LINKDMA(&huart3, hdmatx, hdma_usart3_tx); 

    HAL_NVIC_SetPriority(DMA1_Channel2_3_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(DMA1_Channel2_3_IRQn);
}

