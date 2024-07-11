#include "stm32f0xx_hal.h"
#include "handlers.h"
#include "init_hw.h"



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


