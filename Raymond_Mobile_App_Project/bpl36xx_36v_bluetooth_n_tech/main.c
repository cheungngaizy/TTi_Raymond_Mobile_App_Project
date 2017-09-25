//************************************************************************************** 
//*                                                                         
//*   FILE NAME:        main.c
//*   SUBSYSTEM:        
//*                                                                         
//*   DESCRIPTION:      
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
//*  2017-03-01   Justus Lian / Nicholas              create file,
//*																										version: V00.01.0000: Based on LPC824 
//*	 2017-03-14   Justus Lian / Nicholas              create file,
//*																										version: V00.02.0000: Based on LPC1124
//*	 2017-03-14   Nicholas              							version: V00.03.0000            
//*  2017-03-17   Nicholas              							version: V00.04.0000: add BT N Tech
//*  2017-03-29   Nicholas              							version: V00.05.0000: add MOSFET OTP, Moving Average SOC...
//*-------------------------------------------------------------------


#include <string.h>
#include <stdio.h>
#include "chip.h"
#include "types.h"
#include "lpc112x_api.h"



#define RING_TX_BUFFER_SIZE				(32)
#define RING_RX_BUFFER_SIZE				(32)

#define CHG_COM_LEAD_CODE_TIME   		(4.5 * 10)							// In ms, value = setting * 10	
#define CHG_COM_HIGH_TIME_LOGIC1		(1.5 * 10)
#define CHG_COM_HIGH_TIME_LOGIC0		(0.5 * 10)
#define CHG_COM_LOW_TIME				(0.5 * 10)
#define CHG_COM_BITLENTH				(32)

#define SYS_TICK_INTERVAL				(1)									
#define SYS_TICK_INTERVAL_COUNTER		(SYS_TICK_INTERVAL*10)				

#define BMS_TASK_INTERVAL        		(10)		
#if (BMS_TASK_INTERVAL % SYS_TICK_INTERVAL != 0)
#error "SYS_TICK_INTERVAL doesn't match with BMS_TASK_INTERVAL!"
#endif

#define A_TECH_36V_TASK_INTERVAL		(100)
#if (A_TECH_36V_TASK_INTERVAL % SYS_TICK_INTERVAL != 0)
#error "SYS_TICK_INTERVAL doesn't match with A_TECH_36V_TASK_INTERVAL!"
#endif

#define BT_COM_TASK_INTERVAL			(5)							
#if (BT_COM_TASK_INTERVAL % SYS_TICK_INTERVAL != 0)
#error "SYS_TICK_INTERVAL doesn't match with BT_COM_TASK_INTERVAL!"
#endif

#define D_TECH_COM_TASK_INTERVAL		(5)	
#if (D_TECH_COM_TASK_INTERVAL % SYS_TICK_INTERVAL != 0)
#error "SYS_TICK_INTERVAL doesn't match with D_TECH_COM_TASK_INTERVAL!"
#endif

#define PWR_TOOL_COM_TASK_INTERVAL      (10)
#if (PWR_TOOL_COM_TASK_INTERVAL % SYS_TICK_INTERVAL != 0)
#error "SYS_TICK_INTERVAL doesn't match with PWR_TOOL_COM_TASK_INTERVAL!"
#endif

#define LED_TASK_INTERVAL        		(500)	
#if (LED_TASK_INTERVAL % SYS_TICK_INTERVAL != 0)
#error "SYS_TICK_INTERVAL doesn't match with LED_TASK_INTERVAL!"
#endif

#define BLACK_BOX_TASK_INTERVAL			(1000)
#if (BLACK_BOX_TASK_INTERVAL % SYS_TICK_INTERVAL != 0)
#error "SYS_TICK_INTERVAL doesn't match with BLACK_BOX_TASK_INTERVAL!"
#endif

#define DEBUG_TASK_INTERVAL				(1000)
#if (DEBUG_TASK_INTERVAL % SYS_TICK_INTERVAL != 0)
#error "SYS_TICK_INTERVAL doesn't match with DEBUG_TASK_INTERVAL!"
#endif


#define AFE_I2C_CRC_FORMULAR			(0x0107) 
//#define AFE_ADDR_WRITE           		(0x54)
//#define AFE_ADDR_READ            		(0x55)
#define AFE_ADDR_WRITE           		(0x2A)
#define AFE_ADDR_READ            		(0x2A)

#define AFE_CELL1_REG_ADDR       		(0x23)
#define AFE_PACK_CURR_REG_ADDR   		(0x2D)
#define AFE_CHIP_TEMP_REG_ADDR   		(0x1F)
#define AFE_EVENT_REG_ADDR       		(0x02)
#define AFE_MASK_REG_ADDR        		(0x03)
#define AFE_WATCH_DOG_REG_ADDR   		(0x0D)

#define CURRENT_SENSOR_RESISTOR  		(0.5 * 10)    				 		// 1mOhm , unit in 0.1 mOhm , value = setting*10

#define ADC_12BIT_VAL            		(4096)
#define ABSOLUTE_TMEP_OFFSET     		(273) 								// 273 absulte 0 degree		

#define NTC_DIVIDER_RESISTANCE			(10000)								// In Ohm
#define NTC_VCC_VOLT_VAL            	(3300)								// In mV
#define NTC_TEMP_UPPER_LIMIT        	(ABSOLUTE_TMEP_OFFSET + 110)			// Absolute Temperature, 110 Degree C
#define NTC_TEMP_LOWER_LIMIT        	(ABSOLUTE_TMEP_OFFSET - 25)			// Absolute Temperature, -25 Degree C
#define NTC_RESISTENCE_UPPER_LIMIT  	(124112)        					// In Ohm, NTC Resistance at -25 Degree C
#define NTC_RESISTENCE_LOWER_LIMIT  	(448)								// In Ohm, NTC Resistance at 110 Degree C

//Discharge Macro
#define DSG_LVP_PACK_VOLT        		(25000)								// In mV
#define DSG_LVP_CELL_VOLT        		(2500)								// In mV
#define DSG_OTP_TEMP_VAL         		(ABSOLUTE_TMEP_OFFSET + 70)			// 70 Degree C
#define DSG_OTP_RELEASE_TEMP_VAL 		(ABSOLUTE_TMEP_OFFSET + 55)			// 60 Degree C
#define DSG_LTP_TEMP_VAL         		(ABSOLUTE_TMEP_OFFSET - 20)			// -20 Degree C
#define DSG_LTP_RELEASE_TEMP_VAL 		(ABSOLUTE_TMEP_OFFSET - 15)			// -15 Degree C

#define DSG_OTP_MOSFET_TEMP_VAL 		(ABSOLUTE_TMEP_OFFSET + 105)		// 105 Degree C
#define DSG_OTP_MOSFET_RELEASE_TEMP_VAL (ABSOLUTE_TMEP_OFFSET + 90)			// 90 Degree C

#define DSG_DOC_LVL2             		(120*1000)							// unit in mA , value = setting*1000
#define DSG_DOC_LVL1_DUAL        		(60*1000)							// unit in mA , value = setting*1000

//Charge Macro
#define CHG_OTP_TEMP_VAL         		(ABSOLUTE_TMEP_OFFSET + 50)			// 50 Degree C
#define CHG_LTP_TEMP_VAL         		(ABSOLUTE_TMEP_OFFSET + 0)			// 0 Degree C
#define CHG_LTC_TEMP_VAL         		(ABSOLUTE_TMEP_OFFSET + 10)			// 10 Degree C
#define CHG_OVP_PACK_VOLT        		(42200)								// In mV
#define CHG_OVP_CELL_VOLT        		(4220)							
#define CHG_FULL_PACK_VOLT       		(40500)							
#define CHG_FULL_CELL_VOLT       		(4050)
#define CHG_LOW_VOLT_FAULT_PACK_VOLT	(10000)
#define CHG_LOW_VOLT_FAULT_CELL_VOLT	(1000)
#define CHG_CC_MODE_PACK_VOLT    		(30000)
#define CHG_CC_MODE_CELL_VOLT    		(3000)
#define CHG_CC_FULL_PACK_VOLT    		(41800)
#define CHG_CC_FULL_CELL_VOLT    		(4180)
#define CHG_CHARING_CURR_UNIT			(50)								// Current Unit
#define CHG_0A_CHG_CURRENT       		(0)
#define CHG_LOW_TEMP_CHG_CURRENT 		(750 / CHG_CHARING_CURR_UNIT)  		// value(mA) = setting/CHG_CHARING_CURR_UNIT
#define CHG_LOW_VOLT_CHG_CURRENT 		(750 / CHG_CHARING_CURR_UNIT)
#define CHG_FAST_CHG_CURRENT     		(3400 / CHG_CHARING_CURR_UNIT)		// 3400: 3400 mA
#define CHG_FULL_CHG_CURRENT     		(250 / CHG_CHARING_CURR_UNIT)		// 250: mA
#define CHG_CC_MODE_TIME_VAL     		(1000)								// 1000ms  
#define CHG_FULL_CHG_TIME_VAL    		(18000)								// 18000ms

#define I2C_CLOCK						(100000) 							// In Hz
#define CELL_AMOUNT              		(10)
#define BMS_READ_INFO_INTERVAL			(100)  
#define BMS_FEED_WTD_INTERVAL			(500) 

#define FG_BELOW_25_PERCENT_PACK_VOLT	(33990)
#define FG_BELOW_25_PERCENT_CELL_VOLT 	(3399)
#define FG_25_PERCENT_PACK_VOLT       	(36120)
#define FG_25_PERCENT_CELL_VOLT       	(3612)
#define FG_50_PERCENT_PACK_VOLT       	(37170)
#define FG_50_PERCENT_CELL_VOLT       	(3717)
#define FG_75_PERCENT_PACK_VOLT       	(39580)
#define FG_75_PERCENT_CELL_VOLT       	(3958)

#define ADC_CHL_CELL_TEMP_PIN			gpio_1_00
#define ADC_CHL_CELL_TEMP        		adc6

#define ADC_CHL_MOSFET_TEMP_PIN			gpio_1_01
#define ADC_CHL_MOSFET_TEMP      		adc5

#define ADC_CHL_PACK_DSG_CURR_PIN		gpio_1_11
#define ADC_CHL_PACK_DSG_CURR    		adc1

#define ADC_CHL_PACK_VOLT_PIN			gpio_1_02
#define ADC_CHL_PACK_VOLT        		adc4

#define CHG_COM_PIN						gpio_2_03
#define LDO_ENABLE_PIN					gpio_1_04
#define CHG_MOSFET_ENABLE_PIN			gpio_2_08
#define DSG_MOSFET_ENABLE_PIN			gpio_3_03
#define NTC_PWR_ENABLE_PIN				gpio_2_06
#define T2_PWR_TOOL_WRITE     			gpio_2_09 
#define LED_BT_RED_PIN					gpio_2_00 
#define LED_BT_BLUE_PIN					gpio_3_04

#define LED4_PIN               			gpio_3_00
#define LED3_PIN               			gpio_3_05
#define LED2_PIN             		    gpio_2_05
#define LED1_PIN						gpio_1_05

#define LOAD_DETECT_INTERRUPT_PIN		gpio_0_02
#define LOAD_DETECT_PIN_INT				pin_int2

#define CHG_DETECT_T1_INTERRUPT_PIN		gpio_0_08
#define CHG_DETECT_T1_PIN_INT			pin_int8

#define DOC_DETECT_INTERRUPT_PIN		gpio_0_09
#define DOC_DETECT_PIN_INT				pin_int9

#define T2_PWR_TOOL_READ_INTERRUPT_PIN	gpio_2_07
#define T2_PWR_TOOL_READ_PIN_INT		pin_int31

#define FUEL_GAUGE_KEY_INTERRUPT_PIN	gpio_0_11
#define FUEL_GAUGE_KEY_PIN_INT			pin_int11


#define I2C_CLK_PIN						gpio_0_04
#define I2C_SDA_PIN              		gpio_0_05
#define BT_UART2_TX_PIN          		gpio_1_08  	
#define BT_UART2_RX_PIN					gpio_0_03  	
#define D_TECH_UART1_TX_PIN    		    gpio_0_06	
#define D_TECH_UART1_RX_PIN         	gpio_0_07	

#define DEBUG_UART0_TX_PIN				gpio_1_07	
#define DEBUG_UART0_RX_PIN				gpio_1_06	

#define BALCKBOX_SSP1_SCK				gpio_2_01 			
#define BALCKBOX_SSP1_MISO				gpio_1_10
#define BALCKBOX_SSP1_MOSI				gpio_1_09	
#define BALCKBOX_SSP1_SSEL				gpio_2_04

#define BT_COM_WAKEUP_INTERRUPT_PIN		gpio_2_02
#define ISP_DETECT_INPUT_PIN			gpio_0_01


#define LDO_ENABLE()             		gpio_output(LDO_ENABLE_PIN, 1)
#define LDO_DISABLE()            		gpio_output(LDO_ENABLE_PIN, 0)

#define CHG_ENABLE()            		gpio_output(CHG_MOSFET_ENABLE_PIN, 1)
#define CHG_DISABLE()            		gpio_output(CHG_MOSFET_ENABLE_PIN, 0)

#define DSG_ENABLE()             		gpio_output(DSG_MOSFET_ENABLE_PIN, 1)
#define DSG_DISABLE()            		gpio_output(DSG_MOSFET_ENABLE_PIN, 0)

#define PWR_SW_ENABLE()          		gpio_output(NTC_PWR_ENABLE_PIN, 1)
#define PWR_SW_DISABLE()         		gpio_output(NTC_PWR_ENABLE_PIN, 0)

#define REPLY_DSG_ENABLE()       		gpio_output(T2_PWR_TOOL_WRITE, 1)
#define REPLY_DSG_DISABLE()      		gpio_output(T2_PWR_TOOL_WRITE, 0)

#define BT_DETECT_ENABLE()       		gpio_output(LED_BT_RED_PIN, 1)
#define BT_DETECT_DISABLE()      		gpio_output(LED_BT_RED_PIN, 0)

#define LED4_ON()                		gpio_output(LED4_PIN, 0);
#define LED4_OFF()               		gpio_output(LED4_PIN, 1);
#define LED4_TOGL()              		gpio_output_toggle(LED4_PIN);

#define LED3_ON()                		gpio_output(LED3_PIN, 0);
#define LED3_OFF()               		gpio_output(LED3_PIN, 1);
#define LED3_TOGL()              		gpio_output_toggle(LED3_PIN);

#define LED2_ON()               		gpio_output(LED2_PIN, 0);
#define LED2_OFF()               		gpio_output(LED2_PIN, 1);
#define LED2_TOGL()              		gpio_output_toggle(LED2_PIN);

#define LED1_ON()                		gpio_output(LED1_PIN, 0);
#define LED1_OFF()               		gpio_output(LED1_PIN, 1);
#define LED1_TOGL()              		gpio_output_toggle(LED1_PIN);

#define LED_BT_ON()              		gpio_output(LED_BT_BLUE_PIN, 0);     
#define LED_BT_OFF()             		gpio_output(LED_BT_BLUE_PIN, 1);   
#define LED_BT_TOGL()            		gpio_output_toggle(LED_BT_BLUE_PIN);


#define PROTOCOL_ABSOLUTE_TMEP_ZERO			(300)								// 300 protocol defined 0 degree
#define PROTOCOL_TMEP_OFFSET     			(PROTOCOL_ABSOLUTE_TMEP_ZERO - ABSOLUTE_TMEP_OFFSET) 
#define PROTOCOL_CURRENT_OFFSET				(20000)								// 20000 protocol defined 0 A
#define PROTOCOL_VOLT_OFFSET				(0)

#define PROTOCOL_CHARGE_COMPLETE        	(0x0001)
#define PROTOCOL_LOW_SOC					(0x0002)
#define PROTOCOL_DEEP_DISCHARGED			(0x0004)
#define PROTOCOL_OVER_TEMPERATUR			(0x0008)
#define PROTOCOL_OVER_CURRENT				(0x0010)
#define PROTOCOL_HIGH_STORAGE_TEMP			(0x0020)
#define PROTOCOL_LOW_STORAGE_TEMP			(0x0040)
#define PROTOCOL_LOCK_STATUS				(0x0080)
#define PROTOCOL_OVER_VOLT_PROTECTION		(0x0100)
#define PROTOCOL_OTHER_FAULT				(0x0200)


#define MOVING_AVERAGE_BUFF_SIZE 			30


























/*********************************N_TECH_SART***************************************************/

#define START_DATA_VALUE						(0xAA)
#define STOP_DATA_VALUE							(0x55)

#define MAX_PAYLOAD_SIZE						(11)
#define MIN_PAYLOAD_SIZE						(0)

#define START_DATA_INDEX						(0)															// 0
#define START_DATA_LEN							(1)

#define PAYLOAD_LEN_DATA_INDEX					(START_DATA_INDEX+START_DATA_LEN) 							// 1
#define PAYLOAD_LEN_DATA_LEN					(1)

#define SEQ_ID_DATA_INDEX						(PAYLOAD_LEN_DATA_INDEX+PAYLOAD_LEN_DATA_LEN) 				// 2
#define SEQ_ID_DATA_LEN							(1)

#define SEND_ID_INDEX							(SEQ_ID_DATA_INDEX + SEQ_ID_DATA_LEN)						// 3
#define SEND_ID_DATA_LEN						(1)

#define RECEIVE_ID_INDEX						(SEND_ID_INDEX + SEND_ID_DATA_LEN)							// 4
#define RECEIVE_ID_DATA_LEN						(1)

#define OP_CODE_DATA_INDEX						(RECEIVE_ID_INDEX +RECEIVE_ID_DATA_LEN)						// 5
#define OP_CODE_DATA_LEN						(2)

#define PAYLOAD_DATA_INDEX						(OP_CODE_DATA_INDEX + OP_CODE_DATA_LEN)						// 7


#define CHECKSUM_DATA_INDEX_WITHOUT_PAYLOAD		(OP_CODE_DATA_INDEX + OP_CODE_DATA_LEN + MIN_PAYLOAD_SIZE)	// 7
#define CHECKSUM_DATA_INDEX_WITH_MAX_PAYLOAD	(OP_CODE_DATA_INDEX + OP_CODE_DATA_LEN + MAX_PAYLOAD_SIZE)	// 18 
#define CHECKSUM_DATA_LEN						(1)

#define END_DATA_INDEX_WITHOUT_PAYLOAD			(CHECKSUM_DATA_INDEX_WITHOUT_PAYLOAD + CHECKSUM_DATA_LEN)	// 8
#define END_DATA_INDEX_WITH_MAX_PAYLOAD			(CHECKSUM_DATA_INDEX_WITH_MAX_PAYLOAD + CHECKSUM_DATA_LEN)	// 19
#define END_DATA_LEN							(1)

// the RX buffer should have enough data to worth a check , head + tail + empty payload
#define MIN_DATA_PACKET_SIZE					(END_DATA_INDEX_WITHOUT_PAYLOAD + END_DATA_LEN) 			// 9
// head + tail + possible max payload(Based on BT Data frame length)
#define MAX_DATA_PACKET_SIZE					(END_DATA_INDEX_WITH_MAX_PAYLOAD + END_DATA_LEN)			// 20

#define N_TECH_WAKE_UP_DATA_LEN         (10)

#define UART_RX_BUFFER_PACKET_STORAGE			(3) 														// RX buffer can only storage this many packet received
//#define BT_TX_BUFFER_SIZE						(MAX_DATA_PACKET_SIZE) 										// only need to prepare for one packet
//#define BT_RX_BUFFER_SIZE						(MAX_DATA_PACKET_SIZE*UART_RX_BUFFER_PACKET_STORAGE)
#define BT_TX_BUFFER_SIZE						(32)														// default setting
#define BT_RX_BUFFER_SIZE						(64)														// default setting  

#define SUCESS									(1)
#define FAIL									(0)

#define N_TECH_RESPONSE_SUCESS					(0)
#define N_TECH_RESPONSE_REJECT					(1)





#define N_TECH_OP_CODE_HANDSHAKE                (0x0001)
#define N_TECH_OP_CODE_NAME_SET                	(0x0004)
#define N_TECH_OP_CODE_LOCATION_IND             (0x0005)
#define N_TECH_OP_CODE_READ_TEMPERATURE         (0x0006)

#define N_TECH_OP_CODE_LOCK_BMS                	(0x000B)
#define N_TECH_OP_CODE_UNLOCK_BMS               (0x000C)



#define N_TECH_OP_CODE_REQEST_BATT_USAGE        (0x0014)
#define N_TECH_OP_CODE_REQEST_ALERT_NOTIF       (0x0017)
#define N_TECH_OP_CODE_REQEST_CHG_DSG_CYCLE     (0x0019)
#define N_TECH_OP_CODE_NAME_READ                (0x001A)


#define N_TECH_OP_CODE_READ_BAT_PROFILE         (0x002E)











#define N_TECH_ID_PACK							(0x00)
#define N_TECH_ID_POWER_TOOL					(0x01)
#define N_TECH_ID_CHARGER						(0x02)
#define N_TECH_ID_HOST_DEVICE					(0x03)
#define N_TECH_ID_ACCESSORY						(0x04)
#define N_TECH_ID_BT_MODULE						(0x05)
#define N_TECH_ID_JIG							(0x06)


#define N_TECH_ERROR_CODE_OP_UNSUPPORTED_CMD			(0x01)
#define N_TECH_ERROR_CODE_OP_CHECKSUM_FAIL				(0x02)
#define N_TECH_ERROR_CODE_OP_HARDWARE_FAIL				(0x03)
#define N_TECH_ERROR_CODE_OP_DEVICE_LOCKED				(0x04)
#define N_TECH_ERROR_CODE_OP_DEVICE_BUSSY				(0x05)
#define N_TECH_ERROR_CODE_OP_UNSUPPORTED_DEV_ID			(0x06)
#define N_TECH_ERROR_CODE_OP_NOT_SIGNED_IN				(0x07)

#define N_TECH_CODE_OP_NO_NEED_RESPONDE					(0x0000)
#define N_TECH_CODE_OP_NEED_RESPONDE					(0x1000)
#define N_TECH_CODE_OP_RESPONDE_WITH_PAYLOAD			(0x2000)
#define N_TECH_CODE_OP_RESPONDE_WITH_FAULT_CODE			(0x3000)
#define N_TECH_CODE_OP_MULTI_PACKET_SENDER_NEED_ACK		(0x4000)
#define N_TECH_CODE_OP_MULTI_PACKET_RECEIVER_ACK		(0x5000)
#define N_TECH_CODE_OP_MULTI_PACKET_RECEIVER_NEED_ACK	(0x6000)
#define N_TECH_CODE_OP_MULTI_PACKET_SENDER_ACK			(0x7000)









uint16_t g_op_code_wait_for_response = 0;







typedef struct 
{
	
	uint8_t sequence_id;
	uint8_t send_id;
	uint8_t receive_id;
	uint8_t payload_len;
	uint16_t op_code;
	uint8_t payload_data[MAX_PAYLOAD_SIZE];
	uint8_t packet_need_response;	
	
}n_tech_data_t;



n_tech_data_t g_input_cmd = {0}; 					// data received waiting to process
n_tech_data_t g_output_cmd = {0}; 					// data waiting to send out



/************************************N_TECH_END*******************************************************/


uint16 mov_buff_max_cell_Volt[MOVING_AVERAGE_BUFF_SIZE] = {0};
uint16 mov_buff_min_cell_Volt[MOVING_AVERAGE_BUFF_SIZE] = {0};

uint8 mov_buff_max_cell_Volt_index = 0;
uint8 mov_buff_min_cell_Volt_index = 0;

uint16 mov_buff_max_cell_Volt_result = 0;
uint16 mov_buff_min_cell_Volt_result = 0;


typedef struct
{
	uint16 volt;	// mv
	uint16 soc;		// 1: 0.1%


}soc_data_t;


const soc_data_t SOC_ARRAY[] =
{
	{2496, 0},
	{3301, 5},
	{3442, 10},
	{3471, 15},
	{3528, 20},
	{3563, 25},
	{3589, 30},
	{3606, 35},
	{3619, 40},
	{3634, 45},
	{3652, 50},
	{3688, 55},
	{3734, 60},
	{3773, 65},
	{3812, 70},
	{3856, 75},
	{3907, 80},
	{3961, 85},
	{4018, 90},
	{4082, 95},
	{4173, 100}

};






typedef struct
{
	uint32 resistence;
	uint16 temp;
	
}NTC_Data_T;



NTC_Data_T NTC_array[] = 
{ 
	{ 448, 383},
	{ 593, 378},
	{ 684, 373},
	{ 791, 368},
	{ 918, 363},
	{ 1070,358 },
	{ 1254,353 },
	{ 1936,340 },
	{ 2005,339 },
	{ 2647,331 },
	{ 3106,327 },
	{ 3620,323 },
	{ 4070,320 },
	{ 4771,316 },
	{ 5171,314 },
	{ 5385,313 },
	{ 6086,310 },
	{ 6608,308 },
	{ 7177,306 },
	{ 7797,304 },
	{ 8472,302 },
	{ 9205,300 },
	{ 9595,299 },
	{ 10000,298 },
	{ 10439,297 },
	{ 11902,294 },
	{ 20768,282 },
	{ 37822,270 },
	{ 71284,258 },
	{ 124112,248 } 
	
};

typedef enum
{
	IDLE			= 0x01,
	CHARGING		= 0x02,
	DISCHARGING		= 0x04,
	
}pack_state_of_chg;



typedef union
{
	uint16 status;
	
	struct
	{
		uint8 chg_state:		3;	// D0: 1:idle, 0:not idle; D1: 1: Charging, 0: Not Charging; D2: 1:Discharging, 0: NOt discharging;
		uint8 protect_state:	1;	// 1:Protected, 0: Normal
		uint8 lock_state:		1;	// 1:Locked, 0: unlocked
		uint8 reserve1:			3;
		uint8 reserve2:			8;
		
	}st_pack_state;


}pack_status_t;


typedef struct
{
	uint16 Cell_Volt[CELL_AMOUNT];      //cell_vol[10]//cell_n=5.12V*(val[o2_reg(23:2c)] & 0xfff) / 4095 //Define Cell Number 10
	sint16 AFE_Chip_Temp;      			//temp=(val[o2_reg-1Fh] &0xF000)/16-50
	sint32 Pack_Current;       			//in 100mA //current_A=((2.56V * val[2Dh] / 4095 - 1)/10)/0.0005
	uint16 AFE_Event;          
	uint16 FET_Temp;           			//(adc3_read_val/4096)*3.3v
	uint16 Cell_Temp;          			//(adc1_read_val/4096)*3.3v
	uint16 Pack_Dsg_Current;   			//(adc0_read_val/4096)*3.3v
	uint16 Pack_Vol;          	 		//(adc10_read_val/4096)*3.3v
	uint16 soc;							//0.1AH/bit
	pack_status_t Pack_Status;					 
	uint16 alert_notif;					//D0: Charge completed; D1: Low SOC; D2: Deep Discharged; D3: Over Temp; D4: Over current;
										//D5: High storage Temp; D6: low storage temp; D7: lock_state; D8: OVP; D9: other faults 

	
}Sys_Info_T;


typedef struct
{
  uint16 charge_cycle;			// 1 LSB: 1 cycle
  uint16 dicharge_cycle;		// 1 LSB: 1 cycle
  uint32 charge_acc_time;		// 1 LSB: 1 second
  uint32 discharge_acc_time;	// 1 LSB: 1 second


}bms_log_info_t;









void BMS_Task(void);
void A_Tech_36V_COM_Task(void);
void BT_N_Tech_Task(void);
void D_Teck_Task(void);
void Pwr_Tool_Com_Task(void);
void LED_Task(void);	
void Black_Box_Task(void);
void Debug_Task(void);







typedef enum
{
	TASK_ID_BMS				= 0x00,
	TASK_ID_A_TECH_36V_COM	= 0x01,
	TASK_ID_BT_N_TECH		= 0x02,
	TASK_ID_D_TECH			= 0x03,
	TASK_ID_PWR_TOOL_COM	= 0x04,
	TASK_ID_LED				= 0x05,
    TASK_ID_BLACK_BOX		= 0x06,
    TASK_ID_DEBUG			= 0x07,	
	task_max 
	
}task_id_t;



typedef struct
{
	task_id_t task_id;
	uint16_t task_interval;
	void (*task_entry)(void);
	uint16_t task_tick_counter;	
	
}task_info_t;



volatile task_info_t task_array[] = 
{
	{TASK_ID_BMS, 10, BMS_Task, 0},							//10ms
	{TASK_ID_A_TECH_36V_COM, 100,A_Tech_36V_COM_Task,0},	//100ms
	{TASK_ID_BT_N_TECH, 5,BT_N_Tech_Task,0},				//5ms
	{TASK_ID_D_TECH,5,D_Teck_Task,0},						//5ms
	{TASK_ID_PWR_TOOL_COM,10,Pwr_Tool_Com_Task,0},			//10ms
	{TASK_ID_LED,500,LED_Task,0},							//500ms
	{TASK_ID_BLACK_BOX,1000,Black_Box_Task,0},				//1000ms
	{TASK_ID_DEBUG,1000,Debug_Task,0},						//1000ms
	{task_max,0,0,0}
	
};


#define TASK_TICK_INTERVAL		(1)//ms

void task_tick(void)
{
	uint8_t task_index = 0;
	while(task_array[task_index].task_id < task_max)
	{
		task_array[task_index].task_tick_counter += TASK_TICK_INTERVAL;
		task_index++;
	}
}


void task_scheduler(void)
{
uint8_t task_index = 0;

	while(1)
	{
	
		if(task_array[task_index].task_tick_counter >= task_array[task_index].task_interval
		&& task_array[task_index].task_entry)
		{
			if(task_array[task_index].task_tick_counter > task_array[task_index].task_interval)
			{
				//printf("task delay warning:0x0%X\n" ,task_array[task_index].task_id);
			}
			task_array[task_index].task_tick_counter = 0;
			task_array[task_index].task_entry();
		}

		
		task_index++;
		if(task_array[task_index].task_id >= task_max)
		{
			task_index = 0;
		}
	
	}
}



typedef enum
{
	PACK_RECOVER	= 0,
	PACK_FAST_CHG	= 1, 
	PACK_MOSFET_OFF	= 2, 
	PACK_FULLY_CHG	= 3, 
	PACK_MAINTEN	= 4, 
	PACK_ERROR		= 5,
	PACK_STAT_MAX
	
}Pack_Stat_t;



typedef struct
{
	uint8 crc;
	uint8 chg_curr;
	sint8 s_temp;              			//-128 Degree C to 127 degree C (Pack's Temperature Range: -20 Degree C to 80 Degree C)
	union
	{
		uint8 chg_info;
		struct
		{
			uint8 num_of_cell		: 3;
			uint8 chg_timer  		: 2;
			Pack_Stat_t pack_state	: 3; 
			
		}st_chg_info;	
		
	}un_chg_info;
	
	uint8 send_complete;	
	uint8 ack_enable;
	uint8 send_enable;
}CHG_Com_Data_T;



typedef struct
{
	uint16	cell_vol_max;				// Max Cell Volt
	uint16	cell_vol_min;				// Min Cell Volt
	uint16	cell_vol_aver;				// Aver Cell Volt
	uint8	cell_vol_max_no;
	uint8	cell_vol_min_no;

}Cell_para_t;


typedef enum
{
	LEAD_CODE_LOW	= 0,
	LEAD_CODE_HIGH	= 1,	
	BIT_LOW			= 2,
	BIT_DATA		= 3,
	SEND_COMPLT		= 4,
	SEND_SEQ_MAX
	
}Send_Seq_t;



typedef enum
{
	CHAR_INIT		= 0,
	TEMP_ERROR		= 1,
	VOLT_ERROR		= 2,
	LOW_TEMP_CHAR	= 3,
	LOW_VOLT_CHAR	= 4,
	CC_MODE			= 5,
	CV_MODE			= 6,
	FULLY_CHARD		= 7,
	MAINT_MODE		= 8,
	CHG_STAT_MAX
	
}Chg_Stat_t;



typedef enum
{
	DISCHING		= 0,
	DISC_OTP		= 1, 
	DISC_LTP		= 2, 
	DISC_LVP_LOCK	= 3, 
	DISC_DOC_LOCK	= 4,
	DISC_STAT_MAX
	
}DSG_Stat_t;
	

bms_log_info_t bms_log_info = {0};
Sys_Info_T Sys_Info = {0};
CHG_Com_Data_T CHG_Com_Data = {0};
Cell_para_t Cell_Para = {0};
Send_Seq_t SEND_SEQ;
Chg_Stat_t CHG_STAT;
DSG_Stat_t DSG_STAT;






uint8 g_DOC_SHORT_Detect = 0;
uint8 g_Dsg_Detection = 0;
uint8 g_Chg_Det_cnt = 0;
uint8 g_Chg_Detection = 0;
uint8 g_Fuel_Gauge_Detect = 0;
uint8 g_Pack_Dsg_Curr_adc_flag = 0;
uint8 g_Cell_Temp_adc_flag = 0;
uint8 g_Mosfet_Temp_adc_flag = 0;
uint8 g_Pack_Volt_adc_flag = 0;

uint8 g_i2c_checksum = 0;

Chg_Stat_t charging_state = CHAR_INIT;		// sys_control_t
DSG_Stat_t discharging_sta = DISCHING;		// sys_control_t

uint8 BT_txbuff[RING_TX_BUFFER_SIZE] = {0};
uint8 BT_rxbuff[RING_RX_BUFFER_SIZE] = {0};

uart_config_t uart_BT = 
						{
							uart2, 
							1, 
							0, 
							0, 
							br_115200, 
							{&BT_txbuff[0],RING_TX_BUFFER_SIZE},
							{&BT_rxbuff[0],RING_RX_BUFFER_SIZE}
						};


uint8 D_Tech_txbuff[RING_TX_BUFFER_SIZE] = {0};
uint8 D_Tech_rxbuff[RING_RX_BUFFER_SIZE] = {0};
 
uart_config_t uart_D_Tech = 
						{
							uart1, 
							1, 
							0, 
							0, 
							br_115200, 
							{&D_Tech_txbuff[0],RING_TX_BUFFER_SIZE},
							{&D_Tech_rxbuff[0],RING_RX_BUFFER_SIZE}
						};



void calt_max_cell_volt(uint16 *pbuf, uint8 len, uint8 *pCell, uint16 *pMaxVolt)
{
	uint16 Max=pbuf[0];
	uint8 i;
	
	*pCell = 1;
	for (i=0;i<len;i++)
	{
		if (Max<pbuf[i])
		{
			Max=pbuf[i];	
			*pCell = i+1;
		}
		
	}
	*pMaxVolt = Max;

}



void calt_min_cell_volt(uint16 *pbuf, uint8 len, uint8 *pCell, uint16 *pMinVolt)
{
	uint16 Min=pbuf[0];
	uint8 i;
	
	*pCell = 1;
	for (i=0;i<len;i++)
	{
		if (Min>pbuf[i])
		{
			Min=pbuf[i];	
			*pCell = i+1;
		}
	}

	*pMinVolt = Min;

}



void calt_aver_cell_volt(uint16 *pbuf, uint8 len, uint16 *pAverVolt)
{
	uint16 Max=pbuf[0],Min=pbuf[0],sum=0;
	uint8 i;
	
	for (i=0;i<len;i++)
	{
		if (Max<pbuf[i])
		{
			Max=pbuf[i];		
		}
		if (Min>pbuf[i])
		{
			Min=pbuf[i];		
		}
		sum += pbuf[i];
	}
	*pAverVolt = ((sum-Max-Min)*2/(len-2)+1)/2;
	
}



uint16 calt_moving_average(int new_val,uint16 *pbuf, uint8 len)
{
	static int index = 0;
	uint8 i;
	uint16 sum = 0;		
	
	if (index >= len)
	{
		index = 0;
	}
	pbuf[index] = new_val;
	for (i = 0; i < len; i++)
	{
		sum += pbuf[i];
	}
	index++;
	return(( sum*2/len+1)/2 );	
	
}



void sys_tick_count(uint8 para) 
{
   

	para = para;
	
	task_tick();

	
}



void uart_tx_free(uint8 para)
{
	para = para;

}



void DOC_detect(uint8 para)
{
	para = para;

	DSG_DISABLE();
	g_DOC_SHORT_Detect = 1;
	discharging_sta = DISC_DOC_LOCK;
		
}



void load_detect(uint8 para)
{
	para = para;
	
	g_Dsg_Detection = 1;
}



void chg_detect(uint8 para) 
{
	
	para = para;

	if(g_Chg_Det_cnt < 254)
	{
		g_Chg_Det_cnt++;            
	}
	
	
}



void fuel_gauge_detect(uint8 para)
{

	para = para;

	g_Fuel_Gauge_Detect = 1;

}

void dual_dsg_detec(uint8 para)
{
	para = para;
	
}



void Charge_Data_Com(uint8 para)  // 100 us 
{
  
	static uint16 counter = 0;
	static uint8 Shift_cnt = 0;
	static Send_Seq_t step_state = LEAD_CODE_LOW;
	static uint8 com_data[4] = {0};
	
	para = para;
	
	if(CHG_Com_Data.send_enable)
	{
			switch(step_state)
			{
				case LEAD_CODE_LOW: 
									gpio_output(CHG_COM_PIN, 1);
									counter++;													
									if(counter >= CHG_COM_LEAD_CODE_TIME)
									{
										step_state = LEAD_CODE_HIGH;
										counter = 0;
									}
									memcpy(&com_data[0],&CHG_Com_Data.crc,4);													 
									break;
									
				case LEAD_CODE_HIGH: 
									gpio_output(CHG_COM_PIN, 0);
									counter++;	
									if(counter >= CHG_COM_LEAD_CODE_TIME)
									{
										step_state = BIT_LOW;
										counter = 0;
									}			
									break;
									
				case        BIT_LOW: 
									gpio_output(CHG_COM_PIN, 1);
									counter++;	
									if(counter >= CHG_COM_LOW_TIME)
									{
										 if(Shift_cnt < CHG_COM_BITLENTH) 
										 {														 
											step_state = BIT_DATA;														
										 }
										 else
										 {
											step_state = SEND_COMPLT;																							
										 }
										 
										 counter = 0;
									}		                     
									break;
									
				case       BIT_DATA:
									gpio_output(CHG_COM_PIN, 0);
									counter++;
									if (com_data[Shift_cnt >> 3] & 0x01) 
									{
									 
										if(counter >= CHG_COM_HIGH_TIME_LOGIC1)
										{
											step_state = BIT_LOW;
											counter = 0;
											com_data[Shift_cnt >> 3 ] >>= 1; 
											Shift_cnt++;
										}
									}
									else
									{
										
										if(counter >= CHG_COM_HIGH_TIME_LOGIC0)
										{
											step_state = BIT_LOW;
											counter = 0;
											com_data[Shift_cnt >> 3] >>= 1; 
											Shift_cnt++;
										}
									}																											
									break;
									
				case    SEND_COMPLT:  
									gpio_output(CHG_COM_PIN, 0);
									counter = 0;
									Shift_cnt = 0;
									CHG_Com_Data.send_complete = 1;  
									step_state = LEAD_CODE_LOW;				
									CHG_Com_Data.send_enable = 0;
									break;
									
							default:
									break;
			}
	
	
	}
	else
  { 
		gpio_output(CHG_COM_PIN, 0);
		counter = 0;
		Shift_cnt = 0;
		step_state = LEAD_CODE_LOW;
		CHG_Com_Data.send_complete = 0;
	}
	
}




void charge_detect(uint8 para)  // 100ms
{
	static uint8 chg_pinlvl_cnt = 0;

	para = para;
	 
	if((g_Chg_Det_cnt > 0) || (gpio_input(CHG_DETECT_T1_INTERRUPT_PIN) == 0))
	{
		g_Chg_Detection = 1;
		chg_pinlvl_cnt = 0;
	}
	else
	{
		if(gpio_input(CHG_DETECT_T1_INTERRUPT_PIN))
		{
			if(chg_pinlvl_cnt < 3) 
			{
			  chg_pinlvl_cnt++;
			}
			else
			{
				if(chg_pinlvl_cnt > 0)
				{
				 g_Chg_Detection = 0;

				}
			}	
		
			
		}

	}			

	g_Chg_Det_cnt = 0;

	if(gpio_input(LOAD_DETECT_INTERRUPT_PIN) == 0)
	{
	  g_Dsg_Detection = 0;

	}
			
}



void mrt2_irq_user(uint8 para)  
{                               
	para = para;
	
}



void mrt3_irq_user(uint8 para)
{
	para = para;

}



void read_pack_dsg_curr(uint8 para)
{
	g_Pack_Dsg_Curr_adc_flag = 1;	

} 



void read_cell_temp(uint8 para)
{
	g_Cell_Temp_adc_flag = 1;	

} 



void read_mosfet_temp(uint8 para)
{
	g_Mosfet_Temp_adc_flag = 1;	
	
} 



void read_pack_volt(uint8 para)
{
	g_Pack_Volt_adc_flag = 1;

} 



void i2c_pec_calc(unsigned char dat)
{
   unsigned char i;

   g_i2c_checksum ^= dat;
   
   for (i = 0; i < 8; i++) 
   {
      if (g_i2c_checksum & 0x80)
      {
      	g_i2c_checksum = (g_i2c_checksum << 1) ^ AFE_I2C_CRC_FORMULAR;
      }        
      else
      {
      	g_i2c_checksum = (g_i2c_checksum << 1);
      }
         
   }
   
}




void AFE_write_register(uint8 reg, uint16 dat)
{
	uint8 tx_data[4] = {0};
	i2c_tx_buff_t  i2c0_tx_buff;
	
	tx_data[0] = reg;
	tx_data[1] = dat >> 8;
	tx_data[2] = dat;
	
	g_i2c_checksum = 0;
	i2c_pec_calc(0x54);
	i2c_pec_calc(tx_data[0]);
	i2c_pec_calc(tx_data[1]);
	i2c_pec_calc(tx_data[2]);
	tx_data[3] = g_i2c_checksum;
	
	i2c0_tx_buff.Slave_Addr_WR = AFE_ADDR_WRITE;
	i2c0_tx_buff.tx_buff.p_buff = &tx_data[0];
	i2c0_tx_buff.tx_buff.buff_len = 4;
	i2c_write_blocking(i2c_0, &i2c0_tx_buff);
	
}


		
uint16 AFE_read_register(uint8 o2_reg)
{
	uint8 tx_bf_rx_data[1] = {0}; // read cmd
	uint8 rx_data[3] = {0};       // hi_byte, Low_byte, CRC
	uint16 read_val;
	i2c_tx_buff_t  i2c0_tx_bf_rx_buff;
	i2c_rx_buff_t  i2c0_rx_buff; 	
	
	tx_bf_rx_data[0] = o2_reg;
	i2c0_tx_bf_rx_buff.Slave_Addr_WR = AFE_ADDR_WRITE;	
	i2c0_tx_bf_rx_buff.tx_buff.buff_len = 1;
	i2c0_tx_bf_rx_buff.tx_buff.p_buff = &tx_bf_rx_data[0];
	
	i2c0_rx_buff.Slave_Addr_RD = AFE_ADDR_READ;
	i2c0_rx_buff.rx_buff.buff_len = 3;
	i2c0_rx_buff.rx_buff.p_buff = &rx_data[0];
	i2c_read_blocking(i2c_0, &i2c0_tx_bf_rx_buff, &i2c0_rx_buff);
	read_val = rx_data[0] & 0x0F;
	read_val = (read_val << 8) + rx_data[1];
	return read_val;
	
}



//cell_n=5.12V*(val[o2_reg(23:2c)] & 0xfff) / 4095
void read_cell_vol(uint8 cell_no)
{
	uint32 cell_vol_tmp = 0;
	
	if(cell_no >= CELL_AMOUNT)
	{
	  return;
	}
	
	cell_vol_tmp = AFE_read_register(cell_no + AFE_CELL1_REG_ADDR);
    cell_vol_tmp = ((cell_vol_tmp * 5120) * 2 / 4095 + 1) / 2;
    Sys_Info.Cell_Volt[cell_no] = cell_vol_tmp;
		 
}



//current_A=((2.56V * ADC / 4095 - 1)/10)/0.0005    // In A
void read_pack_current(void)
{
	uint16 o2_cell_cur_reg = AFE_PACK_CURR_REG_ADDR;
	uint16 rx_dat = 0;
	sint32 tem = 0;
	
	rx_dat = AFE_read_register(o2_cell_cur_reg);
	tem = ((2560 * rx_dat) / 4095 - 1000)  * 1000; 
	Sys_Info.Pack_Current = tem*100 /(98 * CURRENT_SENSOR_RESISTOR * (-1));  // In mA 
 
}



void read_AFE_Chip_Temp(void)
{
	uint16 o2_InterTemp_reg = AFE_CHIP_TEMP_REG_ADDR;
    uint16 rx_dat = 0;
    uint16 inter_temp_tmp = 0;
	
	rx_dat = AFE_read_register(o2_InterTemp_reg);
	inter_temp_tmp = rx_dat;
	inter_temp_tmp = (inter_temp_tmp*2/16+1)/2-50;
	Sys_Info.AFE_Chip_Temp = inter_temp_tmp;		
}



void read_AFE_event(void)
{
	uint16 o2_event_reg = AFE_EVENT_REG_ADDR;
	uint16 rx_dat = 0;
	
	rx_dat = AFE_read_register(o2_event_reg);
	Sys_Info.AFE_Event = rx_dat;	
}



uint16 get_temperature(uint16  volt_mv)
{
	uint32 resistence_ntc = 0;
	uint16 temp = 0, i = 0;
	
	resistence_ntc = volt_mv * NTC_DIVIDER_RESISTANCE / (NTC_VCC_VOLT_VAL - volt_mv);
	
	if (resistence_ntc > NTC_RESISTENCE_UPPER_LIMIT)
	{
      temp = NTC_TEMP_LOWER_LIMIT;
	}
	else if (resistence_ntc <= NTC_RESISTENCE_LOWER_LIMIT)
	{
      temp = NTC_TEMP_UPPER_LIMIT;
	}
	else
	{
		for (i = 0; i<sizeof(NTC_array) / sizeof(NTC_Data_T); i++)
		{
			if ((NTC_array[i].resistence<resistence_ntc) && (NTC_array[i + 1].resistence >= resistence_ntc))
			{
				temp = (NTC_array[i].temp - NTC_array[i + 1].temp)*(NTC_array[i + 1].resistence - resistence_ntc) / (NTC_array[i + 1].resistence - NTC_array[i].resistence) + NTC_array[i + 1].temp;

				break;
			}
		}
	}
	
	return temp;
	
}


uint16 get_soc(uint16 volt_mv)
{
   uint8 i = 0;
   uint16 soc = 0;
   uint8 size = (sizeof(SOC_ARRAY) / sizeof(soc_data_t));

	if(volt_mv < SOC_ARRAY[0].volt)
	{
		soc = 0;

	}

	if(volt_mv > SOC_ARRAY[size-1].volt)
	{
		soc = 1000;

	}
 	
	for(i= 0; i< size; i++ )
	{

		if((SOC_ARRAY[i].volt < volt_mv) && (SOC_ARRAY[i+1].volt >= volt_mv))
		{

			soc = (( SOC_ARRAY[i+1].soc - SOC_ARRAY[i].soc) * (volt_mv - SOC_ARRAY[i].volt) / (SOC_ARRAY[i+1].volt - SOC_ARRAY[i].volt) ) + SOC_ARRAY[i].soc;
			soc *= 10;
			 break;
		}

	}


  if(CHG_Com_Data.un_chg_info.st_chg_info.pack_state == PACK_FULLY_CHG)
  {

		soc =  1000;

  }
  else
  {
		if(soc >= 980)
		{
			soc = 990;
		}

  }
	

	return soc;


}



void read_cell_temperature(void)
{
	uint16 temp_cell_vol;
	
	adc_start_non_blocking( ADC_CHL_CELL_TEMP );
	if (g_Cell_Temp_adc_flag)
	{
		g_Cell_Temp_adc_flag = 0;
		temp_cell_vol = adc_read_blocking(ADC_CHL_CELL_TEMP) * NTC_VCC_VOLT_VAL / ADC_12BIT_VAL;			
		Sys_Info.Cell_Temp = get_temperature(temp_cell_vol);		

	}
	
	
}



void read_mosfet_temperature(void)
{
	uint16 temp_fet_vol;
	
	adc_start_non_blocking( ADC_CHL_MOSFET_TEMP );
	if (g_Mosfet_Temp_adc_flag)
	{
		g_Mosfet_Temp_adc_flag = 0;
		
		temp_fet_vol = adc_read_blocking(ADC_CHL_MOSFET_TEMP) * NTC_VCC_VOLT_VAL / ADC_12BIT_VAL;			
		Sys_Info.FET_Temp = get_temperature(temp_fet_vol);		
	}
	
}



void read_pack_dsg_current(void)
{
	uint16 pack_cur_raw;
	
	adc_start_non_blocking( ADC_CHL_PACK_DSG_CURR );
	if (g_Pack_Dsg_Curr_adc_flag)
	{
		g_Pack_Dsg_Curr_adc_flag = 0;
		pack_cur_raw = adc_read_blocking(ADC_CHL_PACK_DSG_CURR) * NTC_VCC_VOLT_VAL / ADC_12BIT_VAL;
		Sys_Info.Pack_Dsg_Current = pack_cur_raw;
	}
}

uint16 Func_Do_Avg(uint16 arg_arr[], uint8 arg_arr_size)
{
    uint8 lv_count = 0;
    uint32 lv_tmov_avg = 0;
    uint16 lv_max = 0,lv_min = 0;

    lv_max = arg_arr[0];
    lv_min = arg_arr[0];
    for(lv_count = 0;lv_count < arg_arr_size;lv_count++)
    {
        if( arg_arr[lv_count] > lv_max)
            lv_max = arg_arr[lv_count];
         if(arg_arr[lv_count] < lv_min)
            lv_min = arg_arr[lv_count];
    }
    for (lv_count = 0; lv_count < arg_arr_size;lv_count++)
        lv_tmov_avg += arg_arr[lv_count];

    lv_tmov_avg -= lv_max;
    lv_tmov_avg -= lv_min;
    lv_count -= 2;                
    lv_tmov_avg /= lv_count;          

    return lv_tmov_avg;
}


uint16 read_pack_vol(void)
{
	uint16 pack_vol_raw = 0;
	uint16 i = 0;

	
	adc_start_non_blocking( ADC_CHL_PACK_VOLT );

	for(i=0; i<50; i++)
	{
		if(g_Pack_Volt_adc_flag)
		{
			break;

		}

	}
	
	if(g_Pack_Volt_adc_flag)
	{
		g_Pack_Volt_adc_flag = 0;
		pack_vol_raw = adc_read_blocking(ADC_CHL_PACK_VOLT) * NTC_VCC_VOLT_VAL / ADC_12BIT_VAL * 52;	
		
		
	}
	return pack_vol_raw;
}



void Discharging_Control(void)  // 10ms
{
	static uint16 DOC_lvl1_Dual_Cnt = 0; 
	static uint16 DOC_lvl2_Cnt = 0; 
    static uint16 LTP_Cnt = 0;
    static uint16 OTP_Cnt = 0;
	static uint16 mosfet_otp_cnt = 0;

	if(Sys_Info.Pack_Status.st_pack_state.lock_state)
	{
		DSG_DISABLE();
		CHG_DISABLE();
		
		return;

	}

	
  	switch(discharging_sta)
	{

		case DISCHING:  	    
							if((Sys_Info.Pack_Vol < DSG_LVP_PACK_VOLT) || (Cell_Para.cell_vol_min < DSG_LVP_CELL_VOLT))
							{
								discharging_sta = DISC_LVP_LOCK;
							}
							else if(Sys_Info.Cell_Temp > (DSG_OTP_TEMP_VAL)) 
							{
							   if(OTP_Cnt < 1 * 100)
							   {
									OTP_Cnt++;
							   }
							   else
							   {
									OTP_Cnt = 0;							   
									discharging_sta = DISC_OTP;

							   }
							 	
							
							}
							else if(Sys_Info.Cell_Temp < (DSG_LTP_TEMP_VAL)) 
							{
								if(LTP_Cnt < 1 * 100)
								{
									LTP_Cnt++;
								}
								else
								{
									LTP_Cnt = 0;								
									discharging_sta = DISC_LTP;
									
								}
							
							   
							}
							else
							{
							  	DSG_ENABLE();	
								CHG_ENABLE();

								OTP_Cnt = 0;
								LTP_Cnt = 0;
								
								if((Sys_Info.Pack_Current * (-1)) > DSG_DOC_LVL2) 
								{
								  if(DOC_lvl2_Cnt < 1 * 10)
									{
									  	DOC_lvl2_Cnt++;
									}
									else
									{
									  	DSG_DISABLE();
										CHG_DISABLE();
									  	discharging_sta = DISC_DOC_LOCK;
										DOC_lvl2_Cnt = 0;
									}
									
								}
								else 
								{
								  DOC_lvl2_Cnt = 0;
								}	
								
								if((Sys_Info.Pack_Current * (-1)) > DSG_DOC_LVL1_DUAL) 
								{
								  if(DOC_lvl1_Dual_Cnt < 1 * 100)
									{
									  	DOC_lvl1_Dual_Cnt++;
									}
									else
									{
									  	DSG_DISABLE();
										CHG_DISABLE();
									  	discharging_sta = DISC_DOC_LOCK;
										DOC_lvl1_Dual_Cnt = 0;
									}	
									
								}
								else
	                            {
								  DOC_lvl1_Dual_Cnt = 0;

								}

								
							}

							if((Sys_Info.FET_Temp > DSG_OTP_MOSFET_TEMP_VAL) || (Sys_Info.FET_Temp < NTC_TEMP_LOWER_LIMIT))
							{
								if(mosfet_otp_cnt < 2 * 100 )
								{
									mosfet_otp_cnt++;

								}
								else
								{
									mosfet_otp_cnt = 0;
									
									DSG_DISABLE();
									CHG_DISABLE();
									discharging_sta = DISC_OTP;
								}

				
							}
							else
							{
								mosfet_otp_cnt = 0;

							}
							
				            break;
		case DISC_OTP:      
							DSG_DISABLE();
							CHG_DISABLE();
							if((Sys_Info.Cell_Temp >= (DSG_LTP_TEMP_VAL)) && (Sys_Info.Cell_Temp <= (DSG_OTP_RELEASE_TEMP_VAL)) )// -20 ~ 60 Degree C, Need to check with Jack _Nicholas
							{

								if( (Sys_Info.FET_Temp >= NTC_TEMP_LOWER_LIMIT) && ((Sys_Info.FET_Temp < DSG_OTP_MOSFET_RELEASE_TEMP_VAL)) )
								{
									discharging_sta = DISCHING;

								}

								
							}




							
							break;
		case DISC_LTP:       
							DSG_DISABLE();
							CHG_DISABLE();
							if((Sys_Info.Cell_Temp >= (DSG_LTP_RELEASE_TEMP_VAL)) && (Sys_Info.Cell_Temp <= (DSG_OTP_TEMP_VAL)) )// -15 ~ 70 Degree C
							{
								discharging_sta = DISCHING;
							}
							break;
		case DISC_LVP_LOCK:
							DSG_DISABLE();
							CHG_DISABLE();		
							break;
		case DISC_DOC_LOCK:  
							DSG_DISABLE(); 
							CHG_DISABLE();
							break;

					default:
						    break;
	
	}

	if(g_DOC_SHORT_Detect)
	{
	  g_DOC_SHORT_Detect = 0;
	  AFE_write_register(AFE_EVENT_REG_ADDR, 0xEFF9); 
	  discharging_sta = DISC_DOC_LOCK;
	  
	}

	
}





void Charging_Control(void) //100ms
{

  	static uint16 CV_Mode_cnt = 0;
	static uint32 FULL_Chg_cnt2 = 0;


	if(Sys_Info.Pack_Status.st_pack_state.lock_state)
	{
		DSG_DISABLE();
		CHG_DISABLE();
		
		return;
	}

	
	switch(charging_state)
	{
	  case      CHAR_INIT: 
							DSG_ENABLE();	
							CHG_ENABLE();
							if( (Sys_Info.Cell_Temp > CHG_OTP_TEMP_VAL) ||  
							    (Sys_Info.Cell_Temp < CHG_LTP_TEMP_VAL)
							  )
							{
							   charging_state = TEMP_ERROR;

							}	
							else if( (Sys_Info.Cell_Temp >= CHG_LTP_TEMP_VAL) &&  
								       (Sys_Info.Cell_Temp <= CHG_LTC_TEMP_VAL)
								   )
							{
							   charging_state = LOW_TEMP_CHAR;
							}
							else
							{
								 if((Sys_Info.Pack_Vol >= CHG_OVP_PACK_VOLT) || (Cell_Para.cell_vol_max >= CHG_OVP_CELL_VOLT))
								 {
									 charging_state = VOLT_ERROR;
								 }
								 else if((Sys_Info.Pack_Vol >= CHG_FULL_PACK_VOLT) || (Cell_Para.cell_vol_max >= CHG_FULL_CELL_VOLT))	 
								 {
									 charging_state = FULLY_CHARD;
																	 
								 }
								 else if((Sys_Info.Pack_Vol >= DSG_LVP_PACK_VOLT) || (Cell_Para.cell_vol_max >= DSG_LVP_CELL_VOLT))
								 {
									 charging_state = CC_MODE;
								 }
								 else if((Sys_Info.Pack_Vol >= CHG_LOW_VOLT_FAULT_PACK_VOLT) || (Cell_Para.cell_vol_max >= CHG_LOW_VOLT_FAULT_CELL_VOLT))
								 {
									 charging_state = LOW_VOLT_CHAR;
								 }
								 else
								 {
										charging_state = VOLT_ERROR;
								 }

							}											 
							break;
												 
		case    TEMP_ERROR: 
							if( (Sys_Info.Cell_Temp >= CHG_LTP_TEMP_VAL) &&
							  (Sys_Info.Cell_Temp <= CHG_LTC_TEMP_VAL)
							)
							{
								charging_state = LOW_TEMP_CHAR;
							}
							else if((Sys_Info.Cell_Temp > CHG_LTC_TEMP_VAL) &&
							       (Sys_Info.Cell_Temp <= CHG_OTP_TEMP_VAL)
							    )
							{
								if((Sys_Info.Pack_Vol >= CHG_OVP_PACK_VOLT) || (Cell_Para.cell_vol_max >= CHG_OVP_CELL_VOLT))
								{
								 	charging_state = VOLT_ERROR;
								 	
								}
								else if((Sys_Info.Pack_Vol >= CHG_FULL_PACK_VOLT) || (Cell_Para.cell_vol_max >= CHG_FULL_CELL_VOLT))	 
								{
								 	charging_state = FULLY_CHARD;
																 
								}
								else if((Sys_Info.Pack_Vol >= DSG_LVP_PACK_VOLT) || (Cell_Para.cell_vol_max >= DSG_LVP_CELL_VOLT))
								{
								 	charging_state = CC_MODE;
								 	
								}
								else if((Sys_Info.Pack_Vol >= CHG_LOW_VOLT_FAULT_PACK_VOLT) || (Cell_Para.cell_vol_max >= CHG_LOW_VOLT_FAULT_CELL_VOLT))
								{
								 	charging_state = LOW_VOLT_CHAR;
								 	
								}
								else 
								{
									charging_state = VOLT_ERROR;
									
								}

							}
							else
							{
								CHG_Com_Data.chg_curr = CHG_0A_CHG_CURRENT; 															
								CHG_Com_Data.un_chg_info.st_chg_info.pack_state = PACK_FAST_CHG;
							} 
							break;
		
		case    VOLT_ERROR: 
							if(((Sys_Info.Pack_Vol >= CHG_LOW_VOLT_FAULT_PACK_VOLT)     && (Sys_Info.Pack_Vol < CHG_FULL_PACK_VOLT)) &&
							((Cell_Para.cell_vol_max >= CHG_LOW_VOLT_FAULT_CELL_VOLT) && (Cell_Para.cell_vol_max < CHG_FULL_CELL_VOLT))
							)
							{
								charging_state = LOW_VOLT_CHAR;
							}
							else 
							{
							 	CHG_Com_Data.chg_curr = CHG_0A_CHG_CURRENT; 															
							  CHG_Com_Data.un_chg_info.st_chg_info.pack_state = PACK_ERROR;
								
							}
																		
							break;
												
		case LOW_TEMP_CHAR: 		
							if(Sys_Info.Cell_Temp > CHG_LTC_TEMP_VAL)													
							{
								charging_state = LOW_VOLT_CHAR;
							}
							else
							{
								if((Sys_Info.Pack_Vol >= CHG_OVP_PACK_VOLT) || (Cell_Para.cell_vol_max >= CHG_OVP_CELL_VOLT))// 42.2V / 4.2V
								{
									 charging_state = VOLT_ERROR;
								}
								else
								{
									CHG_Com_Data.chg_curr = CHG_LOW_TEMP_CHG_CURRENT; 
									CHG_Com_Data.un_chg_info.st_chg_info.pack_state = PACK_FAST_CHG;  
								}
													  
							}

							break;
		
		case LOW_VOLT_CHAR: 
							if((Sys_Info.Pack_Vol >= CHG_CC_MODE_PACK_VOLT) || (Cell_Para.cell_vol_max >= CHG_CC_MODE_CELL_VOLT))
							{
								charging_state = CC_MODE;
							}
							else
							{
								if(Sys_Info.Cell_Temp > CHG_OTP_TEMP_VAL)
								{
									charging_state = TEMP_ERROR;
								}
								else
								{
									CHG_Com_Data.chg_curr = CHG_LOW_VOLT_CHG_CURRENT;
									CHG_Com_Data.un_chg_info.st_chg_info.pack_state = PACK_FAST_CHG;
								}

							}
											
							// Timer
							break;
												
		case       CC_MODE: 
							if( (Sys_Info.Cell_Temp > CHG_OTP_TEMP_VAL) ||  //Out of Temp.Range
								(Sys_Info.Cell_Temp < CHG_LTP_TEMP_VAL)
							  )
							{
								charging_state = TEMP_ERROR;
							}
							else
							{
								if((Sys_Info.Pack_Vol >= CHG_CC_FULL_PACK_VOLT) || (Cell_Para.cell_vol_max >= CHG_CC_FULL_CELL_VOLT))
								{
									charging_state = CV_MODE;
								}
								else
								{
																
									CHG_Com_Data.chg_curr = CHG_FAST_CHG_CURRENT; 
									CHG_Com_Data.un_chg_info.st_chg_info.pack_state = PACK_FAST_CHG;			
								}
							}			                  
							break;
		
		case       CV_MODE: 
							if( (Sys_Info.Cell_Temp > CHG_OTP_TEMP_VAL) ||  
							    (Sys_Info.Cell_Temp < CHG_LTP_TEMP_VAL)
							  )
							{

								charging_state = TEMP_ERROR;
								CV_Mode_cnt = 0;
								FULL_Chg_cnt2 = 0;
							}
							else
							{
								if(CV_Mode_cnt < CHG_CC_MODE_TIME_VAL) 
								{
									CV_Mode_cnt+=BMS_TASK_INTERVAL;

								}
								else
								{
									CV_Mode_cnt = 0;
									if(CHG_Com_Data.chg_curr > CHG_FULL_CHG_CURRENT) 
									{
										if((Sys_Info.Pack_Vol >= CHG_CC_FULL_PACK_VOLT) || (Cell_Para.cell_vol_max >= CHG_CC_FULL_CELL_VOLT))
										{
											CHG_Com_Data.chg_curr--;
										}

										CHG_Com_Data.un_chg_info.st_chg_info.pack_state = PACK_FAST_CHG;			

									}
									else
									{
										CHG_Com_Data.chg_curr = CHG_FULL_CHG_CURRENT;
										CHG_Com_Data.un_chg_info.st_chg_info.pack_state = PACK_FAST_CHG;			

										if(FULL_Chg_cnt2 < CHG_FULL_CHG_TIME_VAL)
										{
											FULL_Chg_cnt2+=CHG_CC_MODE_TIME_VAL;
										}	
										else
										{
											charging_state = FULLY_CHARD;
											CV_Mode_cnt = 0;
											FULL_Chg_cnt2 = 0;
										}

									}		
								}

							}
							break;
		
		case   FULLY_CHARD: 
							CHG_Com_Data.chg_curr = 0;			
							CHG_Com_Data.un_chg_info.st_chg_info.pack_state = PACK_FULLY_CHG;			
							break;
		
		case    MAINT_MODE:
			                break;
		default:
			                break;
	
	
	}
	
	
	CHG_Com_Data.s_temp = Sys_Info.Cell_Temp - ABSOLUTE_TMEP_OFFSET;
	CHG_Com_Data.crc = CHG_Com_Data.chg_curr ^ CHG_Com_Data.s_temp ^ CHG_Com_Data.un_chg_info.chg_info;
	
	
}




void LED_Task(void) // 500ms
{
	static uint8 fg_cnt = 0;
	static uint8 charg_ind_sta = 0;
	
	if(g_Chg_Detection)
	{
   		
		switch(charg_ind_sta)
		{ 
			
			case 0: if(charging_state == FULLY_CHARD)
					{
						LED1_ON();
						LED2_ON();	
						LED3_ON();
						LED4_ON();	
						charg_ind_sta = 4;
					}
					else if((Sys_Info.Pack_Vol < FG_BELOW_25_PERCENT_PACK_VOLT) || 
					        (Cell_Para.cell_vol_max < FG_BELOW_25_PERCENT_CELL_VOLT)
					       )
					{
						LED1_TOGL();
						LED2_OFF();	
						LED3_OFF();
						LED4_OFF();
					}
					else
					{
						charg_ind_sta = 1;
					}	
							
					break;
			case 1: 
					if((Sys_Info.Pack_Vol < FG_25_PERCENT_PACK_VOLT) || 
					   (Cell_Para.cell_vol_max < FG_25_PERCENT_CELL_VOLT)
					  )
					{
						LED1_ON();
						LED2_TOGL();	
						LED3_OFF();
						LED4_OFF();
					}
					else
					{
						charg_ind_sta = 2;
					}	
					break;
			case 2: 
					if((Sys_Info.Pack_Vol < FG_50_PERCENT_PACK_VOLT) || 
					   (Cell_Para.cell_vol_max < FG_50_PERCENT_CELL_VOLT)
					  )
					{
						LED1_ON();
						LED2_ON();	
						LED3_TOGL();
						LED4_OFF();
					}
					else
					{
						charg_ind_sta = 3;
					}	
					break;
			case 3: 	
					if(charging_state == FULLY_CHARD)
					{
						 charg_ind_sta = 4;
							
					}
      				else
      				{
						LED1_ON();
						LED2_ON();	
						LED3_ON();
						LED4_TOGL();
					}																																							  							
					break;		
			case 4: 
					LED1_ON();				 
					LED2_ON();
					LED3_ON();
					LED4_ON(); 
					break;											
			default:
					break;
			
		}	
							
	}
	else if(g_Fuel_Gauge_Detect) // LED on Request
	{
	 
		if(fg_cnt < 6)  // 6 * 500ms = 3s
		{
			fg_cnt++;
			
			if(Cell_Para.cell_vol_aver < FG_BELOW_25_PERCENT_CELL_VOLT)
			{
				LED1_TOGL();
				LED2_OFF();	
				LED3_OFF();
				LED4_OFF();
				
			}
			else if(Cell_Para.cell_vol_aver < FG_25_PERCENT_CELL_VOLT)
			{
			  	LED1_ON();
				LED2_OFF();	
				LED3_OFF();
				LED4_OFF();
			}	
			else if(Cell_Para.cell_vol_aver < FG_50_PERCENT_CELL_VOLT)
			{
			  	LED1_ON();
				LED2_ON();	
				LED3_OFF();
				LED4_OFF();
			}
			else if(Cell_Para.cell_vol_aver < FG_75_PERCENT_CELL_VOLT) 
			{
			  	LED1_ON();
				LED2_ON();	
				LED3_ON();
				LED4_OFF();
			}
			else
			{
			  	LED1_ON();
				LED2_ON();	
				LED3_ON();
				LED4_ON();
			}
							 
		}
		else
		{
			 fg_cnt = 0;
			 g_Fuel_Gauge_Detect = 0;
		}
	   
	}
	else
	{
		charg_ind_sta = 0; 

		LED1_OFF();
		LED2_OFF();	
		LED3_OFF();
		LED4_OFF();
	  
	}	
}








void System_Tick_Init(void)
{
	
	sys_tick_isr_callback_register(sys_tick_count);
	sys_tick_config(SYS_TICK_INTERVAL_COUNTER);

}

void MRT_Init(void)
{

	timer_isr_callback_register(timer0, Charge_Data_Com);  // 100 us interval
	timer_isr_callback_register(timer1, charge_detect);
	timer_isr_callback_register(timer2, mrt2_irq_user);
	timer_isr_callback_register(timer3, mrt3_irq_user);
	timer_config(timer0, 10);      // 10* 10us = 100 us 
	timer_config(timer1, 10000);   // 10000* 10us = 100 ms
	timer_config(timer2, 30000);   // 300ms
	timer_config(timer3, 400);
	timer_enable(timer0, ENABLE);
	timer_enable(timer1, ENABLE);
	timer_enable(timer2, ENABLE);
	timer_enable(timer3, ENABLE);
}

void GPIO_Init(void)
{

	//pin 13 Power SW gpio_25
	gpio_direction(NTC_PWR_ENABLE_PIN, 0);
  	PWR_SW_ENABLE();
  	
	//pin 10 LDO enable gpio_16
	gpio_direction(LDO_ENABLE_PIN, 0);
  	LDO_ENABLE(); 
  	
	//pin 17 CHG COM_output pulse to communicate
	gpio_direction(CHG_COM_PIN, 0);
  	gpio_output(CHG_COM_PIN, 0);
  	
	//pin 16 CHG enable gpio_01
	gpio_direction(CHG_MOSFET_ENABLE_PIN, 0);
  	CHG_ENABLE();
  	
	//pin 24 DSG enable gpio_00
	gpio_direction(DSG_MOSFET_ENABLE_PIN, 0);
  	DSG_ENABLE();  
  	
	//pin 14 BT detection LED pin
	gpio_direction(LED_BT_RED_PIN, 0);
	
	//pin 25 reply for dual discharge gpio_14
	gpio_direction(T2_PWR_TOOL_WRITE, 0);
	
	//pin 28/29/30 LED gpio_21/20/19
	gpio_direction(LED2_PIN, 0); 
	LED2_OFF();	
	
	gpio_direction(LED1_PIN, 0); 
	LED1_OFF();

	gpio_direction(LED_BT_BLUE_PIN, 0); 
	LED_BT_OFF();
	
	gpio_direction(LED4_PIN, 0);
	LED4_OFF();
	
	gpio_direction(LED3_PIN, 0);
	LED3_OFF();
	
}



void Pin_Interrupt_Init(void)
{
	pin_int_config_t doc_det_Pin_int = {DOC_DETECT_PIN_INT, 0, edge_falling};
	pin_int_config_t load_det_pin_int = {LOAD_DETECT_PIN_INT, 0, edge_rising};
	pin_int_config_t chg_det_t1_pin_int = {CHG_DETECT_T1_PIN_INT, 0, edge_falling};
	pin_int_config_t fuel_gauge_key_pin_int = {FUEL_GAUGE_KEY_PIN_INT, 0, edge_rising};
	pin_int_config_t t2_pwr_tool_pin_int = {T2_PWR_TOOL_READ_PIN_INT, 0, edge_falling};
	

	pin_isr_callback_register(DOC_DETECT_PIN_INT , DOC_detect);
	pin_int_config( &doc_det_Pin_int , DOC_DETECT_INTERRUPT_PIN );
	pin_int_enable( DOC_DETECT_PIN_INT, true );	
		
	pin_isr_callback_register(CHG_DETECT_T1_PIN_INT , chg_detect);
	pin_int_config( &chg_det_t1_pin_int , CHG_DETECT_T1_INTERRUPT_PIN );
	pin_int_enable( CHG_DETECT_T1_PIN_INT, true );
	
	pin_isr_callback_register(T2_PWR_TOOL_READ_PIN_INT , dual_dsg_detec);
	pin_int_config( &t2_pwr_tool_pin_int , T2_PWR_TOOL_READ_INTERRUPT_PIN );
	pin_int_enable( T2_PWR_TOOL_READ_PIN_INT, true );
	
	pin_isr_callback_register(LOAD_DETECT_PIN_INT , load_detect);
	pin_int_config( &load_det_pin_int , LOAD_DETECT_INTERRUPT_PIN );
	pin_int_enable( LOAD_DETECT_PIN_INT, true );
	gpio_pull_updown(LOAD_DETECT_INTERRUPT_PIN, gpio_pull_down);
  
	
	
	pin_isr_callback_register(FUEL_GAUGE_KEY_PIN_INT , fuel_gauge_detect);
	pin_int_config( &fuel_gauge_key_pin_int , FUEL_GAUGE_KEY_INTERRUPT_PIN );
	pin_int_enable( FUEL_GAUGE_KEY_PIN_INT, true );
	gpio_pull_updown(FUEL_GAUGE_KEY_INTERRUPT_PIN, gpio_pull_down);

}

void ADC_Init(void)
{

	
	adc_ready_isr_callback_register( ADC_CHL_PACK_DSG_CURR, read_pack_dsg_curr );
	adc_config(thrd0, ADC_CHL_PACK_DSG_CURR, 0, 4095);
	//gpio_pull_updown(ADC_CHL_PACK_DSG_CURR_PIN, gpio_pull_hiz);

	
	adc_ready_isr_callback_register( ADC_CHL_CELL_TEMP, read_cell_temp );
	adc_config(thrd0, ADC_CHL_CELL_TEMP, 0, 4095);
	//gpio_pull_updown(ADC_CHL_CELL_TEMP_PIN, gpio_pull_hiz);	

	
	adc_ready_isr_callback_register( ADC_CHL_MOSFET_TEMP, read_mosfet_temp );
	adc_config(thrd0, ADC_CHL_MOSFET_TEMP, 0, 4095);
	//gpio_pull_updown(ADC_CHL_MOSFET_TEMP_PIN, gpio_pull_hiz);

	adc_ready_isr_callback_register( ADC_CHL_PACK_VOLT, read_pack_volt );
	adc_config(thrd0, ADC_CHL_PACK_VOLT, 0, 4095);
	//gpio_pull_updown(ADC_CHL_PACK_VOLT_PIN, gpio_pull_hiz);
	
}



void I2C_Init(void)
{
  	//Communication protocol

 	i2c_mas_config_t i2c_set={i2c_0, I2C_CLOCK};
	i2c_master_config(&i2c_set, I2C_CLK_PIN, I2C_SDA_PIN);
}



void UART_Init(void)
{
  	//UART For BlUE TOOTH	 	
  	uart_config(&uart_BT, BT_UART2_TX_PIN, BT_UART2_RX_PIN);
  	uart_send_tx_ready_isr_callback_register(uart_BT.uart_no, uart_tx_free);

	//UART For Tool Communication T3 and T4
	uart_config(&uart_D_Tech, D_TECH_UART1_TX_PIN, D_TECH_UART1_RX_PIN);
  	
}



void AFE_Init(void)
{
	AFE_write_register(AFE_EVENT_REG_ADDR, 0xEFF9);		// Clear Flags
	AFE_write_register(AFE_MASK_REG_ADDR, 0xF8EF);		// DOC 150mV @ 300A, 0.5mOhm, @ 150A, 1mOhm
 
}



void Debug_Interface_Init(void)
{
	
	printf_config(uart0, DEBUG_UART0_TX_PIN, DEBUG_UART0_RX_PIN);
	
}




uint8_t Ryobi_pack[11] = {"RYOBI PACK"};

void n_tech_output_cmd_handler(uint16_t op_code, uint8_t receiver)
{

	
	switch(op_code)
	{
		case N_TECH_OP_CODE_HANDSHAKE:
										//g_output_cmd.op_code = N_TECH_OP_CODE_HANDSHAKE;
										//g_output_cmd.receive_id = receiver;
										//g_output_cmd.payload_len
										//g_output_cmd.postmark_sender
										//g_output_cmd.packet_need_response	
										break;
	
		case N_TECH_OP_CODE_NAME_SET:	g_output_cmd.op_code = op_code + N_TECH_CODE_OP_NEED_RESPONDE;
										g_output_cmd.packet_need_response =  1;
										memcpy(&g_output_cmd.payload_data[0], &Ryobi_pack[0], (sizeof(Ryobi_pack)-1));
										g_output_cmd.payload_len = 10;
										g_output_cmd.receive_id = receiver;
										g_output_cmd.send_id = N_TECH_ID_PACK;
										g_output_cmd.sequence_id = 0;
		
										break;
	


		default:						break;
	}
}



uint8_t n_tech_set_BTname_cmd(void)
{
	
	if(g_output_cmd.op_code == 0 && g_op_code_wait_for_response == 0)
	{
		n_tech_output_cmd_handler(N_TECH_OP_CODE_NAME_SET,N_TECH_ID_BT_MODULE);
		return SUCESS;
	}
	else
	{
		return FAIL;
	}
}

#define OLD_CHECKSUM 0

uint8_t cal_CheckSum(uint8_t *Pnt) 
{
	uint8_t sum = 0;
	uint8_t len;
	uint8_t i = 0;



	Pnt++;
	len = *Pnt;
	
	sum=0;
	for(i=0; i<(len+(CHECKSUM_DATA_INDEX_WITHOUT_PAYLOAD-START_DATA_LEN)); i++)
	{
		sum += *Pnt;
		Pnt++;
	}

#if OLD_CHECKSUM == 1

	return (sum);
#else

	return ((~sum) + 1);
#endif

}


uint8_t CheckSum_check(uint8_t *Pnt) 
{
	uint8_t sum = 0;
	uint8_t len;
	uint8_t i = 0;



	Pnt++;
	len = *Pnt;
	
	sum=0;
	for(i=0; i<(len+(CHECKSUM_DATA_INDEX_WITHOUT_PAYLOAD-START_DATA_LEN)); i++)
	{
		sum += *Pnt;
		Pnt++;
	}

#if OLD_CHECKSUM == 1

#else

	sum = ((~sum) + 1);
#endif
	

	if(sum == *Pnt)
	{
		return 1;
	}
	else
	{
		return 0;
	}
	


}




void get_data_from_packet(uint8_t *p_pack)
{


	uint32_t tmp0,tmp1;

	g_input_cmd.sequence_id = *(p_pack+SEQ_ID_DATA_INDEX);
	g_input_cmd.send_id = *(p_pack+SEND_ID_INDEX);
	g_input_cmd.receive_id = *(p_pack+RECEIVE_ID_INDEX);
	g_input_cmd.payload_len = *(p_pack+PAYLOAD_LEN_DATA_INDEX);
	tmp0 = *(p_pack+OP_CODE_DATA_INDEX);
	tmp1 = *(p_pack+OP_CODE_DATA_INDEX+1);

	g_input_cmd.op_code =((tmp1<<8) + tmp0);

	memcpy( &g_input_cmd.payload_data[0] , (p_pack+PAYLOAD_DATA_INDEX) , g_input_cmd.payload_len);

}

uint8_t read_n_tech_data(uint8_t* p_buffer , uint8_t buffer_len)
{
	uint8_t index,data = 0;
	uint8_t *p_tmp = NULL;
	uint8_t *p_packet = NULL;
	uint8_t payload_len = 0;
	uint8_t end_offset = 0;
	uint8_t checksum_verify = 0;

	memset(&g_input_cmd,0,sizeof(g_input_cmd));

	for(index=0; index<buffer_len; index++)
	{
		p_tmp = (uint8_t*)(p_buffer+index);
		data = *p_tmp;
		
		if(data == START_DATA_VALUE)
		{
			
			p_packet = p_tmp; // piont to 0xAA frame head
			
			p_tmp = (uint8_t*)(p_packet+PAYLOAD_LEN_DATA_INDEX);
			payload_len = *p_tmp;

			if(payload_len > MAX_PAYLOAD_SIZE)
			{
				continue; // len exceed sensible value , search start byte again
			}

			//end_offset = index+PAYLOAD_DATA_INDEX+payload_len+PAYLOAD_CRC_DATA_LEN;
			end_offset = index + END_DATA_INDEX_WITHOUT_PAYLOAD + payload_len;

			if(end_offset >= buffer_len)
			{
				break; // packet not complete , wait for more data coming 
			}

			p_tmp = (uint8_t*)(p_buffer+end_offset);
			data = *p_tmp;
			
			if(data != STOP_DATA_VALUE)
			{
				continue; // packet end not match , search start byte again
			}

			checksum_verify = CheckSum_check((uint8_t*)p_packet);// do packet crc check 

			if(checksum_verify) // passed
			{
				get_data_from_packet(p_packet); // update g_input_cmd
				
				index = end_offset+1; // set index to packet end
				break;
			}
			else
			{
				continue; // packet not valid , search start byte again
			}
		}
	}

	if(index > buffer_len) // prevent bug 
	{

		index = buffer_len;
	}
	
	return index;
}


uint8_t n_tech_get_expected_op_code()
{
	

 return ((uint8_t) g_output_cmd.op_code);
}


void n_tech_input_cmd_handler()
{
	uint16_t op_code = 0;
	uint16_t Cell_Temp = 0;
	uint16_t Pack_Current = 0;
    uint16_t Pack_Vol = 0;

	


	op_code = g_input_cmd.op_code & 0x0FFF;
	
	switch(op_code)
	{
		//all supported opcodes
		case N_TECH_OP_CODE_HANDSHAKE:
												g_output_cmd.op_code = op_code + N_TECH_CODE_OP_RESPONDE_WITH_PAYLOAD;
												g_output_cmd.packet_need_response = 0;
												g_output_cmd.receive_id = g_input_cmd.send_id;
												g_output_cmd.send_id = N_TECH_ID_PACK;
												g_output_cmd.sequence_id = 0;

												g_output_cmd.payload_data[0] = N_TECH_RESPONSE_SUCESS;
												g_output_cmd.payload_len = 1;
				
												break;

		case N_TECH_OP_CODE_READ_BAT_PROFILE:
												Cell_Temp = Sys_Info.Cell_Temp + PROTOCOL_TMEP_OFFSET;
												Pack_Current = (Sys_Info.Pack_Current / 100) + PROTOCOL_CURRENT_OFFSET;
												Pack_Vol = (Sys_Info.Pack_Vol / 100) + PROTOCOL_VOLT_OFFSET;

												//Sys_Info.soc = 1000;									// testing Nicholas
												//Sys_Info.Pack_Status.st_pack_state.chg_state = IDLE;	// testing Nicholas
												Sys_Info.Pack_Status.st_pack_state.protect_state = 0;	// testing Nicholas									
												
				
												g_output_cmd.op_code = op_code + N_TECH_CODE_OP_RESPONDE_WITH_PAYLOAD;
												g_output_cmd.packet_need_response = 0;
												g_output_cmd.receive_id = g_input_cmd.send_id;
												g_output_cmd.send_id = N_TECH_ID_PACK;
												g_output_cmd.sequence_id = 0;

												g_output_cmd.payload_len = 9;
												g_output_cmd.payload_data[0] = Cell_Temp & 0xFF;
												g_output_cmd.payload_data[1] = Cell_Temp >> 8;
												g_output_cmd.payload_data[2] = Sys_Info.soc & 0xFF;
												g_output_cmd.payload_data[3] = Sys_Info.soc >> 8;
												g_output_cmd.payload_data[4] = Pack_Current & 0xFF;
												g_output_cmd.payload_data[5] = Pack_Current >> 8;
												g_output_cmd.payload_data[6] = Pack_Vol & 0xFF;
												g_output_cmd.payload_data[7] = Pack_Vol >> 8;
												g_output_cmd.payload_data[8] = Sys_Info.Pack_Status.status & 0xFF;
				
												break;
#if 0

		case N_TECH_OP_CODE_LOCK_BMS:	
												Sys_Info.Pack_Status.st_pack_state.lock_state = 1;
												Sys_Info.alert_notif |= PROTOCOL_LOCK_STATUS;
		
												g_output_cmd.op_code = op_code + N_TECH_CODE_OP_RESPONDE_WITH_PAYLOAD;
												g_output_cmd.packet_need_response = 0;
												g_output_cmd.receive_id = g_input_cmd.send_id;
												g_output_cmd.send_id = N_TECH_ID_PACK;
												g_output_cmd.sequence_id = 0;

												g_output_cmd.payload_len = 1;
												g_output_cmd.payload_data[0] = N_TECH_RESPONSE_SUCESS;

												

												break;



		case N_TECH_OP_CODE_UNLOCK_BMS:	
												Sys_Info.Pack_Status.st_pack_state.lock_state = 0;
												Sys_Info.alert_notif &= ~PROTOCOL_LOCK_STATUS;
		
												g_output_cmd.op_code = op_code + N_TECH_CODE_OP_RESPONDE_WITH_PAYLOAD;
												g_output_cmd.packet_need_response = 0;
												g_output_cmd.receive_id = g_input_cmd.send_id;
												g_output_cmd.send_id = N_TECH_ID_PACK;
												g_output_cmd.sequence_id = 0;

												g_output_cmd.payload_len = 1;
												g_output_cmd.payload_data[0] = N_TECH_RESPONSE_SUCESS;

												break;

#endif
		case N_TECH_OP_CODE_NAME_READ:	
												g_output_cmd.op_code = op_code + N_TECH_CODE_OP_RESPONDE_WITH_PAYLOAD;
												g_output_cmd.packet_need_response = 0;
												g_output_cmd.receive_id = g_input_cmd.send_id;
												g_output_cmd.send_id = N_TECH_ID_PACK;
												g_output_cmd.sequence_id = 0;

												g_output_cmd.payload_len = 10;
												memcpy(&g_output_cmd.payload_data[0], &Ryobi_pack[0], (sizeof(Ryobi_pack)-1));
												        
												break;		




		case N_TECH_OP_CODE_REQEST_ALERT_NOTIF:	



		
																								
												g_output_cmd.op_code = op_code + N_TECH_CODE_OP_RESPONDE_WITH_PAYLOAD;
												g_output_cmd.packet_need_response = 0;
												g_output_cmd.receive_id = g_input_cmd.send_id;
												g_output_cmd.send_id = N_TECH_ID_PACK;
												g_output_cmd.sequence_id = 0;

												g_output_cmd.payload_len = 2;
												g_output_cmd.payload_data[0] = Sys_Info.alert_notif & 0xFF;
												g_output_cmd.payload_data[1] = Sys_Info.alert_notif >> 8;
												

												break;

												

		case N_TECH_OP_CODE_REQEST_BATT_USAGE:	


														
												g_output_cmd.op_code = op_code + N_TECH_CODE_OP_RESPONDE_WITH_PAYLOAD;
												g_output_cmd.packet_need_response = 0;
												g_output_cmd.receive_id = g_input_cmd.send_id;
												g_output_cmd.send_id = N_TECH_ID_PACK;
												g_output_cmd.sequence_id = 0;

												g_output_cmd.payload_len = 8;
												g_output_cmd.payload_data[0] = bms_log_info.discharge_acc_time & 0xFF;
												g_output_cmd.payload_data[1] = bms_log_info.discharge_acc_time >> 8;
												g_output_cmd.payload_data[2] = bms_log_info.discharge_acc_time >> 16;
												g_output_cmd.payload_data[3] = bms_log_info.discharge_acc_time >> 24;

												g_output_cmd.payload_data[4] = bms_log_info.charge_acc_time & 0xFF;
												g_output_cmd.payload_data[5] = bms_log_info.charge_acc_time >> 8;
												g_output_cmd.payload_data[6] = bms_log_info.charge_acc_time >> 16;
												g_output_cmd.payload_data[7] = bms_log_info.charge_acc_time >> 24;

												break;



		case N_TECH_OP_CODE_REQEST_CHG_DSG_CYCLE:	


	
												g_output_cmd.op_code = op_code + N_TECH_CODE_OP_RESPONDE_WITH_PAYLOAD;
												g_output_cmd.packet_need_response = 0;
												g_output_cmd.receive_id = g_input_cmd.send_id;
												g_output_cmd.send_id = N_TECH_ID_PACK;
												g_output_cmd.sequence_id = 0;

												g_output_cmd.payload_len = 4;
												g_output_cmd.payload_data[0] = bms_log_info.charge_cycle & 0xFF;
												g_output_cmd.payload_data[1] = bms_log_info.charge_cycle >> 8;

												g_output_cmd.payload_data[2] = bms_log_info.dicharge_cycle & 0xFF;
												g_output_cmd.payload_data[3] =  bms_log_info.dicharge_cycle  >> 8;

												break;


		case N_TECH_OP_CODE_READ_TEMPERATURE: 
												Cell_Temp = Sys_Info.Cell_Temp + PROTOCOL_TMEP_OFFSET;

												
												g_output_cmd.op_code = op_code + N_TECH_CODE_OP_RESPONDE_WITH_PAYLOAD;
												g_output_cmd.packet_need_response = 0;
												g_output_cmd.payload_data[0] = Cell_Temp & 0xFF;
												g_output_cmd.payload_data[1] = Cell_Temp >> 8;
												g_output_cmd.payload_len = 2;
												g_output_cmd.receive_id = g_input_cmd.send_id;
												g_output_cmd.send_id = N_TECH_ID_PACK;
												g_output_cmd.sequence_id = 0;
				
												break;

		
		

		default:								// return not supported opcode error
												g_output_cmd.op_code = op_code + N_TECH_CODE_OP_RESPONDE_WITH_FAULT_CODE;
												g_output_cmd.packet_need_response = 0;
												g_output_cmd.payload_data[0] = N_TECH_ERROR_CODE_OP_UNSUPPORTED_CMD;
												g_output_cmd.payload_len = 1;
												g_output_cmd.receive_id = g_input_cmd.send_id;
												g_output_cmd.send_id = N_TECH_ID_PACK;
												g_output_cmd.sequence_id = 0;
												break;
					
	}


	
	memset(&g_input_cmd,0,sizeof(g_input_cmd));
}



void n_tech_send_packet(uint8_t *p_buf, uint8_t len)
{
	uint8_t tmp_buffer[MAX_DATA_PACKET_SIZE+N_TECH_WAKE_UP_DATA_LEN];
	// packet build , pass to tx

	
	if(g_output_cmd.packet_need_response)
	{
		g_op_code_wait_for_response = g_output_cmd.op_code; // if this cmd need response
	}
#if 0
	uart_BT.tx_buff.p_buff = p_buf;	
	uart_BT.tx_buff.buff_len = len;	

#else
	memset(tmp_buffer , 0 , sizeof(tmp_buffer));
	memcpy(&tmp_buffer[N_TECH_WAKE_UP_DATA_LEN] , p_buf , len);
	
	uart_BT.tx_buff.p_buff = tmp_buffer;	
	uart_BT.tx_buff.buff_len = len + N_TECH_WAKE_UP_DATA_LEN;
	
#endif	
						
	uart_send_non_blocking(uart_BT.uart_no,&uart_BT.tx_buff);

	
	memset(&g_output_cmd,0,sizeof(g_output_cmd));
}



void n_tech_data_handler(void)
{
	uint8_t tmp_buffer[MAX_DATA_PACKET_SIZE];
	uint16_t rx_buffer_data_len = 0;
	uint8_t tmp_len = 0;
	uint16_t response_op_code = 0;
	uint8_t n_tech_retry_count = 0;
    uint8_t op_waiting_time_out = 0;


	rx_buffer_data_len = uart_get_rx_buffer_data_len(uart_BT.uart_no);


	//check RX buffer if no input cmd 
	if(g_input_cmd.op_code == 0)
	{
		if(rx_buffer_data_len >= MIN_DATA_PACKET_SIZE)
		{
			
			memset(tmp_buffer,0,sizeof(tmp_buffer));

			tmp_len = rx_buffer_data_len;

			if(tmp_len > MAX_DATA_PACKET_SIZE)
			{
				tmp_len = MAX_DATA_PACKET_SIZE;
			}
			

			uart_ReadRxBufferOnly(uart_BT.uart_no, tmp_buffer, tmp_len);
			
			
			tmp_len = read_n_tech_data(tmp_buffer,tmp_len); // return len of data need to remove from rx buffer 
			
			uart_DeleteRxBufferBytes(uart_BT.uart_no,tmp_len);
		}

		if(g_op_code_wait_for_response)
		{
			response_op_code = n_tech_get_expected_op_code();
			if((response_op_code == ((uint8_t)g_input_cmd.op_code)) || n_tech_retry_count > 10) // #define THRESHOLD
			{
				g_op_code_wait_for_response = 0;
				n_tech_retry_count = 0;
			}
			else
			{
				if(op_waiting_time_out > 100)
				{
					if(g_output_cmd.op_code == 0)
					{						
						n_tech_output_cmd_handler((g_output_cmd.op_code - N_TECH_CODE_OP_NEED_RESPONDE),g_output_cmd.receive_id);
						n_tech_retry_count++;
						op_waiting_time_out = 0;
					}
				}
				else
				{
					op_waiting_time_out++;
				}
			}

		}
		
	}
	
	//handle input cmd if output cmd if free to use
	if(g_input_cmd.op_code && g_output_cmd.op_code == 0)
	{
       //printf("g_input_cmd.op_code = %#X",g_input_cmd.op_code);

       
		n_tech_input_cmd_handler(); // process input cmd , clear it , and fill response in g_output_cmd
	}

	if(uart_tx_buff_emty(uart_BT.uart_no))// have no data in tx buffer , means uart tx free
	{
		
		if(g_output_cmd.op_code)
		{

			memset(tmp_buffer,0,sizeof(tmp_buffer));
			
			tmp_buffer[START_DATA_INDEX] = START_DATA_VALUE;
			tmp_buffer[PAYLOAD_LEN_DATA_INDEX] = g_output_cmd.payload_len;
			tmp_buffer[SEQ_ID_DATA_INDEX] = g_output_cmd.sequence_id;
			tmp_buffer[SEND_ID_INDEX] = g_output_cmd.send_id;
			tmp_buffer[RECEIVE_ID_INDEX] = g_output_cmd.receive_id;
			tmp_buffer[OP_CODE_DATA_INDEX] = g_output_cmd.op_code & 0xFF;
			tmp_buffer[OP_CODE_DATA_INDEX+1] = g_output_cmd.op_code >> 8 ;
			memcpy(&tmp_buffer[PAYLOAD_DATA_INDEX],&g_output_cmd.payload_data[0],g_output_cmd.payload_len);
			
			tmp_buffer[CHECKSUM_DATA_INDEX_WITHOUT_PAYLOAD + g_output_cmd.payload_len ] = cal_CheckSum(&tmp_buffer[0]);
			tmp_buffer[CHECKSUM_DATA_INDEX_WITHOUT_PAYLOAD + g_output_cmd.payload_len + CHECKSUM_DATA_LEN] = STOP_DATA_VALUE;

			
			
			n_tech_send_packet(tmp_buffer,(CHECKSUM_DATA_INDEX_WITHOUT_PAYLOAD + g_output_cmd.payload_len + CHECKSUM_DATA_LEN+END_DATA_LEN));// put d tech packet in tx buffer , clear g_output_cmd

			


			op_waiting_time_out = 0;
		}
		
	}
}



void BT_N_Tech_Task(void) // 5ms 
{ 
   static uint8 set_name_flag = 0;

   if(set_name_flag < 100)
   {
	set_name_flag++;

   }
   
   if(set_name_flag == 100)

   {
	n_tech_set_BTname_cmd();
	set_name_flag = 101;
   }
	
	n_tech_data_handler();


}







void Read_AFE_Infor(void) // 10ms
{
	static uint8 cell_cnt = 0;
	
  if(cell_cnt < CELL_AMOUNT)
	{
		read_cell_vol(cell_cnt);
		cell_cnt++;
		
		if(cell_cnt >= CELL_AMOUNT)
		{
			cell_cnt = 0;
		}
	}
	
	read_pack_current();
	read_pack_dsg_current(); 
	
}



void Cal_Cell_Volt(void)  //100ms
{
  //when run this, all cell values should be ready.												
  calt_max_cell_volt(Sys_Info.Cell_Volt, CELL_AMOUNT, &Cell_Para.cell_vol_max_no, &Cell_Para.cell_vol_max);
  calt_min_cell_volt(Sys_Info.Cell_Volt, CELL_AMOUNT, &Cell_Para.cell_vol_min_no, &Cell_Para.cell_vol_min);	
  calt_aver_cell_volt(Sys_Info.Cell_Volt, CELL_AMOUNT, &Cell_Para.cell_vol_aver);

}





uint16 moving_average(uint16 new_val, uint8 *index, uint16 *mov_buff)   
{                                                                        

    uint8 i = 0;
    uint32 sum = 0;
    
    (*index)++;
    
    if((*index) >= MOVING_AVERAGE_BUFF_SIZE)
    {
        (*index) = 0;
    }
    
    mov_buff[(*index)] = new_val;
    
    for(i=0; i<MOVING_AVERAGE_BUFF_SIZE; i++)
    {
        sum += mov_buff[i];
    }
    
    return(sum/MOVING_AVERAGE_BUFF_SIZE);
    
}


void check_max_min_cell_volt_mov_aver(void)
{
	static uint8 i = 0;

   for(;i<MOVING_AVERAGE_BUFF_SIZE; i++)
   {
		mov_buff_max_cell_Volt[i] = Cell_Para.cell_vol_max;	
		mov_buff_min_cell_Volt[i] = Cell_Para.cell_vol_min;	

   }


	mov_buff_max_cell_Volt_result = moving_average(Cell_Para.cell_vol_max, &mov_buff_max_cell_Volt_index, mov_buff_max_cell_Volt);
	mov_buff_min_cell_Volt_result = moving_average(Cell_Para.cell_vol_min, &mov_buff_min_cell_Volt_index, mov_buff_min_cell_Volt);



}


void Read_NTC_infor(void) //100ms
{
  uint16 pack_volt[5] = {0};
  uint8 i = 0;	
	
	for(i=0; i<5; i++)
	{
	 pack_volt[i] = read_pack_vol();
	
	}
	
	Sys_Info.Pack_Vol = Func_Do_Avg(pack_volt,5);
	
	read_mosfet_temperature();	
	read_cell_temperature();				 
							

}

void Feed_Watchdog(void)  // 500ms
{
  read_AFE_event();
  read_AFE_Chip_Temp();
  AFE_write_register(AFE_WATCH_DOG_REG_ADDR, 0xFF75);
}

void A_Tech_36V_COM_Task(void) //100ms
{
	static uint8 Dly_to_Send_Data_cnt = 0; 

   if(Sys_Info.Pack_Status.st_pack_state.lock_state)
   {
     return;
   }
	

	
    if(g_Chg_Detection)  
	{
		 if(Dly_to_Send_Data_cnt < 10) //10 * 50ms = 0.5s < 3s
		 {
				Dly_to_Send_Data_cnt++;
		 }
		 else
		 {
				if(CHG_Com_Data.send_enable == 0)
				{
					
					CHG_Com_Data.send_enable = 1;
						
				}
		 }
	}
	else
	{
		Dly_to_Send_Data_cnt = 0; 
	}
	
}



void calculate_soc(uint16 max_cell_volt, uint16 min_cell_volt, uint8 charge_mode)
{

	if(charge_mode)
	{
		Sys_Info.soc = get_soc(max_cell_volt);
	}
	else
	{
		Sys_Info.soc = get_soc(min_cell_volt);
	}

 

}




#define PACK_CURRENT_OFFSET 1000 //mA

void BMS_Task(void)					// 10ms
{
  	static uint8 Rd_info_Cnt = 0;
  	static uint16 Feed_WD_Cnt = 0;
	static uint8 Dsg_Starup_Flag = 0;
	
	Rd_info_Cnt += BMS_TASK_INTERVAL;
	Feed_WD_Cnt += BMS_TASK_INTERVAL;
	
	
	Read_AFE_Infor(); 
	
	if(Rd_info_Cnt >= BMS_READ_INFO_INTERVAL)
	{
			
		Rd_info_Cnt = 0;
		
		if(Dsg_Starup_Flag < 10)
		{
			Dsg_Starup_Flag ++;
		}
		
		
	  	Cal_Cell_Volt();   				//100ms
		Read_NTC_infor();  				//100ms

		check_max_min_cell_volt_mov_aver();

		calculate_soc(mov_buff_max_cell_Volt_result, mov_buff_min_cell_Volt_result, g_Chg_Detection);
		
		if(g_Chg_Detection)
		{

		
		  Charging_Control();
		  discharging_sta = DISCHING;

		  Sys_Info.Pack_Status.st_pack_state.chg_state = CHARGING;
		  
		}
		else
		{
		  charging_state = CHAR_INIT;

		  if((Sys_Info.Pack_Current - PACK_CURRENT_OFFSET) < (-1000))
		  {
				Sys_Info.Pack_Status.st_pack_state.chg_state = DISCHARGING;

				CHG_Com_Data.un_chg_info.chg_info = 0;
				
		  }
		  else
		  {
				Sys_Info.Pack_Status.st_pack_state.chg_state = IDLE;


		  }


		  
		}	
			
	}
	
	if(Dsg_Starup_Flag > 2)
	{
	  	if(!g_Chg_Detection)
		{
			 Discharging_Control(); 	//10ms
		}
		
	}
		
	if(Feed_WD_Cnt >= BMS_FEED_WTD_INTERVAL)
	{ 
	
		Feed_WD_Cnt = 0;		
		Feed_Watchdog();  				//500ms
	}

		
}



void D_Teck_Task(void) 
{
	uint8 rb_rx_tmp_buff[RING_RX_BUFFER_SIZE] = {0};
	uint32 bytes;
	
	bytes = uart_get_rx_buffer_data_len(uart_D_Tech.uart_no);	
	
	if(bytes) 
	{
		uart_D_Tech.rx_buff.buff_len = bytes;			
		uart_read_rx_buffer(uart_D_Tech.uart_no,&uart_D_Tech.rx_buff);
		
		if(uart_tx_buff_emty(uart_D_Tech.uart_no))
		{
			memcpy(&rb_rx_tmp_buff[0], uart_D_Tech.rx_buff.p_buff, bytes);
			uart_D_Tech.tx_buff.p_buff = &rb_rx_tmp_buff[0];
			uart_D_Tech.tx_buff.buff_len = bytes;						
			uart_send_non_blocking(uart_D_Tech.uart_no,&uart_D_Tech.tx_buff);
		}
		
	}	
		
}



void Pwr_Tool_Com_Task(void)
{

}



void Black_Box_Task(void)
{

}



void Debug_Task(void)
{

//	uint8 i = 0;
//		
//	for(i=0;i<10;i++)
//	{
//		printf("Sys_Info.Cell_Volt[%d] = %d mV\n",i,Sys_Info.Cell_Volt[i]); 				
//	}
//	printf("Sys_Info.Pack_Vol = %d mV\n",Sys_Info.Pack_Vol );
//	printf("Cell_Para.cell_vol_max	= %d mV\n",Cell_Para.cell_vol_max);
//	printf("Cell_Para.cell_vol_min	= %d mV\n",Cell_Para.cell_vol_min);
//	printf("Cell_Para.cell_vol_aver = %d mV\n",Cell_Para.cell_vol_aver);

//	printf("Sys_Info.Pack_Current =  %d (mA)\n", Sys_Info.Pack_Current);
	
//	printf("Sys_Info.soc =  %d \n", (Sys_Info.soc / 10));
	
//	printf("chg_state =  %d \n", Sys_Info.Pack_Status.st_pack_state.chg_state);
	
	
//	printf("Sys_Info.AFE_Event = 0x%X\n",Sys_Info.AFE_Event);
//	
//	printf("Sys_Info.AFE_Chip_Temp = %d DegreeC\n",Sys_Info.AFE_Chip_Temp);  
//	printf("Sys_Info.FET_Temp = %d C\n",(Sys_Info.FET_Temp - ABSOLUTE_TMEP_OFFSET));
//	printf("Sys_Info.Cell_Temp = %d C\n",(Sys_Info.Cell_Temp - ABSOLUTE_TMEP_OFFSET ));

}




int main (void)
{	
	
	system_clock_init();
	MRT_Init();
	GPIO_Init();	
	Pin_Interrupt_Init();	
	ADC_Init();
	I2C_Init();
	UART_Init();
	AFE_Init();
	Debug_Interface_Init();
	System_Tick_Init();

	printf("Start up!\n");

		
	task_scheduler();

  	
	
	
}





