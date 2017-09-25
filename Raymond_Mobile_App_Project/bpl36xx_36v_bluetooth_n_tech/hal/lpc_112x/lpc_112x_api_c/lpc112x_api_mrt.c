#include "chip.h"
#include "string.h"
#include <stdio.h>
#include "types.h"
#include "lpc112x_api.h"

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/
static void (*pFunc_MRT[4]) (uint8 para) = {NULL,NULL,NULL,NULL};

#define TICKRATE_HZ (10)/* 10 ticks per second */
uint32_t timer16_0_count = 0;
uint32_t timer16_1_count = 0;
uint32_t temp_interval_16B0 = 0;
uint32_t temp_interval_16B1 = 0;
/*****************************************************************************
 * Public functions
 ****************************************************************************/

/** \brief  MRT timer_no configuration
 *  \param1 time_no: MRT channels.
 *  \param2 interval: MRT interrupt interval_when set interval as "1", means timer interrupt 10us_100KHz".
 */
void timer_config(timer_no_t time_no, uint32 interval)
{
	//LPC_MRT_CH_T *pMRT;
	LPC_TIMER_T *pTMR;
	uint32_t timerFreq;
	switch (time_no)
	{
		case timer0 : pTMR = LPC_TIMER16_0 ;  break;										
		case timer1 : pTMR = LPC_TIMER16_1 ;  break;				
		case timer2 :	pTMR = LPC_TIMER32_0 ;  break;					
		case timer3 : pTMR = LPC_TIMER32_1 ;  break;		
		default :
			break;			
	}
	Chip_TIMER_Init(pTMR);       // After initial£¬ timer clock is 48M 
	
	/* Timer rate is system clock rate */
	timerFreq = Chip_Clock_GetSystemClockRate();
	Chip_TIMER_Reset(pTMR);
	Chip_TIMER_MatchEnableInt(pTMR, 1);
	if((pTMR == LPC_TIMER32_0)||(pTMR == LPC_TIMER32_1))
	{
		Chip_TIMER_SetMatch(pTMR, 1, (timerFreq / 100000)*interval);	//(timerFreq / 100000) is 10uS,interval value should not bigger than 100000	
	}
	else if( pTMR == LPC_TIMER16_0 )
	{				
			temp_interval_16B0 = interval;
			Chip_TIMER_SetMatch(pTMR, 1, (timerFreq / 100000));		//10us					
	}
	else if( pTMR == LPC_TIMER16_1 )
	{				
			temp_interval_16B1 = interval;
			Chip_TIMER_SetMatch(pTMR, 1, (timerFreq / 100000));		//10us					
	}
	
}

/** \brief  MRT timer_no enable/disable
    \param1 time_no: MRT channel.
    \param2 enable: true for enable, false for disable.
*/
void timer_enable(timer_no_t time_no, uint8 enable)
{
	//LPC_MRT_CH_T *pMRT;
	LPC_TIMER_T *pTMR;
	IRQn_Type  TIMER_IRQn;
	switch (time_no)
		{
			case timer0 : pTMR = LPC_TIMER16_0 ; TIMER_IRQn = TIMER_16_0_IRQn ; break;									
			case timer1 : pTMR = LPC_TIMER16_1 ; TIMER_IRQn = TIMER_16_1_IRQn ; break;	
			case timer2 : pTMR = LPC_TIMER32_0 ; TIMER_IRQn = TIMER_32_0_IRQn ; break;		
			case timer3 : pTMR = LPC_TIMER32_1 ; TIMER_IRQn = TIMER_32_1_IRQn ; break;	
		default :
			  break;			
		}
	if ( enable )
	{		
		Chip_TIMER_ResetOnMatchEnable(pTMR, 1);
		Chip_TIMER_Enable(pTMR);
		
		/* Enable timer interrupt */
		NVIC_ClearPendingIRQ(TIMER_IRQn);
		NVIC_EnableIRQ(TIMER_IRQn);
	}
	else
	{
		Chip_TIMER_Disable ( pTMR ) ;
	}
}


void TIMER16_0_IRQHandler(void)
{
	if (Chip_TIMER_MatchPending(LPC_TIMER16_0, 1)) {
		Chip_TIMER_ClearMatch(LPC_TIMER16_0, 1);
		timer16_0_count ++;
		if(pFunc_MRT[0] != NULL)
		{
				if(timer16_0_count >= (temp_interval_16B0))
				{	
					timer16_0_count = 0;
					pFunc_MRT[0](0);
				}			
		}
	}
}

void TIMER16_1_IRQHandler(void)
{
	if (Chip_TIMER_MatchPending(LPC_TIMER16_1, 1)) {
		Chip_TIMER_ClearMatch(LPC_TIMER16_1, 1);
		timer16_1_count ++;
		if(pFunc_MRT[1] != NULL)
		{
				if(timer16_1_count >= (temp_interval_16B1))
				{					
					timer16_1_count = 0;
					pFunc_MRT[1](0);
				}
			
		}
	}
}
void TIMER32_0_IRQHandler(void)
{
	if (Chip_TIMER_MatchPending(LPC_TIMER32_0, 1)) {
		Chip_TIMER_ClearMatch(LPC_TIMER32_0, 1);
		if(pFunc_MRT[2] != NULL)
						{
							pFunc_MRT[2](0);
						}
	}
}
void TIMER32_1_IRQHandler(void)
{
	if (Chip_TIMER_MatchPending(LPC_TIMER32_1, 1)) {
		Chip_TIMER_ClearMatch(LPC_TIMER32_1, 1);
		if(pFunc_MRT[3] != NULL)
						{
							pFunc_MRT[3](0);
						}
	}
}



/**
 * @brief	MRT interrupt callback register
 * @return	Nothing
 */
void timer_isr_callback_register(timer_no_t time_no, p_handler timer_isr_user) //Set TIMER ISR user function pointer, timer_isr_user: TIEMR ISR calls this function
{
		pFunc_MRT[time_no] = timer_isr_user ;
}
