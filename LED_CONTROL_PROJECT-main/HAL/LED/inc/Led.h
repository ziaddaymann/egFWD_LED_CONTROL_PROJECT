 /******************************************************************************
 *
 * [FILE NAME]: <led.h>
 *
 * [AUTHOR]: <Ziad Ayman>
 *
 * [DATE CREATED]: <04/11/2022>
 *
 * [DESCRIPTION]: <Header file for led>
 *
 *******************************************************************************/


#ifndef LED_H
#define LED_H

#include "../../LIB/Std_Types.h"

/* Set the led ON/OFF according to its configuration Positive logic or negative logic */
#define LED_ON  LOGIC_HIGH
#define LED_OFF LOGIC_LOW

/* Set the LED Port */
#define LED_PORT LED1_PORT_ID

/* Set the LED Pin Number */
#define LED_PIN_NUM LED1_CHANNEL_ID

/* Description: 1. Fill the led configurations structure 
 *              2. Set the PIN direction which the led is connected as OUTPUT pin
 *		3. Initialize the led ON/OFF according to the initial value 
 */

/* Description: Set the LED state to ON */
void LED_setOn(void);

/* Description: Set the LED state to OFF */
void LED_setOff(void);

/*Description: Toggle the LED state */
void LED_toggle(void);

#endif /* LED_H */
