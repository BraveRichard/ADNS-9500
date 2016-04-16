/*
###############################################################################
@copyright			: 	Affinext Technology Sdn Bhd.
@filename 			: 	Sequence.C
@version			: 	1.0.0
Programmer(s) 		: 	cc looi
Created 			: 	23/05/2005
Description 		: 	Sequence/business implemention file
                 
Modified History	: 	Description             
27/01/2005				Initial created. 
03/02/2005				Modified for project used)
09/09/08				Electrical Fast Transient Burst Added(WTD)

###############################################################################
*/

//------------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------------
#include "c8051f3xx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <intrins.h>
#include <stdlib.h>
#include "spi.h"
#include "timer.h"
#include "F3xx_USB0_InterruptServiceRoutine.h"
#include "F3xx_USB0_ReportHandler.h"
#include "F3xx_USB0_Mouse.h"
#include "ADNS9500.h"
#include "F340_FlashPrimitives.h"
#include "delay.h"
#include "crc_check.h"


	////////////////////////////////////////////////////////////////////////////
	// User Code Here: Add business logic 
	////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////
	// End User Code 
	////////////////////////////////////////////////////////////////////////////

#define SW_DELAY 	2
#define SW_DELAY_2 	50

//------------------------------------------------------------------------------------
// Global VARIABLES
//------------------------------------------------------------------------------------
unsigned char data seq0_state, seq1_state, seq2_state, seq3_state, seq4_state;
unsigned char data seq5_state, seq6_state, seq7_state, seq8_state, seq9_state;
unsigned char data seq10_state, seq11_state, seq12_state, led_blink_state;

bdata unsigned char dummy1_bit;
sbit dummy1_bit7 = dummy1_bit^7;
sbit dummy1_bit6 = dummy1_bit^6;
sbit dummy1_bit5 = dummy1_bit^5;
sbit dummy1_bit4 = dummy1_bit^4;
sbit dummy1_bit3 = dummy1_bit^3;
sbit dummy1_bit2 = dummy1_bit^2;
sbit dummy1_bit1 = dummy1_bit^1;
sbit dummy1_bit0 = dummy1_bit^0;

unsigned char sent_buttons;

bdata unsigned char buttons;
sbit left_button	= buttons^0;
sbit right_button	= buttons^1;
sbit middle_button	= buttons^2;

bdata unsigned char z_wheel;
sbit ZA_PRE			= z_wheel^0;
sbit ZB_PRE			= z_wheel^1;
sbit ZA_CUR			= z_wheel^2;
sbit ZB_CUR			= z_wheel^3;

bdata unsigned char LED_DAT;
sbit LED_B0 = LED_DAT^0;
sbit LED_B1 = LED_DAT^1;
sbit LED_B2 = LED_DAT^2;


U16 shutter;
data U16 para1, para2;

S16 count_x, count_y;
S16 motion_x, motion_y; 

data signed char count_z;
data unsigned char squal;

data unsigned char resolution;


unsigned char divider, multiplier;
data unsigned char calibration_mode;
unsigned char display_buffer[5];

data unsigned char motion_burst[12];

U16 srom_size;
unsigned char srom_status, srom_data;
unsigned int srom_addr, srom_count;
bit srom_start, srom_success;

bit resolution_changed;
bit lift_changed;
bit lift_detection_mode;
bit stop_motion;
bit auto_reset;
bit received;
bit mouse_enabled;

unsigned char temp;

data unsigned char IN_PACKET[10];
data unsigned char OUT_PACKET[5];
data unsigned char grab_image, grab_start;
unsigned char image_buffer[452];
data unsigned int pixel_count;




code const unsigned char flash_data[512] _at_ 0x3A00;
code const unsigned char flash_data1[512] _at_ 0x3800;



//------------------------------------------------------------------------------------
// External VARIABLES
//------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------
// Function PROTOTYPES
//------------------------------------------------------------------------------------
void Sequence0(void);
void Sequence1(void);
void Sequence2(void);
void Sequence3(void);
void Sequence4(void);
void Sequence5(void);
void Sequence6(void);
void Sequence7(void);
void Sequence8(void);
void Sequence9(void);
void Sequence10(void);
void Sequence11(void);
void Sequence12(void);

void shift_8bits(unsigned char data_in);

void read_sensor_motion_burst(void);
void read_sensor_motion_registers(void);
void update_led_status(unsigned char res);
void update_led_status_2(unsigned char lift_data);
void led_blink (unsigned char LED_DAT);
void WTD_Reload(void);



//------------------------------------------------------------------------------------
// External Function PROTOTYPES
//------------------------------------------------------------------------------------




//------------------------------------------------------------------------------------
// InitSeq
//------------------------------------------------------------------------------------
void Sequence_Init(void)
{
	seq0_state = 0;
	seq1_state = 0;
	seq2_state = 0;
	seq3_state = 0;
	seq4_state = 0;
	seq5_state = 0;
	seq6_state = 0;
	seq7_state = 0;
	seq8_state = 0;
	seq9_state = 0;
	seq10_state = 0;
	seq11_state = 0;
	seq12_state = 0;

	srom_start = 0;
	srom_success = 0;
	led_blink_state = 0;

	z_wheel = 0;
	ZA_PRE = ZA;
	ZA_CUR = ZA;
	ZB_PRE = ZB;
	ZB_CUR = ZB;
	sent_buttons = 0;
	buttons = 0;
	grab_image = 0;
	grab_start = 0;

	count_x.w = 0;
	count_y.w = 0;
	count_z = 0;
	received = 0;
	resolution_changed = 0;
	lift_changed = 0;
	lift_detection_mode = 0;

	LED_DAT = 0xFF;
	LED_BIT2 = LED_B2;
	LED_BIT1 = LED_B1;
	LED_BIT0 = LED_B0;

	auto_reset = 1;
	mouse_enabled = 1;
/*
	if(LEFT_SW == 0)
	{
		calibration_mode = 1;
	}
	else
	{
		calibration_mode = 0;
	}
*/
	Timer3Init();	
	spi_init();	
	ADNS9500_Init();
	USB0_Init ();

	lift_data = 0x10;
}
//------------------------------------------------------------------------------------
// InitWTD
//------------------------------------------------------------------------------------

void WDT_Enable(void)
{
/*
	// 30 ms
    PCA0MD    &= ~0x40;
    PCA0MD    = 0x00;
    PCA0CPL4  = 0xE9;
    PCA0MD    |= 0x40;
*/	

	
	// 15 ms
	PCA0MD    &= ~0x40;
    PCA0MD    = 0x00;
    PCA0CPL4  = 0x74;
    PCA0MD    |= 0x40;
}

void WDT_Disable(void)
{
	PCA0MD &= ~0x40;                    // Disable Watchdog timer
}

//------------------------------------------------------------------------------------
// ReloadWTD
//------------------------------------------------------------------------------------

void WTD_Reload(void)
{
	//PCA0CPH4 = 0xEA;
	PCA0CPH4 = 0x75;
}


//------------------------------------------------------------------------------------
// Main Sequence
//------------------------------------------------------------------------------------
void Sequence(void)
{		
	Sequence_Init();
/*	if(calibration_mode == 1)
	while(1)
	{
		if(received == 1)
		{
			EA = 0;
			received = 0;
			IN_PACKET[0] = OUT_PACKET[0];
			switch(IN_PACKET[0])
			{
				case WRITE_EEPROM:
					IN_PACKET[1] = 1;
					IN_PACKET[2] = OUT_PACKET[2];
					if(OUT_PACKET[1] == 1)
					{
						FLASH_PageErase(0x3A00 + OUT_PACKET[1]);
					}
					FLASH_ByteWrite (0x3A00 + OUT_PACKET[1], OUT_PACKET[2]);
					break;
				case READ_EEPROM:
					IN_PACKET[1] = 1;
					IN_PACKET[2] = flash_data[OUT_PACKET[1]];
					break;
				case WRITE_SENSOR:
					IN_PACKET[1] = 1;
					IN_PACKET[2] = OUT_PACKET[2];
					spi_write_sensor(OUT_PACKET[1], OUT_PACKET[2]);
					break;
				case READ_SENSOR:
					IN_PACKET[1] = 1;
					IN_PACKET[2] = spi_read_sensor(OUT_PACKET[1]);
					break;
				case TEST_SENSOR:
					IN_PACKET[1] = 1;
					IN_PACKET[2] = OUT_PACKET[2];
					spi_init();
					spi_write_sensor(ADNS9500_CONFIG_BITS_ADDR, 0x08);
					break;
				case ECHO_COMMAND:
					IN_PACKET[1] = OUT_PACKET[1];
					IN_PACKET[2] = OUT_PACKET[2];
					break;
				case RESET_SENSOR:
					IN_PACKET[1] = 1;
					IN_PACKET[2] = OUT_PACKET[2];
					RESET = 1;
					delay_us(100);
					RESET = 0;
					break;
				default:
					break;

			}
			EA = 1;
			if(EP_STATUS[1] == EP_IDLE)
			{
				IN_BUFFER.Ptr = IN_PACKET;
				IN_BUFFER.Length = 3;

				SendPacket();
			}
		}
	}
	else
	{*/

		WDT_Enable();		//Init WTD	

		while(1)
		{
//			Sequence0();	// heart beat
//			WTD_Reload();	// Reload WatchDog

			if(srom_start == 0)
			{
				Sequence1();	// Image Dump
				WTD_Reload();	// Reload WatchDog

				if(grab_start == 0)
				{
					Sequence2();	// Z-Wheel Motion Detection	
					WTD_Reload();	// Reload WatchDog

					Sequence3();	// Left Switch Detection
					WTD_Reload();	// Reload WatchDog

					Sequence4();	// Right Switch Detection
					WTD_Reload();	// Reload WatchDog

					Sequence5();	// Middle Switch Detection
					WTD_Reload();	// Reload WatchDog

					Sequence6();	// Sensor Motion Detection
					WTD_Reload();	// Reload WatchDog

					Sequence7();	// USB Data Packet Send
					WTD_Reload();	// Reload WatchDog

					Sequence8();	// CPI Changing Detection
					WTD_Reload();	// Reload WatchDog

					Sequence9();	// Store CPI Data to EEPROM
					WTD_Reload();	// Reload WatchDog

					Sequence10();	// Lift Detection mode
					WTD_Reload();	// Reload WatchDog
					
					if(mouse_enabled == 1)
					{
						Sequence11();	// Sensor health check
						WTD_Reload();	// Reload WatchDog
					}
				}
			}
			else
			{
				Sequence12();	// SROM download
				WTD_Reload();	// Reload WatchDog			
			}
		}
	
}

//------------------------------------------------------------------------------------
// Sequence0
//------------------------------------------------------------------------------------
/*
void Sequence0(void)
{
 	switch(seq0_state)
	{
		case 0:	
//				if(enable == 1)
				{
				t0_data = 900;	// x 10 milisecond
				time_0 = 1;
				seq0_state =10;
				}
				break;
		case 10:	
				if(time_0==0)
				{
					LED_HB = 0; // on					
					seq0_state = 20;
				}
				break;
		case 20:	
				t0_data = 60;
				time_0 = 1;
				seq0_state = 30;
				break;
		case 30:	
				if(time_0==0)
				{
					LED_HB = 1;	// off
					seq0_state =40;
				}				
				break;
		case 40:	
				seq0_state =0;
				break;
				
		default:	
				break;			
    }
}
*/
//------------------------------------------------------------------------------------
// Sequence1
//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------
// Sequence1
//------------------------------------------------------------------------------------
void Sequence1(void)
{
	switch(seq1_state)
	{
		case 0:
			if(grab_image) // change
			{
				stop_motion = 1;
				image_buffer[0] = 2; 	// report ID
				image_buffer[1] = 0;	// image no ready
				pixel_count = 0;
				grab_image = 0;				
				grab_start = 1;				
				seq1_state  = 2;
			}
			break;
		case 2:
			if(time_1 == 0)
			{
				EA = 0;
				spi_write_sensor(ADNS9500_POWER_UP_RESET_ADDR, ADNS9500_POWER_UP_RESET);				// Check SROM version
				EA = 1;
				t1_data = TIMER1_50_MILISEC;
				time_1 = 1;
				seq1_state  = 4;
			}
			break;
		case 4:
			if(time_1 == 0)
			{
				EA = 0;
				temp = spi_read_sensor(ADNS9500_LASER_CTRL0_ADDR);
				spi_write_sensor(ADNS9500_LASER_CTRL0_ADDR, (temp & ~0x01));				
				EA = 1;
				t1_data = TIMER1_50_MILISEC;
				time_1 = 1;
				seq1_state  = 10;
			}
			break;

		case 10:
			if(time_1 == 0)
			{
				EA = 0;
				spi_write_sensor(ADNS9500_FRAME_CAPTURE_ADDR, 0x93);
				EA = 1;
				t1_data = TIMER1_50_MILISEC;
				time_1 = 1;
				seq1_state = 12;
			}
			break;
		case 12:
			if(time_1 == 0)
			{
				EA = 0;
				spi_write_sensor(ADNS9500_FRAME_CAPTURE_ADDR, 0xC5);
				EA = 1;
				t1_data = TIMER1_50_MILISEC;
				time_1 = 1;
				seq1_state = 20;
			}
			break;
		case 20:
			if(time_1 == 0)
			{
				EA = 0;
				temp = spi_read_sensor(ADNS9500_MOTION_ADDR);
				EA = 1;
				t1_data = TIMER1_50_MILISEC;
				time_1 = 1;
				if((temp & ADNS9500_MOTION_PIXFIRST) == ADNS9500_MOTION_PIXFIRST)
				{
					seq1_state = 22;
				}
			}
			break;
		case 22:
			if(time_1 == 0)
			{
				NCS = 0;
				spi_transfer(ADNS9500_PIXEL_BURST_ADDR & ~0x80);
				t1_data = TIMER1_50_MILISEC;
				time_1 = 1;
				seq1_state = 25;
			}
			break;
		case 25:
			if(time_1 == 0)
			{
				EA = 0;
				temp = spi_transfer(0xFF);
				EA = 1;		
				image_buffer[2 + pixel_count++] = temp << 0;
				if(pixel_count >= PIXEL_NUMBER / 2)
				{
					image_buffer[1] = 1;		// image ready
					seq1_state = 30;
				}
			}
			break;
		case 30:
			if(grab_image)	// change
			{
				image_buffer[0] = 2; 	// report ID
				image_buffer[1] = 0;	// image no ready
				pixel_count = 0;
				grab_image = 0;
				seq1_state = 40;
			}
			break;
		case 40:
			EA = 0;
			temp = spi_transfer(ADNS9500_PIXEL_BURST_ADDR);
			EA = 1;

			image_buffer[2 + pixel_count++] = temp << 0;
			delay_ms(1);
			if(pixel_count >= PIXEL_NUMBER / 2)
			{
				NCS = 1;
//				grab_start = 0;
				image_buffer[1] = 3;		// image ready
				seq1_state = 50;
			}
			break;
		case 50:
			if(auto_reset)
			{
				stop_motion = 0;
				spi_init();

				WDT_Disable();
				ADNS9500_Init();
				WDT_Enable();

	//			update_resolution_display();
				update_led_status(resolution);
			}
			t1_data = TIMER1_1_SEC;
			time_1 = 1;
			seq1_state = 60;
			break;
		case 60:
			if(time_1 == 0)
			{				
				grab_start = 0;
				seq1_state = 0;
				//while(1);
			}
			break;
		default:
			break;

	}
}


//------------------------------------------------------------------------------------
// Sequence2
//------------------------------------------------------------------------------------
void Sequence2(void)
{
	ZA_CUR = ZA;
	ZB_CUR = ZB;

	if(ZA_PRE == 1 && ZA_CUR == 1 && ZB_PRE == 0 && ZB_CUR == 1)
	{
		count_z++;
	}
	else if(ZA_PRE == 1 && ZA_CUR == 1 && ZB_PRE == 1 && ZB_CUR == 0)
	{
		count_z--;
	}
	else if(ZA_PRE == 0 && ZA_CUR == 1 && ZB_PRE == 1 && ZB_CUR == 1)
	{
		count_z--;
	}
	else if(ZA_PRE == 1 && ZA_CUR == 0 && ZB_PRE == 1 && ZB_CUR == 1)
	{
		count_z++;
	}

	ZA_PRE = ZA_CUR;
	ZB_PRE = ZB_CUR;
}

//------------------------------------------------------------------------------------
// Sequence3
//------------------------------------------------------------------------------------
void Sequence3(void)
{
	switch(seq3_state)
	{
		case 0:
			if(LEFT_SW == 0)
			{
				t3_data = SW_DELAY;			// default 5ms timer, modify to 0.5ms
				time_3 = 1;
				seq3_state = 10;
			}
			break;
		case 10:
			if(LEFT_SW == 1)
			{
				seq3_state = 0;
			}
			else if(time_3 == 0)
			{
				left_button = 1;
				seq3_state = 20;
			}
			break;
		case 20:
			if(LEFT_SW == 1)
			{
				t3_data = SW_DELAY;
				time_3 = 1;
				seq3_state = 30;
			}
			break;
		case 30:
			if(LEFT_SW == 0)
			{
				seq3_state = 20;
			}
			else if(time_3 == 0)
			{
				left_button = 0;
				seq3_state = 0;
			}
			break;
		default:
			break;
	}
}



//------------------------------------------------------------------------------------
// Sequence4
//------------------------------------------------------------------------------------
void Sequence4(void)
{
	switch(seq4_state)
	{
		case 0:
			if(RIGHT_SW == 0)
			{
				t4_data = SW_DELAY;
				time_4 = 1;
				seq4_state = 10;
			}
			break;
		case 10:
			if(RIGHT_SW == 1)
			{
				seq4_state = 0;
			}
			else if(time_4 == 0)
			{
				right_button = 1;
				t10_data = 4000;
				time_10 = 1;
				seq4_state = 20;
			}
			break;
		case 20:
			if(RIGHT_SW == 1)
			{
				t4_data = SW_DELAY;
				time_4 = 1;
				seq4_state = 30;
			}
			else if(time_10 == 0)
			{
				seq4_state = 40;
			}
			break;
		case 30:
			if(RIGHT_SW == 0 && lift_detection_mode == 0)
			{
				seq4_state = 20;
			}
			else if(time_4 == 0 && lift_changed == 0)
			{
				right_button = 0;
				update_led_status(resolution);
				seq4_state = 0;
			}
			else if(time_4 == 0 && lift_changed == 1)
			{
				lift_changed = 0;
				update_led_status(resolution);
				seq4_state = 0;
			}
			break;
		case 40:		// enter lift detection mode
			if (RIGHT_SW == 1)
			{
				seq4_state = 30;
			}			
			else 
			{	
				lift_detection_mode = 1;
			}
			break;
		default:
			break;
	}
}


//------------------------------------------------------------------------------------
// Sequence5
//------------------------------------------------------------------------------------
void Sequence5(void)
{
	switch(seq5_state)
	{
		case 0:
			if(MID_SW == 0)
			{
				t5_data = SW_DELAY;
				time_5 = 1;
				seq5_state = 10;
			}
			break;
		case 10:
			if(MID_SW == 1)
			{
				seq5_state = 0;
			}
			else if(time_5 == 0)
			{
				middle_button = 1;
				seq5_state = 20;
			}
			break;
		case 20:
			if(MID_SW == 1)
			{
				t5_data = SW_DELAY;
				time_5 = 1;
				seq5_state = 30;
			}
			break;
		case 30:
			if(MID_SW == 0)
			{
				seq5_state = 20;
			}
			else if(time_5 == 0)
			{
				middle_button = 0;
				seq5_state = 0;
			}
			break;
		default:
			break;
	}
}
//------------------------------------------------------------------------------------
// Sequence6
//------------------------------------------------------------------------------------
void Sequence6(void)
{
	switch(seq6_state)
	{
		case 0:
			EA = 0;
			

//			temp = spi_read_sensor(ADNS9500_LASER_CTRL0_ADDR);	
//			temp = spi_read_sensor(ADNS9500_LASER_CTRL1_ADDR);
//			temp = spi_read_sensor(ADNS9500_LP_CFG0_ADDR);
//			temp = spi_read_sensor(ADNS9500_LP_CFG1_ADDR);
//			temp = spi_read_sensor(ADNS9500_MOTION_ADDR);	
//			temp = spi_read_sensor(ADNS9500_CONFIGURATION_II_ADDR);	

		
			if(MOTION == 0)		// motion interrupt method
			{
				read_sensor_motion_burst();				// read motion burst register
	//			read_sensor_motion_registers();			// read delta_x,delta_y registers

				//shutter.b[0] = spi_read_sensor(ADNS9500_SHUTTER_UPPER_ADDR);
				//shutter.b[1] = spi_read_sensor(ADNS9500_SHUTTER_LOWER_ADDR);
				
				count_x.w += motion_x.w;	// motion accumulate
				count_y.w += motion_y.w;

	
			}  

			//	seq6_state = 0;
			
			EA = 1;
			break;
		default:
			break;
	
	}
}
//------------------------------------------------------------------------------------
// Sequence7
//------------------------------------------------------------------------------------

void Sequence7(void)
{
	switch(seq7_state)
	{
		case 0:
			if(sent_buttons != buttons || count_x.w || count_y.w || count_z)
			{
	//			count_x.w = count_x.w;
	//			count_y.w = -count_y.w;   //for silabs MCU, need to reverse y 
/*				
				IN_PACKET[0] = buttons;
				IN_PACKET[1] = count_x.b[1];
				IN_PACKET[2] = count_y.b[1];
				IN_PACKET[3] = count_z;
				IN_PACKET[4] = count_x.b[1];
				IN_PACKET[5] = count_x.b[0];
				IN_PACKET[6] = count_y.b[1];
				IN_PACKET[7] = count_y.b[0];
*/
//				count_x.w = -300;				

				IN_PACKET[0] = 1;
				IN_PACKET[1] = buttons;
				IN_PACKET[2] = count_x.b[LSB];
				IN_PACKET[3] = count_y.b[LSB];
				IN_PACKET[4] = count_z;
				IN_PACKET[5] = count_x.b[LSB];
				IN_PACKET[6] = count_x.b[MSB];
				IN_PACKET[7] = count_y.b[LSB];
				IN_PACKET[8] = count_y.b[MSB];

				sent_buttons = buttons;
				count_x.w = 0;
				count_y.w = 0;
				count_z = 0;

				t7_data = 2;
				time_7 = 1;						//delay 1ms
				seq7_state = 10;
			}
			break;
		case 10:
			if(EP_STATUS[1] == EP_IDLE)
			{
				IN_BUFFER.Ptr = IN_PACKET;
				IN_BUFFER.Length = 9;

				SendPacket();
				seq7_state = 20;
			}
			break;
		case 20:
			if(EP_STATUS[1] == EP_IDLE)
			//if(time_7 == 0)
			{
				seq7_state = 0;
			}
			break;
		default:
			break;
	}
}

//------------------------------------------------------------------------------------
// Sequence8
//------------------------------------------------------------------------------------
void Sequence8(void)
{
	switch(seq8_state)
	{
		case 0:
			if(CPI_P == 0)
			{
				t8_data = SW_DELAY_2;
				time_8 = 1;
				seq8_state = 10;
			}
			else if(CPI_M == 0)
			{
				t8_data = SW_DELAY_2;
				time_8 = 1;
				seq8_state = 40;
			}
			break;
		case 10:
			if(CPI_P == 1)
			{
				seq8_state = 0;
			}
			if(time_8 == 0)
			{
				if(stop_motion == 0 && lift_detection_mode == 0)
				{
					resolution++;
					if(resolution > MAX_RESOLUTION_SETTINR)
					{
						resolution = MIN_RESOLUTION_SETTINR;
					}
					spi_write_sensor(ADNS9500_CONFIGURATION_I_ADDR, resolution);
					resolution_changed = 1;
					update_led_status(resolution);
		//			update_resolution_display();
				}

				t8_data = 1000;
				time_8 = 1;
				seq8_state = 20;
			}
			break;
		case 20:
			if(time_8 == 0)
			{
				t8_data = 150;
				time_8 = 1;
				if(stop_motion == 0 && lift_detection_mode == 0)
				{
					resolution++;
					if(resolution > MAX_RESOLUTION_SETTINR)
					{
						resolution = MIN_RESOLUTION_SETTINR;
					}
					spi_write_sensor(ADNS9500_CONFIGURATION_I_ADDR, resolution);
					resolution_changed = 1;
					update_led_status(resolution);
	//				update_resolution_display();
				}
			}
			if(CPI_P == 1)
			{
				seq8_state = 0;
			}
			break;
		case 40:
			if(CPI_M == 1)
			{
				seq8_state = 0;
			}
			if(time_8 == 0)
			{
				if(stop_motion == 0 && lift_detection_mode == 0)
				{
					resolution--;
					if(resolution < MIN_RESOLUTION_SETTINR)
					{
						resolution = MAX_RESOLUTION_SETTINR;
					}
					spi_write_sensor(ADNS9500_CONFIGURATION_I_ADDR, resolution);
					resolution_changed = 1;
					update_led_status(resolution);
	//				update_resolution_display();
				}

				t8_data = 1000;
				time_8 = 1;
				seq8_state = 50;
			}
			break;
		case 50:
			if(time_8 == 0)
			{
				t8_data = 150;
				time_8 = 1;
				if(stop_motion == 0 && lift_detection_mode == 0)
				{
					resolution--;
					if(resolution < MIN_RESOLUTION_SETTINR)
					{
						resolution = MAX_RESOLUTION_SETTINR;
					}
					spi_write_sensor(ADNS9500_CONFIGURATION_I_ADDR, resolution);
					resolution_changed = 1;
					update_led_status(resolution);
//					update_resolution_display();
				}
			}
			if(CPI_M == 1)
			{
				seq8_state = 0;
			}
			break;
/*		case 100:
			if(CPI_P == 1 && CPI_M == 1)
			{
				t8_data = 50;
				time_8 = 1;
				seq8_state = 110;
			}
			break;
		case 110:
			if(CPI_P == 0 || CPI_M == 0)
			{
				seq8_state = 100;
			}
			if(time_8 == 0)
			{
				seq8_state = 0;
			}
			break;		*/
		default:
			break;
	}
}
//------------------------------------------------------------------------------------
// Sequence9
//------------------------------------------------------------------------------------
void Sequence9(void)
{
	switch(seq9_state)
	{
		case 0:
			if(resolution_changed)
			{
				time_9 = 1;
				t9_data = 4000;							// if cpi unchanged for >2sec, proceed
				resolution_changed = 0;
				seq9_state = 10;
			}
			break;
		case 10:
			if(resolution_changed)
			{
				seq9_state = 0;
			}
			else if(time_9 == 0)
			{
				seq9_state = 20;
			}
			break;
		case 20:
			spi_write_eeprom(0x0000, resolution);		//store cpi data to eeprom
			seq9_state = 0;
			break;
		default:
			break;
	}
}
//------------------------------------------------------------------------------------
// Sequence10
//------------------------------------------------------------------------------------
void Sequence10(void)
{
	switch(seq10_state)
	{
		case 0:
			if(lift_detection_mode == 1 && RIGHT_SW == 0)
			{
				update_led_status_2(lift_data);
				if(CPI_P == 0)
				{
					t11_data = 50;
					time_11 = 1;
					seq10_state = 10;
				}
				else if(CPI_M == 0)
				{
					t11_data = 5;
					time_11 = 1;
					seq10_state = 20;
				}
			}
			else if(RIGHT_SW == 1)
			{
				lift_detection_mode = 0;
				update_led_status(resolution);
			}
			break;

		case 10:
			if(CPI_P == 1)
			{
				seq10_state = 0;
			}
			if(time_11 == 0)
			{
				lift_data = lift_data + 0x03;
				if (lift_data > 0x1F)
				{
					lift_data = 0x1F;
				}
				spi_write_sensor(ADNS9500_LIFT_DETECTION_THR_ADDR, lift_data);
				update_led_status_2(lift_data);
				lift_changed = 1;
			
				t11_data = 1000;
				time_11 = 1;
				seq10_state = 15;
			}
			break;

		case 15:
			if(time_11 == 0)
			{
				t11_data = 300;
				time_11 = 1;					

				lift_data = lift_data + 0x03;
				if (lift_data > 0x1F)
				{
					lift_data = 0x1F;
				}
				spi_write_sensor(ADNS9500_LIFT_DETECTION_THR_ADDR, lift_data);
				update_led_status_2(lift_data);
				lift_changed = 1;
			}
			if(CPI_P == 1)
			{
				seq10_state = 0;
			}
			break;

		case 20:
			if(CPI_M == 1)
			{
				seq10_state = 0;
			}
			if(time_11 == 0)
			{
				lift_data = lift_data - 0x03;
				if (lift_data > 0x1F)
				{
					lift_data = 0x01;
				}
		   		spi_write_sensor(ADNS9500_LIFT_DETECTION_THR_ADDR, lift_data);
				update_led_status_2(lift_data);
				lift_changed = 1;

				t11_data = 1000;
				time_11 = 1;
				seq10_state = 25;
			}
			break;
		case 25:
			if(time_11 == 0)
			{
				lift_data = lift_data - 0x03;
				if (lift_data > 0x1F)   	// unsigned number, so won't become negative
				{
					lift_data = 0x01;
				}
		   		spi_write_sensor(ADNS9500_LIFT_DETECTION_THR_ADDR, lift_data);
				update_led_status_2(lift_data);
				lift_changed = 1;
			}
			if(CPI_M == 1)
			{
				seq10_state = 0;
			}
			break;
		default:
			break;
	}
}

//------------------------------------------------------------------------------------
// Sequence11
//------------------------------------------------------------------------------------
void Sequence11(void)
{
	unsigned char temp;

	switch(seq11_state)
	{
		case 0:
			
			// Use timer control, check sensor every 100ms
			t13_data = 200;			// x 10ms
			time_13 = 1;			
			seq11_state = 10;
			break;

		case 10:
			
			if(time_13 == 0)
			{	
				seq11_state = 15;
			}
			break;

		case 15:
			
			EA = 0;
			temp = spi_read_sensor(ADNS9500_MOTION_ADDR);
			EA = 1;
			seq11_state = 20;
			// If Fault = 1 and/or LP_Valid = 0, then power up reset the sensor again
			if(((temp & 0x40) == 0x40) || ((temp & 0x20) == 0x00))
			{
				WDT_Disable();
				ADNS9500_Init(); 
				WDT_Enable();
				seq11_state = 0;
			}		
			
			
			break;
		case 20:
			
			EA = 0;
			temp = spi_read_sensor(ADNS9500_OBSERVATION_ADDR);
			EA = 1;

			// If OB6 = 1, then power up reset the sensor again
			if((temp & 0x40) == 0x00)
			{
				WDT_Disable();
				ADNS9500_Init(); 
				WDT_Enable();
			}		
			
			seq11_state = 0;
			break;
		
		default:
			break;
	}
}

//------------------------------------------------------------------------------------
// Sequence12
//------------------------------------------------------------------------------------
void Sequence12(void)
{
	switch(seq12_state)
	{
		case 0:
			if(srom_success)
			{
				EA = 0;

				WDT_Disable();
				FLASH_PageErase(SROM_ADDR + 0x0000);
				FLASH_PageErase(SROM_ADDR + 0x0200);
				FLASH_PageErase(SROM_ADDR + 0x0400);
				FLASH_PageErase(SROM_ADDR + 0x0600);
				FLASH_PageErase(SROM_ADDR + 0x0800);
				FLASH_PageErase(SROM_ADDR + 0x0A00);
				FLASH_PageErase(SROM_ADDR + 0x0C00);				

				srom_size.b[0] = FLASH_ByteRead(SROM_TEMP_ADDR + 0x0C00);
				srom_size.b[1] = FLASH_ByteRead(SROM_TEMP_ADDR + 0x0C01);
				for(srom_count = 0; srom_count < srom_size.w; srom_count++)
				{
					srom_data = FLASH_ByteRead(SROM_TEMP_ADDR + srom_count);
					FLASH_ByteWrite((SROM_ADDR + srom_count), srom_data);
				}	
				FLASH_ByteWrite ((SROM_ADDR + 0x0C00), srom_size.b[0]);
				FLASH_ByteWrite ((SROM_ADDR + 0x0C01), srom_size.b[1]);
				FLASH_ByteWrite ((SROM_ADDR + 0x0C02), 1);

				srom_start = 0;
				srom_success = 0;
				ADNS9500_Init();

				WDT_Enable();
				EA = 1;
			}
			break;
		case 10:
			 
			break;
		default:
			break;
	}
}


/*void shift_8bits(unsigned char data_in)
{

	dummy1_bit = data_in;
	DIN = dummy1_bit7; CLK = 0; CLK = 1;
	DIN = dummy1_bit6; CLK = 0; CLK = 1;
	DIN = dummy1_bit5; CLK = 0; CLK = 1;
	DIN = dummy1_bit4; CLK = 0; CLK = 1;
	DIN = dummy1_bit3; CLK = 0; CLK = 1;
	DIN = dummy1_bit2; CLK = 0; CLK = 1;
	DIN = dummy1_bit1; CLK = 0; CLK = 1;
	DIN = dummy1_bit0; CLK = 0; CLK = 1;

}*/
void read_sensor_motion_registers(void)
{
	unsigned char temp;

	temp = spi_read_sensor(ADNS9500_MOTION_ADDR);

	motion_x.b[LSB] = spi_read_sensor(ADNS9500_DELTAX_L_ADDR);
	motion_x.b[MSB] = spi_read_sensor(ADNS9500_DELTAX_H_ADDR);
				
	motion_y.b[LSB] = spi_read_sensor(ADNS9500_DELTAY_L_ADDR);
	motion_y.b[MSB] = spi_read_sensor(ADNS9500_DELTAY_H_ADDR);
}

void read_sensor_motion_burst(void)
{
	unsigned char temp;
	
	spi_write_sensor(ADNS9500_MOTION_BURST_ADDR, 0x50);
	delay_us(100);  // wait for Tsrad >100us (default 1 frame)
	NCS = 0;
	delay1();
	spi_transfer(ADNS9500_MOTION_BURST_ADDR);	//send address to sensor before start spi_read
	delay_us(50);

	///// start motion burst /////
	temp 			= spi_transfer(0xff); //bit 0
	temp			= spi_transfer(0xff); //bit 1
	motion_x.b[LSB] = spi_transfer(0xff); //bit 2
	motion_x.b[MSB] = spi_transfer(0xff); //bit 3
	motion_y.b[LSB] = spi_transfer(0xff); //bit 4
	motion_y.b[MSB] = spi_transfer(0xff); //bit 5
	///// end motion burst /////

	NCS = 1;							
}

void update_led_status(unsigned char res)
{ 
	switch(res)
	{
		case 1:
		case 2:
		case 3:
		case 4:			
		case 5:			
		case 6:
			LED_BIT2 = 1;			//off
			LED_BIT1 = 1;			//off
			LED_BIT0 = 1;			//off
			break;
		case 7:
		case 8:
		case 9:
		case 10:
		case 11:
		case 12:
		case 13:
			LED_BIT2 = 1;			//off
			LED_BIT1 = 1;			//off
			LED_BIT0 = 0;			//on
			break;
		case 14:
		case 15:
		case 16:
		case 17:
		case 18:			
		case 19:			
		case 20:
			LED_BIT2 = 1;			//off
			LED_BIT1 = 0;			//on
			LED_BIT0 = 1;			//off
			break;
		case 21:
		case 22:
		case 23:
		case 24:
		case 25:
		case 26:
		case 27:
			LED_BIT2 = 1;			//off
			LED_BIT1 = 0;			//on
			LED_BIT0 = 0;			//on
			break;
		case 28:
		case 29:
		case 30:
		case 31:
		case 32:
		case 33:
		case 34:
			LED_BIT2 = 0;			//on
			LED_BIT1 = 1;			//off
			LED_BIT0 = 1;			//off
			break;
		case 35:			
		case 36:
		case 37:
		case 38:
		case 39:
		case 40:
		case 41:
			LED_BIT2 = 0;			//on
			LED_BIT1 = 1;			//off
			LED_BIT0 = 0;			//on
			break;
		case 42:
		case 43:
		case 44:
		case 45:
		case 46:
		case 47:
		case 48:
			LED_BIT2 = 0;			//on
			LED_BIT1 = 0;			//on
			LED_BIT0 = 1;			//off
			break;
		case 49:
		case 50:
		case 51:
		case 52:
		case 53:
		case 54:
		case 55:
		case 56:
			LED_BIT2 = 0;			//on
			LED_BIT1 = 0;			//on
			LED_BIT0 = 0;			//on
			break;
			
		default:
			break;
	}
}

void update_led_status_2(unsigned char lift_data)
{ 
	switch(lift_data)
	{
		case 1:
			LED_DAT = 0xFF; 		//all off	
			led_blink(LED_DAT);
			break;	
		case 4:
			LED_DAT = 0xF3; 		//all off	
			led_blink(LED_DAT);
			break;
		case 7:
			LED_DAT = 0xFD; 		//all off	
			led_blink(LED_DAT);
			break;
		case 10:
			LED_DAT = 0xF1; 		//all off	
			led_blink(LED_DAT);
			break;
		case 13:
			LED_DAT = 0xFE; 		//all off	
			led_blink(LED_DAT);
			break;
		case 16:
			LED_DAT = 0xFA; 		//all off	
			led_blink(LED_DAT);
			break;
		case 19:
			LED_DAT = 0xFC; 		//all off	
			led_blink(LED_DAT);
			break;
		case 22:
			LED_DAT = 0xF8; 		//all off	
			led_blink(LED_DAT);
			break;
		case 25:
			LED_DAT = 0xF8; 		//all off	
			led_blink(LED_DAT);
			break;
		case 28:
			LED_DAT = 0xF8; 		//all off	
			led_blink(LED_DAT);
			break;
		case 31:
			LED_DAT = 0xF8; 		//all off	
			led_blink(LED_DAT);
			break;
		default:
			break;
			
	}
}

void led_blink(unsigned char led_dat)
{
 	switch(led_blink_state)
	{
		case 0:	
			t12_data = 1800;	// x 5 milisecond
			time_12 = 1;
			led_blink_state =10;
			break;
		case 10:	
			if(time_12==0)
			{
			 // on				
			 	P2 = (P2 & ~0x07) | (led_dat & 0x07);	// preserve port 2 bit 3-7
				led_blink_state = 20;
			}
			break;
		case 20:	
			t12_data = 120;						// off state
			time_12 = 1;
			led_blink_state = 30;
			break;
		case 30:	
			if(time_12==0)
			{
				switch(lift_data)
				{
					case 0x19:
						LED_BIT2 = 1;			//off (blinking)
						LED_BIT1 = 1;			//off (blinking)
						LED_BIT0 = 0;			//on  (static)
						break;
					case 0x1C:
						LED_BIT2 = 1;			//off (blinking)
						LED_BIT1 = 0;			//on  (static) 
						LED_BIT0 = 1;			//off (blinking)
						break;
					case 0x1F:
						LED_BIT2 = 1;			//off (blinking)
						LED_BIT1 = 0;			//off (static)
						LED_BIT0 = 0;			//off (static)
						break;
					default:
						P2 = (P2 & ~0x07) | 0x07;
						break;
				}
				led_blink_state =40;
			}				
			break;
		case 40:	
			led_blink_state = 0;
			break;
		default:	
			break;			
	}
}