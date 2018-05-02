 /* ******** Display.h ************** 
 * Samuel Zhang, Zi Zhou Wang
 * 4/2/2018
 * images and data points
 */
/****** displayWeatherIcon ***********
 * Display weather icon from corresponding weather code
 * Inputs: an integer from the weather api that corresponds to weather status
 * Output: None
 */
void displayWeatherIcon(int icon);

/****displayDigits *********
* Print out digits
* Inputs: hour and minute numbers
* Outputs: None
*/
void displayDigits(int hour, int min);

//displayJustTime
//output time in top left relative to night/day mode
//input: none
//output: none
void displayJustTime(void);

//*************displayAlarmScreenDefault********************************************
// Draws alarm screen
// Input: none
//  Output: none
void displayAlarmDefault(void);

//*************displayAlarmSelect1********************************************
// Select Alarm 1
// Input: none
//  Output: none
void displayAlarmSelect1(void);

//*************displayAlarmSelect2********************************************
// Select Alarm 2
// Input: none
//  Output: none
void displayAlarmSelect2(void);

//*************displayAlarmSelect1Tog********************************************
// Select Alarm 1 Toggle
// Input: none
//  Output: none
void displayAlarmSelect1Tog(void);

//*************displayAlarmSelect1Tog********************************************
// Select Alarm 2 Toggle
// Input: none
//  Output: none
void displayAlarmSelect2Tog(void);

//*************displayAlarmBack********************************************
// Select Alarm Back
// Input: none
//  Output: none
void displayAlarmBack(void);

//*************displayAlarm1Default********************************************
// Draws the alarm 1 default template
// Input: none
//  Output: none
void displayAlarm1Default(void);

//*************displayAlarm1HourSelect********************************************
// Draws the alarm 1 hour select screen
// Input: none
//  Output: none
void displayAlarm1HourSelect(void);

//*************displayAlarm1MinSelect********************************************
// Draws the alarm 1 min select screen
// Input: none
//  Output: none
void displayAlarm1MinSelect(void);

//*************displayAlarm1Hour********************************************
// Draws the alarm 1 hour screen
// Input: none
//  Output: none
void displayAlarm1Hour(void);

//*************displayAlarm1Min********************************************
// Draws the alarm 1 min screen
// Input: none
//  Output: none
void displayAlarm1Min(void);

//*************displayAlarm1Enable********************************************
// Draws the alarm 1 enable screen
// Input: none
//  Output: none
void displayAlarm1Enable(void);

//*************displayAlarm1Back********************************************
// Draws the alarm 1 back screen
// Input: none
//  Output: none
void displayAlarm1Back(void);

//*************displayAlarm2Default********************************************
// Draws the alarm 2 default template
// Input: none
//  Output: none
void displayAlarm2Default(void);

//*************displayAlarm2HourSelect********************************************
// Draws the alarm 2 hour select screen
// Input: none
//  Output: none
void displayAlarm2HourSelect(void);

//*************displayAlarm2MinSelect********************************************
// Draws the alarm 2 min select screen
// Input: none
//  Output: none
void displayAlarm2MinSelect(void);

//*************displayAlarm2Hour********************************************
// Draws the alarm 2 hour screen
// Input: none
//  Output: none
void displayAlarm2Hour(void);

//*************displayAlarm2Min********************************************
// Draws the alarm 2 min screen
// Input: none
//  Output: none
void displayAlarm2Min(void);

//*************displayAlarm2Enable********************************************
// Draws the alarm 2 enable screen
// Input: none
//  Output: none
void displayAlarm2Enable(void);

//*************displayAlarm2Back********************************************
// Draws the alarm 2 back screen
// Input: none
//  Output: none
void displayAlarm2Back(void);

//*************displayLoading********************************************
// display loading
// Input: none
//  Output: none
void displayLoading(void);

//*************displayLoadingSong********************************************
// display loading
// Input: none
//  Output: none
void displayLoadingSong(void);

//*************displayMain********************************************
// Draws the Main Screen
// Input: str, the mode to display at bottom center of screen
//  Output: none
void displayMain(char* str);

//*************displaySW********************************************
//  Draws stopwatch screen
//  Inputs: none
//  Output: none
void diaplySW(void);

//*************displaySWBack********************************************
//  Draws stopwatch back screen
//  Inputs: none
//  Output: none
void displaySWBack(void);

//*************displaySWSRS********************************************
//  Draws stopwatch start/stop screen
//  Inputs: none
//  Output: none
void displaySWSRS(void);

//*************displaySWReset********************************************
//  Draws stopwatch reset screen
//  Inputs: none
//  Output: none
void displaySWReset(void);


//*************displayWeatherLoad********************************************
//  Draws loading weather with back option
//  Inputs: none
//  Output: none
void displayWeatherLoad(void);

//*************displayWeatherScreen********************************************
//  Displays weather information
//  Inputs: none
//  Output: none
void displayWeatherScreen(void);

//*************displaySettings********************************************
// display settings menu
// Input: integer for option, -1 is selecting back button
//  Output: none
void displaySettings(int i);

//*************displayTimerDefault********************************************
// Draws the timer default template
// Input: none
//  Output: none
void displayTimerDefault(void);

//*************displayTimerHourSelect********************************************
// Draws the timer hour select screen
// Input: none
//  Output: none
void displayTimerHourSelect(void);

//*************displayTimerMinSelect********************************************
// Draws the timer min select screen
// Input: none
//  Output: none
void displayTimerMinSelect(void);

//*************displayTimerSecSelect********************************************
// Draws the timer min select screen
// Input: none
//  Output: none
void displayTimerSecSelect(void);

//*************displayTimerHour********************************************
// Draws the timer hour screen
// Input: none
//  Output: none
void displayTimerHour(void);

//*************displayTimerMin********************************************
// Draws the alarm 1 min screen
// Input: none
//  Output: none
void displayTimerMin(void);

//*************displayTimerSec********************************************
// Draws the alarm 1 min screen
// Input: none
//  Output: none
void displayTimerSec(void);

//*************displayTimerEnable********************************************
// Draws the timer enable screen
// Input: none
//  Output: none
void displayTimerEnable(void);

//*************displayTimerReset*******************************************
// Draws the timer enable screen
// Input: none
//  Output: none
void displayTimerReset(void);

//*************displayTimerBack********************************************
// Draws the timer back screen
// Input: none
//  Output: none
void displayTimerBack(void);


//*************displayS2Units********************************************
// Draws weather settings units
// Input: none
//  Output: none
void displayS2Units(void);

//*************displayS2back********************************************
// Draws weather settings back
// Input: none
//  Output: none
void displayS2Back(void);

//*************displayS3Night********************************************
// Draws nightmode settings select
// Input: none
//  Output: none
void displayS3Night(void);

//*************displayS3Back********************************************
// Draws nightmode settings back
// Input: none
//  Output: none
void displayS3Back(void);

//*************displayS3Back********************************************
// display song list with song ID selected
// Input: integer with song ID selected (-1 for back)
//  Output: none
void displayS1Songs(int i);

//*************displayAlarmLock********************************************
// Draws the alarm lock template
// Input: string to output for mode
//  Output: none
void displayAlarmLock(char* str);

//*************displayAlarmLockSnooze********************************************
// Draws the alarm lock snooze screen
// Input: string to output for mode
//  Output: none
void displayAlarmLockSnooze(char* str);

//*************displayAlarmLockStop********************************************
// Draws the alarm lock stop screen
// Input: string to output for mode
//  Output: none
void displayAlarmLockStop(char* str);

//*************displayTimerNotif********************************************
// Draws the timer going off
// Input: none
//  Output: none
void displayTimerNotif(void);
