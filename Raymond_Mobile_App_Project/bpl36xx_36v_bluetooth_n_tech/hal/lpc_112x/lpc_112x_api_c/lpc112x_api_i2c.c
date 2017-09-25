#include "chip.h"
#include "string.h"
#include <stdio.h>
#include "types.h"
#include "lpc112x_api.h"


/* I2CM transfer record */
static I2C_XFER_T  i2cmXferRec;

#define I2C_FASTPLUS_BIT     0

/*******************************************

*****************************************/

void i2c_master_config(i2c_mas_config_t *p_i2c_no, gpio_no_t gpio_scl, gpio_no_t gpio_sda)
{
	I2C_ID_T i2c_num;
	uint32_t jk;
	jk = p_i2c_no->i2c_no;
	switch(jk){
		case i2c_0:
		     i2c_num = I2C0 ;break;	
		default:		
					break;
	}	
	
	if((gpio_scl == gpio_0_04) && (gpio_sda == gpio_0_05))
	{   	
		Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO0_4, IOCON_FUNC1 | I2C_FASTPLUS_BIT);
		Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO0_5, IOCON_FUNC1 | I2C_FASTPLUS_BIT);		
	}
	/* Initialize I2C */
	Chip_I2C_Init(i2c_num);
	Chip_I2C_SetClockRate(i2c_num, p_i2c_no->i2c_frq);
	Chip_I2C_SetMasterEventHandler(i2c_num, Chip_I2C_EventHandlerPolling);
}

uint8 i2c_write_blocking(uint8 i2c_no, i2c_tx_buff_t *p_i2c_tx_buff)
{
	I2C_ID_T  i2c_num;
	switch(i2c_no){
		case i2c_0:
		     i2c_num = I2C0 ;break;	
		default:
					break;
	}
	
	i2cmXferRec.slaveAddr = p_i2c_tx_buff->Slave_Addr_WR ;
	i2cmXferRec.status = I2C_STATUS_DONE;
	i2cmXferRec.txSz = p_i2c_tx_buff->tx_buff.buff_len;
	i2cmXferRec.rxSz = I2C_STATUS_DONE;
	i2cmXferRec.txBuff = p_i2c_tx_buff->tx_buff.p_buff;

	Chip_I2C_MasterTransfer(i2c_num, &i2cmXferRec);

	if (i2cmXferRec.status == I2C_STATUS_DONE)
		return true;
	else
		return false;
}

uint8 i2c_read_blocking(uint8 i2c_no, i2c_tx_buff_t *p_i2c_tx_buff, i2c_rx_buff_t *p_i2c_rx_buff)
{
  I2C_ID_T i2c_num;
	switch(i2c_no){
		case i2c_0:
		     i2c_num = I2C0;	break;	
		default:
					break;
	}
	
	/* Setup I2C transfer record */
	
	i2cmXferRec.slaveAddr = p_i2c_rx_buff->Slave_Addr_RD ;
	i2cmXferRec.status = I2C_STATUS_DONE;
	i2cmXferRec.txSz = p_i2c_tx_buff->tx_buff.buff_len;
	i2cmXferRec.rxSz = p_i2c_rx_buff->rx_buff.buff_len;
	i2cmXferRec.txBuff = p_i2c_tx_buff->tx_buff.p_buff;
	i2cmXferRec.rxBuff = p_i2c_rx_buff->rx_buff.p_buff ;
	
	Chip_I2C_MasterTransfer(i2c_num, &i2cmXferRec);
	
	if (i2cmXferRec.status == I2C_STATUS_DONE)
		return true;
	else
		return false;
}






