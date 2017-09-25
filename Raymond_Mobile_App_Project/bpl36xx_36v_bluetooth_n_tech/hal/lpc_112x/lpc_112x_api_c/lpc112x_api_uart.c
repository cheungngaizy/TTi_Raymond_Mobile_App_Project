#include "chip.h"
#include "string.h"
#include <stdio.h>
#include "types.h"
#include "lpc112x_api.h"


/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/
/* Transmit and receive ring buffers */
static RINGBUFF_T txring[3], rxring[3];

/* UART Transmision Ready Function Pointer */
static p_handler pfun_uart_tx_rdy_isr_user[3] = {NULL,NULL,NULL};

/*****************************************************************************
 * Public functions
 ****************************************************************************/
/**
 * @brief	UART0 interrupt handler using ring buffers
 * @return	Nothing
 */
void UART_IRQHandler(void)
{
	Chip_UART_IRQRBHandler(LPC_UART0, &rxring[0], &txring[0]);
	
	if ((Chip_UART_ReadLineStatus(LPC_UART0) & UART_LSR_THRE) != 0) {
		if (RingBuf_GetUsedBytes(&txring[0]) == 0){
			if(pfun_uart_tx_rdy_isr_user[0] != NULL){
			   pfun_uart_tx_rdy_isr_user[0](0);
			}
		}
	}
}

/**
 * @brief	UART1 interrupt handler using ring buffers
 * @return	Nothing
 */
void UART1_IRQHandler(void)
{
	Chip_UART_IRQRBHandler(LPC_UART1, &rxring[1], &txring[1]);
	
	if ((Chip_UART_ReadLineStatus(LPC_UART1) & UART_LSR_THRE) != 0) {
		if (RingBuf_GetUsedBytes(&txring[1]) == 0){
			if(pfun_uart_tx_rdy_isr_user[1] != NULL){
			   pfun_uart_tx_rdy_isr_user[1](0);
			}
		}
	}
}

/**
 * @brief	UART2 interrupt handler using ring buffers
 * @return	Nothing
 */
void UART2_IRQHandler(void)
{
	Chip_UART_IRQRBHandler(LPC_UART2, &rxring[2], &txring[2]);
	
	if ((Chip_UART_ReadLineStatus(LPC_UART2) & UART_LSR_THRE) != 0) {
		if (RingBuf_GetUsedBytes(&txring[2]) == 0){
			if(pfun_uart_tx_rdy_isr_user[2] != NULL){
			   pfun_uart_tx_rdy_isr_user[2](0);
			}
		}
	}
}


/**
 * @brief	UART Configuration
 * @param	p_uart_t: UART Configuration structure
 * @param	tx_gpio_no: UART Transmision PIN
 * @param rx_gpio_no: UART Receiving PIN
 * @return	Nothing
 */
void uart_config(uart_config_t *p_uart_t, gpio_no_t tx_gpio_no, gpio_no_t rx_gpio_no)
{
	LPC_UART_T    *p_112x_uart; 
  uint8 DataLen = 0, parity = 0, stoplen = 0;
	IRQn_Type Uart_Irq;
	bool	printf_config_stauts = false;
	
	switch(p_uart_t->uart_no)
	{
	  case uart0 : p_112x_uart = LPC_UART0;
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
								 if ( (tx_gpio_no == gpio_0_06) && (rx_gpio_no == gpio_0_07)){
									Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO0_6, (IOCON_FUNC3 | IOCON_MODE_INACT)); /* TXD */
									Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO0_7, (IOCON_FUNC3 | IOCON_MODE_INACT)); /* RXD */
									printf_config_stauts = true;
								 }
			           break;
		
		case uart2 : p_112x_uart = LPC_UART2;
								 if ( (tx_gpio_no == gpio_1_08) && (rx_gpio_no == gpio_0_03)){
									Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO1_8, (IOCON_FUNC3 | IOCON_MODE_INACT)); /* TXD */
									Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO0_3, (IOCON_FUNC3 | IOCON_MODE_INACT)); /* RXD */
									printf_config_stauts = true;
			           }
								 break;
		
		default:     break;
	
	}


	switch(p_uart_t->uart_no)
	{
	  case uart0 : p_112x_uart = LPC_UART0;
		             Uart_Irq = UART0_IRQn;
			           break;
		case uart1 : p_112x_uart = LPC_UART1;
		             Uart_Irq = UART1_IRQn;
			           break;
		case uart2 : p_112x_uart = LPC_UART2;
		             Uart_Irq = UART2_IRQn;
			           break;
		default:     break;
	
	}
	
	switch(p_uart_t ->bitlen)
	{
	  case 0: DataLen = UART_LCR_WLEN7;
			     break;
		case 1: DataLen = UART_LCR_WLEN8;
			     break;
		default:
			     break;
	
	}	
	
	switch(p_uart_t->parity)
  {
	  case 0: parity = UART_LCR_PARITY_DIS;
			      break;
		case 1: parity = UART_LCR_PARITY_EVEN;
			      break;
		case 2: parity = UART_LCR_PARITY_ODD;
			      break;
		default:
			      break;
	
	}
	
	switch(p_uart_t->stop_len)
  {
	  case 0: stoplen = UART_LCR_SBS_1BIT;
			      break;
		case 1: stoplen = UART_LCR_SBS_2BIT;
			      break;
		default:
			      break;
	
	}

	/* Setup UART for 115.2K8N1 */
	Chip_UART_Init(p_112x_uart);
	Chip_UART_SetBaud(p_112x_uart, p_uart_t->baudrate);
	Chip_UART_ConfigData(p_112x_uart, ( DataLen | parity | stoplen));
	Chip_UART_SetupFIFOS(p_112x_uart, (UART_FCR_FIFO_EN | UART_FCR_TRG_LEV2));
	Chip_UART_TXEnable(p_112x_uart);

	/* Before using the ring buffers, initialize them using the ring buffer init function */
	RingBuf_Init(&rxring[p_uart_t->uart_no], p_uart_t->rx_buff.p_buff, p_uart_t->rx_buff.buff_len);
	RingBuf_Init(&txring[p_uart_t->uart_no], p_uart_t->tx_buff.p_buff, p_uart_t->tx_buff.buff_len);
	 
	//RingBuffer_Init(&rxring, rxbuff, 1, UART_RRB_SIZE); remove by WPI Wecan Chen 2017.02.22
	//RingBuffer_Init(&txring, txbuff, 1, UART_SRB_SIZE); remove by WPI Wecan Chen 2017.02.22

	/* Enable receive data and line status interrupt */
	Chip_UART_IntEnable(p_112x_uart, (UART_IER_RBRINT | UART_IER_RLSINT));

	/* preemption = 1, sub-priority = 1 */
	NVIC_SetPriority(Uart_Irq, 1);
	NVIC_EnableIRQ(Uart_Irq);
}

/**
 * @brief	UART Send In Non-blocking Way
 * @param	uart_no: UART Module Number
 * @param	p_tx_uart_buff: UART Transmission Buffer Pointer
 * @return	Nothing
 */
void uart_send_non_blocking(uart_no_t uart_no, buff_t *p_tx_uart_buff)
{
	LPC_UART_T    *p_112x_uart;
	
	switch(uart_no)
	{
	  case uart0 : p_112x_uart = LPC_UART0;
			           break;
		case uart1 : p_112x_uart = LPC_UART1;
			           break;
		case uart2 : p_112x_uart = LPC_UART2;
			           break;
		default:     break;
	
	}
	
  Chip_UART_SendRB(p_112x_uart, &txring[uart_no],  p_tx_uart_buff->p_buff, p_tx_uart_buff->buff_len);
  
}

/**
 * @brief	UART Transmission Ready ISR Callback Register
 * @param	uart_no: UART Module Number
 * @param	uart_tx_rdy_isr_user: UART Transmission Ready ISR USER Function Pointer
 * @return	Nothing
 */
void uart_send_tx_ready_isr_callback_register(uart_no_t uart_no, p_handler uart_tx_rdy_isr_user)
{

  pfun_uart_tx_rdy_isr_user[uart_no] = uart_tx_rdy_isr_user;

}

/**
 * @brief	UART Transmission Buffer Empty
 * @param	uart_no: UART Module Number
 * @return 1: Empty, 0: Not Empty 	
 */
uint8 uart_tx_buff_emty(uart_no_t uart_no)
{
  uint8 Uart_Tx_Free_Flag = 0;
	
	if (RingBuf_GetUsedBytes(&txring[uart_no]) == 0)
	{
	  Uart_Tx_Free_Flag = 1;
	}
	else
	{
	  Uart_Tx_Free_Flag = 0;
	}

	return(Uart_Tx_Free_Flag);	
}

/**
 * @brief	Get UART Receiving Buffer Length
 * @param	uart_no: UART Module Number
 * @return Receiving Buffer Length
 */
uint16 uart_get_rx_buffer_data_len(uart_no_t uart_no)
{
	uint16_t len = 0;
	
	len = RingBuf_GetUsedBytes(&rxring[uart_no]);
	return len;

}

/**
 * @brief	Read the data in Receiving Buffer
 * @param	uart_no: UART Module Number
 * @param	p_rx_uart_buff: UART Receiving Buffer Pointer
 * @return	Nothing
 */
void uart_read_rx_buffer(uart_no_t uart_no, buff_t *p_rx_uart_buff)
{
  uint8_t buff[100];       

  RingBuf_Read(&rxring[uart_no], &buff[0], p_rx_uart_buff->buff_len);
	memcpy(p_rx_uart_buff->p_buff, &buff[0], p_rx_uart_buff->buff_len);
  
}

/**
 * @brief	Read the data in Receiving Buffer but don't delete
 * @param	uart_no: UART Module Number
 * @param	pcopyBuf: copy Buffer Pointer
 * @param	bytes: bytes to copy
 * @return	Nothing
 */
void uart_ReadRxBufferOnly(uart_no_t uart_no, uint8_t* pCopyBuf, uint16_t bytes)
{
	/* copy only */
  	RingBuf_Copy(&rxring[uart_no], pCopyBuf, bytes);	  
}

/**
 * @brief	delete bytes
 * @param	uart_no: UART Module Number
 * @param	byteLen: bytes to delete
 * @return	Nothing
 */
void uart_DeleteRxBufferBytes(uart_no_t uart_no, uint16_t byteLen)
{
	RingBuf_Free(&rxring[uart_no], byteLen);
}

