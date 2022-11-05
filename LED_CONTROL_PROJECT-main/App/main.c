 /******************************************************************************
 *
 * [FILE NAME]: <main.c>
 *
 * [AUTHOR]: <Ziad Ayman>
 *
 * [DATE CREATED]: <04/11/2022>
 *
 * [DESCRIPTION]: <Source file for main>
 *
 *******************************************************************************/



#include "../../HAL/LED/inc/Led.h"
#include "../../MCAL/IntCtrl/inc/IntCtrl.h"
#include "../../MCAL/DIO/inc/Dio.h"
#include "../../MCAL/PORT/inc/Port.h"
#include "../../MCAL/SYSTICK/inc/SysTick.h"

static uint8 New_Tick_Flag = 0;

void SystickNewTick(void)
{
	/*a flag used as indication for new tick in the systick timer and then a new second indication */
    New_Tick_Flag = 1;
}

int main(void)
{
	Dio_Init(&Config);
  Port_Init(&Port_Configuration);
	IntCtrl_Init(&IntCtrl_Configuration);
	
	/*setting call back function to be called at executing of the systick ISR */
	SysTick_SetCallBack(SystickNewTick);
	
	/*setting time for generating interrupt to be every one second */
	SysTick_Start(1000);
	
	while(1)
	{
		if(New_Tick_Flag == 1)
		{
			LED_toggle();
			New_Tick_Flag = 0;
		}
	}
}
