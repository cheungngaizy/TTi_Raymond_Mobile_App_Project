#include "chip.h"
#include "string.h"
#include <stdio.h>
#include "types.h"
#include "lpc112x_api.h"
/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/
static void (*pFunc_ADC_Ready[9]) (uint8 para) = {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
static void (*pFunc_ADC_Thrd[9]) (uint8 para) = {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
static uint16 g_ADC_set_chl_mask;
static uint16 g_ADC_conv_chl_mask;

/*****************************************************************************
 * Public functions
 ****************************************************************************/

/**
 * @brief	Configure ADC Threshold/Channels/Threshold values
 * @param	adc_thrd_chl : Threshold register channels (0 for threshold register 0, 1 for thrd register 1)
 * @param	adc_chl : Channels of ADC(adc0 for Channel 0, and so on)
 * @param   threshold_low : Threshold low data value (should be 12-bit value)
 * @param   threshold_hi  : Threshold high data value (should be 12-bit value)
 * @return	None
 */
void adc_config(adc_thrd_t adc_thrd_chl, adc_chl_t adc_chl, uint16 threshold_low, uint16 threshold_hi)  //Set ADC settings, threshold_low: Low Limit ADC value, set it 0 to disable, threshold_hi: High Limit ADC value, set it 0xFFF to disable it
{
	Chip_ADC_Init(LPC_ADC, 0);
	Chip_ADC_StartCalibration(LPC_ADC);
	while (!(Chip_ADC_IsCalibrationDone(LPC_ADC))) {}
	Chip_ADC_SetClockRate(LPC_ADC, ADC_MAX_SAMPLE_RATE);

	/* Configure Pins as Fixed Function ADC in the Switch Matrix */
	switch (adc_chl)
	{
		case adc1 :
				Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO1_11, (IOCON_FUNC1 | IOCON_ADMODE_EN));
				break;
		case adc2 :
				Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO1_4,  (IOCON_FUNC1 | IOCON_ADMODE_EN));
				break;
		case adc3 :
				Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO1_3,  (IOCON_FUNC2 | IOCON_ADMODE_EN));
				break;
		case adc4 :
				Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO1_2,  (IOCON_FUNC2 | IOCON_ADMODE_EN));
				break;
		case adc5 :
				Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO1_1,  (IOCON_FUNC2 | IOCON_ADMODE_EN));
				break;
		case adc6 :
				Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO1_0,  (IOCON_FUNC2 | IOCON_ADMODE_EN));
				break;
		case adc7 :
				Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO0_11, (IOCON_FUNC2 | IOCON_ADMODE_EN));
				break;
		case adc8 :
				Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO1_10, (IOCON_FUNC1 | IOCON_ADMODE_EN));
				break;
		case adc_max :
				//Reserved
				break;
		default :
				break;
	}
	
	/* Setup thrd channel low and high values */
	if (adc_thrd_chl)
	{
		/* adc_chl assigns to thrd1 */
		Chip_ADC_SetThrLowValue(LPC_ADC, 1, threshold_low);
		Chip_ADC_SetThrHighValue(LPC_ADC, 1, threshold_hi);
		LPC_ADC->CHAN_THRSEL = (LPC_ADC->CHAN_THRSEL & 0UL) | ( 1UL << adc_chl ) ;
	}
	else
	{
		/* adc_chl assigns to thrd0 */
		Chip_ADC_SetThrLowValue(LPC_ADC, 0, threshold_low);
		Chip_ADC_SetThrHighValue(LPC_ADC, 0, threshold_hi);
		LPC_ADC->CHAN_THRSEL = LPC_ADC->CHAN_THRSEL  & (~( 1UL << adc_chl )) ;
	}
	
	/* Setup a sequencer A to do the following:
	   Perform ADC conversion of select ADC channels adc_chl */
	LPC_ADC->SEQ_CTRL[ADC_SEQA_IDX] |= (1 << (adc_chl) | ADC_SEQ_CTRL_MODE_EOS);
	
	/* Enable all the ADCMPINTEN_n to outside mode, SEQA_INTEN, OVR_INTEN, all the channels to outside int */
	LPC_ADC->INTEN = (LPC_ADC->INTEN & 0x00000000) | 0x02AAAAA9;
	/* Enable ADC SEQA/THCMP NVIC interrupt */
	NVIC_EnableIRQ(ADC_A_IRQn);
	/* Enable sequencer */
	Chip_ADC_EnableSequencer(LPC_ADC, ADC_SEQA_IDX);

}

/**
 * @brief	Start ADC
 * @param	adc_chl : Channel of ADC(adc0 for Channel 0, and so on)
 * @return	None
 */
void adc_start_non_blocking(adc_chl_t adc_chl)
{
	/* Manual start for ADC conversion sequence A */
	Chip_ADC_StartSequencer(LPC_ADC, ADC_SEQA_IDX);

	/* ADC_CHL_MASK as a mark, to judge which channel is using */
	g_ADC_set_chl_mask |= (1 << adc_chl);

}

static void adc_scan(void)
{
	uint8 ADC_i;
	uint32_t rawSample;
	
	for (ADC_i=1; ADC_i<adc_max; ADC_i++)
	{
		if (g_ADC_set_chl_mask & (1 << ADC_i))
		{
			/*Notice: When DataReg is read, DR_DATAVALID will be clear at once*/
			rawSample = Chip_ADC_GetDataReg(LPC_ADC, ADC_i);
			if (rawSample & (ADC_DR_OVERRUN | ADC_DR_DATAVALID))
			{
				g_ADC_set_chl_mask &= ~(1 << ADC_i);
				g_ADC_conv_chl_mask |= (1 << ADC_i);
				
				if(pFunc_ADC_Ready[ADC_i] != NULL)
				{
					pFunc_ADC_Ready[ADC_i](0);	//recalling ADC_chl_i In range
				}
				
				switch ( ADC_DR_THCMPRANGE(rawSample) )
				{						
					//Below Range
					case 0x01:					
						if(pFunc_ADC_Thrd[ADC_i] != NULL)
						{
							pFunc_ADC_Thrd[ADC_i](0);	//recalling ADC_chl_i Below range
						}
						break;
					
					//Above Range
					case 0x02:
						if(pFunc_ADC_Thrd[ADC_i] != NULL)
						{
							pFunc_ADC_Thrd[ADC_i](1);	//recalling ADC_chl_i Above range
						}
						break;
						
					default :
						break;
				}
			}
		}
	}
}

/**
 * @brief	Handle interrupt from ADC sequencer A
 * @return	Nothing
 */
void ADCA_IRQHandler(void)
{
	adc_scan();
	/* Clear any pending interrupts */
	Chip_ADC_ClearFlags(LPC_ADC, Chip_ADC_GetFlags(LPC_ADC));	
}

/**
 * @brief	ADC_chl_i In range callback register
 * @return	Nothing
 */
void adc_ready_isr_callback_register(adc_chl_t adc_chl, p_handler adc_rdy_isr_user)     //Set ADC Ready ISR user function pointer. After calling adc_start_non_blocking function, the user ISR function will be called.
{
	pFunc_ADC_Ready[adc_chl] = adc_rdy_isr_user ;
}

/**
 * @brief	ADC_chl_i threshold compare(above/below range) callback register
 * @return	Nothing
 */
void adc_thrhld_isr_callback_register(adc_chl_t adc_chl, p_handler adc_thrhld_isr_user) //Set ADC Low/ High Threshold ISR user function pointer
{
	pFunc_ADC_Thrd[adc_chl] = adc_thrhld_isr_user;
}

uint8 adc_poll_adc_ready(adc_chl_t adc_chl) //Poll the ADC Convert Ready Status before calling adc_read_blocking function
{
	uint8 poll_adc_ready = 0;
	if (g_ADC_conv_chl_mask & (1<<adc_chl))
	{
		poll_adc_ready = 1;
		g_ADC_conv_chl_mask &= ~(1 << adc_chl);
	}
	return poll_adc_ready;
}

uint16 adc_read_blocking(adc_chl_t adc_chl) //Read ADC value on the selected ADC Channel in blocking way 
{
	uint32 adc_raw_val;
	uint16 adc_val;
	adc_raw_val = Chip_ADC_GetDataReg( LPC_ADC, adc_chl );
	adc_val = ADC_DR_RESULT( adc_raw_val );
	return adc_val;
}

/**
 * @brief	read adc result with blocking way.
 * @param   number of adc sample channel.
 * @return	sample result; 0xFFFF=fail.
 */
uint16_t adc_read_with_blocking(adc_chl_t adc_chl)
{
	uint16_t retValue = 0xFFFF;
	uint32_t adcStatus;
	uint32_t readOverTime = 100000;
	while(readOverTime)
		{
		adcStatus = Chip_ADC_GetDataReg(LPC_ADC, adc_chl);
		if(adcStatus & (ADC_DR_OVERRUN | ADC_DR_DATAVALID))	
			{
			retValue = adc_read_blocking(adc_chl);
			break;
			}
		readOverTime--;
		}	
	return retValue;
}
