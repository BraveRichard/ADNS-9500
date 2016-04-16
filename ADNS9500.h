
//------------------------------------------------------------------------------------
// Constant
//------------------------------------------------------------------------------------
#define DEFAULT_CFG0		0x4A
#define DEFAULT_CFG1		0xB5


//------------------------------------------------------------------------------------
// Register
//------------------------------------------------------------------------------------
// Motion Register Individual Bit Field Settings
#define ADNS9500_MOTION_MOT			0x80
#define ADNS9500_MOTION_FAULT		0x40
#define ADNS9500_MOTION_LPVALID		0x20
#define ADNS9500_MOTION_OVF			0x10
#define ADNS9500_MOTION_PIXFIRST	0x01

// Configuration I Register Settings
#define ADNS9500_RES100CPI			0x01		
#define ADNS9500_RES200CPI			0x02
#define ADNS9500_RES300CPI			0x03
#define ADNS9500_RES400CPI			0x04
#define ADNS9500_RES500CPI	  		0x05
#define ADNS9500_RES600CPI			0x06	
#define ADNS9500_RES700CPI			0x07
#define ADNS9500_RES800CPI			0x08
#define ADNS9500_RES900CPI			0x09
#define ADNS9500_RES1000CPI			0x0A
#define ADNS9500_RES1100CPI			0x0B
#define ADNS9500_RES1200CPI			0x0C
#define ADNS9500_RES1300CPI			0x0D
#define ADNS9500_RES1400CPI			0x0E
#define ADNS9500_RES1500CPI			0x0F
#define ADNS9500_RES1600CPI			0x10
#define ADNS9500_RES1700CPI			0x11
#define ADNS9500_RES1800CPI			0x12
#define ADNS9500_RES1900CPI			0x13
#define ADNS9500_RES2000CPI			0x14
#define ADNS9500_RES2100CPI			0x15
#define ADNS9500_RES2200CPI			0x16
#define ADNS9500_RES2300CPI			0x17
#define ADNS9500_RES2400CPI			0x18
#define ADNS9500_RES2500CPI			0x19	
#define ADNS9500_RES2600CPI			0x1A
#define ADNS9500_RES2700CPI			0x1B
#define ADNS9500_RES2800CPI			0x1C
#define ADNS9500_RES2900CPI			0x1D
#define ADNS9500_RES3000CPI			0x1E
#define ADNS9500_RES3100CPI			0x1F	
#define ADNS9500_RES3200CPI			0x20
#define ADNS9500_RES3300CPI			0x21
#define ADNS9500_RES3400CPI			0x22
#define ADNS9500_RES3500CPI			0x23
#define ADNS9500_RES3600CPI			0x24	
#define ADNS9500_RES3700CPI			0x25
#define ADNS9500_RES3800CPI			0x26
#define ADNS9500_RES3900CPI			0x27
#define ADNS9500_RES4000CPI			0x28
#define ADNS9500_RES4100CPI			0x29
#define ADNS9500_RES4200CPI			0x2A

// Configuration II Bits Register Individual Bit Field Settings
#define ADNS9500_FREST1				0x80
#define ADNS9500_FREST0				0x40
#define ADNS9500_REST_EN			0x20
#define ADNS9500_NAGC				0x10
#define ADNS9500_FIXED_FR			0x08
#define ADNS9500_RPT_MOD			0x04
#define ADNS9500_SYS_TEST			0x01

// Laser CTRL0 Bits Register Individual Bit Field Settings
#define ADNS9500_RANGE				0x80
#define ADNS9500_FORCE_DISABLED		0x01

// Laser CTRL1 Bits Register Individual Bit Field Settings
#define ADNS9500_RANGE_C			0x80

// Configuration IV Bits Register Individual Bit Field Settings
#define ADNS9500_SROM_SIZE_1_5K		0x00
#define ADNS9500_SROM_SIZE_3_0K		0x02

// Power Up Reset Register Settings
#define ADNS9500_POWER_UP_RESET		0x5A


// ADNS9500 Register Addresses
#define ADNS9500_PRODUCTID_ADDR							0x00
#define ADNS9500_REVISIONID_ADDR						0x01
#define ADNS9500_MOTION_ADDR							0x02
#define ADNS9500_DELTAX_L_ADDR							0x03
#define ADNS9500_DELTAX_H_ADDR							0x04
#define ADNS9500_DELTAY_L_ADDR							0x05
#define ADNS9500_DELTAY_H_ADDR							0x06
#define ADNS9500_SQUAL_ADDR								0x07
#define ADNS9500_PIXEL_SUM_ADDR							0x08
#define ADNS9500_MAXIMUM_PIXEL_ADDR						0x09
#define ADNS9500_MINIMUM_PIXEL_ADDR						0x0A
#define ADNS9500_SHUTTER_LOWER_ADDR						0x0B
#define ADNS9500_SHUTTER_UPPER_ADDR						0x0C
#define ADNS9500_FRAME_PERIOD_LOWER_ADDR				0x0D
#define ADNS9500_FRAME_PERIOD_UPPER_ADDR				0x0E
#define ADNS9500_CONFIGURATION_I_ADDR					0x0F
#define ADNS9500_CONFIGURATION_II_ADDR					0x10
#define ADNS9500_FRAME_CAPTURE_ADDR						0x12
#define ADNS9500_SROM_ENABLE_ADDR						0x13
#define ADNS9500_RUN_DOWNSHIFT_ADDR						0x14
#define ADNS9500_REST1_RATE_ADDR						0x15
#define ADNS9500_REST1_DOWNSHIFT_ADDR					0x16
#define ADNS9500_REST2_RATE_ADDR						0x17
#define ADNS9500_REST2_DOWNSHIFT_ADDR					0x18
#define ADNS9500_REST3_RATE_ADDR						0x19
#define ADNS9500_FRAME_PERIOD_MAX_BOUND_LOWER_ADDR		0x1A
#define ADNS9500_FRAME_PERIOD_MAX_BOUND_UPPER_ADDR		0x1B
#define ADNS9500_FRAME_PERIOD_MIN_BOUND_LOWER_ADDR		0x1C
#define ADNS9500_FRAME_PERIOD_MIN_BOUND_UPPER_ADDR		0x1D
#define ADNS9500_SHUTTER_MAX_BOUND_LOWER_ADDR 			0x1E
#define ADNS9500_SHUTTER_MAX_BOUND_UPPER_ADDR 			0x1F
#define ADNS9500_LASER_CTRL0_ADDR						0x20
#define ADNS9500_LASER_CTRL1_ADDR						0x21
#define ADNS9500_LP_CFG0_ADDR							0x22
#define ADNS9500_LP_CFG1_ADDR							0x23
#define ADNS9500_OBSERVATION_ADDR						0x24
#define ADNS9500_DATA_OUT_LOWER_ADDR					0x25
#define ADNS9500_DATA_OUT_UPPER_ADDR					0x26
#define ADNS9500_SROM_ID_ADDR							0x2A
#define ADNS9500_LIFT_DETECTION_THR_ADDR				0x2E
#define ADNS9500_CONFIGURATION_V_ADDR					0x2F
#define ADNS9500_CONFIGURATION_IV_ADDR					0x39
#define ADNS9500_POWER_UP_RESET_ADDR					0x3A
#define ADNS9500_SHUTDOWN_ADDR							0x3B
#define ADNS9500_INVERSE_PRODUCT_ID_ADDR				0x3F
#define ADNS9500_MOTION_BURST_ADDR						0x50
#define ADNS9500_SROM_LOAD_BURST_ADDR					0x62
#define ADNS9500_PIXEL_BURST_ADDR						0x64


//------------------------------------------------------------------------------------
// External Variables
//------------------------------------------------------------------------------------
extern data unsigned char lift_data;


//------------------------------------------------------------------------------------
// External Function
//------------------------------------------------------------------------------------
extern void ADNS9500_Init(void);
extern void ADNS9500_check_sequence(void);
extern void ADNS9500_power_up_sequence(void);
extern void ANDS9500_SROM_load_sequence(void);
extern void ADNS9500_laser_power_enable_sequence(void);
extern void ADNS9500_load_resolution(void);