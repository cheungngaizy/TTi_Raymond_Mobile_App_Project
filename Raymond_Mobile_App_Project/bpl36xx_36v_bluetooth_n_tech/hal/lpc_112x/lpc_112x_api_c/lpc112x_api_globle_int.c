#include "chip.h"
#include "string.h"
#include <stdio.h>
#include "types.h"
#include "lpc112x_api.h"


/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/


/*****************************************************************************
 * Public functions
 ****************************************************************************/
 /**
 * @brief	globle_int_enable
 * @param	Nothing
 * @return Nothing
 */
void globle_int_enable(void)
{
  __enable_irq();

}	

/**
 * @brief	globle_int_disable
 * @param	Nothing
 * @return Nothing
 */
void globle_int_disable(void)
{
 __disable_irq();
}






























