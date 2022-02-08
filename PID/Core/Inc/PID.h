/*
 * PID.h
 *
 *  Created on: Apr 23, 2021
 *      Author: ENGIN
 */


#ifndef INC_PID_H_
#define INC_PID_H_

#include "main.h"



extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;



/* to able to convert 8 bit to 32 bit data from uart*/

typedef union {
	int32_t Ui_32;
	struct{
		uint8_t Ui_8[4];
	};
}int8_32;

/*pid values*/
typedef struct {

	int32_t Error  ;
	int8_32 Upid   ;
	int8_32 Req_Frequancy ;
	int8_32 Fed_Frequancy ;
	/*Proportional variable */
	uint8_t  Kp ;
	int32_t Up ;

	/*Integral variable */
	uint8_t  Ki ;
	int32_t Ui ;
	int32_t Ui_Sum ;

	/*Derivative variable */
	uint8_t  Kd ;
	int32_t Ud ;
	int32_t Error_Old ;
}PID;

PID pid;

/*uart*/
uint8_t Uart2_rx_buffer[1];
uint8_t Uart2_recive_array[8];
uint8_t Uart2_tx_buffer[24];
uint8_t echo;

/* timer*/
uint8_t Tim_10_Us ;
int8_32 Tim_clock;


void PID_Init(void);
void PID_Calculate_Set(void);




#endif /* INC_PID_H_ */
