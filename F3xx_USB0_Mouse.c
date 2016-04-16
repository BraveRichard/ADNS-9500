//-----------------------------------------------------------------------------
// F320_USB0_Mouse.c
//-----------------------------------------------------------------------------
// Copyright 2005 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
//
// Source file for routines that mouse data.
//
//
// How To Test:    See Readme.txt
//
//
// FID:            
// Target:         C8051F320
// Tool chain:     Keil C51 7.50 / Keil EVAL C51
//                 Silicon Laboratories IDE version 2.6
// Command Line:   See Readme.txt
// Project Name:   F3xx_MouseExample
//
// Release 1.1
//    -Minor code comment changes
//    -16 NOV 2006
// Release 1.0
//    -Initial Revision (PD)
//    -07 DEC 2005
//


#include "c8051f3xx.h"
#include "F3xx_USB0_Mouse.h"
#include "F3xx_USB0_InterruptServiceRoutine.h"
#include "F3xx_USB0_Register.h"

//-----------------------------------------------------------------------------
// Definitions
//-----------------------------------------------------------------------------

#define SYSCLK             12000000    // SYSCLK frequency in Hz

// USB clock selections (SFR CLKSEL)
#define USB_4X_CLOCK       0x00        // Select 4x clock multiplier, for USB
#define USB_INT_OSC_DIV_2  0x10        // Full Speed
#define USB_EXT_OSC        0x20
#define USB_EXT_OSC_DIV_2  0x30
#define USB_EXT_OSC_DIV_3  0x40
#define USB_EXT_OSC_DIV_4  0x50

// System clock selections (SFR CLKSEL)
#define SYS_INT_OSC        0x00        // Select to use internal oscillator
#define SYS_EXT_OSC        0x01        // Select to use an external oscillator
#define SYS_4X_DIV_2       0x02

//-----------------------------------------------------------------------------

void Sysclk_Init (void);
void Port_Init (void);
void USB0_Init (void);
void System_Init (void);

//-----------------------------------------------------------------------------
// Interrupt Service Routines
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Initialization Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// System_Init(void)
//-----------------------------------------------------------------------------
//
// Return Value - None
// Parameters - None
//
// This top-level initialization routine calls all support routine.
//
// ----------------------------------------------------------------------------
void System_Init (void)
{
   PCA0MD &= ~0x40;                    // Disable Watchdog timer
   Sysclk_Init ();                     // initialize system clock
   Port_Init ();	                     // configure cross bar
}

//-----------------------------------------------------------------------------
// USB0_Init
//-----------------------------------------------------------------------------
//
// Return Value - None
// Parameters - None
//
// - Initialize USB0
// - Enable USB0 interrupts
// - Enable USB0 transceiver
// - Enable USB0 with suspend detection
//
// ----------------------------------------------------------------------------
void USB0_Init (void)
{

   POLL_WRITE_BYTE (POWER, 0x08);      // Force Asynchronous USB Reset
   POLL_WRITE_BYTE (IN1IE, 0x07);      // Enable Endpoint 0-1 in interrupts
   POLL_WRITE_BYTE (OUT1IE,0x07);      // Enable Endpoint 0-1 out interrupts
   POLL_WRITE_BYTE (CMIE, 0x07);       // Enable Reset, Resume, and Suspend
                                       // interrupts
   USB0XCN = 0xE0;                     // Enable transceiver; select low speed
   POLL_WRITE_BYTE (CLKREC,0x80);      // Enable clock recovery, single-step
                                       // mode disabled

   EIE1 |= 0x02;                       // Enable USB0 Interrupts
                                       // Enable USB0 by clearing the USB
   POLL_WRITE_BYTE (POWER, 0x01);      // Inhibit Bit and enable suspend
                                       // detection

}


//-----------------------------------------------------------------------------
// Sysclk_Init
//-----------------------------------------------------------------------------
//
// Return Value - None
// Parameters - None
//
// Initialize system clock to maximum frequency.
//
// ----------------------------------------------------------------------------
void Sysclk_Init (void)
{
    int i = 0;
    CLKMUL    = 0x80;
    for (i = 0; i < 20; i++);    // Wait 5us for initialization
    CLKMUL    |= 0xC0;
    while ((CLKMUL & 0x20) == 0);
    CLKSEL    = 0x02;
    OSCICN    = 0x83;			// SYSCLK 24Mhz
}

//-----------------------------------------------------------------------------
// Port_Init
//-----------------------------------------------------------------------------
//
// Return Value - None
// Parameters - None
//
// - Configure the Crossbar and GPIO ports.
//
// ----------------------------------------------------------------------------
void Port_Init (void)
{
    // P0.0  -  Unassigned,  Open-Drain, Digital
    // P0.1  -  Unassigned,  Open-Drain, Digital
    // P0.2  -  Unassigned,  Open-Drain, Digital
    // P0.3  -  Unassigned,  Open-Drain, Digital
    // P0.4  -  Unassigned,  Open-Drain, Digital
    // P0.5  -  Unassigned,  Open-Drain, Digital
    // P0.6  -  Unassigned,  Open-Drain, Digital
    // P0.7  -  Unassigned,  Open-Drain, Digital

    // P1.0  -  Unassigned,  Open-Drain, Digital
    // P1.1  -  Unassigned,  Open-Drain, Digital
    // P1.2  -  Unassigned,  Open-Drain, Digital
    // P1.3  -  Unassigned,  Open-Drain, Digital
    // P1.4  -  Unassigned,  Open-Drain, Digital
    // P1.5  -  Unassigned,  Open-Drain, Digital
    // P1.6  -  Unassigned,  Open-Drain, Digital
    // P1.7  -  Unassigned,  Open-Drain, Digital
    // P2.0  -  Unassigned,  Open-Drain, Digital
    // P2.1  -  Unassigned,  Open-Drain, Digital
    // P2.2  -  Unassigned,  Open-Drain, Digital
    // P2.3  -  Unassigned,  Open-Drain, Digital

    XBR1      = 0x40;		//enable weak pull up, enable crossbar
}

