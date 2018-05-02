// Music.h
// Store music file representations and note processing
// Lab 5
// Samuel Zhang and Joel Wang
// 2/19/18

#include "..\cc3100\simplelink\include\simplelink.h"

// if desired interrupt frequency is f, Timer1A_Init parameter is busfrequency/f
#define F16HZ (80000000/16)
#define F20KHZ (80000000/20000)
#define F64HZ	(80000000/64)
#define ENVELOPE_SCALE 100000

#define A0 	28
#define A0S	29
#define B0 	31
#define C1 	33
#define C1S	35
#define	D1	37
#define D1S	39
#define	E1	41
#define	F1	44
#define	F1S	46
#define	G1	49
#define	G1S	52
#define	A1	55
#define	A1S	58
#define	B1	62
#define C2	65
#define	C2S	69
#define	D2	73
#define	D2S	78
#define	E2	82
#define	F2	87
#define	F2S	92
#define	G2	98
#define	G2S	104
#define	A2	110
#define	A2S	117
#define	B2	123
#define	C3	130
#define C3S	139
#define	D3	147
#define D3S	156
#define	E3	165
#define	F3	175
#define	F3S	185
#define	G3	196
#define	G3S	208
#define	A3	220
#define	A3S	233
#define	B3	247
#define	C4	261
#define	C4S	277
#define D4	294
#define	D4S	311
#define	E4	330	
#define	F4	349
#define	F4S	370
#define	G4	392
#define	G4S	415
#define	A4	440
#define	A4S	466
#define	B4	494
#define	C5	523
#define	C5S	554
#define	D5	587
#define	D5S	622
#define	E5	659
#define	F5	698	
#define	F5S	740
#define	G5	784
#define	G5S	831
#define	A5	880	
#define	A5S	932
#define	B5	988
#define	C6	1047
#define	C6S	1109
#define	D6	1174
#define	D6S	1245
#define	E6	1319
#define	F6	1397
#define	F6S	1480
#define	G6	1568
#define	G6S	1661
#define	A6	1760
#define	A6S	1865
#define	B6	1980
#define	C7	2093
#define	C7S	2218
#define	D7	2349
#define	D7S	2489
#define	E7	2637
#define	F7	2794
#define	F7S	2960
#define	G7	3136
#define	G7S	3322
#define	A7	3520
#define	A7S	3729
#define	B7	3951
#define	C8	4186
#define A 440
#define B 494
//#define D 294
#define D 588
#define G 392

#define Whole 16
#define Half 8
#define Quarter 4
#define Eighth 2
#define Sixteenth 1




/** Represents all data about a note that will be played. */
typedef struct {
	uint16_t pitch;    //frequency for note
	uint16_t duration; //number of beats for note
} Note;

/** Represents a song that will be played. */
typedef struct {
	uint16_t note_Num; 
	Note *notes;       //all of the notes in the song
} Song;

extern Note song_Buffer[200];
extern Song global_Song;
extern uint32_t noteIndex;
extern uint16_t duration_Counter;

//*************ProcessNote********************************************
//  Incremenet through music based on initialized reload value in Timer1 and set pitch
//  Inputs: None
//  Output: none
void ProcessNote(void);
