  /******************************************************************************
 *
 * [FILE NAME]: <IntCtrl.c>
 *
 * [AUTHOR]: <Ziad Ayman>
 *
 * [DATE CREATED]: <03/11/2022>
 *
 * [DESCRIPTION]: <Source file for INTCTRL>
 *
 *******************************************************************************/


#include "../../MCAL/IntCtrl/inc/IntCtrl.h"
#include "../../MCAL/IntCtrl/inc/IntCtrl_Regs.h"



STATIC uint8 IntCtrl_Status = INTCTRL_NOT_INITIALIZED;

/************************************************************************************
* Service Name: IntCtrl_Init
* Parameters (in): confifPtr - Pointer to post-build configuration data
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: Initializes the Port Driver module.
************************************************************************************/

void IntCtrl_Init(const IntCtrl_ConfigType* confifPtr)
{
	EXCEPTIONS_STATE;
	
	APINT = GROUP_SUBGROUP_FORMAT;
	
	uint8 i;
	
	for(i=0; i<INTCTRL_CONFIGURED_CHANNLES; i++)
	{
		if(confifPtr->Channels[i].InterruptPeripheralGates == SYSTICK)
		{
			/* Assign priority Level 1 to the SysTick Interrupt */
			NVIC_SYSTEM_PRI3_REG =  (NVIC_SYSTEM_PRI3_REG & 0x1FFFFFFF) | ((confifPtr->Channels[i].IntCtrl_GroupPriority) << 29);
		}
		else
		{
			/*do nothing */ 
		}
	}
}