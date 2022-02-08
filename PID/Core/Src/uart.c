/*
 * uart.c
 *
 *  Created on: Apr 25, 2021
 *      Author: ENGIN
 */

#include "main.h"

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(echo == 1)
	{
		HAL_UART_Transmit(&huart2,Uart2_rx_buffer,1,0);
	}
	if(echo == 0)
	{
		uint8_t chk = 0 ;

		for(uint8_t i = 1; i < 8 ;i++)
		{
			Uart2_recive_array[(i-1)] =	Uart2_recive_array[i] ;
		}
		Uart2_recive_array[7]	=	Uart2_rx_buffer[0]	  ;

		if( Uart2_recive_array[7] == 0xff && Uart2_recive_array[6] == 0x48 && Uart2_recive_array[1] == 0xff  )
		{
			for(uint8_t i = 1 ; i<8 ;i++)
			{
				chk = chk + Uart2_recive_array[i];
			}
			if(chk == Uart2_recive_array[0])
			{
				pid.Req_Frequancy.Ui_8[3]	=	Uart2_recive_array[5] ;
				pid.Req_Frequancy.Ui_8[2]	=	Uart2_recive_array[4] ;
				pid.Req_Frequancy.Ui_8[1]	=	Uart2_recive_array[3] ;
				pid.Req_Frequancy.Ui_8[0]	=	Uart2_recive_array[2] ;
			}


		}
		else if( Uart2_recive_array[6] == 0xfc && Uart2_recive_array[5] == 0x26 && Uart2_recive_array[1] == 0xfc )
		{
			for(uint8_t i = 1 ; i<7 ;i++)
			{
				chk = chk + Uart2_recive_array[i];
			}
			if(chk == Uart2_recive_array[0])
			{
				pid.Kp	=	Uart2_recive_array[2] ;
				pid.Ki	=	Uart2_recive_array[3] ;
				pid.Kd	=	Uart2_recive_array[4] ;
			}
		}
	}


	HAL_UART_Receive_IT(&huart2,Uart2_rx_buffer,1);

  /* Prevent unused argument(s) compilation warning */
  UNUSED(huart);
  /* NOTE: This function should not be modified, when the callback is needed,
           the HAL_UART_RxCpltCallback could be implemented in the user file
   */
}

void Uart2_send_FFreq_Upid_Tim(void)
{
	Uart2_tx_buffer[15] = 0xff;
	Uart2_tx_buffer[14] = 0x48;
	Uart2_tx_buffer[13] = pid.Fed_Frequancy.Ui_8[3];
	Uart2_tx_buffer[12] = pid.Fed_Frequancy.Ui_8[2];
	Uart2_tx_buffer[11] = pid.Fed_Frequancy.Ui_8[1];
	Uart2_tx_buffer[10] = pid.Fed_Frequancy.Ui_8[0];
	Uart2_tx_buffer[9]  = pid.Upid.Ui_8[3];
	Uart2_tx_buffer[8]  = pid.Upid.Ui_8[2];
	Uart2_tx_buffer[7]  = pid.Upid.Ui_8[1];
	Uart2_tx_buffer[6]  = pid.Upid.Ui_8[0];
	Uart2_tx_buffer[5]  = Tim_clock.Ui_8[3];
	Uart2_tx_buffer[4]  = Tim_clock.Ui_8[2];
	Uart2_tx_buffer[3]  = Tim_clock.Ui_8[1];
	Uart2_tx_buffer[2]  = Tim_clock.Ui_8[0];
	Uart2_tx_buffer[1]  = 0xff;
	Uart2_tx_buffer[0]  = 0xff+Tim_clock.Ui_8[0]+Tim_clock.Ui_8[1]+Tim_clock.Ui_8[2]+Tim_clock.Ui_8[3]+
							pid.Upid.Ui_8[0]+pid.Upid.Ui_8[1]+pid.Upid.Ui_8[2]+pid.Upid.Ui_8[3]+
								pid.Fed_Frequancy.Ui_8[0]+pid.Fed_Frequancy.Ui_8[1]+
									pid.Fed_Frequancy.Ui_8[2]+pid.Fed_Frequancy.Ui_8[3]+
									0x48+0xff;
	HAL_UART_Transmit_IT(&huart2, Uart2_tx_buffer, 16);
}
