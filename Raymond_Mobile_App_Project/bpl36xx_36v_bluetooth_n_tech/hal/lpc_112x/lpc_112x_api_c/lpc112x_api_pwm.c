/*****************************************************************************
Function Description :

Timer                            Pin

16-bit Timer0_Channel0    --     P0-8 & P3-0
16-bit Timer0_Channel1    --     P0-9
16-bit Timer0_Channel2    --     P0-10
16-bit Timer1_Channel0    --     P1-9
16-bit Timer1_Channel1    --     P2-4 & P1-10
32-bit Timer0_Channel0    --     P2-5 & P1-6
32-bit Timer0_Channel1    --     P2-6 & P1-7
32-bit Timer0_Channel2    --     P2-7 & P0-1
32-bit Timer1_Channel0    --     P1-1
32-bit Timer1_Channel1    --     P1-2
32-bit Timer1_Channel2    --     P1-3

*****************************************************************************/

#include "chip.h"
#include "string.h"
#include <stdio.h>
#include "types.h"
#include "lpc112x_api.h"

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/
LPC_TIMER_T *GetCurrentTMR;
int8_t matchnum;
/*****************************************************************************
 * Public functions
 ****************************************************************************/

/* Enable the PWM Function */
STATIC INLINE void Chip_TIMER_PWMEnable(LPC_TIMER_T *pTMR, int8_t pwmnum)
{
	pTMR->PWMC |= 1<< pwmnum;
}

void Chip_TIMER_REG_RST(LPC_TIMER_T *pTMR)
{
	pTMR->CCR = 0;
	pTMR->CTCR = 0;
	pTMR->EMR = 0;
	pTMR->IR = 0;
	pTMR->MCR = 0;
	pTMR->MR[0] = 0;
	pTMR->MR[1] = 0;
	pTMR->MR[2] = 0;
	pTMR->MR[3] = 0;
	pTMR->PC = 0;
	pTMR->PR = 0;
	pTMR->PWMC = 0;
	pTMR->TC = 0;
	pTMR->TCR = 0;
}


/**
 * @brief	PWM configuration
 * @param	pwm_no: The selected PWM channel
 * @param	frq: PWM frequency values
 * @param	gpio_no: pwm_no use gpio_no as output pin
 * @return	Nothing
 */
void pwm_config(pwm_no_t pwm_no, uint32 frq ,gpio_no_t gpio_no) //frq: 1Hz
{
	uint32_t timerFreq;
	
	if(gpio_no == gpio_0_08 || gpio_no == gpio_3_00)
	{
		Chip_TIMER_REG_RST(LPC_TIMER16_0);
		Chip_TIMER_Init(LPC_TIMER16_0);
		timerFreq = Chip_Clock_GetSystemClockRate();
		Chip_Clock_EnablePeriphClock(SYSCON_CLOCK_GPIO);
		if(gpio_no == gpio_0_08){
			Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO0_8,  (IOCON_FUNC2 | IOCON_MODE_PULLUP));
			Chip_GPIO_SetPinDIROutput(LPC_GPIO, 0, 8);
		}else if(gpio_no == gpio_3_00){
			Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO3_0,  (IOCON_FUNC2 | IOCON_MODE_PULLUP));
			Chip_GPIO_SetPinDIROutput(LPC_GPIO, 3, 0);
		}
		Chip_TIMER_Reset(LPC_TIMER16_0);
		Chip_TIMER_SetMatch(LPC_TIMER16_0, 3, (timerFreq / frq));
		Chip_TIMER_ResetOnMatchEnable(LPC_TIMER16_0, 3);
		Chip_TIMER_PWMEnable(LPC_TIMER16_0,0);
		Chip_TIMER_MatchEnableInt(LPC_TIMER16_0, 0);
		GetCurrentTMR = LPC_TIMER16_0;
		matchnum = 0;
	}else if(gpio_no == gpio_0_09)
	{
		Chip_TIMER_REG_RST(LPC_TIMER16_0);
		Chip_TIMER_Init(LPC_TIMER16_0);
		timerFreq = Chip_Clock_GetSystemClockRate();
		Chip_Clock_EnablePeriphClock(SYSCON_CLOCK_GPIO);
		Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO0_9,  (IOCON_FUNC2 | IOCON_MODE_PULLUP));
		Chip_GPIO_SetPinDIROutput(LPC_GPIO, 0, 9);
		Chip_TIMER_Reset(LPC_TIMER16_0);
		Chip_TIMER_MatchEnableInt(LPC_TIMER16_0, 1);
		Chip_TIMER_ResetOnMatchEnable(LPC_TIMER16_0, 3);
		Chip_TIMER_PWMEnable(LPC_TIMER16_0,1);
		Chip_TIMER_SetMatch(LPC_TIMER16_0, 3, (timerFreq / frq));
		GetCurrentTMR = LPC_TIMER16_0;
		matchnum = 1;
	}else if(gpio_no == gpio_0_10)
	{
		Chip_TIMER_REG_RST(LPC_TIMER16_0);
		Chip_TIMER_Init(LPC_TIMER16_0);
		timerFreq = Chip_Clock_GetSystemClockRate();
		Chip_Clock_EnablePeriphClock(SYSCON_CLOCK_GPIO);
		Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO0_10,  (IOCON_FUNC3 | IOCON_MODE_PULLUP));
		Chip_GPIO_SetPinDIROutput(LPC_GPIO, 0, 10);
		Chip_TIMER_Reset(LPC_TIMER16_0);
		Chip_TIMER_MatchEnableInt(LPC_TIMER16_0, 2);
		Chip_TIMER_ResetOnMatchEnable(LPC_TIMER16_0, 3);
		Chip_TIMER_PWMEnable(LPC_TIMER16_0,2);
		Chip_TIMER_SetMatch(LPC_TIMER16_0, 3, (timerFreq / frq));
		GetCurrentTMR = LPC_TIMER16_0;
		matchnum = 2;
	}else if(gpio_no == gpio_1_09)
	{
		Chip_TIMER_REG_RST(LPC_TIMER16_1);
		Chip_TIMER_Init(LPC_TIMER16_1);
		timerFreq = Chip_Clock_GetSystemClockRate();
		Chip_Clock_EnablePeriphClock(SYSCON_CLOCK_GPIO);
		Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO1_9,  (IOCON_FUNC1 | IOCON_MODE_PULLUP));
		Chip_GPIO_SetPinDIROutput(LPC_GPIO, 1, 9);
		Chip_TIMER_Reset(LPC_TIMER16_1);
		Chip_TIMER_SetMatch(LPC_TIMER16_1, 3, (timerFreq / frq));
		Chip_TIMER_ResetOnMatchEnable(LPC_TIMER16_1, 3);
		Chip_TIMER_PWMEnable(LPC_TIMER16_1,0);
		Chip_TIMER_MatchEnableInt(LPC_TIMER16_1, 0);	
		GetCurrentTMR = LPC_TIMER16_1;
		matchnum = 0;
	}else if(gpio_no == gpio_2_04 || gpio_no == gpio_1_10)
	{
		Chip_TIMER_REG_RST(LPC_TIMER16_1);
		Chip_TIMER_Init(LPC_TIMER16_1);
		timerFreq = Chip_Clock_GetSystemClockRate();
		Chip_Clock_EnablePeriphClock(SYSCON_CLOCK_GPIO);
		if(gpio_no == gpio_2_04){
			Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO2_4,  (IOCON_FUNC1 | IOCON_MODE_PULLUP));
			Chip_GPIO_SetPinDIROutput(LPC_GPIO, 2, 4);
		}else if(gpio_no == gpio_1_10){
			Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO1_10,  (IOCON_FUNC2 | IOCON_MODE_PULLUP));
			Chip_GPIO_SetPinDIROutput(LPC_GPIO, 1, 10);
		}
		Chip_TIMER_Reset(LPC_TIMER16_1);
		Chip_TIMER_SetMatch(LPC_TIMER16_1, 3, (timerFreq / frq));
		Chip_TIMER_ResetOnMatchEnable(LPC_TIMER16_1, 3);
		Chip_TIMER_PWMEnable(LPC_TIMER16_1,1);
		Chip_TIMER_MatchEnableInt(LPC_TIMER16_1, 1);		
		GetCurrentTMR = LPC_TIMER16_1;
		matchnum = 1;
	}else if(gpio_no == gpio_2_05 || gpio_no == gpio_1_06)
	{
		Chip_TIMER_REG_RST(LPC_TIMER32_0);
		Chip_TIMER_Init(LPC_TIMER32_0);
		timerFreq = Chip_Clock_GetSystemClockRate();
		Chip_Clock_EnablePeriphClock(SYSCON_CLOCK_GPIO);
		if(gpio_no == gpio_2_05){
			Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO2_5,  (IOCON_FUNC1 | IOCON_MODE_PULLUP));
			Chip_GPIO_SetPinDIROutput(LPC_GPIO, 2, 5);
		}else if(gpio_no == gpio_1_06){
			Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO1_6,  (IOCON_FUNC2 | IOCON_MODE_PULLUP));
			Chip_GPIO_SetPinDIROutput(LPC_GPIO, 1, 6);
		}
		Chip_TIMER_Reset(LPC_TIMER32_0);
		Chip_TIMER_SetMatch(LPC_TIMER32_0, 3, (timerFreq / frq));
		Chip_TIMER_ResetOnMatchEnable(LPC_TIMER32_0, 3);
		Chip_TIMER_PWMEnable(LPC_TIMER32_0,0);
		Chip_TIMER_MatchEnableInt(LPC_TIMER32_0, 0);	
		GetCurrentTMR = LPC_TIMER32_0;
		matchnum = 0;
	}else if(gpio_no == gpio_2_06 || gpio_no == gpio_1_07)
	{
		Chip_TIMER_REG_RST(LPC_TIMER32_0);
		Chip_TIMER_Init(LPC_TIMER32_0);
		timerFreq = Chip_Clock_GetSystemClockRate();
		Chip_Clock_EnablePeriphClock(SYSCON_CLOCK_GPIO);
		if(gpio_no == gpio_2_06){
			Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO2_6,  (IOCON_FUNC1 | IOCON_MODE_PULLUP));
			Chip_GPIO_SetPinDIROutput(LPC_GPIO, 2, 6);
		}else if(gpio_no == gpio_1_07){
			Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO1_7,  (IOCON_FUNC2 | IOCON_MODE_PULLUP));
			Chip_GPIO_SetPinDIROutput(LPC_GPIO, 1, 7);
		}
		Chip_TIMER_Reset(LPC_TIMER32_0);
		Chip_TIMER_SetMatch(LPC_TIMER32_0, 3, (timerFreq / frq));
		Chip_TIMER_ResetOnMatchEnable(LPC_TIMER32_0, 3);
		Chip_TIMER_PWMEnable(LPC_TIMER32_0,1);
		Chip_TIMER_MatchEnableInt(LPC_TIMER32_0, 1);
		GetCurrentTMR = LPC_TIMER32_0;
		matchnum = 1;
	}else if(gpio_no == gpio_0_01 || gpio_no == gpio_2_07)
	{
		Chip_TIMER_REG_RST(LPC_TIMER32_0);
		Chip_TIMER_Init(LPC_TIMER32_0);
		timerFreq = Chip_Clock_GetSystemClockRate();
		Chip_Clock_EnablePeriphClock(SYSCON_CLOCK_GPIO);
		if(gpio_no == gpio_2_07){
			Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO2_7,  (IOCON_FUNC1 | IOCON_MODE_PULLUP));
			Chip_GPIO_SetPinDIROutput(LPC_GPIO, 2, 7);
		}else if(gpio_no == gpio_0_01){
			Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO0_1,  (IOCON_FUNC2 | IOCON_MODE_PULLUP));
			Chip_GPIO_SetPinDIROutput(LPC_GPIO, 0, 1);
		}
		Chip_TIMER_Reset(LPC_TIMER32_0);
		Chip_TIMER_SetMatch(LPC_TIMER32_0, 3, (timerFreq / frq));
		Chip_TIMER_ResetOnMatchEnable(LPC_TIMER32_0, 3);
		Chip_TIMER_PWMEnable(LPC_TIMER32_0,2);
		Chip_TIMER_MatchEnableInt(LPC_TIMER32_0, 2);
		GetCurrentTMR = LPC_TIMER32_0;
		matchnum = 2;
	}else if(gpio_no == gpio_1_01)
	{
		Chip_TIMER_REG_RST(LPC_TIMER32_1);
		Chip_TIMER_Init(LPC_TIMER32_1);
		timerFreq = Chip_Clock_GetSystemClockRate();
		Chip_Clock_EnablePeriphClock(SYSCON_CLOCK_GPIO);
		Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO1_1,  (IOCON_FUNC3 | IOCON_MODE_PULLUP));
		Chip_GPIO_SetPinDIROutput(LPC_GPIO, 1, 1);
		Chip_TIMER_Reset(LPC_TIMER32_1);
		Chip_TIMER_MatchEnableInt(LPC_TIMER32_1, 0);
		Chip_TIMER_ResetOnMatchEnable(LPC_TIMER32_1, 3);
		Chip_TIMER_PWMEnable(LPC_TIMER32_1,0);
		Chip_TIMER_SetMatch(LPC_TIMER32_1, 3, (timerFreq / frq));
		GetCurrentTMR = LPC_TIMER32_1;
		matchnum = 0;
	}else if(gpio_no == gpio_1_02)
	{
		Chip_TIMER_REG_RST(LPC_TIMER32_1);
		Chip_TIMER_Init(LPC_TIMER32_1);
		timerFreq = Chip_Clock_GetSystemClockRate();
		Chip_Clock_EnablePeriphClock(SYSCON_CLOCK_GPIO);
		Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO1_2,  (IOCON_FUNC3 | IOCON_MODE_PULLUP));
		Chip_GPIO_SetPinDIROutput(LPC_GPIO, 1, 2);
		Chip_TIMER_Reset(LPC_TIMER32_1);
		Chip_TIMER_MatchEnableInt(LPC_TIMER32_1, 1);
		Chip_TIMER_ResetOnMatchEnable(LPC_TIMER32_1, 3);
		Chip_TIMER_PWMEnable(LPC_TIMER32_1,1);
		Chip_TIMER_SetMatch(LPC_TIMER32_1, 3, (timerFreq / frq));
		GetCurrentTMR = LPC_TIMER32_1;
		matchnum = 1;
	}else if(gpio_no == gpio_1_03)
	{
		Chip_TIMER_REG_RST(LPC_TIMER32_1);
		Chip_TIMER_Init(LPC_TIMER32_1);
		timerFreq = Chip_Clock_GetSystemClockRate();
		Chip_Clock_EnablePeriphClock(SYSCON_CLOCK_GPIO);
		Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO1_3,  (IOCON_FUNC3 | IOCON_MODE_PULLUP));
		Chip_GPIO_SetPinDIROutput(LPC_GPIO, 1, 3);
		Chip_TIMER_Reset(LPC_TIMER32_1);
		Chip_TIMER_MatchEnableInt(LPC_TIMER32_1, 2);
		Chip_TIMER_ResetOnMatchEnable(LPC_TIMER32_1, 3);
		Chip_TIMER_PWMEnable(LPC_TIMER32_1,2);
		Chip_TIMER_SetMatch(LPC_TIMER32_1, 3, (timerFreq / frq));
		GetCurrentTMR = LPC_TIMER32_1;
		matchnum = 2;
	}
	
}

/**
 * @brief	PWM channel enable/disable
 * @param	pwm_no	: The PWM channels
 * @param	enable	: true for enable, false for disable
 * @return	Nothing
 */
void pwm_enable(pwm_no_t pwm_no,  uint8 enable)
{
	if ( enable )
	{	
		Chip_TIMER_Enable(LPC_TIMER16_0);
		Chip_TIMER_Enable(LPC_TIMER16_1);
		Chip_TIMER_Enable(LPC_TIMER32_0);
		Chip_TIMER_Enable(LPC_TIMER32_1);
	}
	else
	{
		Chip_TIMER_Disable(LPC_TIMER16_0);
		Chip_TIMER_Disable(LPC_TIMER16_1);
		Chip_TIMER_Disable(LPC_TIMER32_0);
		Chip_TIMER_Disable(LPC_TIMER32_1);
	}
	
}



/* Get the Value of Match Register */
STATIC INLINE uint32_t Chip_TIMER_GetMatch(LPC_TIMER_T *pTMR, int8_t matchnum)
{
	return pTMR->MR[matchnum];
}


/**
 * @brief	Converts a percentage to ticks
 * @param	pwm_no	: The PWM channel
 * @param	duty	: Percentage to convert (0 - 1000)
 * @return	nothing
 * @note  Example: setting duty=438, pwm_output_duty=43.8%
 */
void pwm_set_duty(pwm_no_t pwm_no, uint16 duty)
{
	uint32 frq;
	duty = 1000 - duty;
	frq = Chip_TIMER_GetMatch(GetCurrentTMR, 3);
	Chip_TIMER_SetMatch(GetCurrentTMR, matchnum, (frq * duty / 1000));
}
