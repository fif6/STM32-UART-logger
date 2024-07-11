#include "stm32f0xx_hal.h"
#include <inttypes.h>
#include <string.h>

//extern UART_HandleTypeDef huart3;
//UART_HandleTypeDef* uart_h = NULL;

#define MSG_LEN 30
#define QUEUE_LEN 10

volatile struct {
    UART_HandleTypeDef* huart_; // UART handle
    uint8_t uart_is_busy_;
    uint8_t has_urgent_msg_;
    uint8_t queue_cur_items_;
    uint8_t last_transfered_by_;
    uint8_t q_lock_;

    char* read_pos_;
    char* write_pos_;
    char* pos_of_end_;
} config;
char buf_urgent[MSG_LEN];
char buf_queue[MSG_LEN * QUEUE_LEN];


void Log_Init(UART_HandleTypeDef* huart) {
    config.huart_ = huart;
    config.uart_is_busy_ = 0;
    config.has_urgent_msg_ = 0;
    config.queue_cur_items_ = 0;
    config.q_lock_ = 0;

    config.read_pos_ = buf_queue; // init value
    config.write_pos_ = buf_queue; // init value
    config.pos_of_end_ = buf_queue+(MSG_LEN*QUEUE_LEN); // init value. 
}

void Log_Queue(const char* msg) { // Non-Blocking call
    while (config.q_lock_ == 1) {
        __ASM("nop"); // Waiting for the queue to become free
    }
    config.q_lock_ = 1;

    if (config.queue_cur_items_ >= QUEUE_LEN) {
        // queue is full. The message will be discarded.
        config.q_lock_ = 0;
        return;
    }
    config.queue_cur_items_++; // accepting the message and increasing queue length

    size_t msg_strlen = strlen(msg);
    size_t msg_len = (msg_strlen > MSG_LEN) ? MSG_LEN : msg_strlen;

    //char* item_addr = buf_queue + ((config.queue_cur_items_ - 1) * MSG_LEN);
    //memcpy(item_addr, msg, msg_len);

    
    memcpy(config.write_pos_, msg, msg_len);
    config.write_pos_ += MSG_LEN;
    if ( config.write_pos_ == config.pos_of_end_ ) {
        config.write_pos_ = buf_queue; // set position to the start address of a buffer
    }

    if (config.uart_is_busy_ == 0) { 
        // Initial transfer immediatly
        config.uart_is_busy_ = 1;
        config.last_transfered_by_ = 1; // queue
        //HAL_UART_Transmit_DMA(&huart3, (uint8_t*)config.read_pos_, msg_len);
        HAL_UART_Transmit_DMA(config.huart_, (uint8_t*)config.read_pos_, msg_len);
        

        config.read_pos_ += MSG_LEN;
        if ( config.read_pos_ == config.pos_of_end_ ) {
            config.read_pos_ = buf_queue; // set position to the start address of a buffer
        }

        config.q_lock_ = 0;
        return;
    }

    config.q_lock_ = 0;
    return; //-> queued transfer
}

void Log_UART_TransferComplete(UART_HandleTypeDef* huart) { // Last transfer ends callback by INTERRUPT
    if (config.huart_->Instance != huart->Instance) {
        // It the message isn't for us ...
        return;
    }
    if (config.last_transfered_by_ == 1) { // if by queue
        // queue item transfer complete. Decreasing the index
        config.queue_cur_items_--;
    }
    

    if (config.has_urgent_msg_ == 1) {
        // Freeing stack for new call of Log_Urgent();
        config.last_transfered_by_ = 0; // flag clear
        config.has_urgent_msg_ = 0; // flag clear
        config.uart_is_busy_ = 0; // flag clear
        return;
    }


    // We haven't urgent MSG but we can have messsages in the queue
    if (config.queue_cur_items_ > 0) { 
        //config.uart_is_busy_ = 1; // duplicating the flag
        config.last_transfered_by_ = 1; // queue
        
        size_t msg_strlen = strlen(config.read_pos_);
        size_t msg_len = (msg_strlen > MSG_LEN) ? MSG_LEN : msg_strlen;

        //HAL_UART_Transmit_DMA(&huart3, (uint8_t*)config.read_pos_, msg_len);
        HAL_UART_Transmit_DMA(config.huart_, (uint8_t*)config.read_pos_, msg_len);

        config.read_pos_ += MSG_LEN;
        if ( config.read_pos_ == config.pos_of_end_ ) {
            config.read_pos_ = buf_queue; // set position to the start address of a buffer
        }

        return;
    }

    // no messages. Nothing to do
    config.last_transfered_by_ = 0; // flag clear
    config.uart_is_busy_ = 0; // flag clear
    return;
}


void Log_Urgent(const char* msg) { // Half-Bloking call
    config.has_urgent_msg_ = 1;
    while ( config.uart_is_busy_ == 1 ) { // wait for prev urgent transfer ends
        __ASM("nop");
    }
    config.uart_is_busy_ = 1;
    
    size_t msg_strlen = strlen(msg);
    size_t msg_len = (msg_strlen > MSG_LEN) ? MSG_LEN : msg_strlen;
    memcpy(buf_urgent, msg, msg_len);

    config.last_transfered_by_ = 2; // type urgent
    //HAL_UART_Transmit_DMA(&huart3, (uint8_t*)buf_urgent, msg_len);
    HAL_UART_Transmit_DMA(config.huart_, (uint8_t*)buf_urgent, msg_len);
}


// Redeclare HAL weak function HAL_UART_TxCpltCallback()
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
   Log_UART_TransferComplete(huart);
}
