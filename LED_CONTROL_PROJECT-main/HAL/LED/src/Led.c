 /******************************************************************************
 *
 * [FILE NAME]: <led.c>
 *
 * [AUTHOR]: <Ziad Ayman>
 *
 * [DATE CREATED]: <04/11/2022>
 *
 * [DESCRIPTION]: <Source file for led>
 *
 *******************************************************************************/



#include "../../MCAL/DIO/inc/Dio.h"
#include "../../HAL/LED/inc/Led.h"



void LED_setOn(void)
{
    Dio_WriteChannel(DioConf_LED1_CHANNEL_ID_INDEX,LED_ON);  /* LED ON */
}


void LED_setOff(void)
{
    Dio_WriteChannel(DioConf_LED1_CHANNEL_ID_INDEX,LED_OFF); /* LED OFF */
}


void LED_toggle(void)
{
    Dio_LevelType state = Dio_FlipChannel(DioConf_LED1_CHANNEL_ID_INDEX);
}

