/*
 * uart.h
 *
 *  Created on: Apr 25, 2021
 *      Author: ENGIN
 */

#ifndef INC_UART_H_
#define INC_UART_H_
#include "main.h"

extern UART_HandleTypeDef huart2;
extern PID pid;
extern uint8_t Uart2_rx_buffer[1];
extern uint8_t Uart2_recive_array[8];
extern uint8_t Uart2_tx_buffer[24];

void Uart2_send_FFreq_Upid_Tim(void);
void Uart2_echo(void);

#endif /* INC_UART_H_ */
