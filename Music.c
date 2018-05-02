// Music.c
// Store music file representations and note processing
// Lab 5
// Samuel Zhang and Joel Wang
// 2/19/18

#include "Music.h"
#include "main.h"
#include "../inc/tm4c123gh6pm.h"
#include "LED.h"


#define PF1 (*((volatile uint32_t *)0x40025008))




Note BeautyAndTheBeastL[] = {
{ 0 , Quarter} ,
{ B3 , Quarter} ,
{ C4 , Quarter} ,
{ D4 , Quarter} ,
{ E4 , Quarter} ,
{ D4 , Eighth} ,
{ C4 , Eighth} ,
{ D4 ,Half} ,
{ 0 , Quarter} ,
{ B3 , Quarter} ,
{ C4 , Quarter} ,
{ D4 , Quarter} ,
{ E4 , Quarter} ,
{ D4 , Eighth} ,
{ C4 , Eighth} ,
{ D4 , Quarter} ,
{ C4 , Quarter} ,
{ B3 , Half} ,
{ B3 , Eighth} ,
{ D4 , Eighth} ,
{ F4S , Eighth} ,
{ G4, Eighth} ,
{ C4 , Quarter} ,
{ C4 , Half} ,
{ 0 , Quarter} ,
{0, Whole}
};

Note song_Buffer[200];
Song global_Song = {25, BeautyAndTheBeastL };
uint32_t noteIndex = 0;
uint16_t duration_Counter = 0;



//*************ProcessNote********************************************
//  Incremenet through music based on initialized reload value in Timer1 and set pitch
//  Inputs: None
//  Output: none
void ProcessNote(void) {
	
	LED_BlueToggle();
	if(noteIndex > global_Song.note_Num) {
		NVIC_ST_RELOAD_R = 0;
		Rewind_Tone();
	}
	else if(duration_Counter >= (global_Song.notes[noteIndex].duration*2)) {
		noteIndex++;
		duration_Counter = 0;
		if(global_Song.notes[noteIndex].pitch == 0) {
			NVIC_ST_RELOAD_R = 0;
		}
		else {
			NVIC_ST_RELOAD_R = 80000000 / (global_Song.notes[noteIndex].pitch*64);
		}
	}
	else {
		duration_Counter++;
		
	}
	LED_BlueToggle();

}
