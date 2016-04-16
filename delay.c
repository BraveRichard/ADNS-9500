//------------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------------
#include "c8051f3xx.h"
#include <intrins.h>


//------------------------------------------------------------------------------------
// Function PROTOTYPES
//------------------------------------------------------------------------------------
void delay_us(unsigned char delay);
void delay_ms(unsigned char delay);
void delay1(void);


//------------------------------------------------------------------------------------
// Function 
//------------------------------------------------------------------------------------
void delay_us(unsigned char delay)
{
	for(;delay > 0; delay--);
}


void delay_ms(unsigned char delay)
{
	for(;delay > 0; delay--)
	{
		delay_us(100);
		delay_us(100);
		delay_us(100);
		delay_us(100);
		delay_us(100);
		delay_us(100);
		delay_us(100);
		delay_us(100);
	}
}


void delay1(void)
{
	_nop_(); _nop_(); _nop_(); _nop_();	_nop_(); _nop_(); _nop_(); _nop_();	_nop_(); _nop_();
	_nop_(); _nop_(); _nop_(); _nop_();	_nop_(); _nop_(); _nop_(); _nop_();	_nop_(); _nop_();
	_nop_(); _nop_(); _nop_(); _nop_();	
	//1us

	_nop_(); _nop_(); _nop_(); _nop_();	_nop_(); _nop_(); _nop_(); _nop_(); _nop_(); _nop_();
	_nop_(); _nop_(); _nop_(); _nop_();	_nop_(); _nop_(); _nop_(); _nop_();	_nop_(); _nop_();
	_nop_(); _nop_(); _nop_(); _nop_();	
	//2us

	_nop_(); _nop_(); _nop_(); _nop_();	_nop_(); _nop_(); _nop_(); _nop_();	_nop_(); _nop_();
	_nop_(); _nop_(); _nop_(); _nop_();	_nop_(); _nop_(); _nop_(); _nop_(); _nop_(); _nop_(); 
	_nop_(); _nop_(); _nop_(); _nop_();	
	//3us
}

