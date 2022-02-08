/*
 * PID.c
 *
 *  Created on: Apr 23, 2021
 *      Author: ENGIN
 */

#include "main.h"

void PID_Init(void)
{
	pid . Kp        = 1	;
	pid . Ki        = 1 ;
	pid . Kd        = 1 ;
	pid . Ui_Sum    = 0 ;
	pid . Error_Old = 0 ;
	echo            = 1 ;
	Tim_10_Us       = 10;
	Tim_clock.Ui_32 = 0 ;

}

void PID_Calculate_Set(void)
{
	if(pid.Req_Frequancy.Ui_32 > 1000-1){

		pid.Error = pid.Req_Frequancy.Ui_32 - pid.Fed_Frequancy.Ui_32 ;

		pid.Up = pid.Kp *
				   pid.Error ;

		pid.Ui = pid.Ki *
				    pid.Error +
					  pid.Ui_Sum;

		pid.Ui_Sum = pid.Ui ;

		pid.Ud = pid.Kd *
				   (pid.Error -
			         pid.Error_Old) ;

		pid.Error_Old = pid.Error ;

		pid.Upid.Ui_32 = pid.Up +
				       pid.Ui +
					     pid.Ud ;

		if(pid.Upid.Ui_32 < 1000-1 ){pid.Upid.Ui_32 = 1000;}
		if(pid.Upid.Ui_32 > 10000 ){pid.Upid.Ui_32 = 10000;}
		TIM1->CCR1 = pid.Upid.Ui_32-1;
	}
}
