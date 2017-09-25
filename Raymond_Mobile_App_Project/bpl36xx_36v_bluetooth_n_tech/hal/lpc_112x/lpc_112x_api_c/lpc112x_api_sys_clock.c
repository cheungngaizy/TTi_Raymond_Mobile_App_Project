#include "chip.h"
#include "string.h"
#include <stdio.h>
#include "types.h"
#include "lpc112x_api.h"


/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/
#define SYS_IRC_CLOCK            (12000000)
#define EX_RATE_IN               (0)


/* System oscillator rate and clock rate on the CLKIN pin */
const uint32_t OscRateIn = SYS_IRC_CLOCK;
const uint32_t ExtRateIn = EX_RATE_IN;

/*****************************************************************************
 * Public functions
 ****************************************************************************/
void system_clock_init(void)
{
	Chip_Clock_EnablePeriphClock(SYSCON_CLOCK_IOCON);
	Chip_SetupIrcClocking();
	SystemCoreClockUpdate();
}







