  /******************************************************************************
 *
 * [FILE NAME]: <Dio.c>
 *
 * [AUTHOR]: <Ziad Ayman>
 *
 * [DATE CREATED]: <03/11/2022>
 *
 * [DESCRIPTION]: <Source file for DIO>
 *
 *******************************************************************************/

#include "../../MCAL/DIO/inc/Dio.h"
#include "../../MCAL/DIO/inc/Dio_Regs.h"

STATIC const Dio_ConfigChannel * Dio_PortChannels = NULL_PTR;
STATIC uint8 status = DIO_NOT_INITIALIZED;

/************************************************************************************
* Service Name: Dio_Init
* Service ID[hex]: 0x10
* Sync/Async: Synchronous
* Reentrancy: Non reentrant
* Parameters (in): configPtr - Pointer to post-build configuration data
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: Function to Initialize the Dio module.
************************************************************************************/
void Dio_Init(const Dio_Config * configPtr)
{
	
		/*
		 * Set the module state to DIO_INITIALIZED and point to the PB configuration structure using a global pointer.
		 * This global pointer is global to be used by other functions to read the PB configuration structures
		 */
		status       = DIO_INITIALIZED;
		Dio_PortChannels = configPtr->Channels; /* address of the first Channels structure --> Channels[0] */
	
}

/************************************************************************************
* Service Name: Dio_WriteChannel
* Service ID[hex]: 0x01
* Sync/Async: Synchronous
* Reentrancy: Reentrant
* Parameters (in): channel_id - ID of DIO channel.
*                  Level - Value to be written.
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: Function to set a Level of a channel.
************************************************************************************/
void Dio_WriteChannel(Dio_ChannelType channel_id, Dio_LevelType Level)
{
	volatile uint32 * portPtr = NULL_PTR;
	
	
		/* Point to the correct PORT register according to the Port Id stored in the port_number member */
		switch(Dio_PortChannels[channel_id].port_number)
		{
        case 0:    portPtr = &GPIO_PORTA_DATA_REG;
		               break;
		    case 1:    portPtr = &GPIO_PORTB_DATA_REG;
		               break;
		    case 2:    portPtr = &GPIO_PORTC_DATA_REG;
		               break;
		    case 3:    portPtr = &GPIO_PORTD_DATA_REG;
		               break;
        case 4:    portPtr = &GPIO_PORTE_DATA_REG;
		               break;
        case 5:    portPtr = &GPIO_PORTF_DATA_REG;
		               break;
		}
		if(Level == LOGIC_HIGH)
		{
			/* Write Logic High */
			SET_BIT(*portPtr,Dio_PortChannels[channel_id].channel_number);
		}
		else if(Level == LOGIC_LOW)
		{
			/* Write Logic Low */
			CLEAR_BIT(*portPtr,Dio_PortChannels[channel_id].channel_number);
		}
	

}

/************************************************************************************
* Service Name: Dio_ReadChannel
* Service ID[hex]: 0x00
* Sync/Async: Synchronous
* Reentrancy: Reentrant
* Parameters (in): channel_id - ID of DIO channel.
* Parameters (inout): None
* Parameters (out): None
* Return value: Dio_LevelType
* Description: Function to return the value of the specified DIO channel.
************************************************************************************/
Dio_LevelType Dio_ReadChannel(Dio_ChannelType channel_id)
{
	volatile uint32 * portPtr = NULL_PTR;
	Dio_LevelType output = LOGIC_LOW;

	
		/* Point to the correct PORT register according to the Port Id stored in the port_number member */
		switch(Dio_PortChannels[channel_id].port_number)
		{
        case 0:    portPtr = &GPIO_PORTA_DATA_REG;
		               break;
		    case 1:    portPtr = &GPIO_PORTB_DATA_REG;
		               break;
		    case 2:    portPtr = &GPIO_PORTC_DATA_REG;
		               break;
		    case 3:    portPtr = &GPIO_PORTD_DATA_REG;
		               break;
        case 4:    portPtr = &GPIO_PORTE_DATA_REG;
		               break;
        case 5:    portPtr = &GPIO_PORTF_DATA_REG;
		               break;
		}
		/* Read the required channel */
		if(BIT_IS_SET(*portPtr,Dio_PortChannels[channel_id].channel_number))
		{
			output = LOGIC_HIGH;
		}
		else
		{
			output = LOGIC_LOW;
		}
	
        return output;
}



/************************************************************************************
* Service Name: Dio_FlipChannel
* Service ID[hex]: 0x11
* Sync/Async: Synchronous
* Reentrancy: Reentrant
* Parameters (in): channel_id - ID of DIO channel.
* Parameters (inout): None
* Parameters (out): None
* Return value: Dio_LevelType
* Description: Function to flip the Level of a channel and return the Level of the channel after flip.
************************************************************************************/
#if (DIO_FLIP_CHANNEL_API == STD_ON)
Dio_LevelType Dio_FlipChannel(Dio_ChannelType channel_id)
{
	volatile uint32 * portPtr = NULL_PTR;
	Dio_LevelType output = LOGIC_LOW;
	

		/* Point to the correct PORT register according to the Port Id stored in the port_number member */
		switch(Dio_PortChannels[channel_id].port_number)
		{
        case 0:    portPtr = &GPIO_PORTA_DATA_REG;
		               break;
		    case 1:    portPtr = &GPIO_PORTB_DATA_REG;
		               break;
		    case 2:    portPtr = &GPIO_PORTC_DATA_REG;
		               break;
		    case 3:    portPtr = &GPIO_PORTD_DATA_REG;
		               break;
        case 4:    portPtr = &GPIO_PORTE_DATA_REG;
		               break;
        case 5:    portPtr = &GPIO_PORTF_DATA_REG;
		               break;
		}
		/* Read the required channel and write the required Level */
		if(BIT_IS_SET(*portPtr,Dio_PortChannels[channel_id].channel_number))
		{
			CLEAR_BIT(*portPtr,Dio_PortChannels[channel_id].channel_number);
			output = LOGIC_LOW;
		}
		else
		{
			SET_BIT(*portPtr,Dio_PortChannels[channel_id].channel_number);
			output = LOGIC_HIGH;
		}
	
        return output;
}
#endif
