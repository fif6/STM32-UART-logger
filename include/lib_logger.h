#ifndef LIB_LOGGER_H_
#define LIB_LOGGER_H_

#include "stm32f0xx_hal.h"

void Log_Init(UART_HandleTypeDef*); // Initial function

void Log_Urgent(char*); // Send a message via UART with a high priority. Half-Blocking mode!

void Log_Queue(char*); // Put message into the queue (low priority) - non blocking.

void Log_UART_TransferComplete(UART_HandleTypeDef*); // This function need to be called from an IRQ handler when the UART transfer complete.
/*
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
   Log_UART_TransferComplete(huart);
}
*/


#endif // LIB_LOGGER_H_