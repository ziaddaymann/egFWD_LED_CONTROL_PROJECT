  /******************************************************************************
 *
 * [FILE NAME]: <Dio.h>
 *
 * [AUTHOR]: <Ziad Ayman>
 *
 * [DATE CREATED]: <03/11/2022>
 *
 * [DESCRIPTION]: <Header file for DIO>
 *
 *******************************************************************************/

#ifndef DIO_H
#define DIO_H


#define DIO_INITIALIZED                (1U)
#define DIO_NOT_INITIALIZED            (0U)

#include "../../LIB/Std_Types.h"


#include "../../MCAL/DIO/inc/Dio_Cfg.h"


#include "../../LIB/Common_Macros.h"



/*******************************************************************************
 *                              Module Data Types                              *
 *******************************************************************************/

/* Type definition for Dio_ChannelType used by the DIO APIs */
typedef uint8 Dio_ChannelType;

/* Type definition for Dio_PortType used by the DIO APIs */
typedef uint8 Dio_PortType;

/* Type definition for Dio_LevelType used by the DIO APIs */
typedef uint8 Dio_LevelType;

/* Type definition for Dio_PortLevelType used by the DIO APIs */
typedef uint8 Dio_PortLevelType;

/* Structure for Dio_ChannelGroup */
typedef struct
{
  /* Mask which defines the positions of the channel group */
  uint8 mask;
  /* Position of the group from LSB */
  uint8 offset;
  /* This shall be the port ID which the Channel group is defined. */
  Dio_PortType index;
} Dio_ChannelGroupType;

typedef struct
{
	/* Member contains the ID of the Port that this channel belongs to */
	Dio_PortType port_number;
	/* Member contains the ID of the Channel*/
	Dio_ChannelType channel_number;
}Dio_ConfigChannel;

/* Data Structure required for initializing the Dio Driver */
typedef struct Dio_Config
{
	Dio_ConfigChannel Channels[DIO_CONFIGURED_CHANNLES];
} Dio_Config;

/*******************************************************************************
 *                      Function Prototypes                                    *
 *******************************************************************************/

/* Function for DIO read Port API */
Dio_LevelType Dio_ReadChannel(Dio_ChannelType channel_id);

/* Function for DIO write Port API */
void Dio_WriteChannel(Dio_ChannelType channel_id, Dio_LevelType Level);

/* Function for DIO Initialization API */
void Dio_Init(const Dio_Config * confifPtr);

#if (DIO_FLIP_CHANNEL_API == STD_ON)
/* Function for DIO flip channel API */
Dio_LevelType Dio_FlipChannel(Dio_ChannelType channel_id);
#endif



/*******************************************************************************
 *                       External Variables                                    *
 *******************************************************************************/

/* Extern PB structures to be used by Dio and other modules */
extern const Dio_Config Config;

#endif /* DIO_H */
