  /******************************************************************************
 *
 * [FILE NAME]: <Port.c>
 *
 * [AUTHOR]: <Ziad Ayman>
 *
 * [DATE CREATED]: <03/11/2022>
 *
 * [DESCRIPTION]: <Source file for PORT>
 *
 *******************************************************************************/
 
 

#include "../../MCAL/PORT/inc/Port.h"
#include "../../MCAL/PORT/inc/Port_Regs.h"



STATIC const Port_ConfigChannel * Port_PortChannels = NULL_PTR;
STATIC uint8 Port_Status = PORT_DIO_NOT_INITIALIZED;

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
void Port_Init(const Port_ConfigType* confifPtr)
{
        
  Port_Status = PORT_DIO_INITIALIZED;
  Port_PortChannels = confifPtr->Channels; /* address of the first Channels structure --> Channels[0] */
  
  uint8 i = 0;
  for(i = 0; i < PORT_CONFIGURED_CHANNLES; i++)
  {
    if( (i > 15) && (i < 20) )
    {
      /* Do Nothing ...  this is the JTAG pins */
    }
    else
    {
  
        
    volatile uint32 * PortGpio_Ptr = NULL_PTR; /* point to the required Port Registers base address */
    volatile uint32 delay = 0;
    
    switch(Port_PortChannels[i].port_number)
    {
        case  0: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTA_BASE_ADDRESS; /* PORTA Base Address */
		 break;
	case  1: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTB_BASE_ADDRESS; /* PORTB Base Address */
		 break;
	case  2: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTC_BASE_ADDRESS; /* PORTC Base Address */
		 break;
	case  3: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTD_BASE_ADDRESS; /* PORTD Base Address */
		 break;
        case  4: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTE_BASE_ADDRESS; /* PORTE Base Address */
		 break;
        case  5: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTF_BASE_ADDRESS; /* PORTF Base Address */
		 break;
    }
    
    /* Enable clock for PORT and allow time for clock to start*/
    SYSCTL_REGCGC2_REG |= (1<<Port_PortChannels[i].port_number);
    delay = SYSCTL_REGCGC2_REG;
    
    if( ((Port_PortChannels[i].port_number == 3) && (Port_PortChannels[i].channel_number == 7)) || ((Port_PortChannels[i].port_number == 5) && (Port_PortChannels[i].channel_number == 0)) ) /* PD7 or PF0 */
    {
        *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_LOCK_REG_OFFSET) = 0x4C4F434B;                     /* Unlock the GPIOCR register */   
        SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_COMMIT_REG_OFFSET) , Port_PortChannels[i].channel_number);  /* Set the corresponding bit in GPIOCR register to allow changes on this pin */
    }
    else if( (Port_PortChannels[i].port_number == 2) && (Port_PortChannels[i].channel_number <= 3) ) /* PC0 to PC3 */
    {
        /* Do Nothing ...  this is the JTAG pins */
    }
    else
    {
        /* Do Nothing ... No need to unlock the commit register for this pin */
    }
    
    if(Port_PortChannels[i].Mode == PORT_PIN_MODE_DIO)
    {
    CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , Port_PortChannels[i].channel_number);      /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
    CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , Port_PortChannels[i].channel_number);             /* Disable Alternative function for this pin by clear the corresponding bit in GPIOAFSEL register */
    *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) &= ~(0x0000000F << (Port_PortChannels[i].channel_number * 4));     /* Clear the PMCx bits for this pin */
    SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , Port_PortChannels[i].channel_number);        /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
    }
    else if(Port_PortChannels[i].Mode == PORT_PIN_MODE_ADC)
    {
    SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , Port_PortChannels[i].channel_number);      /* Set the corresponding bit in the GPIOAMSEL register to Enable analog functionality on this pin */
    CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , Port_PortChannels[i].channel_number);             /* Disable Alternative function for this pin by clear the corresponding bit in GPIOAFSEL register */
    *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) &= ~(0x0000000F << (Port_PortChannels[i].channel_number * 4));     /* Clear the PMCx bits for this pin */
    CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , Port_PortChannels[i].channel_number);        /* Clear the corresponding bit in the GPIODEN register to disable digital functionality on this pin */
    }
    else if(Port_PortChannels[i].Mode == PORT_PIN_MODE_UART)
    {
    CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , Port_PortChannels[i].channel_number);      /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
    SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , Port_PortChannels[i].channel_number);             /* Enable Alternative function for this pin by setting the corresponding bit in GPIOAFSEL register */
    *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |= (~(0x0000000F << (Port_PortChannels[i].channel_number * 4)) | (Port_PortChannels[i].Mode));     /* Set the PMCx bits for this pin depending on the mode */
    SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , Port_PortChannels[i].channel_number);        /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
    }
    else if(Port_PortChannels[i].Mode == PORT_PIN_MODE_SSI)
    {
    CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , Port_PortChannels[i].channel_number);      /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
    SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , Port_PortChannels[i].channel_number);             /* Enable Alternative function for this pin by setting the corresponding bit in GPIOAFSEL register */
    *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |= (~(0x0000000F << (Port_PortChannels[i].channel_number * 4)) | (Port_PortChannels[i].Mode));     /* Set the PMCx bits for this pin depending on the mode */
    SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , Port_PortChannels[i].channel_number);        /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
    }
    else if(Port_PortChannels[i].Mode == PORT_PIN_MODE_I2C)
    {
    CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , Port_PortChannels[i].channel_number);      /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
    SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , Port_PortChannels[i].channel_number);             /* Enable Alternative function for this pin by setting the corresponding bit in GPIOAFSEL register */
    *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |= (~(0x0000000F << (Port_PortChannels[i].channel_number * 4)) | (Port_PortChannels[i].Mode));     /* Set the PMCx bits for this pin depending on the mode */
    SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , Port_PortChannels[i].channel_number);        /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
    }
    else if(Port_PortChannels[i].Mode == PORT_PIN_MODE_M0PWM)
    {
    CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , Port_PortChannels[i].channel_number);      /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
    SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , Port_PortChannels[i].channel_number);             /* Enable Alternative function for this pin by setting the corresponding bit in GPIOAFSEL register */
    *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |= (~(0x0000000F << (Port_PortChannels[i].channel_number * 4)) | (Port_PortChannels[i].Mode));     /* Set the PMCx bits for this pin depending on the mode */
    SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , Port_PortChannels[i].channel_number);        /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
    }
    else if(Port_PortChannels[i].Mode == PORT_PIN_MODE_M1PWM)
    {
    CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , Port_PortChannels[i].channel_number);      /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
    SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , Port_PortChannels[i].channel_number);             /* Enable Alternative function for this pin by setting the corresponding bit in GPIOAFSEL register */
    *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |= (~(0x0000000F << (Port_PortChannels[i].channel_number * 4)) | (Port_PortChannels[i].Mode));     /* Set the PMCx bits for this pin depending on the mode */
    SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , Port_PortChannels[i].channel_number);        /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
    }
    else if(Port_PortChannels[i].Mode == PORT_PIN_MODE_CAN)
    {
    CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , Port_PortChannels[i].channel_number);      /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
    SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , Port_PortChannels[i].channel_number);             /* Enable Alternative function for this pin by setting the corresponding bit in GPIOAFSEL register */
    *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |= (~(0x0000000F << (Port_PortChannels[i].channel_number * 4)) | (Port_PortChannels[i].Mode));     /* Set the PMCx bits for this pin depending on the mode */
    SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , Port_PortChannels[i].channel_number);        /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
    }
    else
    {
    CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , Port_PortChannels[i].channel_number);      /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
    CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , Port_PortChannels[i].channel_number);             /* Disable Alternative function for this pin by clear the corresponding bit in GPIOAFSEL register */
    *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) &= ~(0x0000000F << (Port_PortChannels[i].channel_number * 4));     /* Clear the PMCx bits for this pin */
    SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , Port_PortChannels[i].channel_number);        /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
    }
            
            
    if(Port_PortChannels[i].Ch_Dir == PORT_PIN_OUT)
    {
	SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIR_REG_OFFSET) , Port_PortChannels[i].channel_number);                /* Set the corresponding bit in the GPIODIR register to configure it as output pin */
        
        if(Port_PortChannels[i].InitialValue == PORT_PIN_Level_HIGH)
        {
            SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DATA_REG_OFFSET) , Port_PortChannels[i].channel_number);          /* Set the corresponding bit in the GPIODATA register to provide initial value 1 */
        }
        else
        {
            CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DATA_REG_OFFSET) , Port_PortChannels[i].channel_number);        /* Clear the corresponding bit in the GPIODATA register to provide initial value 0 */
        }
    }
    else if(Port_PortChannels[i].Ch_Dir == PORT_PIN_IN)
    {
        CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIR_REG_OFFSET) , Port_PortChannels[i].channel_number);             /* Clear the corresponding bit in the GPIODIR register to configure it as input pin */
        
        if(Port_PortChannels[i].resistor == PORT_PIN_PULL_UP_RESISTOR)
        {
            SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_PULL_UP_REG_OFFSET) , Port_PortChannels[i].channel_number);       /* Set the corresponding bit in the GPIOPUR register to enable the internal pull up pin */
        }
        else if(Port_PortChannels[i].resistor == PORT_PIN_PULL_DOWN_RESISTOR)
        {
            SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_PULL_DOWN_REG_OFFSET) , Port_PortChannels[i].channel_number);     /* Set the corresponding bit in the GPIOPDR register to enable the internal pull down pin */
        }
        else
        {
            CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_PULL_UP_REG_OFFSET) , Port_PortChannels[i].channel_number);     /* Clear the corresponding bit in the GPIOPUR register to disable the internal pull up pin */
            CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_PULL_DOWN_REG_OFFSET) , Port_PortChannels[i].channel_number);   /* Clear the corresponding bit in the GPIOPDR register to disable the internal pull down pin */
        }
    }
    else
    {
        /* Do Nothing */
    }
    
    
  }
 }
}


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
#if (PORT_SET_PIN_DIRECTION_API == STD_ON)
void Port_SetPinDirection(Port_PinType Pin,Port_PinDirectionType Direction)
{
        
    volatile uint32 * PortGpio_Ptr = NULL_PTR; /* point to the required Port Registers base address */
    volatile uint32 delay = 0;
    
    switch(Port_PortChannels[Pin].port_number)
    {
        case  0: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTA_BASE_ADDRESS; /* PORTA Base Address */
		 break;
	case  1: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTB_BASE_ADDRESS; /* PORTB Base Address */
		 break;
	case  2: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTC_BASE_ADDRESS; /* PORTC Base Address */
		 break;
	case  3: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTD_BASE_ADDRESS; /* PORTD Base Address */
		 break;
        case  4: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTE_BASE_ADDRESS; /* PORTE Base Address */
		 break;
        case  5: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTF_BASE_ADDRESS; /* PORTF Base Address */
		 break;
    }
  if(Direction == PORT_PIN_OUT)
  {
    SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIR_REG_OFFSET) , Port_PortChannels[Pin].channel_number);    /* Set the corresponding bit in the GPIODIR register to configure it as output pin */
  }
  else
  {
    CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIR_REG_OFFSET) , Port_PortChannels[Pin].channel_number);   /* Clear the corresponding bit in the GPIODIR register to configure it as input pin */
  }
}
#endif /* (PORT_SET_PIN_DIRECTION_API == STD_ON) */



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
void Port_RefreshPortDirection(void)
{

        uint8 i;
  for(i = 0; i < PORT_CONFIGURED_CHANNLES; i++)
  {
    if( (i > 15) && (i < 20) )
    {
      /* Do Nothing ...  this is the JTAG pins */
    }
    else
    {
      if( (Port_PortChannels[i].DirectionChangeability) == PORT_PIN_DIRECTION_CHANGEABLE)
      {
        /* Do Nothing  */
      }
      else
      {
      volatile uint32 * PortGpio_Ptr = NULL_PTR; /* point to the required Port Registers base address */
      volatile uint32 delay = 0;
    
        switch(Port_PortChannels[i].port_number)
        {
          case  0: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTA_BASE_ADDRESS; /* PORTA Base Address */
	  	 break;
	  case  1: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTB_BASE_ADDRESS; /* PORTB Base Address */
	    	 break;
      	  case  2: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTC_BASE_ADDRESS; /* PORTC Base Address */
		 break;
	  case  3: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTD_BASE_ADDRESS; /* PORTD Base Address */
		 break;
          case  4: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTE_BASE_ADDRESS; /* PORTE Base Address */
		 break;
          case  5: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTF_BASE_ADDRESS; /* PORTF Base Address */
		 break;
        }
        if((Port_PortChannels[i].Ch_Dir) == PORT_PIN_OUT)
        {
          SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIR_REG_OFFSET) , Port_PortChannels[i].channel_number);    /* Set the corresponding bit in the GPIODIR register to configure it as output pin */
        }
        else
        {
          CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIR_REG_OFFSET) , Port_PortChannels[i].channel_number);    /* Clear the corresponding bit in the GPIODIR register to configure it as Input pin */
        }
        
      }
    
    }
        
  }
}




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
#if (PORT_SET_PIN_MODE_API == STD_ON)
void Port_SetPinMode(Port_PinType Pin,Port_PinModeType Mode)
{
        
        if((Pin > 15) && (Pin < 20))
        {
          /* Do Nothing ...  this is the JTAG pins */
        }
        else
        {
        
          volatile uint32 * PortGpio_Ptr = NULL_PTR; /* point to the required Port Registers base address */
          volatile uint32 delay = 0;
    
          switch(Port_PortChannels[Pin].port_number)
          {
             case  0: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTA_BASE_ADDRESS; /* PORTA Base Address */
		 break;
	     case  1: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTB_BASE_ADDRESS; /* PORTB Base Address */
		 break;
	     case  2: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTC_BASE_ADDRESS; /* PORTC Base Address */
		 break;
	     case  3: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTD_BASE_ADDRESS; /* PORTD Base Address */
		 break;
             case  4: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTE_BASE_ADDRESS; /* PORTE Base Address */
		 break;
             case  5: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTF_BASE_ADDRESS; /* PORTF Base Address */
		 break;
         }
         if(Mode == PORT_PIN_MODE_DIO)
         {
           CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , Port_PortChannels[Pin].channel_number);      /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
           CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , Port_PortChannels[Pin].channel_number);             /* Disable Alternative function for this pin by clear the corresponding bit in GPIOAFSEL register */
           *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) &= ~(0x0000000F << (Port_PortChannels[Pin].channel_number * 4));     /* Clear the PMCx bits for this pin */
           SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , Port_PortChannels[Pin].channel_number);        /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
         }
         else if(Mode == PORT_PIN_MODE_ADC)
         {
           SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , Port_PortChannels[Pin].channel_number);      /* Set the corresponding bit in the GPIOAMSEL register to Enable analog functionality on this pin */
           CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , Port_PortChannels[Pin].channel_number);             /* Disable Alternative function for this pin by clear the corresponding bit in GPIOAFSEL register */
           *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) &= ~(0x0000000F << (Port_PortChannels[Pin].channel_number * 4));     /* Clear the PMCx bits for this pin */
           CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , Port_PortChannels[Pin].channel_number);        /* Clear the corresponding bit in the GPIODEN register to disable digital functionality on this pin */
         }
         else if(Mode == PORT_PIN_MODE_UART)
         {
          CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , Port_PortChannels[Pin].channel_number);      /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
          SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , Port_PortChannels[Pin].channel_number);             /* Enable Alternative function for this pin by setting the corresponding bit in GPIOAFSEL register */
          *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) &= ~(0x0000000F << (Port_PortChannels[Pin].channel_number * 4));     /* Clear the PMCx bits for this pin */
          *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |= (~(0x0000000F << (Port_PortChannels[Pin].channel_number * 4)) | Mode);     /* Set the PMCx bits for this pin depending on the mode */
          SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , Port_PortChannels[Pin].channel_number);        /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
         }
         else if(Mode == PORT_PIN_MODE_SSI)
         {
          CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , Port_PortChannels[Pin].channel_number);      /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
          SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , Port_PortChannels[Pin].channel_number);             /* Enable Alternative function for this pin by setting the corresponding bit in GPIOAFSEL register */
          *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) &= ~(0x0000000F << (Port_PortChannels[Pin].channel_number * 4));     /* Clear the PMCx bits for this pin */
          *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |= (~(0x0000000F << (Port_PortChannels[Pin].channel_number * 4)) | Mode);     /* Set the PMCx bits for this pin depending on the mode */
          SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , Port_PortChannels[Pin].channel_number);        /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
         }
         else if(Mode == PORT_PIN_MODE_I2C)
         {
          CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , Port_PortChannels[Pin].channel_number);      /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
          SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , Port_PortChannels[Pin].channel_number);             /* Enable Alternative function for this pin by setting the corresponding bit in GPIOAFSEL register */
          *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) &= ~(0x0000000F << (Port_PortChannels[Pin].channel_number * 4));     /* Clear the PMCx bits for this pin */
          *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |= (~(0x0000000F << (Port_PortChannels[Pin].channel_number * 4)) | Mode);     /* Set the PMCx bits for this pin depending on the mode */
          SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , Port_PortChannels[Pin].channel_number);        /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
         }
         else if(Mode == PORT_PIN_MODE_M0PWM)
         {
          CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , Port_PortChannels[Pin].channel_number);      /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
          SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , Port_PortChannels[Pin].channel_number);             /* Enable Alternative function for this pin by setting the corresponding bit in GPIOAFSEL register */
          *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) &= ~(0x0000000F << (Port_PortChannels[Pin].channel_number * 4));     /* Clear the PMCx bits for this pin */
          *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |= (~(0x0000000F << (Port_PortChannels[Pin].channel_number * 4)) | Mode);     /* Set the PMCx bits for this pin depending on the mode */
          SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , Port_PortChannels[Pin].channel_number);        /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
         }
         else if(Mode == PORT_PIN_MODE_M1PWM)
         {
          CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , Port_PortChannels[Pin].channel_number);      /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
          SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , Port_PortChannels[Pin].channel_number);             /* Enable Alternative function for this pin by setting the corresponding bit in GPIOAFSEL register */
          *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) &= ~(0x0000000F << (Port_PortChannels[Pin].channel_number * 4));     /* Clear the PMCx bits for this pin */
          *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |= (~(0x0000000F << (Port_PortChannels[Pin].channel_number * 4)) | Mode);     /* Set the PMCx bits for this pin depending on the mode */
          SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , Port_PortChannels[Pin].channel_number);        /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
         }
         else if(Mode == PORT_PIN_MODE_CAN)
         {
          CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , Port_PortChannels[Pin].channel_number);      /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
          SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , Port_PortChannels[Pin].channel_number);             /* Enable Alternative function for this pin by setting the corresponding bit in GPIOAFSEL register */
          *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) &= ~(0x0000000F << (Port_PortChannels[Pin].channel_number * 4));     /* Clear the PMCx bits for this pin */
          *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |= (~(0x0000000F << (Port_PortChannels[Pin].channel_number * 4)) | Mode);     /* Set the PMCx bits for this pin depending on the mode */
          SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , Port_PortChannels[Pin].channel_number);        /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
         }
         else
         {
          CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , Port_PortChannels[Pin].channel_number);      /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
          CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , Port_PortChannels[Pin].channel_number);             /* Disable Alternative function for this pin by clear the corresponding bit in GPIOAFSEL register */
          *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) &= ~(0x0000000F << (Port_PortChannels[Pin].channel_number * 4));     /* Clear the PMCx bits for this pin */
          SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , Port_PortChannels[Pin].channel_number);        /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
         }
         
         
       }
    
  
}
#endif /* (PORT_SET_PIN_MODE_API == STD_ON) */



