  /******************************************************************************
 *
 * [FILE NAME]: <IntCtrl_Cfg.h>
 *
 * [AUTHOR]: <Ziad Ayman>
 *
 * [DATE CREATED]: <03/11/2022>
 *
 * [DESCRIPTION]: <Header file for Pre Compile Confg>
 *
 *******************************************************************************/


#ifndef IntCtrl_CFG_H
#define IntCtrl_CFG_H

#include "../../MCAL/IntCtrl/inc/IntCtrl_regs.h"

#define Enable_Exceptions() __asm("CPSIE I")
#define Disable_Exceptions() __asm("CPSID I")
#define Enable_Faults() __asm("CPSIE F")
#define Disable_Faults() __asm("CPSID F")



#define EXCEPTIONS_STATE Enable_Exceptions()

#define FAULTS_STATE Enable_Faults()

#define BASE_PRI 0
/*
* 8 group priorities  --->  ( ( (APINT & x0000FFFF) | 0x05FA0000 ) | (0x4 << 8) )
* 4 group priorities  --->  ( ( (APINT & x0000FFFF) | 0x05FA0000 ) | (0x5 << 8) )
* 2 group priorities  --->  ( ( (APINT & x0000FFFF) | 0x05FA0000 ) | (0x6 << 8) )
* 1 group priorities  --->  ( ( (APINT & 0x0000FFFF) | 0x05FA0000 ) | (0x7 << 8) )
*/
#define xxx ( ( (APINT & 0x0000FFFF) | 0x05FA0000 ) | (0x4 << 8) )

#define xx_y  ( ( (APINT & 0x0000FFFF) | 0x05FA0000 ) | (0x5 << 8) )

#define x_yy ( ( (APINT & 0x0000FFFF) | 0x05FA0000 ) | (0x6 << 8) )

#define yyy ( ( (APINT & 0x0000FFFF) | 0x05FA0000 ) | (0x7 << 8) )

#define GROUP_SUBGROUP_FORMAT  xxx

#define INTCTRL_CONFIGURED_CHANNLES (1U)








#endif /* IntCtrl_CFG_H */
