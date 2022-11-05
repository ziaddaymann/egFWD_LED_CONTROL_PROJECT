  /******************************************************************************
 *
 * [FILE NAME]: <Dio_Cfg.h>
 *
 * [AUTHOR]: <Ziad Ayman>
 *
 * [DATE CREATED]: <03/11/2022>
 *
 * [DESCRIPTION]: <Header file for Pre Compile Configurations>
 *
 *******************************************************************************/

#ifndef DIO_CFG_H
#define DIO_CFG_H


/* Pre-compile option for presence of Dio_FlipChannel API */
#define DIO_FLIP_CHANNEL_API                (STD_ON)

/* Number of the configured Dio Channels */
#define DIO_CONFIGURED_CHANNLES              (1U)


#define DioConf_LED1_CHANNEL_ID_INDEX        (uint8)0x00


/* DIO Configured Port ID's  */
#define LED1_PORT_ID                (Dio_PortType)5 /* PORTF */

/* DIO Configured Channel ID's */
#define LED1_CHANNEL_ID             (Dio_ChannelType)1 /* Pin 1 in PORTF */

#endif /* DIO_CFG_H */
