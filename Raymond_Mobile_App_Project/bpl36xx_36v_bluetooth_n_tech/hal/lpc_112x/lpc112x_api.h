//************************************************************************************** 
//*                                                                         
//*   FILE NAME:        lpc112x_api.h
//*   SUBSYSTEM:        MCU functions
//*                                                                         
//*   DESCRIPTION:      This file provides api fucntion.
//*        
//*   COMPANY:          TTI - CPT Firmware Team
//*
//*
//**************************************************************************************
//*
//*     COPYRIGHT:
//*        This document is considered confidential and is maintained as trade secret by
//*        TTI CPT FW team. 
//* 
//*        TTI Corporation.  All rights reserved.
//* 
//*
//**************************************************************************************

//*-------------------------------------------------------------------  
//* CHANGE HISTORY                                                         
//*----------------------------------------------------------------------  
//*  Date          Author                             Reference
//*  ========      ========                           ==========================
//*  2016-05-13    Justus Lian                        create file
//*
//*
//*-------------------------------------------------------------------

//*-------------------------------* 
//*    Include Files              * 
//*-------------------------------*


//*-------------------------------* 
//*    Symbol Definitions         * 
//*-------------------------------*


#ifndef __lPC112X_API_H__
#define __LPC112X_API_H__

//*-------------------------------* 
//*    Type Definitions           *
//*-------------------------------*

typedef enum
{
   gpio_0_00 = 0,
   gpio_0_01 = 1,
   gpio_0_02 = 2,
   gpio_0_03 = 3,
   gpio_0_04 = 4,
   gpio_0_05 = 5,
   gpio_0_06 = 6,
   gpio_0_07 = 7,
   gpio_0_08 = 8,
   gpio_0_09 = 9,
   gpio_0_10 = 10,
   gpio_0_11 = 11,
   
   gpio_1_00 = 12,
   gpio_1_01 = 13,
   gpio_1_02 = 14,
   gpio_1_03 = 15, 
   gpio_1_04 = 16,
   gpio_1_05 = 17,
   gpio_1_06 = 18,
   gpio_1_07 = 19,
   gpio_1_08 = 20,
   gpio_1_09 = 21,
   gpio_1_10 = 22,
   gpio_1_11 = 23,
   
   gpio_2_00 = 24,
   gpio_2_01 = 25,
   gpio_2_02 = 26,
   gpio_2_03 = 27,
   gpio_2_04 = 28,
   gpio_2_05 = 29,
   gpio_2_06 = 30,
   gpio_2_07 = 31,
   gpio_2_08 = 32,
   gpio_2_09 = 33,
	 //gpio_2_10 = 34;		// User manual had define this function , but don't assign physical
   
   gpio_3_00 = 35,
	 //gpio_3_02 = 36,		// User manual had define this function , but don't assign physical
   gpio_3_03 = 37,
   gpio_3_04 = 38,
   gpio_3_05 = 39,
   
   gpio_max
   
}gpio_no_t;            //GPIO Pin Number



typedef enum
{ 
   gpio_pull_hiz  = 0,  //Enable GPIO High Impedance Mode
   gpio_pull_down = 1,  //Enable GPIO Pull-down Resister, except RESET/PIO0_5
   gpio_pull_up   = 2,  //Enable GPIO Pull-up Resister, except I2C PIN: PIO0_10 and PIO0_11
   gpio_pull_max
   
}gpio_pull_t;          //GPIO PULL Settings
   
   

typedef enum
{
	timer0 = 0,
	timer1 = 1,
	timer2 = 2,
	timer3 = 3,
	timer_max
   
}timer_no_t;           //TIMER Module Number


/* remove this function by WPI Wecan Chen 2017.03.08
typedef enum
{
   pwm0 = 0,
   pwm1 = 1,
   pwm2 = 2,
   pwm3 = 3,
   pwm4 = 4,
   pwm5 = 5,
   pwm_max
   
}pwm_no_t;             //PWM Module Number
*/


typedef enum
{	
	//adc0  = 0,				// User manual didn't define this function
	adc1  = 1,
	adc2  = 2,
	adc3  = 3,
	adc4  = 4,
	adc5  = 5,
	adc6  = 6,
	adc7  = 7,
	adc8  = 8,
	adc_max
   
}adc_chl_t;            //ADC Channel Number



typedef enum
{
   thrd0 = 0,           
   thrd1 = 1
   
}adc_thrd_t;          //ADC Threshold Number



typedef struct
{
   uint8 *p_buff;       //Data-to-transmit Buffer Pointer
   uint16 buff_len;     //Buffer Length

}buff_t;               //Transmission Buffer



typedef enum
{
   uart0  = 0,
   uart1  = 1,
   uart2  = 2,
   uart_max
   
}uart_no_t;            //UART Module Number



typedef enum
{
   br_9600   = 9600,    //9600   bps
   br_19200  = 19200,   //19200  bps
   br_38400  = 38400,   //38400  bps
   br_115200 = 115200,  //115200 bps
   br_128000 = 128000,  //128000 bps
   br_256000 = 256000,  //256000 bps  
   br_max
   
}baudrate_t;           //Baud Rate



typedef struct
{
  uart_no_t uart_no;
	uint8 bitlen;        //0: UART 7 bit length mode, 1: UART 8 bit length mode, 
   uint8 parity;        //0: No parity, 1: Even parity, 2: Odd parity
   uint8 stop_len;      //0: One Stop Bit, 1: Two Stop Bits
   baudrate_t baudrate; //Baud Rate
   buff_t tx_buff;      //Data-to-transmit Buffer Pointer
   buff_t rx_buff;      //Data Received Buffer Pointer
   
}uart_config_t;        //UART Configuration



typedef enum
{
   i2c_0 = 0,
   i2c_max
   
}i2c_no_t;              //I2C Module Number



typedef struct
{
   i2c_no_t i2c_no;      //I2C Module Number
   uint32 i2c_frq;       //I2C frequency in Hz

}i2c_mas_config_t;      //I2C Master Configuration



typedef struct
{   
   buff_t tx_buff;       //I2C Transmission data, including Command, Register Address and Data
   uint8 Slave_Addr_WR;  //Slave Device Write Address
   
}i2c_tx_buff_t;         //I2C Buffer to Transmit



typedef struct
{   
   buff_t rx_buff;       //I2C Read Buffer, where the I2C received data stores
   uint8 Slave_Addr_RD;  //Slave Device Read Address
   
}i2c_rx_buff_t;         //I2C Buffer Received



typedef enum
{
	spi_0 = 0,            
	spi_1 = 1,            
	spi_max
   
}spi_no_t;              //SPI Module Number



typedef enum
{
	spi_mode_0 = 0,       //The SPI captures serial data on the first clock transition of the transfer
	spi_mode_1 = 1,       //The SPI changes serial data on the first clock transition of the transfer
	spi_mode_2 = 2,       //Same as mode 0 with SCK inverted
	spi_mode_3 = 3,       //Same as mode 1 with SCK inverted
	spi_mode_max
   
}spi_mode_no_t;         //SPI Mode

typedef enum
{
	first_msb = 0,
	first_lsb = 1,
	transfer_dir_max
	
}transfer_dir_t;

typedef enum
{
	per_frame_8bits = 0,
	per_frame_16bits = 1,
	per_frame_bits_max
	
}per_frame_bits_t;

typedef struct
{
   spi_no_t spi_no;            //SPI Module Number
   spi_mode_no_t spi_mode_no;  //SPI Mode 
   uint32 spi_frq;             //SPI Frequency in Hz
	transfer_dir_t transfer_dir;
	per_frame_bits_t per_frame_bits;
		
}spi_mas_config_t;            //SPI Master Configuration



typedef enum
{
   pin_int0 = 0,
   pin_int1 = 1,
   pin_int2 = 2,
   pin_int3 = 3,
   pin_int4 = 4,
   pin_int5 = 5,
   pin_int6 = 6,
   pin_int7 = 7,
   pin_int8 = 8,
	 pin_int9 = 9,
	 pin_int10 = 10,
	 pin_int11 = 11,	
	 pin_int12 = 12,
	 pin_int13 = 13,
	 pin_int14 = 14,
	 pin_int15 = 15,
	 pin_int16 = 16,
	 pin_int17 = 17,
	 pin_int18 = 18,
	 pin_int19 = 19,
	 pin_int20 = 20,
	 pin_int21 = 21,
	 pin_int22 = 22,
	 pin_int23 = 23,
	 pin_int24 = 24,
	 pin_int25 = 25,
	 pin_int26 = 26,
	 pin_int27 = 27,
	 pin_int28 = 28,
	 pin_int29 = 29,
	 pin_int30 = 30,
	 pin_int31 = 31,
	 pin_int32=  32,
	 pin_int33 = 33,
	 pin_int34 = 34,
	 pin_int35 = 35,
	 pin_int36 = 36,
	 pin_int37 = 37,
   pin_int_max
  
}pin_int_chl_t;               //Pin Interrupt Channel



typedef enum
{
   edge_falling = 0,
   edge_rising = 1,
   edge_dual = 2,
   edge_max
   
}trig_edg_t;                  //Trigger Event



typedef struct
{
  pin_int_chl_t pin_int_chl;  //Pin Interrupt Channel
   uint8 trg_edg_lvl;          //Pin Interrupt Trigger Mode, 0: Triger PIN INT On Edge, 1: Triger PIN INT On Level
   
   union
   {
      trig_edg_t edg;           //Trigger on Edge
      uint8 lvl;                //Trigger on LvL, 0: Active on Low Levl,     1: Active on High Level       
   
   }trg_even;                  //Trigger Event
   
}pin_int_config_t;            //Pin Interrupt Configuration
   


typedef enum
{
	bod_rstlvl0 = 0,            //Brown-out Reset at 1.46 ~ 1.63V
	bod_rstlvl1 = 1,            //Brown-out Reset at 2.06 ~ 2.15V
	bod_rstlvl2 = 2,            //Brown-out Reset at 2.35 ~ 2.43v
	bod_rstlvl3 = 3,            //Brown-out Reset at 2.63 ~ 2.71v
	bod_rstlv_max
   
}bod_rstlvl_t;                //Brown-out Reset Level



typedef enum
{
	bod_intlvl0 = 0,            //Reserved
	bod_intlvl1 = 1,            //Brown-out Interrupt at 2.22 ~ 2.35V
	bod_intlvl2 = 2,            //Brown-out Interrupt at 2.52 ~ 2.66V
	bod_intlvl3 = 3,            //Brown-out Interrupt at 2.80 ~ 2.90V
	bod_max             
   
}bod_intlvl_t;                //Brown-out Interrupt Level



typedef struct
{
   bod_rstlvl_t bod_rstlvl;    //Brown-out Reset Level
   uint8 bod_rst_en;           //Brown-out Reset Enable, 0: Disable, 1: Enable
   bod_intlvl_t bod_intlvl;    //Brown-out Interrupt Level
   uint8 bod_int_en;           //Brown-out Interrupt Enable, 0: Disable, 1: Enable
   
}bod_config_t;                //Brown-out Configuration

//*-------------------------------* 
//*    Structure Templates        * 
//*-------------------------------*

//*-------------------------------* 
//*    External Functions         * 
//*-------------------------------*
//SYSCLOCK
void system_clock_init(void);

//SYSTEM TICK
void sys_tick_config(uint32 interval);                             //Set SYSTEM TICK interval, interval: 1 per 100us
void sys_tick_isr_callback_register(p_handler sys_tick_isr_user);  //Set SYSTEM TICK ISR user function pointer, sys_tick_isr_user: SYSTEM TICK ISR calls this function

//PRINTF
void printf_config(uart_no_t uart_no, gpio_no_t tx_gpio_no, gpio_no_t rx_gpio_no); //Configure Printf setting, with Baud Rate 115200bps, 8Bit, 1 stop bit, no Parity

//ADC
void adc_config(adc_thrd_t adc_thrd_chl, adc_chl_t adc_chl, uint16 threshold_low, uint16 threshold_hi);  //Set ADC settings, threshold_low: Low Limit ADC value, set it 0 to disable, threshold_hi: High Limit ADC value, set it 0xFFF to disable it
void adc_start_non_blocking(adc_chl_t adc_chl);                                                          //Set ADC Channel started to convert
uint8 adc_poll_adc_ready(adc_chl_t adc_chl);                                                             //Poll the ADC Convert Ready Status before calling adc_read_blocking function
uint16 adc_read_blocking(adc_chl_t adc_chl);                                                             //Read ADC value on the selected ADC Channel in blocking way 
uint16 adc_read_with_blocking(adc_chl_t adc_chl);
void adc_ready_isr_callback_register(adc_chl_t adc_chl, p_handler adc_rdy_isr_user);                     //Set ADC Ready ISR user function pointer. After calling adc_start_non_blocking function, the user ISR function will be called.
void adc_thrhld_isr_callback_register(adc_chl_t adc_chl, p_handler adc_thrhld_isr_user);                 //Set ADC Low/ High Threshold ISR user function pointer

//GPIO 
void gpio_direction(gpio_no_t gpio_no, uint8 direction);           //Set GPIO PIN direction, direction: 0, output, 1, input
void gpio_pull_updown(gpio_no_t gpio_no, gpio_pull_t pull);      //Set GPIO PIN pull state 
void gpio_output(gpio_no_t gpio_no, uint8 value);                //Set GPIO PIN output value
uint8 gpio_input(gpio_no_t gpio_no);                               //Read GPIO PIN value
void gpio_output_toggle(gpio_no_t gpio_no);                                  //Set GPIO PIN output toggle

//UART
void uart_config(uart_config_t *p_uart_t, gpio_no_t tx_gpio_no, gpio_no_t rx_gpio_no);               //Configure UART settings
void uart_send_non_blocking(uart_no_t uart_no, buff_t *p_tx_uart_buff);                              //Send UART Data in non-blocking way. After calling this function, uart_tx_rdy_isr_user function will be called when 1 byte transmission is completed.
void uart_send_tx_ready_isr_callback_register(uart_no_t uart_no, p_handler uart_tx_rdy_isr_user);    //After 1 byte of UART data transmision is completed, uart_tx_rdy_isr_user function will be called.    
uint8 uart_tx_buff_emty(uart_no_t uart_no);                                                          //UART Transmission Free
uint16 uart_get_rx_buffer_data_len(uart_no_t uart_no);                                               //After receiving a frame of data, calling this function will return number of bytes received.
void uart_read_rx_buffer(uart_no_t uart_no, buff_t *p_rx_uart_buff);                                 //Read the received frame of data, the length of the frame is the same as the return value of get_uart_rx_buffer_data_len function. 
void uart_ReadRxBufferOnly(uart_no_t uart_no, uint8_t* pCopyBuf, uint16_t bytes);                    // Read the Rx buffer, copy only setting bytes.
void uart_DeleteRxBufferBytes(uart_no_t uart_no, uint16_t byteLen);                                          // Delete setting bytes in rx buffer.

//GLOBLE INT
void globle_int_enable(void);           //Enable GLOBLE INTERRUPT
void globle_int_disable(void);          //Disable GLOBLE INTERRUPT

//I2C
void i2c_master_config(i2c_mas_config_t *p_i2c_no, gpio_no_t gpio_scl, gpio_no_t gpio_sda);          //Configure I2C settins as Master Mode
uint8 i2c_write_blocking(uint8 i2c_no, i2c_tx_buff_t *p_i2c_tx_buff);                                //Write I2C data in blocking way, p_i2c_tx_buff includes Slave Write Address, Write Command, Register Addr and Data To Write
uint8 i2c_read_blocking(uint8 i2c_no, i2c_tx_buff_t *p_i2c_tx_buff, i2c_rx_buff_t *p_i2c_rx_buff);   //Read I2C data in blocking way, when reading the I2C data, firstly need to write Slave Write Address, Read Command, included in p_i2c_tx_buff.     


//PIN_INT
void pin_int_config(pin_int_config_t *p_pin_int, gpio_no_t gpio_int);               //Configure PIN INTERRUPT settings                                                                                            
void pin_isr_callback_register(pin_int_chl_t pin_int_chl, p_handler pin_isr_user);  //Set pin_isr_user function pointer, pin_isr_user: PIN INTERRUPT ISR calls this function 
void pin_int_enable(pin_int_chl_t pin_int_chl, uint8 enable);                         //Enable/Disable PIN INTERRUPT


//TIMER
void timer_config(timer_no_t time_no, uint32 interval);                          //Set TIMER interval, 1 per 10us  
void timer_enable(timer_no_t time_no, uint8 enable);                             //Enable/ disable TIMER, enable: 1, enable, 0, disable
void timer_isr_callback_register(timer_no_t time_no, p_handler timer_isr_user);  //Set TIMER ISR user function pointer, timer_isr_user: TIEMR ISR calls this function
       
/* remove this function by WPI Wecan Chen 2017.03.08			 
//PWM
void pwm_config(pwm_no_t pwm_no, uint32 frq ,gpio_no_t gpio_no);       //Configure PWM initial settings, frq: pwm frquency, 1 per 1Hz, 6 PWM channels share the same SCT, the frq value should be the same.  
void pwm_enable(pwm_no_t pwm_no, uint8 enable);                        //Enable/ disable PWM channel, enable: 1, enable, 0, disable
void pwm_set_duty(pwm_no_t pwm_no, uint16 duty);                       //Set PWM channel duty, duty: 0-1000
*/
#if 0
//SPI
void spi_master_config(spi_mas_config_t *p_spi_no, gpio_no_t gpio_miso, gpio_no_t gpio_mosi, gpio_no_t gpio_clk, gpio_no_t gpio_ssel); //Configure SPI settings
void spi_write_blocking(uint8 spi_no, buff_t *p_spi_tx_buff);               //Write SPI data in blocking way, transmit the data in p_spi_tx_buff
void spi_read_blocking(uint8 spi_no, buff_t *p_spi_rx_buff);                //Read SPI data in blocking way, store the received data in p_spi_rx_buff

//POWER SAVING MODE
void power_saving_mode_enter(void);     //Enter POWR SAVING MODE
void power_saving_mode_exit(void);      //Exiter POWER SAVING MDOE, MCU is waken up by the any interrupt. Add user code such as system initial in this function if needed.

//NON-VOLATILE MEMORY
void nvm_config(uint8 region_no, uint32 start_addr, uint16 len);                   //Configure NVM settings
uint8 nvm_write_blocking(uint8 region_no, uint32 addr, uint8 *p_buf, uint16 len);  //Write NVM in blocking way
void nvm_read_blocking(uint8 region_no, uint32 addr, uint8 *p_buf, uint16 len);    //Read NVM in blocking way

//BOD
uint8 bod_config(bod_config_t *p_bod);          //Configure BOD settings
void bod_isr_callback(p_handler bod_isr_user);  //Set bod_isr_user function pointer, bod_isr_user: BOD ISR calls this function 

//*-------------------------------* 
//*    Local Functions            * 
//*-------------------------------*

//*-------------------------------* 
//*    Macro Functions            * 
//*-------------------------------*

#endif
#endif

//-------------
// End of File
//-------------





