  /******************************************************************************
 *
 * [FILE NAME]: <IntCtrl.h>
 *
 * [AUTHOR]: <Ziad Ayman>
 *
 * [DATE CREATED]: <03/11/2022>
 *
 * [DESCRIPTION]: <Header file for INTCTRL>
 *
 *******************************************************************************/


#ifndef INTCTRL_H
#define INTCTRL_H


#define INTCTRL_INITIALIZED                (1U)
#define INTCTRL_NOT_INITIALIZED            (0U)

/* Standard AUTOSAR types */
#include "../../LIB/Std_Types.h"

/* Dio Pre-Compile Configuration Header file */
#include "../../MCAL/IntCtrl/inc/IntCtrl_Cfg.h"



/* Non AUTOSAR files */
#include "../../LIB/Common_Macros.h"

#define INTCTRL_INITIALIZED                (1U)
#define INTCTRL_NOT_INITIALIZED            (0U)

/*******************************************************************************
 *                              Module Data Types                              *
 *******************************************************************************/

 
/* Type definition for IntCtrl_InterruptType used by the IntCtrl APIs */
typedef enum 
{
	RESET = 1, 
	NMI = 2,
	HARD_FAULT = 3,
	MEMORY_MANAGEMENT_FAULT = 4,
	BUS_FAULT = 5,
	USAGE_FAULT = 6,
	SYSTICK = 15,
	GPIO_PORT_A = 16
}IntCtrl_InterruptType;



/* Structure for Dio_ChannelGroup */
typedef struct
{
	IntCtrl_InterruptType InterruptPeripheralGates;

	uint8 IntCtrl_GroupPriority;

	uint8 IntCtrl_SubGroupPriority;

} IntCtrl_ConfigChannel;



/* Data Structure required for initializing the Dio Driver */
typedef struct IntCtrl_ConfigType
{
	IntCtrl_ConfigChannel Channels[INTCTRL_CONFIGURED_CHANNLES];

} IntCtrl_ConfigType;

/*******************************************************************************
 *                      Function Prototypes                                    *
 *******************************************************************************/

void IntCtrl_Init(const IntCtrl_ConfigType* confifPtr);


/*******************************************************************************
 *                       External Variables                                    *
 *******************************************************************************/

 /* Extern PB structures to be used by Dio and other modules */
extern const IntCtrl_ConfigType IntCtrl_Configuration;

#endif /* INTCTRL_H */
