#include <avr/io.h>
#include <util/delay.h>
#include "include/rfm12b.h"
#include "include/bitLib.h"

//#define DATA_RATE 0xC619
#define DATA_RATE 0xC60b

/** \file rfm12b.c
 * \brief RFM12B module specific code.
 *
 * This module contains all code needed to initialize and
 * use the RFM12B module from hopeRF.
 */



/** \brief Reset the FIFO buffer.
 *
 * A Call to this procedure resets the internal FIFO buffer.
 */
void rf12_fifoReset()
{
	rf12_send_command(0xCA81);
	rf12_send_command(0xCA83);
}

/** \brief Setup the AVR ports used by the Module
 *
 * Set up the correct data direction and state for the
 * ports used by the RFM12B as specifed in rfm12b.h
 */
void rf12_portSetup()
{
	RF_PORT = 0x0;
	HI(CS);
	HI(SDI);
	LO(SCK);
	RF_DDR = (1<<CS) | (1<<SDI) | (1<<SCK);
	HI(NIRQ);
	HI(CS);
	LO(SDI);
	HI(SDO);
}

/** \brief Initialize the RFM12B module.
 * 
 */
int rf12_init() 
{
	_delay_ms(2000);					// wait until POR done
	rf12_send_command(0x80D7); //EL,EF,868band,12.0pF
	rf12_send_command(0x8299); //er,!ebb,ET,ES,EX,!eb,!ew,DC (bug was here)
	rf12_send_command(0xA640); //freq select
	rf12_send_command(DATA_RATE); // 10kbps
	rf12_receiverControlCommand(1,0,1,3,5);
	rf12_send_command(0xC2AF); //AL,!ml,DIG,DQD4
	rf12_send_command(0xCA81); //FIFO8,SYNC,!ff,DR (FIFO level = 8)
	rf12_send_command(0xCED4); //SYNC=2DD4;
	rf12_send_command(0xC483); //@PWR,NO RSTRIC,!st,!fi,OE,EN
	rf12_send_command(0x9850); //mp,90kHz,MAX OUT
	rf12_send_command(0xCC17); //OB1,!OB0, LPX,!ddy,DDIT,BW0
	rf12_send_command(0xE000); //NOT USE
	rf12_send_command(0xC800); //NOT USE
	rf12_send_command(0xC040); //1.66MHz,2.2V
	return 1;
}

/** \brief Switch transciever into RX mode.
 *
 * Switch into RX mode as fast as possible.
 */
void rf12_switchToRx()
{
	rf12_send_command(0x0000);
	rf12_send_command(0x8299);
	rf12_receiverControlCommand(1,0,1,3,5);
}

/** \brief Switch transciever into TX mode.
 *
 * Switch into TX mode as fast as possible.
 */
void rf12_switchToTx()
{
	rf12_send_command(0x0000);
	rf12_send_command(0x8205);
	rf12_send_command(0x80D7);
	rf12_send_command(0x8239);
}

/** \brief Initialize Transciever into TX mode.
 *
 */
int rf12_initTx()
{
	unsigned int tmp;
	tmp = rf12_send_command(0x0000);
	rf12_send_command(0x8205);
	rf12_send_command(0xB800);
	while (! (PINC&1) )
		rf12_send_command(0x0000);
	rf12_send_command(0x80D7); // 433Mhz BAND, FIFO mode, cap
	rf12_send_command(0x8239); //er,!ebb,ET,ES,EX,!eb,!ew,DC (bug was here)
	rf12_send_command(0xA640); //freq select
	rf12_send_command(DATA_RATE); //4.8kbps
	rf12_send_command(0x94A0); //VDI,FAST,134kHz,0dBm,-103dBm
	rf12_send_command(0xC2AC); //AL,!ml,DIG,DQD4
	rf12_send_command(0xCA81); //FIFO8,SYNC,!ff,DR (FIFO level = 8)
	rf12_send_command(0xCED4); //SYNC=2DD4;
	rf12_send_command(0xC483); //@PWR,NO RSTRIC,!st,!fi,OE,EN
	rf12_send_command(0x9850); //(!)mp,90kHz,MAX OUT
	rf12_send_command(0xCC17); //(!)OB1,!OB0, LPX,!ddy,DDIT,BW0
	rf12_send_command(0xE000); //NOT USE
	rf12_send_command(0xC800); //NOT USE
	rf12_send_command(0xC040); //1.66MHz,2.2V
	return 1;
}

/** \brief Setup the reciever control command
 *  \param vdi			0 = nINT, 1 = VDI
 *  \param response		0 = fast, 1 = Medium , 2 = Slow , 3 = Always on
 *  \param bandwidth		1 = 400kHz, 2 = 340kHz, 3 = 270kHz, 4 = 200kHz, 5 = 134, 7 = 67kHz
 *  \param LNA_GAIN		0 = 0, 1 = -6dBm, 2 = -14dBm, 3 = -20dBm
 *  \param RSSI_THRESHOLD	0 = -103, 1 = -97, 2 = -91, 3 = -85, 4 = -79, 5 = -73
 *
 */
void rf12_receiverControlCommand(unsigned char vdi,unsigned char response,unsigned char bandwidth,unsigned char LNA_GAIN,unsigned char RSSI_THRESHOLD)
{
	rf12_send_command(0x9000|((vdi&1)<<10)|((response&3)<<8)|((bandwidth&7)<<5)|((LNA_GAIN&3)<<3)|(RSSI_THRESHOLD&7));
}

/** \breif Setup the data filter
 *
 * Setup the register for data filter
 * \param clkRecoveryMode	0 = Auto, 1 = Manual	 
 */
void rf12_dataFilterCommand(uint8_t clkRecoveryMode,uint8_t clkRecoverySpeed,uint8_t dataFilterType,uint8_t dataQualityThreshold)
{
}



/** \brief Software SPI send.
 * 
 * Software SPI implementation to communicate with
 * RFM12B module.
 * 
 * \param cmd	uint16_t to send to RFM12B
 *
 */
unsigned int rf12_send_command(unsigned int cmd) 
{
	unsigned char i;
	unsigned int recv;
	recv = 0;
	LO(SCK);
	LO(CS);
	for(i=0; i<16; i++) 
	{
		if(cmd&0x8000) 
		{
			HI(SDI); 
		}
		else
		{ 
			LO(SDI);
		}
		
		HI(SCK);
		recv = recv << 1;
		if( RF_PIN&(1<<SDO) ) 
		{
			recv|=0x0001;
		}
		
		LO(SCK);
		cmd = cmd << 1;
	}
	HI(CS);
	return recv;
}

/** \brief Set baudrate of transciever
 *
 * \param baud uint8_t baudrate
 */
void rf12_set_baud(unsigned short baud)
{
	if (baud<664)
		baud=664;
	if (baud<5400)						// Baudrate= 344827,58621/(R+1)/(1+CS*7)
		rf12_send_command(0xC680|((43104/baud)-1));	// R=(344828/8)/Baud-1
	else
		rf12_send_command(0xC600|((344828UL/baud)-1));	// R=344828/Baud-1
}

/** \brief Set power of transciever in dBm
 *
 * \param power Power see datasheet
 * \param mod Mod see datasheet
 */
void rf12_set_power(unsigned char power, unsigned char mod)
{	
	rf12_send_command(0x9800|(power&7)|((mod&15)<<4));
}

/** \brief Set frequency of transciever.
 *
 * \param freq frequency
 */
void rf12_set_freq(unsigned short freq)
{	if (freq<96)						// 430,2400MHz
		freq=96;
	else if (freq>3903)					// 439,7575MHz
		freq=3903;
	rf12_send_command(0xA000|freq);
}

/** \breif Wait until the FIFO is ready
 *
 */
static inline void rf12_ready(void)
{
	cbi(RF_PORT, CS);
	asm("nop");
	asm("nop");
	while (  !( PINC & (1 << 4) ));			// wait until FIFO ready
	sbi(RF_PORT, CS);
}

/** \brief Transmit byte
 *
 * \param val uint8_t to transmit
 */
void rf12_txbyte(unsigned char val)
{
	while( PINC&1 );
	rf12_send_command(0xB800 + val);
}

/** \brief Set trigger level for RSSI statusbit (8)
 *
 * \param level 0 - 6 
 */
void rf12_setRSSI(uint8_t level)
{
	rf12_send_command(0x94A0 | level);
	_delay_ms(1);	
}

/** \brief Returns 1 when a carrier is detected to avoid RF collision.
  *
  */
inline uint8_t rf12_getRSSI(void)
{
	return (rf12_send_command(0x0000) >> 8) & 1;
}



/** \brief Send data via RF
 *
 */
void rf12_send(char* data,unsigned char size)
{
	unsigned char cnt = 0;
	rf12_send_command(0x0000);
	rf12_fifoReset();
	rf12_txbyte(0xaa);
	rf12_txbyte(0xaa);
	rf12_txbyte(0xaa);
	rf12_txbyte(0x2d);
	rf12_txbyte(0xd4);
	for (cnt=0;cnt<size;cnt++)
	{
		rf12_txbyte(data[cnt]);
	}
	rf12_txbyte(0xaa);
	rf12_txbyte(0xaa);
	rf12_txbyte(0xaa);		
}
