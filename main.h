
extern unsigned int weatherID;
extern char tempString[13];
extern char songList[4][14];
extern char forecast[150];


//setDayTime
//sets daytime variable based on global hour
//input/output: none
void setDayTime(void);

//*************Start_Tone********************************************
//  Plays music if music was paused
//  Inputs: None
//  Output: none
void Start_Tone(void);

//*************Rewind_Tone********************************************
//  Play music from the start
//  Inputs: None
//  Output: none
void Rewind_Tone(void);

//*************Stop_Tone********************************************
// Pauses music
//  Inputs: None
//  Output: none
void Stop_Tone(void);

//getWeather
//sets global variables that store weatherID and temperature
//also sets global time
//inputs: none
//outputs: none
void getWeather(void);
