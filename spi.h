
//------------------------------------------------------------------------------------
// CONSTANT
//------------------------------------------------------------------------------------
#define READ_DATA			0x03
#define WRITE_DATA			0x02
#define WRITE_DISABLE		0x04
#define WRITE_ENABLE		0x06
#define READ_STATUS			0x05
#define WRITE_STATUS		0x01

#define WRITE_EEPROM		0x20
#define READ_EEPROM			0x21
#define WRITE_SENSOR		0x10
#define READ_SENSOR			0x11
#define TEST_SENSOR			0x50
#define ECHO_COMMAND		0x60
#define RESET_SENSOR		0x70


//------------------------------------------------------------------------------------
// External Function
//------------------------------------------------------------------------------------

extern void spi_init(void);
extern unsigned char spi_read_byte(void);
extern void spi_write_byte(unsigned char data_in);
extern unsigned char spi_transfer(unsigned char data_in);
extern unsigned char spi_read_eeprom(unsigned int addr);
extern void spi_write_eeprom(unsigned int addr, unsigned char data_in);
extern unsigned char spi_read_sensor(unsigned char addr);
extern void spi_write_sensor(unsigned char addr, unsigned char data_in);


