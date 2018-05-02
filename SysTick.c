// SysTick.h
// Runs on LM4F120/TM4C123
// Provide functions that initialize the SysTick module, wait at least a
// designated number of clock cycles, and wait approximately a multiple
// of 10 milliseconds using busy wait.  After a power-on-reset, the
// LM4F120 gets its clock from the 16 MHz precision internal oscillator,
// which can vary by +/- 1% at room temperature and +/- 3% across all
// temperature ranges.  If you are using this module, you may need more
// precise timing, so it is assumed that you are using the PLL to set
// the system clock to 50 MHz.  This matters for the function
// SysTick_Wait10ms(), which will wait longer than 10 ms if the clock is
// slower.
// Daniel Valvano
// September 11, 2013

/* This example accompanies the book
   "Embedded Systems: Real Time Interfacing to Arm Cortex M Microcontrollers",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2015
   Program 2.11, Section 2.6

 Copyright 2015 by Jonathan W. Valvano, valvano@mail.utexas.edu
    You may use, edit, run or distribute this file
    as long as the above copyright notice remains
 THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 VALVANO SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/
 */
#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
#include "DAC.h"
#include "Music.h"
#include "Main.h"
#include "LED.h"
#define PF1 (*((volatile uint32_t *)0x40025008))

#define NVIC_ST_CTRL_COUNT      0x00010000  // Count flag
#define NVIC_ST_CTRL_CLK_SRC    0x00000004  // Clock Source
#define NVIC_ST_CTRL_INTEN      0x00000002  // Interrupt enable
#define NVIC_ST_CTRL_ENABLE     0x00000001  // Counter mode
#define NVIC_ST_RELOAD_M        0x00FFFFFF  // Counter load value

// Initialize SysTick with busy wait running at bus clock.
void SysTick_Init(void){
  NVIC_ST_CTRL_R = 0;                   // disable SysTick during setup
  NVIC_ST_RELOAD_R = 0;  // maximum reload value
  NVIC_ST_CURRENT_R = 0;                // any write to current clears it
                                        // enable SysTick with core clock
  //NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R & 0x00FFFFFF) | 0x40000000; //pri 2
	NVIC_ST_CTRL_R = 0x0;
}

//constant waveforms for different instruments
const unsigned short Wave[64] = {  
  1024,1122,1215,1302,1378,1440,1486,1514,1524,1514,1486,
  1440,1378,1302,1215,1122,1024,926,833,746,670,608,
  562,534,524,534,562,608,670,746,833,926,1024,1122,1215,1302,1378,1440,1486,1514,1524,1514,1486,
  1440,1378,1302,1215,1122,1024,926,833,746,670,608,
  562,534,524,534,562,608,670,746,833,926};


static uint32_t soundIndex = 0;
static Note *prevNote = 0;


// SysTick Interrupt Handler
void SysTick_Handler(void) {

		LED_GreenToggle();
		
		//delay sound so that you can tell which note is which
		if(soundIndex > 2560)
			DAC_Out(Wave[soundIndex&63]);
			
		//determine if soundIndex needs to be restarted if new note is being played
		Note *currentNote = &global_Song.notes[noteIndex];
		if(currentNote != prevNote) {
			prevNote = currentNote;
			soundIndex &= 63;
		} 
	
   	soundIndex += 1;
		LED_GreenToggle();

	
}



