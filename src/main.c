#include "stm32f0xx_hal.h"
#include "handlers.h"
#include "init_hw.h"
#include <stdio.h>
#include <string.h>
#include "etc.h"
#include "lib_logger.h"



UART_HandleTypeDef huart3 = {0}; // UART-3, globally
DMA_HandleTypeDef hdma_usart3_tx = {0}; // DMA for UART3 TX, globally



int main(void) {

    UART3_Init();

   
    Log_Init(&huart3);
    
    Log_Queue("q-0123456789abcdef0123456789ABCDEF\r\n");
    Log_Queue("q-b\r\n");
    Log_Queue("q-c\r\n");
    Log_Urgent("u-A\r\n");
    Log_Urgent("u-B\r\n");
    Log_Urgent("u-C\r\n");

    return 1;
}

