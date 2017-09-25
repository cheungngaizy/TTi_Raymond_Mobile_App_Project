#include "chip.h"
#include "string.h"
#include <stdio.h>
#include "types.h"
#include "lpc112x_api.h"


/* Pin Mode mask */
#define PIN_FUNC_MASK           (0x7 <<  0)
#define PIN_FUNC_BITNUM         (0)

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/
static void (*pFunc_PIN_INT[40]) (uint8 para)={NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,
                                               NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,
                                               NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,
                                               NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL};
uint8_t GPIO_port;
uint32_t pinMask;
uint16 IRQ_flag;
extern unsigned int iocon_pin_val[];

/*****************************************************************************
 * Public functions
 ****************************************************************************/

/**
 * @brief	PIN_INT configuration
 * @param	*p_pin_int	: Set PinChannel, InterruptMode(Level | Edge)
 * @param	gpio_int	: The matching pin_int_n to handler with
 * @return	Nothing
 * @note	Enable the IRQ_n in NVIC when choosing pin_int_n
 */
void pin_int_config(pin_int_config_t *p_pin_int, gpio_no_t gpio_int)
{	
	uint8_t GPIO_int_pin;
	uint32_t reg, func_value;
	
	/* GPIO clock Enable */
	Chip_Clock_EnablePeriphClock(SYSCON_CLOCK_GPIO);
	reg = LPC_IOCON->REG[(iocon_pin_val[gpio_int])] & ~(PIN_FUNC_MASK);
	switch(gpio_int)
	{
		case gpio_0_00 : func_value = ((IOCON_FUNC1 | IOCON_MODE_PULLUP) << PIN_FUNC_BITNUM); break;
		case gpio_0_10 : func_value = ((IOCON_FUNC1 | IOCON_MODE_PULLUP) << PIN_FUNC_BITNUM); break;
		case gpio_0_11 : func_value = ((IOCON_FUNC1 | IOCON_MODE_PULLUP | IOCON_DIGMODE_EN) << PIN_FUNC_BITNUM); break;		
		case gpio_1_00 : func_value = ((IOCON_FUNC1 | IOCON_MODE_PULLUP | IOCON_DIGMODE_EN) << PIN_FUNC_BITNUM); break;
		case gpio_1_01 : func_value =	((IOCON_FUNC1 | IOCON_MODE_PULLUP | IOCON_DIGMODE_EN) << PIN_FUNC_BITNUM); break;
		case gpio_1_02 : func_value =	((IOCON_FUNC1 | IOCON_MODE_PULLUP | IOCON_DIGMODE_EN) << PIN_FUNC_BITNUM); break;
		case gpio_1_03 : func_value =	((IOCON_FUNC1 | IOCON_MODE_PULLUP | IOCON_DIGMODE_EN) << PIN_FUNC_BITNUM); break;
		default:				 func_value = ((IOCON_FUNC0 | IOCON_MODE_PULLUP | IOCON_DIGMODE_EN) << PIN_FUNC_BITNUM); break;
	}
	LPC_IOCON->REG[(iocon_pin_val[gpio_int])] = ( reg | func_value );
	
	if(gpio_int <= gpio_0_11)
  {
	   GPIO_port = 0;
	   GPIO_int_pin = gpio_int;

	}else if(gpio_int <= gpio_1_11)
	       {
						GPIO_port = 1;
						GPIO_int_pin = gpio_int - gpio_1_00;
	
		     }else if(gpio_int <= gpio_2_09)
				        {
									 GPIO_port = 2;
									 GPIO_int_pin = gpio_int - gpio_2_00;
		            }else {			
													GPIO_port = 3;	
													if(gpio_int == gpio_3_00)	GPIO_int_pin = 0;
													if(gpio_int == gpio_3_03)	GPIO_int_pin = 3;	
													if(gpio_int == gpio_3_04)	GPIO_int_pin = 4;
													if(gpio_int == gpio_3_05)	GPIO_int_pin = 5;
									
							        }
	pinMask = (1 << GPIO_int_pin);
											
	/* Configure GPIO pin as input pin */
  Chip_GPIO_SetPinDIRInput(LPC_GPIO, GPIO_port, GPIO_int_pin);
											
	/* Configure pin interrupt as edge sensitive or level
	 0: Triger PIN INT On Edge, 1: Triger PIN INT On Level */
	if (p_pin_int->trg_edg_lvl){
		
		/* Configure pin interrupt as level sensitive */
		Chip_GPIO_SetPinModeLevel(LPC_GPIO, GPIO_port, pinMask);
		
		 /* 0: Interrupt in Low Levl, 1: Interrupt in High Level */
		if (p_pin_int->trg_even.lvl){	Chip_GPIO_SetModeHigh(LPC_GPIO, GPIO_port, pinMask);}
		else	{Chip_GPIO_SetModeLow(LPC_GPIO, GPIO_port, pinMask);}
	}
	else
	{
		/* Configure pin interrupt as edge sensitive */
		Chip_GPIO_SetPinModeEdge(LPC_GPIO, GPIO_port, pinMask);
		switch (p_pin_int->trg_even.edg)
		{
			case edge_falling:
				Chip_GPIO_SetModeLow(LPC_GPIO, GPIO_port, pinMask);
				break;
			
			case edge_rising:
				Chip_GPIO_SetModeHigh(LPC_GPIO, GPIO_port, pinMask);
				break;
			
			case edge_dual:
				Chip_GPIO_SetModeLow(LPC_GPIO, GPIO_port, pinMask);
				Chip_GPIO_SetModeHigh(LPC_GPIO, GPIO_port, pinMask);
				break;
			
			case edge_max:
				//Reserved
				break;
			
			default :	break;
		}
	}
	
	/* Enable GPIO pin interrupt */
	Chip_GPIO_EnableInt(LPC_GPIO, GPIO_port, pinMask);
		
	/* Enable wakeup for GPIO pin  */
	if ( GPIO_port < 2 ){
		Chip_SYSCON_EnableStartPin(GPIO_port * 12 + GPIO_int_pin);
		Chip_SYSCON_ResetStartPin(GPIO_port * 12 + GPIO_int_pin);
	}
	/* Enable all clocks, even those turned off at wake power up */
	Chip_SYSCON_SetWakeup(~(SYSCON_SLPWAKE_IRCOUT_PD | SYSCON_SLPWAKE_IRC_PD |
							SYSCON_SLPWAKE_FLASH_PD | SYSCON_SLPWAKE_SYSOSC_PD | SYSCON_SLPWAKE_SYSPLL_PD));

}

/**
 * @brief	PININT_n interrupt entrance, divert into pFun_PIN_INT[n]()
 * @return	Nothing
 */
void PIOINT0_IRQHandler(void)
{	
	IRQ_flag = Chip_GPIO_GetMaskedInts(LPC_GPIO, 0);
	switch(IRQ_flag)
	{
		case 1<<0:  if (pFunc_PIN_INT[0] != NULL ){pFunc_PIN_INT[0](0);break;}
		case 1<<1:  if (pFunc_PIN_INT[1] != NULL ){pFunc_PIN_INT[1](0);break;}
		case 1<<2:  if (pFunc_PIN_INT[2] != NULL ){pFunc_PIN_INT[2](0);break;}
		case 1<<3:  if (pFunc_PIN_INT[3] != NULL ){pFunc_PIN_INT[3](0);break;}
		case 1<<4:  if (pFunc_PIN_INT[4] != NULL ){pFunc_PIN_INT[4](0);break;}
		case 1<<5:  if (pFunc_PIN_INT[5] != NULL ){pFunc_PIN_INT[5](0);break;}
		case 1<<6:  if (pFunc_PIN_INT[6] != NULL ){pFunc_PIN_INT[6](0);break;}
		case 1<<7:  if (pFunc_PIN_INT[7] != NULL ){pFunc_PIN_INT[7](0);break;}
		case 1<<8:  if (pFunc_PIN_INT[8] != NULL ){pFunc_PIN_INT[8](0);break;}
		case 1<<9:  if (pFunc_PIN_INT[9] != NULL ){pFunc_PIN_INT[9](0);break;}
		case 1<<10:  if (pFunc_PIN_INT[10] != NULL ){pFunc_PIN_INT[10](0);break;}
		case 1<<11:  if (pFunc_PIN_INT[11] != NULL ){pFunc_PIN_INT[11](0);break;}
		default :	break;
	}
	Chip_GPIO_ClearInts(LPC_GPIO, 0, 0x0FFF);
}

void PIOINT1_IRQHandler(void)
{
	IRQ_flag = Chip_GPIO_GetMaskedInts(LPC_GPIO, 1);
	switch(IRQ_flag)
	{
		case 1<<0:  if (pFunc_PIN_INT[12] != NULL ){pFunc_PIN_INT[12](0);break;}
		case 1<<1:  if (pFunc_PIN_INT[13] != NULL ){pFunc_PIN_INT[13](0);break;}
		case 1<<2:  if (pFunc_PIN_INT[14] != NULL ){pFunc_PIN_INT[14](0);break;}
		case 1<<3:  if (pFunc_PIN_INT[15] != NULL ){pFunc_PIN_INT[15](0);break;}
		case 1<<4:  if (pFunc_PIN_INT[16] != NULL ){pFunc_PIN_INT[16](0);break;}
		case 1<<5:  if (pFunc_PIN_INT[17] != NULL ){pFunc_PIN_INT[17](0);break;}
		case 1<<6:  if (pFunc_PIN_INT[18] != NULL ){pFunc_PIN_INT[18](0);break;}
		case 1<<7:  if (pFunc_PIN_INT[19] != NULL ){pFunc_PIN_INT[19](0);break;}
		case 1<<8:  if (pFunc_PIN_INT[20] != NULL ){pFunc_PIN_INT[20](0);break;}
		case 1<<9:  if (pFunc_PIN_INT[21] != NULL ){pFunc_PIN_INT[21](0);break;}
		case 1<<10:  if (pFunc_PIN_INT[22] != NULL ){pFunc_PIN_INT[22](0);break;}
		case 1<<11:  if (pFunc_PIN_INT[23] != NULL ){pFunc_PIN_INT[23](0);break;}
		default :	break;
	}
	Chip_GPIO_ClearInts(LPC_GPIO, 1, 0x0FFF);
}

void PIOINT2_IRQHandler(void)
{
	IRQ_flag = Chip_GPIO_GetMaskedInts(LPC_GPIO, 2);
	switch(IRQ_flag)
	{
	  case 1<<0:  if (pFunc_PIN_INT[24] != NULL ){pFunc_PIN_INT[24](0);break;}
		case 1<<1:  if (pFunc_PIN_INT[25] != NULL ){pFunc_PIN_INT[25](0);break;}
		case 1<<2:  if (pFunc_PIN_INT[26] != NULL ){pFunc_PIN_INT[26](0);break;}
		case 1<<3:  if (pFunc_PIN_INT[27] != NULL ){pFunc_PIN_INT[27](0);break;}
		case 1<<4:  if (pFunc_PIN_INT[28] != NULL ){pFunc_PIN_INT[28](0);break;}
		case 1<<5:  if (pFunc_PIN_INT[29] != NULL ){pFunc_PIN_INT[29](0);break;}
		case 1<<6:  if (pFunc_PIN_INT[30] != NULL ){pFunc_PIN_INT[30](0);break;}
		case 1<<7:  if (pFunc_PIN_INT[31] != NULL ){pFunc_PIN_INT[31](0);break;}
		case 1<<8:  if (pFunc_PIN_INT[32] != NULL ){pFunc_PIN_INT[32](0);break;}
		case 1<<9:  if (pFunc_PIN_INT[33] != NULL ){pFunc_PIN_INT[33](0);break;}
		default :	break;
	}
	Chip_GPIO_ClearInts(LPC_GPIO, 2, 0x0FFF);

}

void PIOINT3_IRQHandler(void)
{
	IRQ_flag = Chip_GPIO_GetMaskedInts(LPC_GPIO, 3);
	switch(IRQ_flag)
	{
	  case 1<<0:  if (pFunc_PIN_INT[34] != NULL ){pFunc_PIN_INT[34](0);break;}
		case 1<<3:  if (pFunc_PIN_INT[35] != NULL ){pFunc_PIN_INT[35](0);break;}
		case 1<<4:  if (pFunc_PIN_INT[36] != NULL ){pFunc_PIN_INT[36](0);break;}
		case 1<<5:  if (pFunc_PIN_INT[37] != NULL ){pFunc_PIN_INT[37](0);break;}
		default :	break;
	}
	Chip_GPIO_ClearInts(LPC_GPIO, 3, 0x0FFF);
}


/**
 * @brief	PIN_INT_IRQ callback register
 * @param	pin_int_chl	: The pin interrupt channels
 * @param	pin_isr_user	: The matching pin_int_n to handler with
 * @return	Nothing
 * @note	Enable the IRQ_n in NVIC when choosing pin_int_n
 */
void pin_isr_callback_register(pin_int_chl_t pin_int_chl, p_handler pin_isr_user)
{
	pFunc_PIN_INT[pin_int_chl] = pin_isr_user;
}

/**
 * @brief	pin interrupt channel enable/disable
 * @param	pin_int_chl	: The pin interrupt channels
 * @param	enable	: true for enable, false for disable
 * @return	Nothing
 * @note	Using close/open PIN_INT_IRQ_n in NVIC to enable/disable pin_int_n interrupt
 */
void pin_int_enable(pin_int_chl_t pin_int_chl, uint8 enable)
{
	IRQn_Type PININT_NVIC_NAME;
	if ( pin_int_chl <= pin_int11 )	PININT_NVIC_NAME = PIO0_IRQn;
	else if ( pin_int_chl <= pin_int23 ) PININT_NVIC_NAME = PIO1_IRQn;
	else if ( pin_int_chl <= pin_int33 )	PININT_NVIC_NAME = PIO2_IRQn;
	else PININT_NVIC_NAME = PIO3_IRQn;
	
	if (enable)	NVIC_EnableIRQ(PININT_NVIC_NAME);
	else	NVIC_DisableIRQ(PININT_NVIC_NAME);
}
