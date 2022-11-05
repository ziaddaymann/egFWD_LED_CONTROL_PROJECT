  /******************************************************************************
 *
 * [FILE NAME]: <IntCtrl_PBcfg.c>
 *
 * [AUTHOR]: <Ziad Ayman>
 *
 * [DATE CREATED]: <03/11/2022>
 *
 * [DESCRIPTION]: <Source file for Post Compile Confg>
 *
 *******************************************************************************/

#include "../../MCAL/IntCtrl/inc/IntCtrl.h"

/* PB structure used with IntCtrl_Init API */
const IntCtrl_ConfigType IntCtrl_Configuration = {SYSTICK,1,2};
