  /******************************************************************************
 *
 * [FILE NAME]: <Port.h>
 *
 * [AUTHOR]: <Ziad Ayman>
 *
 * [DATE CREATED]: <03/11/2022>
 *
 * [DESCRIPTION]: <Header file for Port>
 *
 *******************************************************************************/

#ifndef PORT_H
#define PORT_H

/*
 * Macros for Port Status
 */
#define PORT_DIO_INITIALIZED                (1U)
#define PORT_DIO_NOT_INITIALIZED            (0U)

#include "../../LIB/Std_Types.h"

#include "../../MCAL/PORT/inc/Port_Cfg.h"


 /* Non AUTOSAR files */
#include "../../LIB/Common_Macros.h"
   


/*******************************************************************************
 *                              Module Definitions                             *
 *******************************************************************************/
/* GPIO Registers base addresses */
#define GPIO_PORTA_BASE_ADDRESS           0x40004000
#define GPIO_PORTB_BASE_ADDRESS           0x40005000
#define GPIO_PORTC_BASE_ADDRESS           0x40006000
#define GPIO_PORTD_BASE_ADDRESS           0x40007000
#define GPIO_PORTE_BASE_ADDRESS           0x40024000
#define GPIO_PORTF_BASE_ADDRESS           0x40025000

/* GPIO Registers offset addresses */
#define PORT_DATA_REG_OFFSET              0x3FC
#define PORT_DIR_REG_OFFSET               0x400
#define PORT_ALT_FUNC_REG_OFFSET          0x420
#define PORT_PULL_UP_REG_OFFSET           0x510
#define PORT_PULL_DOWN_REG_OFFSET         0x514
#define PORT_DIGITAL_ENABLE_REG_OFFSET    0x51C
#define PORT_LOCK_REG_OFFSET              0x520
#define PORT_COMMIT_REG_OFFSET            0x524
#define PORT_ANALOG_MODE_SEL_REG_OFFSET   0x528
#define PORT_CTL_REG_OFFSET               0x52C

/*******************************************************************************
 *                              Module Data Types                              *
 *******************************************************************************/
/* Type definition for Port_PortType used by the PORT APIs (Doesn't exist in AUTOSAR 4.0.3 PORT SWS Document.*/
typedef uint8 Port_PortType;

 /* Type definition for Port_PinType used by the PORT APIs */
typedef uint8 Port_PinType;

 /* Type definition for Port_PinDirectionType used by the PORT APIs */
typedef uint8 Port_PinDirectionType;

 /* Type definition for Port_PinModeType used by the PORT APIs */
typedef uint8 Port_PinModeType;

/* Structure for Port_Channel */
typedef struct
{
	/* Member contains the ID of the Port that this channel belongs to */
	Port_PortType port_number;
	/* Member contains the ID of the Channel*/
	Port_PinType channel_number;
        /* Member contains the direction of the Channel */
        Port_PinDirectionType Ch_Dir;
        /* Member contains the Mode of the Channel */
        Port_PinModeType Mode;
        /* Member contains the Initial Value of the Channel */
        uint8 InitialValue;
        /* Member indicates whether the internal resistor is enabled as pull up or pull down or off */
        uint8 resistor;
        /* Member indicates whether the Channel's Direction is changeable or not */
        uint8 DirectionChangeability;
        /* Member indicates whether the Channel's Mode is changeable or not */
        uint8 ModeChangeability;
        
}Port_ConfigChannel;

/* Data Structure required for initializing the Port Driver */
typedef struct Port_ConfigType
{
	Port_ConfigChannel Channels[PORT_CONFIGURED_CHANNLES];
}Port_ConfigType;

/*******************************************************************************
 *                      Function Prototypes                                    *
 *******************************************************************************/

/************************************************************************************
* Service Name: Port_Init
* Service ID[hex]: 0x00
* Sync/Async: Synchronous
* Reentrancy: Non Reentrant
* Parameters (in): confifPtr - Pointer to post-build configuration data
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: Initializes the Port Driver module.
************************************************************************************/
void Port_Init(const Port_ConfigType* confifPtr);



/************************************************************************************
* Service Name: Port_SetPinDirection
* Service ID[hex]: 0x10
* Sync/Async: Synchronous
* Reentrancy: Reentrant
* Parameters (in): 
    -Pin       --> Port Pin ID number
    -Direction --> Port Pin Direction
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: Sets the port pin direction
************************************************************************************/
void Port_SetPinDirection(Port_PinType Pin,Port_PinDirectionType Direction);



/************************************************************************************
* Service Name: Port_RefreshPortDirection
* Service ID[hex]: 0x20
* Sync/Async: Synchronous
* Reentrancy: Non Reentrant
* Parameters (in): None
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: Refreshes port direction.
************************************************************************************/
void Port_RefreshPortDirection(void);




/************************************************************************************
* Service Name: Port_SetPinMode
* Service ID[hex]: 0x40
* Sync/Async: Synchronous
* Reentrancy: Reentrant
* Parameters (in): 
    -Pin   --> Port Pin ID number
    -Mode  --> New Port Pin mode to be set on port pin
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: Sets the port pin mode. 
************************************************************************************/
void Port_SetPinMode(Port_PinType Pin,Port_PinModeType Mode);




/*******************************************************************************
 *                       External Variables                                    *
 *******************************************************************************/

/* Extern PB structures to be used by Port and other modules */
extern const Port_ConfigType Port_Configuration;

#endif /* PORT_H */
