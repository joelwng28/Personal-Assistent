/* ******** Switch.c ************** 
 * Samuel Zhang, Zi Zhou Wang
 * 2/20/2018
 * switch init, debouncing, and function control
 * Lab 5
 * Saadallah
 * 2/20/2018
 */

#include "../inc/tm4c123gh6pm.h"
#include <stdint.h>
#include "LED.h"
#include "Globals.h"


#define PE3 (*((volatile uint32_t *)0x40024020))
#define PE2 (*((volatile uint32_t *)0x40024010))
#define PC6 (*((volatile uint32_t *)0x40006100))
#define PD2 (*((volatile uint32_t *)0x40007010))
	
#define PF1 (*((volatile uint32_t *)0x40025008))

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode

//flags for timer to handle interrupts
static uint8_t PE3IntFlag = 0;
static uint8_t PE2IntFlag = 0;
static uint8_t PCIntFlag = 0;
static uint8_t PDIntFlag = 0;

//registers for holding previous state of ports to ensure no debouncing
volatile static unsigned long PE3Last;
volatile static unsigned long PE2Last;
volatile static unsigned long PCLast;
volatile static unsigned long PDLast;


//*************Timer0Arm********************************************
//  Initializes one-shot 20ms timer for debouncing
//  Inputs: None
//  Output: none
static void Timer3Arm(void){
	SYSCTL_RCGCTIMER_R |= 0x08;
  TIMER3_CTL_R = 0x00000000;    // 1) disable TIMER3A during setup
  TIMER3_CFG_R = 0x00000000;    // 2) configure for 32-bit mode
  TIMER3_TAMR_R = 0x0000001;    // 3) 1-SHOT mode
  TIMER3_TAILR_R = 320000;      // 4) 20ms reload value
  TIMER3_TAPR_R = 0;            // 5) bus clock resolution
  TIMER3_ICR_R = 0x00000001;    // 6) clear TIMER0A timeout flag
  TIMER3_IMR_R = 0x00000001;    // 7) arm timeout interrupt
  NVIC_PRI8_R = (NVIC_PRI8_R&0x00FFFFFF)|0x80000000; // 8) priority 4
// interrupts enabled in the main program after all devices initialized
// vector number 35, interrupt number 19
  NVIC_EN1_R = 1<<(35-32);      // 9) enable IRQ 35 in NVIC
  TIMER3_CTL_R = 0x00000001;    // 10) enable TIMER0A
	
}

//*************GPIOArm********************************************
//  Reenables switch interrupts
//  Inputs: None
//  Output: none
void GPIOArm(void) {
	if(PE3IntFlag) {
		PE3Last = PE3;					//switch state
		PE3IntFlag = 0;
		GPIO_PORTE_ICR_R = 0x08;      //clear flag3
		GPIO_PORTE_IM_R |= 0x08;      //arm interrupt on PE3
		NVIC_PRI1_R = (NVIC_PRI1_R&0xFFFFFF00) | 0x000000A0;	//PortE priority = 5
	}
	else if(PE2IntFlag) {
		PE2Last = PE2;					//switch state
		PE2IntFlag = 0;
		GPIO_PORTE_ICR_R = 0x04;      //clear flag2
		GPIO_PORTE_IM_R |= 0x04;      //arm interrupt on PE2
		NVIC_PRI1_R = (NVIC_PRI1_R&0xFFFFFF00) | 0x000000A0;	//PortE priority = 5
	}
	else if(PCIntFlag) {
		PCLast = PC6;					//switch state
		PCIntFlag = 0;
		GPIO_PORTC_ICR_R = 0x40;      //clear flag6
		GPIO_PORTC_IM_R |= 0x40;      //arm interrupt on PC6
		NVIC_PRI0_R = (NVIC_PRI0_R&0xFF00FFFF) | 0x00A00000; 	//PortC priority = 5

	}	
	else if(PDIntFlag) {
		PDLast = PD2;					//switch state
		PDIntFlag = 0;
		GPIO_PORTD_ICR_R = 0x04;      //clear flag2
		GPIO_PORTD_IM_R |= 0x04;      //arm interrupt on PD2 

		NVIC_PRI0_R = (NVIC_PRI0_R&0x00FFFFFF) | 0xA0000000; 	//PortD priority = 5
		
	}
}

//*************Timer0A_Handler********************************************
//  Disarms Timer0A and reeanbles appropriate switch interrupt
//  Inputs: None
//  Output: none
void Timer3A_Handler(void){
  TIMER3_IMR_R = 0x00000000;    	// disarm timeout interrupt
  GPIOArm();   										// start GPIO
}


//*************Switches_Init********************************************
//  Initialize "Up" "Down" and "Select" Switches
//  Inputs: None
//  Output: none
void Switches_Init(void) {
	SYSCTL_RCGCGPIO_R |= 0x14; 	//activate clock for Port E and Port C
	SYSCTL_RCGCGPIO_R |= 0x08; 	//activate clock for Port D
	//initialize PE3
	GPIO_PORTE_DIR_R &= ~0x08; 	//make PE3 in
	GPIO_PORTE_DEN_R |= 0x08;		//digital enable PE3
	GPIO_PORTE_AFSEL_R &= ~0x08; 	// disable alt funct on PE3
	GPIO_PORTE_AMSEL_R &= ~0x08;	// disable analog on PE3
	GPIO_PORTE_PCTL_R &= ~0x000F000; // configure PE3 as GPIO
	GPIO_PORTE_IS_R &= ~0x08;		//PE3 is edge sensitive
	GPIO_PORTE_IBE_R |= 0x08;	//PE3 is both edges
	GPIO_PORTE_IM_R |= 0x08; 		//arm interrupt on PE3
	NVIC_PRI1_R = (NVIC_PRI1_R&0xFFFFFF00) | 0x000000A0;	//PortE priority = 5
	//initialize PC6 
	GPIO_PORTC_DIR_R &= ~0x40; 	//make PC6 in
	GPIO_PORTC_DEN_R |= 0x40;		//digital enable PC6
	GPIO_PORTC_AFSEL_R &= ~0x40; 	// disable alt funct on PC6
	GPIO_PORTC_AMSEL_R &= ~0x40;	// disable analog on PC6
	GPIO_PORTC_PCTL_R &= ~0x0F000000; // configure PC6 as GPIO
	GPIO_PORTC_IS_R &= ~0x40;		//PC6 is edge sensitive
	GPIO_PORTC_IBE_R |= 0x40;	//PC6 is both edges
	GPIO_PORTC_IM_R |= 0x40; 		//arm interrupt on PC6
	NVIC_PRI0_R = (NVIC_PRI0_R&0xFF00FFFF) | 0x00A00000; 	//PortC priority = 5
	//initialize PE2
	GPIO_PORTE_DIR_R &= ~0x04; 	//make PE2 in
	GPIO_PORTE_DEN_R |= 0x04;		//digital enable PE2
	GPIO_PORTE_AFSEL_R &= ~0x04; 	// disable alt funct on PE2
	GPIO_PORTE_AMSEL_R &= ~0x04;	// disable analog on PE2
	GPIO_PORTE_PCTL_R &= ~0x0000F00; // configure PE2 as GPIO
	GPIO_PORTE_IS_R &= ~0x04;		//PE2 is edge sensitive
	GPIO_PORTE_IBE_R |= 0x04;	//PE2 is both edges
	GPIO_PORTE_IM_R |= 0x04; 		//arm interrupt on PE2
	//initialize PD2
	GPIO_PORTD_DIR_R &= ~0x04; 	//make PD2 in
	GPIO_PORTD_DEN_R |= 0x04;		//digital enable PD2
	GPIO_PORTD_AFSEL_R &= ~0x04; 	// disable alt funct on PD2
	GPIO_PORTD_AMSEL_R &= ~0x04;	// disable analog on PD2
	GPIO_PORTD_PCTL_R &= ~0x00000F00; // configure PD2 as GPIO
	GPIO_PORTD_IS_R &= ~0x04;		//PD2 is edge sensitive
	GPIO_PORTD_IBE_R |= 0x04;	//PD2 is both edges
	GPIO_PORTD_IM_R |= 0x04; 		//arm interrupt on PD2
	NVIC_PRI0_R = (NVIC_PRI0_R&0x00FFFFFF) | 0xA0000000; 	//PortD priority = 5
	
	NVIC_EN0_R = 16;				//Enable interrupts for PortE
	NVIC_EN0_R = 8;				//Enable interrupts for PortD
	NVIC_EN0_R = 4;				//Enable interrupts for PortC

	PE3Last = PE3;					//Set initial state
	PE2Last = PE2;
	PCLast = PC6;					//Set initial state
	PDLast = PD2;					//Set initial state
}

long sr;	//for critical sections


void Up(void) {
	LED_GreenToggle();
	switch(current_State) {
		case MAINBLANK:
			current_State = MAINSETTINGS;
			break;
		case MAINWEATHER:
			current_State = MAINBLANK;
			break;
		case MAINALARMS:
			current_State = MAINWEATHER;
			break;
		case MAINSTOPWATCH:
			current_State = MAINALARMS;
			break;
		case MAINTIMER:
			current_State = MAINSTOPWATCH;
			break;
		case MAINSETTINGS:
			current_State = MAINTIMER;
			break;
		case SWTOG:
			current_State = SWBACK;
			break;
		case SWRESET:
			current_State = SWTOG;
			break;
		case SWBACK:
			current_State = SWRESET;
			break;
		case TIMERSETH:
			if(timer_Hour == 99) { timer_Hour = 0; }
			else {timer_Hour++;}
			break;
		case TIMERSETM:
			if(timer_Min == 59) { timer_Min = 0; }
			else {timer_Min++;}
			break;
		case TIMERSETS:
			if(timer_Sec == 59) { timer_Sec = 0; }
			else {timer_Sec++;}
			break;
		case TIMERHHOVER:
			current_State = TIMERBACK;
			break;
		case TIMERMHOVER:
			current_State = TIMERHHOVER;
			break;
		case TIMERSHOVER:
			current_State = TIMERMHOVER;
			break;
		case TIMERBACK:
			current_State = TIMERRESET;
			break;
		case TIMERTOG:
			current_State = TIMERSHOVER;
			break;
		case TIMERRESET:
			current_State = TIMERTOG;
			break;
		case SETTINGS1:
			current_State = SETTINGSBACK;
			break;
		case SETTINGS2:
			current_State = SETTINGS1;
			break;
		case SETTINGS3:
			current_State = SETTINGS2;
			break;
		case SETTINGSBACK:
			current_State = SETTINGS3;
			break;
		case SETTINGS1SONG1:
			current_State = SETTINGS1BACK;
			break;
		case SETTINGS1SONG2:
			current_State = SETTINGS1SONG1;
			break;
		case SETTINGS1SONG3:
			current_State = SETTINGS1SONG2;
			break;
		case SETTINGS1SONG4:
			current_State = SETTINGS1SONG3;
			break;
		case SETTINGS1BACK:
			current_State = SETTINGS1SONG4;
			break;
		case SETTINGS2UNITS:
			current_State = SETTINGS2BACK;
			break;
		case SETTINGS2BACK:
			current_State = SETTINGS2UNITS;
			break;
		case SETTINGS3NIGHT:
			current_State = SETTINGS3BACK;
			break;
		case SETTINGS3BACK:
			current_State = SETTINGS3NIGHT;
			break;
		case ALARMSELECT1:
			current_State = ALARMBACK;
			break;
		case ALARMSELECT1TOG:
			current_State = ALARMSELECT1;
			break;
		case ALARMSELECT2:
			current_State = ALARMSELECT1TOG;
			break;
		case ALARMSELECT2TOG:
			current_State = ALARMSELECT2;
			break;
		case ALARMBACK:
			current_State = ALARMSELECT2TOG;
			break;
		case A1HHOVER:
			current_State = A1BACK;
			break;
		case A1H:
			if(alarm1_Hour == 23) {
					alarm1_Hour = 0;
			}
			else {
				alarm1_Hour++;
			}
			break;
		case A1MHOVER:
			current_State = A1HHOVER;
			break;
		case A1M:
			if(alarm1_Min == 59) {
					alarm1_Min = 0;
			}
			else {
				alarm1_Min++;
			}
			break;
		case A1ENABLE:
			current_State = A1MHOVER;
			break;
		case A1BACK:
			current_State = A1ENABLE;
			break;
		case A2HHOVER:
			current_State = A2BACK;
			break;
		case A2H:
			if(alarm2_Hour == 23) {
					alarm2_Hour = 0;
			}
			else {
				alarm2_Hour++;
			}
			break;
		case A2MHOVER:
			current_State = A2HHOVER;
			break;
		case A2M:
			if(alarm2_Min == 59) {
					alarm2_Min = 0;
			}
			else {
				alarm2_Min++;
			}
			break;
		case A2ENABLE:
			current_State = A2MHOVER;
			break;
		case A2BACK:
			current_State = A2ENABLE;
			break;
		case ALARMSNOOZE:
			current_State = ALARMSTOP;
			break;
		case ALARMSTOP:
			current_State = ALARMSNOOZE;
			break;
		case SNOOZESNOOZE:
			current_State = SNOOZESTOP;
			break;
		case SNOOZESTOP:
			current_State = SNOOZESNOOZE;
			break;
		default:
			interrupt_Flag--;
	}
}


void Down(void) {
	LED_BlueToggle();
	switch(current_State) {
		case MAINBLANK:
			current_State = MAINWEATHER;
			break;
		case MAINWEATHER:
			current_State = MAINALARMS;
			break;
		case MAINALARMS:
			current_State = MAINSTOPWATCH;
			break;
		case MAINSTOPWATCH:
			current_State = MAINTIMER;
			break;
		case MAINTIMER:
			current_State = MAINSETTINGS;
			break;
		case MAINSETTINGS:
			current_State = MAINBLANK;
			break;
		case SWTOG:
			current_State = SWRESET;
			break;
		case SWRESET:
			current_State = SWBACK;
			break;
		case SWBACK:
			current_State = SWTOG;
			break;
		case TIMERSETH:
			if(timer_Hour == 0) { timer_Hour = 99; }
			else {timer_Hour--;}
			break;
		case TIMERSETM:
			if(timer_Min == 0) { timer_Min = 59; }
			else {timer_Min--;}
			break;
		case TIMERSETS:
			if(timer_Sec == 0) { timer_Sec = 59; }
			else {timer_Sec--;}
			break;
		case TIMERHHOVER:
			current_State = TIMERMHOVER;
			break;
		case TIMERMHOVER:
			current_State = TIMERSHOVER;
			break;
		case TIMERSHOVER:
			current_State = TIMERTOG;
			break;
		case TIMERBACK:
			current_State = TIMERHHOVER;
			break;
		case TIMERTOG:
			current_State = TIMERRESET;
			break;
		case TIMERRESET:
			current_State = TIMERBACK;
			break;
		case SETTINGS1:
			current_State = SETTINGS2;
			break;
		case SETTINGS2:
			current_State = SETTINGS3;
			break;
		case SETTINGS3:
			current_State = SETTINGSBACK;
			break;
		case SETTINGSBACK:
			current_State = SETTINGS1;
			break;
		case SETTINGS1SONG1:
			current_State = SETTINGS1SONG2;
			break;
		case SETTINGS1SONG2:
			current_State = SETTINGS1SONG3;
			break;
		case SETTINGS1SONG3:
			current_State = SETTINGS1SONG4;
			break;
		case SETTINGS1SONG4:
			current_State = SETTINGS1BACK;
			break;
		case SETTINGS1BACK:
			current_State = SETTINGS1SONG1;
			break;
		case SETTINGS2UNITS:
			current_State = SETTINGS2BACK;
			break;
		case SETTINGS2BACK:
			current_State = SETTINGS2UNITS;
			break;
		case SETTINGS3NIGHT:
			current_State = SETTINGS3BACK;
			break;
		case SETTINGS3BACK:
			current_State = SETTINGS3NIGHT;
			break;
		case ALARMSELECT1:
			current_State = ALARMSELECT1TOG;
			break;
		case ALARMSELECT1TOG:
			current_State = ALARMSELECT2;
			break;
		case ALARMSELECT2:
			current_State = ALARMSELECT2TOG;
			break;
		case ALARMSELECT2TOG:
			current_State = ALARMBACK;
			break;
		case ALARMBACK:
			current_State = ALARMSELECT1;
			break;
		case A1HHOVER:
			current_State = A1MHOVER;
			break;
		case A1H:
			if(alarm1_Hour == 0) {
				alarm1_Hour = 23;
			}
			else {
				alarm1_Hour--;
			}
			break;
		case A1MHOVER:
			current_State = A1ENABLE;
			break;
		case A1M:
			if(alarm1_Min == 0) {
				alarm1_Min = 59;
			}
			else {
				alarm1_Min--;
			}
			break;
		case A1ENABLE:
			current_State = A1BACK;
			break;
		case A1BACK:
			current_State = A1HHOVER;
			break;
		case A2HHOVER:
			current_State = A2MHOVER;
			break;
		case A2H:
			if(alarm2_Hour == 0) {
				alarm2_Hour = 23;
			}
			else {
				alarm2_Hour--;
			}	
			break;
		case A2MHOVER:
			current_State = A2ENABLE;
			break;
		case A2M:
			if(alarm2_Min == 0) {
				alarm2_Min = 59;
			}
			else {
				alarm2_Min--;
			}
			break;
		case A2ENABLE:
			current_State = A2BACK;
			break;
		case A2BACK:
			current_State = A2HHOVER;
			break;
		case ALARMSNOOZE:
			current_State = ALARMSTOP;
			break;
		case ALARMSTOP:
			current_State = ALARMSNOOZE;
			break;
		case SNOOZESNOOZE:
			current_State = SNOOZESTOP;
			break;
		case SNOOZESTOP:
			current_State = SNOOZESNOOZE;
			break;
		default:
			interrupt_Flag--;
	}
}


void Select(void) {
	LED_BlueToggle();
		switch(current_State) {
		case MAINWEATHER:
			current_State = WEATHERLOAD;
			break;
		case MAINALARMS:
			current_State = ALARMSELECT1;
			break;
		case MAINSTOPWATCH:
			current_State = SWTOG;
			break;
		case MAINTIMER:
			current_State = TIMERHHOVER;
			break;
		case MAINSETTINGS:
			current_State = SETTINGS1;
			break;
		case SWTOG:
			if(stopwatch_Enable) { stopwatch_Enable = 0; }
			else { stopwatch_Enable = 1; }
			break;
		case SWRESET:
			stopwatch_Hour = 0; stopwatch_Min = 0; stopwatch_Sec = 0;	
			break;
		case SWBACK:
			current_State = MAINSTOPWATCH;
			break;
		case WEATHERLOAD:
			current_State = MAINWEATHER;
			break;
		case WEATHERBACK:
			current_State = MAINWEATHER;
			break;
		case TIMERSETH:
			current_State = TIMERHHOVER;
			break;
		case TIMERSETM:
			current_State = TIMERMHOVER;
			break;
		case TIMERSETS:
			current_State = TIMERSHOVER;
			break;
		case TIMERHHOVER:
			current_State = TIMERSETH;
			break;
		case TIMERMHOVER:
			current_State = TIMERSETM;
			break;
		case TIMERSHOVER:
			current_State = TIMERSETS;
			break;
		case TIMERBACK:
			current_State = MAINTIMER;
			break;
		case TIMERTOG:
			if(timer_Enable) { timer_Enable = 0; }
			else { timer_Enable = 1; }
			break;
		case TIMERRESET:
			timer_Enable = 0; timer_Hour = 0; timer_Min = 0; timer_Sec = 0; 
			break;
		case SETTINGS1:
			if(songID == 1) { current_State = SETTINGS1SONG1; }
			else if(songID == 2) { current_State = SETTINGS1SONG2; }
			else if(songID == 3) { current_State = SETTINGS1SONG3; }
			else if(songID == 4) { current_State = SETTINGS1SONG4; }
			else { 
				current_State = SETTINGS1SONG1;
				songID = 1;
				songChanged = 1;
			}
			break;
		case SETTINGS2:
			current_State = SETTINGS2UNITS;
			break;
		case SETTINGS3:
			current_State = SETTINGS3NIGHT;
			break;
		case SETTINGSBACK:
			current_State = MAINSETTINGS;
			break;
		case SETTINGS1SONG1:
			if(songID != 1) {
				songID = 1;
				songChanged = 1;
			}
			break;
		case SETTINGS1SONG2:
			if(songID != 2) {
				songID = 2;
				songChanged = 1;
			}
			break;
		case SETTINGS1SONG3:
			if(songID != 3) {
				songID = 3;
				songChanged = 1;
			}
			break;
		case SETTINGS1SONG4:
			if(songID != 4) {
				songID = 4;
				songChanged = 1;
			}
			break;
		case SETTINGS1BACK:
			current_State = SETTINGS1;
			break;
		case SETTINGS2UNITS:
			if(SIUnits) {
				SIUnits = 0;
			}
			else {
				SIUnits = 1;
			}
			units_Changed = 1;
			break;
		case SETTINGS2BACK:
			current_State = SETTINGS2;
			break;
		case SETTINGS3NIGHT:
			if(forceNight) {
				forceNight = 0;
			}
			else {
				forceNight = 1;
			}
			break;
		case SETTINGS3BACK:
			current_State = SETTINGS3;
			break;
		case ALARMSELECT1:
			current_State = A1HHOVER;
			break;
		case ALARMSELECT1TOG:
			if(alarm1_Enable) { alarm1_Enable = 0; }
			else { alarm1_Enable = 1;	}			
			break;
		case ALARMSELECT2:
			current_State = A2HHOVER;
			break;
		case ALARMSELECT2TOG:
			if(alarm2_Enable) { alarm2_Enable = 0; }
			else { alarm2_Enable = 1;	}	
			break;
		case ALARMBACK:
			current_State = MAINALARMS;
			break;
		case A1HHOVER:
			current_State = A1H;
			break;
		case A1H:
			current_State = A1HHOVER;
			break;
		case A1MHOVER:
			current_State = A1M;
			break;
		case A1M:
			current_State = A1MHOVER;
			break;
		case A1ENABLE:
			if(alarm1_Enable) { alarm1_Enable = 0; }
			else { alarm1_Enable = 1;	}		
			break;
		case A1BACK:
			current_State = ALARMSELECT1;
			break;
		case A2HHOVER:
			current_State = A2H;
			break;
		case A2H:
			current_State = A2HHOVER;
			break;
		case A2MHOVER:
			current_State = A2M;
			break;
		case A2M:
			current_State = A2MHOVER;
			break;
		case A2ENABLE:
			if(alarm2_Enable) { alarm2_Enable = 0; }
			else { alarm2_Enable = 1;	}
			break;
		case A2BACK:
			current_State = ALARMSELECT2;
			break;
		case ALARMSNOOZE:
			current_State = MAINBLANK;
			snooze_Enable = 1;
			snooze_Sec = global_Sec;
			if(global_Min == 59) {
				snooze_Min = 0;
				if(global_Hour == 23) {
						snooze_Hour = 0;
				}
				else {
					snooze_Hour = global_Hour+1;
				}
			}
			else {
				snooze_Hour = global_Hour;
				snooze_Min = global_Min + 1;
			}
			alarm_Lock = 0;
			//turn off sound
			break;
		case ALARMSTOP:
			current_State = MAINBLANK;
			alarm_Lock = 0;
			//turn off sound
			break;
		case SNOOZESNOOZE:
			current_State = MAINBLANK;
			snooze_Enable = 1;
			snooze_Sec = global_Sec;
			if(global_Min == 59) {
				snooze_Min = 0;
				if(global_Hour == 23) {
						snooze_Hour = 0;
				}
				else {
					snooze_Hour = global_Hour+1;
				}
			}
			else {
				snooze_Hour = global_Hour;
				snooze_Min = global_Min + 1;
			}
			alarm_Lock = 0;
			//turn off sound
			break;
		case SNOOZESTOP:
			current_State = MAINBLANK;
			alarm_Lock = 0;
			//turn off sound
			break;
		case TIMERLOCK:
			current_State = MAINBLANK;
			alarm_Lock = 0;
			//turn off sound
			break;
		default:
			interrupt_Flag--;
	}
}

void Music(void) {
	LED_GreenToggle();
	if(current_State == ALARMSNOOZE || current_State == ALARMSTOP || current_State == SNOOZESNOOZE || current_State == SNOOZESTOP) {
		current_State = MAINBLANK;
			snooze_Enable = 1;
			snooze_Sec = global_Sec;
			if(global_Min == 59) {
				snooze_Min = 0;
				if(global_Hour == 23) {
						snooze_Hour = 0;
				}
				else {
					snooze_Hour = global_Hour+1;
				}
			}
			else {
				snooze_Hour = global_Hour;
				snooze_Min = global_Min + 1;
			}
			alarm_Lock = 0;
			//turn off sound
	}
	else {
			interrupt_Flag--;
	}
}

//*************GPIOPortE_Handler********************************************
//  Completes Up Functionality
//  Inputs: None
//  Output: none
void GPIOPortE_Handler(void) {
	if(GPIO_PORTE_RIS_R&0x08) {		//poll PE3
		GPIO_PORTE_IM_R &= ~0x08;		//disarm intterupt on PE3
		PE3IntFlag = 1;
		if(!PE3Last) {
			interrupt_Flag = 1;
			sr = StartCritical();						//start critical section
				Select();
			
			EndCritical(sr);						//end critical section
		}
		else {}
	}
	if(GPIO_PORTE_RIS_R&0x04) { 	//poll PE2
		GPIO_PORTE_IM_R &= ~0x04;		//disarm intterupt on PE2
		PE2IntFlag = 1;
		if(!PE2Last) {
			interrupt_Flag = 1;
			sr = StartCritical();						//start critical section
			Up();
			
			EndCritical(sr);						//end critical section
		}
		else {}
	}
	
	Timer3Arm();								//start one shot
}

//*************GPIOPortC_Handler********************************************
//  Completes Down Functionality
//  Inputs: None
//  Output: none
void GPIOPortC_Handler(void) {
	GPIO_PORTC_IM_R &= ~0x40;		//disarm intterupt on PC6
	PCIntFlag = 1;
	if(!PCLast) {
		interrupt_Flag = 1;
		sr = StartCritical();						//start critical section
		Down();
		EndCritical(sr);				//end critical section
	}
	else {}
	
	Timer3Arm();								//start one shot
}


//*************GPIOPortD_Handler********************************************
//  Completes Music Functionality
//  Inputs: None
//  Output: none
void GPIOPortD_Handler(void) {
	GPIO_PORTD_IM_R &= ~0x04;		//disarm intterupt on PD2
	PDIntFlag = 1;
	if(!PDLast) {
		interrupt_Flag = 1;
		sr = StartCritical();						//start critical section
		Music();
		EndCritical(sr);						//end critical section
	}
	else {}
	
	Timer3Arm();								//start one shot
}






