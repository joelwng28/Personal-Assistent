 /* ******** Globals.h ************** 
 * Samuel Zhang, Zi Zhou Wang
 * 2/6/2018
 * images and data points
 * Lab 3
 * Saadallah
 * 2/6/2018
 */
 
extern uint16_t current_State;
extern uint8_t updateTime;

extern uint8_t songID;
extern uint8_t songChanged; 
extern uint8_t playing;

extern uint8_t alarm_Lock;

extern uint8_t interrupt_Flag;
extern uint8_t time_Interrupt;

extern uint8_t dayTime; extern uint8_t forceNight;
 
//state values
#define LOADING					0
#define MAINBLANK 			1
#define MAINWEATHER 		2
#define MAINALARMS			4
#define	MAINSTOPWATCH		5
#define MAINTIMER				6
#define MAINSETTINGS		9
#define SWTOG						20
#define SWRESET					21
#define SWBACK					22
#define WEATHERLOAD			30
#define WEATHERBACK			33
#define TIMERHHOVER			45
#define TIMERMHOVER			46
#define TIMERSETH				40
#define TIMERSETM				41
#define TIMERTOG				42
#define TIMERRESET			43
#define TIMERBACK				44
#define TIMERSHOVER			47
#define TIMERSETS				48

#define ALARMSELECT1		100
#define ALARMSELECT2		101
#define ALARMSELECT1TOG	112
#define ALARMSELECT2TOG	113
#define ALARMBACK				116
#define A1HHOVER				102
#define A1H				 			103
#define A1MHOVER				104
#define A1M							105
#define A1BACK					106
#define A2HHOVER				107
#define A2H							108
#define A2MHOVER				109
#define A2M							110
#define A2BACK					111
#define A1ENABLE        114
#define A2ENABLE 				115

#define SETTINGS1				210
#define SETTINGS1SONG1	211
#define SETTINGS1SONG2	212
#define SETTINGS1SONG3	213
#define SETTINGS1SONG4	214
#define SETTINGS1BACK		219
#define SETTINGS2				220
#define SETTINGS2UNITS	221
#define SETTINGS2BACK		229
#define SETTINGS3				230
#define SETTINGS3NIGHT	231
#define SETTINGS3BACK		239
#define SETTINGS4				240
#define SETTINGSBACK		299

#define ALARMSNOOZE			900
#define ALARMSTOP 			901
#define TIMERLOCK				902
#define SNOOZESNOOZE		903
#define SNOOZESTOP			904

 
 
extern uint8_t global_Hour; extern uint8_t global_Min; extern uint8_t global_Sec;
extern uint8_t timer_Hour; extern uint8_t timer_Min; extern uint8_t timer_Sec; extern uint8_t timer_Enable;
extern uint8_t alarm1_Hour; extern uint8_t alarm1_Min; extern uint8_t alarm1_Sec; extern uint8_t alarm1_Enable;
extern uint8_t alarm2_Hour; extern uint8_t alarm2_Min; extern uint8_t alarm2_Sec; extern uint8_t alarm2_Enable;
extern uint8_t snooze_Hour; extern uint8_t snooze_Min; extern uint8_t snooze_Sec; extern uint8_t snooze_Enable;
extern uint8_t stopwatch_Hour; extern uint8_t stopwatch_Min; extern uint8_t stopwatch_Sec; extern uint8_t stopwatch_Enable;

extern uint8_t SIUnits; extern uint8_t units_Changed;

extern const unsigned short icon0B[];
extern const unsigned short icon0W[];
extern const unsigned short icon1B[];
extern const unsigned short icon1W[];
extern const unsigned short icon3B[];
extern const unsigned short icon3W[];
extern const unsigned short icon4B[];
extern const unsigned short icon4W[];
extern const unsigned short icon5B[];
extern const unsigned short icon5W[];
extern const unsigned short icon6B[];
extern const unsigned short icon6W[];
extern const unsigned short icon7B[];
extern const unsigned short icon7W[];
extern const unsigned short icon8B[];
extern const unsigned short icon8W[];
extern const unsigned short icon12B[];
extern const unsigned short icon12W[];
extern const unsigned short icon13B[];
extern const unsigned short icon13W[];
extern const unsigned short icon17B[];
extern const unsigned short icon17W[];
extern const unsigned short icon18B[];
extern const unsigned short icon18W[];
extern const unsigned short icon19B[];
extern const unsigned short icon19W[];
extern const unsigned short icon20B[];
extern const unsigned short icon20W[];
extern const unsigned short icon22B[];
extern const unsigned short icon22W[];
extern const unsigned short icon23B[];
extern const unsigned short icon23W[];
extern const unsigned short icon25B[];
extern const unsigned short icon25W[];
extern const unsigned short icon26B[];
extern const unsigned short icon26W[];
extern const unsigned short icon27B[];
extern const unsigned short icon27W[];
extern const unsigned short icon28B[];
extern const unsigned short icon28W[];
extern const unsigned short icon31B[];
extern const unsigned short icon31W[];
extern const unsigned short icon32B[];
extern const unsigned short icon32W[];
extern const unsigned short icon36B[];
extern const unsigned short icon36W[];
extern const unsigned short icon38B[];
extern const unsigned short icon38W[];
extern const unsigned short icon40B[];
extern const unsigned short icon40W[];
extern const unsigned short icon41B[];
extern const unsigned short icon41W[];
extern const unsigned short icon44B[];
extern const unsigned short icon44W[];

extern const unsigned short naB[];
extern const unsigned short naW[];

extern const unsigned short zero[];
extern const unsigned short one[];
extern const unsigned short two[];
extern const unsigned short three[];
extern const unsigned short four[];
extern const unsigned short five[];
extern const unsigned short six[];
extern const unsigned short seven[];
extern const unsigned short eight[];
extern const unsigned short nine[];
extern const unsigned short ten[];
extern const unsigned short colon[];

extern const unsigned short zeroW[];
extern const unsigned short oneW[];
extern const unsigned short twoW[];
extern const unsigned short threeW[];
extern const unsigned short fourW[];
extern const unsigned short fiveW[];
extern const unsigned short sixW[];
extern const unsigned short sevenW[];
extern const unsigned short eightW[];
extern const unsigned short nineW[];
extern const unsigned short tenW[];
extern const unsigned short colonW[];


extern const unsigned short clock[];
extern const unsigned short clock1[];
extern const unsigned short clock2[];
extern const unsigned short clock3[];

extern const unsigned int secX[];
extern const unsigned int secY[];
extern const unsigned int minX[];
extern const unsigned int minY[];
extern const unsigned int hourX[];
extern const unsigned int hourY[];
