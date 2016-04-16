
//------------------------------------------------------------------------------------
// Global VARIABLES
//------------------------------------------------------------------------------------
unsigned char crc_return;


//------------------------------------------------------------------------------------
// Function PROTOTYPES
//------------------------------------------------------------------------------------
unsigned char Eight_Bit_CRC(unsigned char crc, unsigned char byte_in);


//------------------------------------------------------------------------------------
// Function 
//------------------------------------------------------------------------------------
// 8 bits crc calculation. Initial value is 0.
// polynomial = X8 + X5 + X4 + 1
// data is an 8 bit number; crc is a 8 bit number
unsigned char Eight_Bit_CRC(unsigned char crc, unsigned char byte_in)
{ 
	unsigned char index; // shift counter
	unsigned char fb;
	
	index = 8; // initialise the shift counter
	do
	{ 
		fb = (crc ^ byte_in) & 0x01;
		byte_in >>= 1;
		crc >>= 1;
		
		if(fb)
		{ 
			crc ^= 0x8c;
		}
	} while(--index);
	
	return crc;
}
