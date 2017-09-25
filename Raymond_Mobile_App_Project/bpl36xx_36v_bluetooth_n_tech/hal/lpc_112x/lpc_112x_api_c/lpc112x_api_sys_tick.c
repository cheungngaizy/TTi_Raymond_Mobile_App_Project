#include "chip.h"
#include "string.h"
#include <stdio.h>
#include "types.h"
#include "lpc112x_api.h"

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/
static void (*pFunc_sys_tick) (uint8 para) = {NULL};

/*****************************************************************************
 * Public functions
 ****************************************************************************/

/**
 * @brief	system tick configuration
 * @param	interval: system tick interval, when set 1, means 100us interval.
 * @return Nothing
 * @note:
 */
void sys_tick_config(uint32 interval)
{
	uint32 sys_clk;
	/* Get CPU core clock */
	sys_clk = Chip_Clock_GetSystemClockRate() / 10000;
	SysTick_Config ( sys_clk * interval );
}


/**
 * @brief	system tick interrupt callback register
 * @param	sys_tick_isr_user: user customize 
 * @return	Nothing
 * @note: Set SYSTEM TICK ISR user function pointer, sys_tick_isr_user: SYSTEM TICK ISR calls this function
 */
void sys_tick_isr_callback_register(p_handler sys_tick_isr_user)
{
	pFunc_sys_tick = sys_tick_isr_user ;
}


/**
 * @brief	Handle interrupt from SysTick function, delivery to pFunc()
 * @return	Nothing
 */
void SysTick_Handler(void)
{
		//handle pFunc instead
	if(pFunc_sys_tick != NULL)
		{
			pFunc_sys_tick(0);
		}	
}
