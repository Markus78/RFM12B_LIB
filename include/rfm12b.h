#ifndef RFM12B_H
#define RFM12B_H

// Change according to hardware platform used this is for LINNODE v0.1 ( my HW platform based on atmega328p )
// Modify to suit your needs

#define RF_PORT PORTC
#define RF_DDR DDRC
#define RF_PIN PINC
#define SDI 3 		// SPI Data input (RFM12B side)
#define SCK 5 		// SPI clock
#define CS 2		// SPI SS (chip select)
#define SDO 4 		// SPI Data output (RFM12B side)
#define NIRQ 0 		// (PORTC)

#define RF12FREQ(freq)	((unsigned short)((freq-430.0)/0.0025))	
// DEFINES

#define RF_CMD_STATUS_READ		0x0000		// Read status Register
#define RF_RECEIVER_ON  		0x82D9
#define RF_XMITTER_ON   		0x8239
#define RF_IDLE_MODE    		0x8209
#define RF_SLEEP_MODE   		0x8201
#define RF_WAKEUP_MODE  		0x8203
#define RFM12_0000_RGIT_FFIT	0x8000		// TX ready for next byte or FIFO received data Status										
#define RFM12_0000_POR			0x4000		// Power on Reset Status
#define RFM12_0000_RGUR_FFOV	0x2000		// TX Register underun or RX FIFO Overflow Status
#define RFM12_0000_WKUP			0x1000		// Wakeup Timer overflow Status
#define RFM12_0000_EXT			0x0800		// Interrup on external source Status
#define RFM12_0000_LBD			0x0400		// Low battery detect Status
#define RFM12_0000_FFEM			0x0200		// FIFO Empty Status
#define RFM12_0000_ATS			0x0100		// Antenna Tuning Signal Detect Status
#define RFM12_0000_RSSI			0x0080		// Received Signal Strength Indicator Status
#define RFM12_0000_DQD			0x0040		// Data Quality Dedector Status
#define RFM12_0000_CRL			0x0020		// Clock Recovery Locked status
#define RFM12_0000_ATGL			0x0010		// Toggling in each AFC Cycle
#define RFM12_0000_OFFS_SIGN	0x0008		// Measured Offset Frequency Sign Value 1='+', 0='-' 
#define RFM12_0000_OFFS			0x0004		// Measured offset Frequency value (3 bits) 
#define RFM12_0000_OFFS_MASK	0x0003		// Measured offset mask

	extern void			rf12_send(char* data,unsigned char size);
	extern void			rf12_switchToRx(void);
	extern void			rf12_switchToTx(void);
	extern void			rf12_fifoReset(void);
	extern void			rf12_portSetup(void);
	extern int 			rf12_init(void);
	extern int			rf12_initTx(void);
	extern unsigned int rf12_send_command(unsigned int cmd);
	extern void 		rf12_set_baud(unsigned short baud);
	extern void			rf12_set_power(unsigned char power, unsigned char mod);
	extern void			rf12_receiverControlCommand(unsigned char vdi,unsigned char response,unsigned char bandwidth,unsigned char LNA_GAIN,unsigned char RSSI_THRESHOLD);
	extern void 		rf12_set_freq(unsigned short freq);
	extern unsigned char  rf12_getRSSI(void);
	extern void			rf12_setRSSI(unsigned char level);
#endif

