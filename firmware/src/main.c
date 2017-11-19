/*******************************************************************************
  MPLAB Harmony Project Main Source File

  Company:
    Microchip Technology Inc.
  
  File Name:
    main.c

  Summary:
    This file contains the "main" function for an MPLAB Harmony project.

  Description:
    This file contains the "main" function for an MPLAB Harmony project.  The
    "main" function calls the "SYS_Initialize" function to initialize the state 
    machines of all MPLAB Harmony modules in the system and it calls the 
    "SYS_Tasks" function from within a system-wide "super" loop to maintain 
    their correct operation. These two functions are implemented in 
    configuration-specific files (usually "system_init.c" and "system_tasks.c")
    in a configuration-specific folder under the "src/system_config" folder 
    within this project's top-level folder.  An MPLAB Harmony project may have
    more than one configuration, each contained within it's own folder under
    the "system_config" folder.
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2013-2014 released Microchip Technology Inc.  All rights reserved.

//Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
 *******************************************************************************/
// DOM-IGNORE-END


// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include <xc.h>
#include <string.h>
#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include "system/common/sys_module.h"   // SYS function prototypes
#include "sys/kmem.h"                   // KVA_TO_PA macro
#include "ws2812_pic32mx.h"
#include "main.h"

void init_timer45();
inline void start_timer45();
void reset_timer45();
void delay_ms(unsigned int count);
void delay_ms_b(unsigned int count);

// *****************************************************************************
// *****************************************************************************
// Section: Main Entry Point
// *****************************************************************************
// *****************************************************************************
extern unsigned char isSwNextPattern;
unsigned int g1 = 0, b1 = 0;
unsigned int i = 0;

unsigned int c1;

// Alternating colors array. Red-Green-Red-Green.... and Green-Red-Green-Red, you know the rest!
const unsigned int alternateRG_0[16] = { 0xff0000, 0x00ff00, 0xff0000, 0x00ff00,
                                     0xff0000, 0x00ff00, 0xff0000, 0x00ff00,
                                     0xff0000, 0x00ff00, 0xff0000, 0x00ff00,
                                     0xff0000, 0x00ff00, 0xff0000, 0x00ff00 };

const unsigned int alternateRG_1[16] = { 0x00ff00, 0xff0000, 0x00ff00, 0xff0000,
                                     0x00ff00, 0xff0000, 0x00ff00, 0xff0000,
                                     0x00ff00, 0xff0000, 0x00ff00, 0xff0000,
                                     0x00ff00, 0xff0000, 0x00ff00, 0xff0000 };

const unsigned int alternateRB_0[16] = { 0xff0000, 0x0000ff,0xff0000, 0x0000ff,
                                         0xff0000, 0x0000ff,0xff0000, 0x0000ff,
                                         0xff0000, 0x0000ff,0xff0000, 0x0000ff,
                                         0xff0000, 0x0000ff,0xff0000, 0x0000ff };

const unsigned int alternateRB_1[16] = { 0x0000ff, 0xff0000, 0x0000ff, 0xff0000,
                                         0x0000ff, 0xff0000, 0x0000ff, 0xff0000,
                                         0x0000ff, 0xff0000, 0x0000ff, 0xff0000,
                                         0x0000ff, 0xff0000, 0x0000ff, 0xff0000 };

int main ( void )
{
    
    
    /* Initialize all MPLAB Harmony modules, including application(s). */
    SYS_Initialize ( NULL );
    
    TRISACLR = 0x0001;
    LATACLR = 0xffff;
    TRISBCLR = 0xFFFF;
    
    //DRV_TMR0_Start();       // Manually initialize the TMR2 (labelled TMR0 in Harmony Configurator).
    //DRV_OC0_Enable();       // and the OC1 Peripheral too (labelled OC0 in Harmony Configurator).     
    //DRV_OC0_Start();        //
    
    IFS0bits.T2IF = 0;
    
    init_timer45();
    ws2812_init_DMA();
   
    delay_ms_b(10);
    
    while ( true )
    {
        /* Maintain state machines of all polled MPLAB Harmony modules. */
        //SYS_Tasks ( );
        
        // This is open for improvisation.
        ptn_rainbow(32);
        ptn_alternateTwoColors(alternateRG_0,alternateRG_1, 500);
        ptn_rainbow(32);
        ptn_runningLights(0xff0000, 1, 64);
        ptn_rainbow(32);
        ptn_alternateTwoColors(alternateRG_0,alternateRG_1, 500);
        ptn_fade(0x00ff00,6, 64);
        ptn_rainbow(32);
        ptn_runningLights(0x00ff00, 1, 100);
        ptn_rainbow(32);
        ptn_alternateTwoColors(alternateRG_0,alternateRG_1, 500);
        ptn_fadeRG(6,64);
        ptn_alternateTwoColors(alternateRB_1,alternateRB_0, 500);
        ptn_rainbow(16);
        ptn_fadeRB(6,64);
        ptn_alternateTwoColors(alternateRG_0,alternateRG_1, 500);
        ptn_runningLights(0xff0000, 1, 64);
        ptn_rainbow(16);
        ptn_fadeBG(6,64);
        ptn_alternateTwoColors(alternateRG_0,alternateRG_1, 500);
        ptn_runningLights(0x0000ff, 1, 100);
        ptn_rainbow(64);
        ptn_alternateTwoColors(alternateRG_0,alternateRG_1, 500);
        ptn_runningLights(0xffffff, 1, 64);
        ptn_fade(0x0000ff, 6, 64);
        ptn_rainbow(64);
        ptn_alternateTwoColors(alternateRG_0,alternateRG_1, 500);
        
        //asm volatile("wait");           // Sleep mode. (PIC32MX - xc32)
    }
   /* Execution should not come here during normal operation */

    return ( EXIT_FAILURE );
}

// This is a semi-non-blocking delay - it checks for if the next pattern is already up or not. If it is up,
// the delay is cancelled and immediately branched out. Give and take 1ms because of the internal loop where it
// waits for the TMR1 to reach the 1ms value.
void delay_ms(unsigned int count)
{
	T1CON = 0x8030;		// turn on timer, prescaler to 256 (type B timer)
	while(count-- && !isSwNextPattern)
	{
		TMR1 = 0;
		while(TMR1 < 0x4e);
	}
	T1CONbits.ON = 0;
}

// The timer45 is a full 32-bit one. Maximum delay is well, about 15 hours.
// Here I programmed it to be 2 1/2 minutes, so 150sec/12.8uS = 0xB2D05E.
void init_timer45() {
    
    T4CON = 0x00;
    T4CONbits.T32 = 1;       // 32-bit mode
    T4CONbits.TCKPS = 0b111; // 1:256 prescale value
    
    //PR4 = 0x312d-1;        // Testing only!!
    //PR5 = 0x0001;
    
    // One minute: (60/12.8uS)
    //PR4 = 0x868c-1;
    //PR5 = 0x47;
    
    // Two and half minutes: (150/12.8uS)
    PR4 = 0xD05E - 0x01;
    PR5 = 0xB2;
    
    IPC5bits.T5IP = 0b110;
    IPC5bits.T5IS = 0b00;
    IFS0bits.T5IF = 0;
    IEC0bits.T5IE = 1;
    
    //T4CONbits.ON = 1;
        
}

// Once the timer times up, it resets the whole timer and resets the next pattern.
void reset_timer45() {
    
    T4CONbits.ON = 0;
    TMR4 = 0x0000;
    TMR5 = 0x0000;
    isSwNextPattern = 0;
    
}

// Self explanatory - start the timer45 module immediately.
inline void start_timer45() {
    
    T4CONbits.ON = 1;
    
}

// Blocking delay for other functions not in switching pattern mode.
void delay_ms_b(unsigned int count) {
    
    T1CON = 0x8030;		// turn on timer, prescaler to 256 (type B timer)
	while(count--)
	{
		TMR1 = 0;
		while(TMR1 < 0x4e);
	}
	T1CONbits.ON = 0;  
    
}

/*******************************************************************************
 End of File
*/

