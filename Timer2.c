// Timer2.c
// Runs on LM4F120/TM4C123
// Use TIMER2 in 32-bit periodic mode to request interrupts at a periodic rate
// Daniel Valvano
// May 5, 2015

/* This example accompanies the book
   "Embedded Systems: Real Time Interfacing to Arm Cortex M Microcontrollers",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2015
  Program 7.5, example 7.6

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
#include "LED.h"
#include "Globals.h"
#include "main.h"

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode


// ***************** Timer2_Init ****************
// Activate Timer2 interrupts to run user task periodically
// Inputs:  task is a pointer to a user function
//          period in units (1/clockfreq)
// Outputs: none
void Timer2_Init(){
  SYSCTL_RCGCTIMER_R |= 0x04;   // 0) activate timer2
  TIMER2_CTL_R = 0x00000000;    // 1) disable timer2A during setup
  TIMER2_CFG_R = 0x00000000;    // 2) configure for 32-bit mode
  TIMER2_TAMR_R = 0x00000002;   // 3) configure for periodic mode, default down-count settings
  TIMER2_TAILR_R = 80000000;    // 4) every second for a 80MHz Clock
  TIMER2_TAPR_R = 0;            // 5) bus clock resolution
  TIMER2_ICR_R = 0x00000001;    // 6) clear timer2A timeout flag
  TIMER2_IMR_R = 0x00000001;    // 7) arm timeout interrupt
  NVIC_PRI5_R = (NVIC_PRI5_R&0x00FFFFFF)|0x20000000; // 8) priority 1
// interrupts enabled in the main program after all devices initialized
// vector number 39, interrupt number 23
  NVIC_EN0_R = 1<<23;           // 9) enable IRQ 23 in NVIC
  TIMER2_CTL_R = 0x00000001;    // 10) enable timer2A
}

void Timer2A_Handler(void){
  TIMER2_ICR_R = TIMER_ICR_TATOCINT;// acknowledge TIMER2A timeout
	
	LED_RedToggle();
	
	long sr = StartCritical();
	
	setDayTime();

	global_Sec++;
	
	if(global_Sec == 60){
		time_Interrupt = 1;
		global_Sec = 0;
		global_Min++;
		
	}
	if(global_Min == 60){
		global_Min = 0;
		global_Hour++;
		interrupt_Flag = 1;
		updateTime = 1;
	}
	global_Hour = global_Hour%24;
	
	
	if((global_Hour == alarm1_Hour && global_Min == alarm1_Min && global_Sec == alarm1_Sec && alarm1_Enable) ||
		(global_Hour == alarm2_Hour && global_Min == alarm2_Min && global_Sec == alarm2_Sec && alarm2_Enable))		//turn on alarm if alarm1 or alarm2 go off
	{
			interrupt_Flag = 1;
			current_State = ALARMSNOOZE;
			alarm_Lock = 1;
	}
	
	if(global_Hour == snooze_Hour && global_Min == snooze_Min && global_Sec == snooze_Sec && snooze_Enable) {	//turn on alarm if snooze goes off
			current_State = SNOOZESNOOZE;
			interrupt_Flag = 1;
			alarm_Lock = 1;
			snooze_Enable = 0;
	}
	
	
	//stopwatch (first second might be a bit inac+curate)
	if(stopwatch_Enable) {
		if(current_State >= SWTOG && current_State <= SWBACK) {
			interrupt_Flag = 1;
		}
		stopwatch_Sec++;
		if(stopwatch_Sec == 60){
			stopwatch_Sec = 0;
			stopwatch_Min++;
		}
		if(stopwatch_Min == 60){
			stopwatch_Min = 0;
			stopwatch_Hour++;
		}
		stopwatch_Hour = stopwatch_Hour%24;
	}	
	
	//timer
	if(timer_Enable) {
		if(current_State >= TIMERSETH && current_State <= TIMERSETS) {
			if(current_State != TIMERTOG || current_State != TIMERBACK || current_State != TIMERRESET) {
				current_State = TIMERTOG;
			}
			interrupt_Flag = 1;
		}
		if(timer_Sec == 0 && timer_Min == 0 && timer_Hour == 0) {
			current_State = TIMERLOCK;
			alarm_Lock = 1;
			interrupt_Flag = 1;
			timer_Enable = 0;
			
		}
		else {
				if(timer_Sec != 0) {
					timer_Sec--;
				}
				else {
					timer_Sec = 59;
					if(timer_Min != 0) {
						timer_Min--;
					}
					else {
						timer_Min = 59;
						timer_Hour--;
					}
				}
		
		}
		
	}
	
	
	EndCritical(sr);
	LED_RedToggle();
}

