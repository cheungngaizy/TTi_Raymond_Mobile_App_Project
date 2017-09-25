/*
 * @brief	IO redirection support
 *
 * This file adds re-direction support to the library for various
 * projects. It can be configured in one of 3 ways - no redirection,
 * redirection via a UART, or redirection via semihosting. If DEBUG
 * is not defined, all printf statements will do nothing with the
 * output being throw away. If DEBUG is defined, then the choice of
 * output is selected by the DEBUG_SEMIHOSTING define. If the
 * DEBUG_SEMIHOSTING is not defined, then output is redirected via
 * the UART. If DEBUG_SEMIHOSTING is defined, then output will be
 * attempted to be redirected via semihosting. If the UART method
 * is used, then the Board_UARTPutChar and Board_UARTGetChar
 * functions must be defined to be used by this driver and the UART
 * must already be initialized to the correct settings.
 *
 * @note
 * Copyright(C) NXP Semiconductors, 2012
 * All rights reserved.
 *
 * @par
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * LPC products.  This software is supplied "AS IS" without any warranties of
 * any kind, and NXP Semiconductors and its licensor disclaim any and
 * all warranties, express or implied, including all implied warranties of
 * merchantability, fitness for a particular purpose and non-infringement of
 * intellectual property rights.  NXP Semiconductors assumes no responsibility
 * or liability for the use of the software, conveys no license or rights under any
 * patent, copyright, mask work right, or any other intellectual property rights in
 * or to any products. NXP Semiconductors reserves the right to make changes
 * in the software without notification. NXP Semiconductors also makes no
 * representation or warranty that such application will be suitable for the
 * specified use without further testing or modification.
 *
 * @par
 * Permission to use, copy, modify, and distribute this software and its
 * documentation is hereby granted, under NXP Semiconductors' and its
 * licensor's relevant copyrights in the software, without fee, provided that it
 * is used in conjunction with NXP Semiconductors microcontrollers.  This
 * copyright, permission, and disclaimer notice must appear in all copies of
 * this code.
 */
#include "chip.h"
#include "string.h"
#include <stdio.h>
#include "types.h"
#include "lpc112x_api.h"

#include <stdio.h>
#include <rt_misc.h>


#define DEBUG_ENABLE            

LPC_UART_T    *printf_uart_no = NULL;

void printf_config(uart_no_t uart_no, gpio_no_t tx_gpio_no, gpio_no_t rx_gpio_no)
{
	LPC_UART_T    *p_112x_uart;
	bool	printf_config_stauts = false;
	
	switch(uart_no)
	{
	  case uart0 : p_112x_uart = LPC_UART0;
		             printf_uart_no = LPC_UART0;
		
								 if ( tx_gpio_no == gpio_1_07){
									Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO1_7, (IOCON_FUNC1 | IOCON_MODE_INACT)); /* TXD */
									printf_config_stauts = true;
								 }
		
								 if ( tx_gpio_no == gpio_2_08){
									Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO2_8, (IOCON_FUNC2 | IOCON_MODE_INACT)); /* TXD */
									printf_config_stauts = true;
								 }
		
								 if ( tx_gpio_no == gpio_3_05){
									Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO3_5, (IOCON_FUNC2 | IOCON_MODE_INACT)); /* TXD */
									printf_config_stauts = true;
								 }
								 
								 if( printf_config_stauts ){
									 if ( rx_gpio_no == gpio_1_06 ){
										Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO1_6, (IOCON_FUNC1 | IOCON_MODE_INACT)); /* RXD */
										printf_config_stauts = true;
									 }
									 if ( rx_gpio_no == gpio_2_07 ){
										Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO2_7, (IOCON_FUNC2 | IOCON_MODE_INACT)); /* RXD */
										printf_config_stauts = true;
									 }
									 if ( rx_gpio_no == gpio_3_04 ){
										Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO3_4, (IOCON_FUNC2 | IOCON_MODE_INACT)); /* RXD */
										printf_config_stauts = true;
									 }
								 }

			           break;
		
		case uart1 : p_112x_uart = LPC_UART1;
		             printf_uart_no = LPC_UART1;
								 if ( (tx_gpio_no == gpio_0_06) && (rx_gpio_no == gpio_0_07)){
									Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO0_6, (IOCON_FUNC3 | IOCON_MODE_INACT)); /* TXD */
									Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO0_7, (IOCON_FUNC3 | IOCON_MODE_INACT)); /* RXD */
									printf_config_stauts = true;
								 }
			           break;
		
		case uart2 : p_112x_uart = LPC_UART2;
		             printf_uart_no = LPC_UART2;
								 if ( (tx_gpio_no == gpio_1_08) && (rx_gpio_no == gpio_0_03)){
									Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO1_8, (IOCON_FUNC3 | IOCON_MODE_INACT)); /* TXD */
									Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO0_3, (IOCON_FUNC3 | IOCON_MODE_INACT)); /* RXD */
									printf_config_stauts = true;
			           }
								 break;
		
		default:     break;
	
	}
	if ( printf_config_stauts ){
		/* Setup UART for 115.2K8N1 */
		Chip_UART_Init(p_112x_uart);
		Chip_UART_SetBaud(p_112x_uart, 115200);
		Chip_UART_ConfigData(p_112x_uart, (UART_LCR_WLEN8 | UART_LCR_SBS_1BIT));
		Chip_UART_SetupFIFOS(p_112x_uart, (UART_FCR_FIFO_EN | UART_FCR_TRG_LEV2));
		Chip_UART_TXEnable(p_112x_uart);
	}
}

/* Keil (Realview) support */
#if defined(__CC_ARM)

#if defined(DEBUG_ENABLE)
#if defined(DEBUG_SEMIHOSTING)
#define ITM_Port8(n)    (*((volatile unsigned char *) (0xE0000000 + 4 * n)))
#define ITM_Port16(n)   (*((volatile unsigned short *) (0xE0000000 + 4 * n)))
#define ITM_Port32(n)   (*((volatile unsigned long *) (0xE0000000 + 4 * n)))

#define DEMCR           (*((volatile unsigned long *) (0xE000EDFC)))
#define TRCENA          0x01000000


/* Write to SWO */
void _ttywrch(int ch)
{
	if (DEMCR & TRCENA) {
		while (ITM_Port32(0) == 0) {}
		ITM_Port8(0) = ch;
	}
}

#else
static INLINE void BoardOutChar(char ch)
{
	Chip_UART_SendBlocking(printf_uart_no, &ch, 1);
}

#endif /* defined(DEBUG_SEMIHOSTING) */
#endif /* defined(DEBUG_ENABLE) */

struct __FILE {
	int handle;
};

FILE __stdout;
FILE __stdin;
FILE __stderr;

void *_sys_open(const char *name, int openmode)
{
	return 0;
}

int fputc(int c, FILE *f)
{
#if defined(DEBUG_ENABLE)
#if defined(DEBUG_SEMIHOSTING)
	_ttywrch(c);
#else
	BoardOutChar((char) c);
#endif
#endif
	return 0;
}

/* Gets a character from the UART, returns EOF if no character is ready */
int Board_UARTGetChar(void)
{
#if defined(DEBUG_ENABLE)
	uint8_t data;

	if (Chip_UART_Read(printf_uart_no, &data, 1) == 1) {
		return (int) data;
	}
#endif
	return EOF;
}

int fgetc(FILE *f)
{
#if defined(DEBUG_ENABLE) && !defined(DEBUG_SEMIHOSTING)
	return Board_UARTGetChar();
#else
	return 0;
#endif
}

int ferror(FILE *f)
{
	return EOF;
}

void _sys_exit(int return_code)
{
label:
	__WFI();
	goto label;	/* endless loop */
}

#endif /* defined (__CC_ARM) */

/* IAR support */
#if defined(__ICCARM__)
/*******************
 *
 * Copyright 1998-2003 IAR Systems.  All rights reserved.
 *
 * $Revision: 30870 $
 *
 * This is a template implementation of the "__write" function used by
 * the standard library.  Replace it with a system-specific
 * implementation.
 *
 * The "__write" function should output "size" number of bytes from
 * "buffer" in some application-specific way.  It should return the
 * number of characters written, or _LLIO_ERROR on failure.
 *
 * If "buffer" is zero then __write should perform flushing of
 * internal buffers, if any.  In this case "handle" can be -1 to
 * indicate that all handles should be flushed.
 *
 * The template implementation below assumes that the application
 * provides the function "MyLowLevelPutchar".  It should return the
 * character written, or -1 on failure.
 *
 ********************/

#include <yfuns.h>

#if defined(DEBUG_ENABLE) && !defined(DEBUG_SEMIHOSTING)

_STD_BEGIN

#pragma module_name = "?__write"

/*
   If the __write implementation uses internal buffering, uncomment
   the following line to ensure that we are called with "buffer" as 0
   (i.e. flush) when the application terminates. */
size_t __write(int handle, const unsigned char *buffer, size_t size)
{
#if defined(DEBUG_ENABLE)
	size_t nChars = 0;

	if (buffer == 0) {
		/*
		   This means that we should flush internal buffers.  Since we
		   don't we just return.  (Remember, "handle" == -1 means that all
		   handles should be flushed.)
		 */
		return 0;
	}

	/* This template only writes to "standard out" and "standard err",
	   for all other file handles it returns failure. */
	if (( handle != _LLIO_STDOUT) && ( handle != _LLIO_STDERR) ) {
		return _LLIO_ERROR;
	}

	for ( /* Empty */; size != 0; --size) {
		BoardOutChar(*buffer++);
		++nChars;
	}

	return nChars;
#else
	return size;
#endif /* defined(DEBUG_ENABLE) */
}

_STD_END
#endif

#endif /* defined (__ICCARM__) */

#if defined( __GNUC__ )
/* Include stdio.h to pull in __REDLIB_INTERFACE_VERSION__ */
#include <stdio.h>

#if defined(__NEWLIB__)
	#define WRITEFUNC _write
	#define READFUNC _read
#else
#if (__REDLIB_INTERFACE_VERSION__ >= 20000)
/* We are using new Redlib_v2 semihosting interface */
	#define WRITEFUNC __sys_write
	#define READFUNC __sys_readc
#else
/* We are using original Redlib semihosting interface */
	#define WRITEFUNC __write
	#define READFUNC __readc
#endif
#endif /* __NEWLIB__ */

#if defined(DEBUG_ENABLE)
#if defined(DEBUG_SEMIHOSTING)
/* Do nothing, semihosting is enabled by default in LPCXpresso */
#endif /* defined(DEBUG_SEMIHOSTING) */
#endif /* defined(DEBUG_ENABLE) */

#if !defined(DEBUG_SEMIHOSTING)
int WRITEFUNC(int iFileHandle, char *pcBuffer, int iLength)
{
#if defined(DEBUG_ENABLE)
	unsigned int i;
	for (i = 0; i < iLength; i++) {
		BoardOutChar(pcBuffer[i]);
	}
#endif

	return iLength;
}

#if !defined(__NEWLIB__)
/* Called by bottom level of scanf routine within RedLib C library to read
   a character. With the default semihosting stub, this would read the character
   from the debugger console window (which acts as stdin). But this version reads
   the character from the LPC1768/RDB1768 UART. */
int READFUNC(void)
{
#if defined(DEBUG_ENABLE)
	int c = Board_UARTGetChar();
	return c;

#else
	return (int) -1;
#endif
}
#else
/*
 * **WARNING**: THIS FUNCTION IS NON-BLOCKING
 * Not just STDIN all inputs handled via UART
 * Read function for newlib is added as a non-blocking function
 * the application should check for the size to identify the number
 * of bytes received */
int READFUNC(int iFileHandle, char *pcBuffer, int iLength)
{
	int idx;
	for (idx = 0; idx < iLength; idx++) {
		int c = Board_UARTGetChar();
		if (c == EOF) break;
		pcBuffer[idx] = c;
	}
	return idx;
}
#endif

#endif /* !defined(DEBUG_SEMIHOSTING) */
#endif /* defined ( __GNUC__ ) */
