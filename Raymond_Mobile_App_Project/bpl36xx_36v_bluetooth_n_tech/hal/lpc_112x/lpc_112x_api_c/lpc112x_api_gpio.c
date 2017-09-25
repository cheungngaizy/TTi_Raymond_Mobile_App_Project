#include "chip.h"
#include "string.h"
#include <stdio.h>
#include "types.h"
#include "lpc112x_api.h"

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/
unsigned int iocon_pin_val[]={ 	0x03,0x04,0x07,0x30,0x0C,0x0D,0x31,0x32,0x18,0x19,0x1A,0x1D,
																0x1E,0x1F,0x20,0x24,0x25,0x28,0x29,0x2A,0x06,0x0E,0x1B,0x26,
																0x02,0x0A,0x17,0x23,0x10,0x11,0x00,0x08,0x09,0x15,0x16,
																0x21,0x27,0x2B,0x0F,0x12}; 																			
/**
 * @brief IOCON Register bit definitions
 */
/* Pin Mode mask */
#define PIN_MODE_MASK           (0x3 <<  3)
#define PIN_MODE_BITNUM         (3)

/* Pin Mode mask */
#define PIN_FUNC_MASK           (0x7 <<  0)
#define PIN_FUNC_BITNUM         (0)
																		
/**
 * @brief IOCON Pin Modes enum
 */
typedef enum CHIP_PIN_MODE {
	PIN_MODE_INACTIVE = 0,	/*!< Inactive mode */
	PIN_MODE_PULLDN = 1,	/*!< Pull Down mode */
	PIN_MODE_PULLUP = 2,	/*!< Pull up mode */
	PIN_MODE_REPEATER = 3,	/*!< Repeater mode */
} CHIP_PIN_MODE_T;																		
																															
/*****************************************************************************
 * Public functions
 ****************************************************************************/
static void pinmodesetting(LPC_IOCON_T *pIOCON, uint8_t i, CHIP_PIN_MODE_T mode);



/**
 * @brief	Set direction of GPIO pin_no																		
 * @param	gpio_no		: setting GPIO pin_no direction
 * @param	direction	: 0 for output, 1 for input
 * @return	Nothing
 * @note	To avoid gpio port 0 pin2&pin3 work as SWDIO/SWCLK mode if using pin2/3
 */
void gpio_direction(gpio_no_t gpio_no, uint8_t direction)
{
	uint32_t reg, func_value;
	/* GPIO clock Enable */
	Chip_Clock_EnablePeriphClock(SYSCON_CLOCK_GPIO);
	reg = LPC_IOCON->REG[(iocon_pin_val[gpio_no])] & ~(PIN_FUNC_MASK);
	switch(gpio_no)
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
	LPC_IOCON->REG[(iocon_pin_val[gpio_no])] = ( reg | func_value );
	
	if ( direction ) 
	{
		// port 0
		if(gpio_no <= gpio_0_11){
			Chip_GPIO_SetPinDIRInput(LPC_GPIO, 0, gpio_no); 		
		}
		
		// port 1
		if((gpio_no >= gpio_1_00) && (gpio_no <= gpio_1_11)){
			Chip_GPIO_SetPinDIRInput(LPC_GPIO, 1, (gpio_no - gpio_1_00)); 				
		}
		
		// port 2
		if((gpio_no >= gpio_2_00) && (gpio_no <= gpio_2_09)){
			Chip_GPIO_SetPinDIRInput(LPC_GPIO, 2, (gpio_no - gpio_2_00)); 
		}
		
		// port 3
		if((gpio_no >= gpio_3_00) && (gpio_no <= gpio_3_05)){
			if(gpio_no == gpio_3_00)	Chip_GPIO_SetPinDIRInput(LPC_GPIO, 3, 0); 
			if(gpio_no == gpio_3_03)	Chip_GPIO_SetPinDIRInput(LPC_GPIO, 3, 3); 	
			if(gpio_no == gpio_3_04)	Chip_GPIO_SetPinDIRInput(LPC_GPIO, 3, 4); 
			if(gpio_no == gpio_3_05)	Chip_GPIO_SetPinDIRInput(LPC_GPIO, 3, 5);			
		}
	}
	else
	{	
		// port 0
		if(gpio_no <= gpio_0_11){
			Chip_GPIO_SetPinDIROutput(LPC_GPIO, 0, gpio_no); 
		}
		
		// port 1
		if((gpio_no >= gpio_1_00) && (gpio_no <= gpio_1_11)){
			Chip_GPIO_SetPinDIROutput(LPC_GPIO, 1, (gpio_no - gpio_1_00)); 
		}
		
		// port 2
		if((gpio_no >= gpio_2_00) && (gpio_no <= gpio_2_09)){
			Chip_GPIO_SetPinDIROutput(LPC_GPIO, 2, (gpio_no - gpio_2_00)); 
		}
		
		// port 3
		if((gpio_no >= gpio_3_00) && (gpio_no <= gpio_3_05))
		{
			if(gpio_no == gpio_3_00)	Chip_GPIO_SetPinDIROutput(LPC_GPIO, 3, 0); 
			if(gpio_no == gpio_3_03)	Chip_GPIO_SetPinDIROutput(LPC_GPIO, 3, 3); 	
			if(gpio_no == gpio_3_04)	Chip_GPIO_SetPinDIROutput(LPC_GPIO, 3, 4); 
			if(gpio_no == gpio_3_05)	Chip_GPIO_SetPinDIROutput(LPC_GPIO, 3, 5);	
		}
	}
}


/**
 * @brief	Sets pull-up or pull-down mode for a pin
 * @param	pIOCON	: The base of IOCON peripheral on the chip
 * @param	pin		: Pin number
 * @param	mode	: Mode (Pull-up/Pull-down mode)
 * @return	Nothing
 * @note	Do not use with pins PIO10 and PIO11.
 */

static void pinmodesetting(LPC_IOCON_T *pIOCON, uint8_t i, CHIP_PIN_MODE_T mode)
{
	uint32_t reg;
	reg = pIOCON->REG[(iocon_pin_val[i])] & ~(PIN_MODE_MASK);
	pIOCON->REG[(iocon_pin_val[i])] = reg | (mode << PIN_MODE_BITNUM);
}


/**
 * @brief	GPIO pin config for diff mode(only work in input mode)
 * @param	gpio_no: select which gpio pin to be set
 * @param	pull: set mode<inactive/pullup/pulldown/repeater>
 * @return	Nothing
 */
void gpio_pull_updown(gpio_no_t gpio_no, gpio_pull_t pull)      //Set GPIO PIN pull state 
{
	/*Enable IOCON clock*/
	Chip_Clock_EnablePeriphClock( SYSCON_CLOCK_IOCON );
	
	switch ( pull )
	{
		case gpio_pull_hiz:
			pinmodesetting ( LPC_IOCON, gpio_no, PIN_MODE_INACTIVE );
			break ;
		
		case gpio_pull_down:
			pinmodesetting ( LPC_IOCON, gpio_no, PIN_MODE_PULLDN );
			break ;
		
		case gpio_pull_up:
			pinmodesetting ( LPC_IOCON, gpio_no, PIN_MODE_PULLUP );
			break ;
		
		case gpio_pull_max:
			//Reserved
			break ;
		
		default :
			break;
		
	}
	
	/*Disable IOCON clock, Once configured, disable IOCON clock to conserve power.*/
	//Chip_Clock_DisablePeriphClock( SYSCON_CLOCK_IOCON );
}


/**
 * @brief	Set a GPIO pin state via the GPIO byte register
 * @param	gpio_no	: GPIO pin_no to set 
 * @param	value	: true for high, false for low
 * @return	Nothing
 */
void gpio_output(gpio_no_t gpio_no, uint8 value)                //Set GPIO PIN output value
{
	// port 0
	if(gpio_no <= gpio_0_11){
		Chip_GPIO_SetPinState(LPC_GPIO, 0, gpio_no,value);
	}
	
	// port 1
	if((gpio_no >= gpio_1_00) && (gpio_no <= gpio_1_11)){
		Chip_GPIO_SetPinState(LPC_GPIO, 1, (gpio_no-gpio_1_00), value);			
	}
	
	// port 2
	if((gpio_no >= gpio_2_00) && (gpio_no <= gpio_2_09)){		
		Chip_GPIO_SetPinState(LPC_GPIO, 2, (gpio_no-gpio_2_00), value);		
	}
	
	//port 3
	if((gpio_no >= gpio_3_00) && (gpio_no<=gpio_3_05)){
		if(gpio_no == gpio_3_00)	Chip_GPIO_SetPinState(LPC_GPIO, 3, 0, value); 
		if(gpio_no == gpio_3_03)	Chip_GPIO_SetPinState(LPC_GPIO, 3, 3, value); 
		if(gpio_no == gpio_3_04)	Chip_GPIO_SetPinState(LPC_GPIO, 3, 4, value); 
		if(gpio_no == gpio_3_05)	Chip_GPIO_SetPinState(LPC_GPIO, 3, 5, value); 
	}
}


/**
 * @brief	Read a GPIO pin state via the GPIO byte register
 * @param	pin		: GPIO pin_no to read
 * @return	true if the GPIO pin_no is high, false if low
 */
uint8 gpio_input(gpio_no_t gpio_no)		//Read GPIO PIN value
{
	uint8_t port, temp_gpio_no;

	// port 0
	if( gpio_no <= gpio_0_11){
		port = 0;	temp_gpio_no = gpio_no;
	}
	
	// port 1
	if((gpio_no >= gpio_1_00) && (gpio_no <= gpio_1_11)){
		port = 1;	temp_gpio_no = gpio_no - gpio_1_00;
	}
	
	// port 2
	if((gpio_no >= gpio_2_00) && (gpio_no <= gpio_2_09)){
		port = 2;	temp_gpio_no = gpio_no - gpio_2_00;
	}
	
	// port 3
	if((gpio_no >= gpio_3_00) && (gpio_no <= gpio_3_05)){
			port = 3;
			if(gpio_no == gpio_3_00)	temp_gpio_no = gpio_no - gpio_3_00;
			if(gpio_no == gpio_3_03)	temp_gpio_no = gpio_no - gpio_3_00 + 1;
			if(gpio_no == gpio_3_04)	temp_gpio_no = gpio_no - gpio_3_00 + 1;
			if(gpio_no == gpio_3_05)	temp_gpio_no = gpio_no - gpio_3_00 + 1;
	}
	
	return (LPC_GPIO[port].DATA[1 << temp_gpio_no]) != 0;	
}

/**
 * @brief	Toggle GPIO pin state via the GPIO byte register
 * @param	pin		: GPIO pin_no to toggle
 * @return	Nothing
 */
void gpio_output_toggle(gpio_no_t gpio_no)											 //Set GPIO PIN output toggle
{
	// port 0
	if(gpio_no <= gpio_0_11){
		Chip_GPIO_SetPinToggle(LPC_GPIO, 0, gpio_no);
	}
	
	// port 1
	if((gpio_no >= gpio_1_00) && (gpio_no <= gpio_1_11)){
		Chip_GPIO_SetPinToggle(LPC_GPIO, 1, (gpio_no - gpio_1_00));
	}
	
	// port 2
	if((gpio_no >= gpio_2_00) && (gpio_no <= gpio_2_09)){
		Chip_GPIO_SetPinToggle(LPC_GPIO, 2, (gpio_no - gpio_2_00));
	}
	
	// port 3
	if((gpio_no >= gpio_3_00) && (gpio_no <= gpio_3_05)){
			if(gpio_no == gpio_3_00)	Chip_GPIO_SetPinToggle(LPC_GPIO, 3, 0);
			if(gpio_no == gpio_3_03)	Chip_GPIO_SetPinToggle(LPC_GPIO, 3, 3);
			if(gpio_no == gpio_3_04)	Chip_GPIO_SetPinToggle(LPC_GPIO, 3, 4);
			if(gpio_no == gpio_3_05)	Chip_GPIO_SetPinToggle(LPC_GPIO, 3, 5);
	}
}
