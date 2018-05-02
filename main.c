/*
 * main.c - Example project for UT.6.02x Embedded Systems - Shape the World
 * Jonathan Valvano and Ramesh Yerraballi
 * September 14, 2016
 * Hardware requirements 
     TM4C123 LaunchPad, optional Nokia5110
     CC3100 wifi booster and 
     an internet access point with OPEN, WPA, or WEP security
 
 * derived from TI's getweather example
 * Copyright (C) 2014 Texas Instruments Incorporated - http://www.ti.com/
 *
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
*/

/*
 * Application Name     -   Get weather
 * Application Overview -   This is a sample application demonstrating how to
                            connect to openweathermap.org server and request for
              weather details of a city.
 * Application Details  -   http://processors.wiki.ti.com/index.php/CC31xx_SLS_Get_Weather_Application
 *                          doc\examples\sls_get_weather.pdf
 */
 /* CC3100 booster pack connections (unused pins can be used by user application)
Pin  Signal        Direction      Pin   Signal     Direction
P1.1  3.3 VCC         IN          P2.1  Gnd   GND      IN
P1.2  PB5 UNUSED      NA          P2.2  PB2   IRQ      OUT
P1.3  PB0 UART1_TX    OUT         P2.3  PE0   SSI2_CS  IN
P1.4  PB1 UART1_RX    IN          P2.4  PF0   UNUSED   NA
P1.5  PE4 nHIB        IN          P2.5  Reset nRESET   IN
P1.6  PE5 UNUSED      NA          P2.6  PB7  SSI2_MOSI IN
P1.7  PB4 SSI2_CLK    IN          P2.7  PB6  SSI2_MISO OUT
P1.8  PA5 UNUSED      NA          P2.8  PA4   UNUSED   NA
P1.9  PA6 UNUSED      NA          P2.9  PA3   UNUSED   NA
P1.10 PA7 UNUSED      NA          P2.10 PA2   UNUSED   NA

Pin  Signal        Direction      Pin   Signal      Direction
P3.1  +5  +5 V       IN           P4.1  PF2 UNUSED      OUT
P3.2  Gnd GND        IN           P4.2  PF3 UNUSED      OUT
P3.3  PD0 UNUSED     NA           P4.3  PB3 UNUSED      NA
P3.4  PD1 UNUSED     NA           P4.4  PC4 UART1_CTS   IN
P3.5  PD2 UNUSED     NA           P4.5  PC5 UART1_RTS   OUT
P3.6  PD3 UNUSED     NA           P4.6  PC6 UNUSED      NA
P3.7  PE1 UNUSED     NA           P4.7  PC7 NWP_LOG_TX  OUT
P3.8  PE2 UNUSED     NA           P4.8  PD6 WLAN_LOG_TX OUT
P3.9  PE3 UNUSED     NA           P4.9  PD7 UNUSED      IN (see R74)
P3.10 PF1 UNUSED     NA           P4.10 PF4 UNUSED      OUT(see R75)

UART0 (PA1, PA0) sends data to the PC via the USB debug cable, 115200 baud rate
Port A, SSI0 (PA2, PA3, PA5, PA6, PA7) sends data to Nokia5110 LCD

*/
#include "../inc/tm4c123gh6pm.h"
#include "..\cc3100\simplelink\include\simplelink.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/fpu.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"
#include "utils/cmdline.h"
#include "application_commands.h"
#include "LED.h"
#include <string.h>
#include <stdio.h>
#include "ST7735.h"
#include "PLL.h"
#include "SysTick.h"
#include "Switch.h"
#include "Display.h"
#include "Timer2.h" //Clock Timer
#include "Globals.h"
#include "DAC.h"
#include "Music.h"
#include "SysTick.h"
#include "Timer1.h"

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode

//#define SSID_NAME  "valvanoAP" /* Access point name to connect to */
#define SEC_TYPE   SL_SEC_TYPE_WPA
//#define PASSKEY    "12345678"  /* Password in case of secure AP */ 
#define SSID_NAME  "Honor 8"
#define PASSKEY    "bunnizor"
#define BAUD_RATE   115200
#define PF1 (*((volatile uint32_t *)0x40025008))



static char *temperaturePtr;
char tempString[13];
uint32_t weatherID;
char songList[4][14];
char forecast[150];

void UART_Init(void){
  SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
  GPIOPinConfigure(GPIO_PA0_U0RX);
  GPIOPinConfigure(GPIO_PA1_U0TX);
  GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
  UARTStdioConfig(0,BAUD_RATE,50000000);
}

#define MAX_RECV_BUFF_SIZE  1024
#define MAX_SEND_BUFF_SIZE  512
#define MAX_HOSTNAME_SIZE   40
#define MAX_PASSKEY_SIZE    32
#define MAX_SSID_SIZE       32


#define SUCCESS             0

#define CONNECTION_STATUS_BIT   0
#define IP_AQUIRED_STATUS_BIT   1

/* Application specific status/error codes */
typedef enum{
    DEVICE_NOT_IN_STATION_MODE = -0x7D0,/* Choosing this number to avoid overlap w/ host-driver's error codes */

    STATUS_CODE_MAX = -0xBB8
}e_AppStatusCodes;


/* Status bits - These are used to set/reset the corresponding bits in 'g_Status' */
typedef enum{
    STATUS_BIT_CONNECTION =  0, /* If this bit is:
                                 *      1 in 'g_Status', the device is connected to the AP
                                 *      0 in 'g_Status', the device is not connected to the AP
                                 */

    STATUS_BIT_IP_AQUIRED,       /* If this bit is:
                                 *      1 in 'g_Status', the device has acquired an IP
                                 *      0 in 'g_Status', the device has not acquired an IP
                                 */

}e_StatusBits;


#define SET_STATUS_BIT(status_variable, bit)    status_variable |= (1<<(bit))
#define CLR_STATUS_BIT(status_variable, bit)    status_variable &= ~(1<<(bit))
#define GET_STATUS_BIT(status_variable, bit)    (0 != (status_variable & (1<<(bit))))
#define IS_CONNECTED(status_variable)           GET_STATUS_BIT(status_variable, \
                                                               STATUS_BIT_CONNECTION)
#define IS_IP_AQUIRED(status_variable)          GET_STATUS_BIT(status_variable, \
                                                               STATUS_BIT_IP_AQUIRED)


typedef struct{
    UINT8 SSID[MAX_SSID_SIZE];
    INT32 encryption;
    UINT8 password[MAX_PASSKEY_SIZE];
}UserInfo;

/*
 * GLOBAL VARIABLES -- Start
 */

char Recvbuff[MAX_RECV_BUFF_SIZE];
char SendBuff[MAX_SEND_BUFF_SIZE];
char HostName[MAX_HOSTNAME_SIZE];
unsigned long DestinationIP;
int SockID;


typedef enum{
    CONNECTED = 0x01,
    IP_AQUIRED = 0x02,
    IP_LEASED = 0x04,
    PING_DONE = 0x08

}e_Status;
UINT32  g_Status = 0;
/*
 * GLOBAL VARIABLES -- End
 */


 /*
 * STATIC FUNCTION DEFINITIONS  -- Start
 */

static int32_t configureSimpleLinkToDefaultState(char *);


/*
 * STATIC FUNCTION DEFINITIONS -- End
 */


void Crash(uint32_t time){
  while(1){
    for(int i=time;i;i--){};
    LED_RedToggle();
  }
}

//*************Start_Tone********************************************
//  Plays music if music was paused
//  Inputs: None
//  Output: none
void Start_Tone(void) {
	//initialize timers to begin playing
	TIMER1_CTL_R = 0x01;
	NVIC_ST_RELOAD_R = 80000000 / (global_Song.notes[noteIndex].pitch*64);
	NVIC_ST_CTRL_R = 0x7;
}

//*************Rewind_Tone********************************************
//  Play music from the start
//  Inputs: None
//  Output: none
void Rewind_Tone(void) {
	noteIndex = 0;
	NVIC_ST_RELOAD_R = 80000000 / (global_Song.notes[noteIndex].pitch*64);
	duration_Counter = 0;
}

//*************Stop_Tone********************************************
// Pauses music
//  Inputs: None
//  Output: none
void Stop_Tone(void) {

	noteIndex = 0;
	
	//disable interrupts
	NVIC_ST_CTRL_R = 0x0;
	TIMER1_CTL_R = 0x00000000;
}



//setDayTime
//sets daytime variable based on global hour
//input/output: none
void setDayTime() {
	if(global_Hour > 19  || global_Hour < 7) {
		dayTime = 0;
	}
	else {
		dayTime = 1;
	}
}

//setTime
//sets time from json - adjusts for daylight savings + timezones
//input: json from openweatherserver
//ouptut: none
void setTime(char *json) {
	char *timePtr;
	timePtr = strstr(json, "2018 ");
				
	char timeString[20];
	strncpy(timeString, timePtr, 20);
			
	char hourTen = timeString[5];
	char hourOne = timeString[6];
	char minTen = timeString[8];
	char minOne = timeString[9];
	char secTen = timeString[11];
	char secOne = timeString[12];
	
	global_Hour = (hourTen - 48)*10 + (hourOne - 48);
	global_Min = (minTen - 48) * 10 + (minOne - 48);
	global_Sec = (secTen - 48) * 10 + (secOne - 48);
	
	if(global_Hour < 5) {
		global_Hour = 24 + global_Hour - 5;
	}
	else {
		global_Hour -= 5;
	}
	
	//set dayTime
	setDayTime();

	
}

//getNoteFreqFromString
//takes in string of note and returns frequency in integer value
//input: char* str
//ouput: int for respective frequency
int getNoteFreqFromString(char* str) {
	if(strcmp(str, "A0") == 0) { return 28;}
	else if(strcmp(str, "0") == 0) { return 0; }
	else if(strcmp(str, "A0S") == 0) { return 29;}
	else if(strcmp(str, "B0") == 0) { return 31;}
	else if(strcmp(str, "C1") == 0) { return 33;}
	else if(strcmp(str, "C1S") == 0) { return 35;}
	else if(strcmp(str, "D1") == 0) { return 37;}
	else if(strcmp(str, "D1S") == 0) { return 39;}
	else if(strcmp(str, "E1") == 0) { return 41;}
	else if(strcmp(str, "F1") == 0) { return 44;}
	else if(strcmp(str, "F1S") == 0) { return 46;}
	else if(strcmp(str, "G1") == 0) { return 49;}
	else if(strcmp(str, "G1S") == 0) { return 52;}
	else if(strcmp(str, "A1") == 0) { return 55;}
	else if(strcmp(str, "A1S") == 0) { return 58;}
	else if(strcmp(str, "B1") == 0) { return 62;}
	else if(strcmp(str, "C2") == 0) { return 65;}
	else if(strcmp(str, "C2S") == 0) { return 69;}
	else if(strcmp(str, "D2") == 0) { return 73;}
	else if(strcmp(str, "D2S") == 0) { return 78;}
	else if(strcmp(str, "E2") == 0) { return 82;}
	else if(strcmp(str, "F2") == 0) { return 87;}
	else if(strcmp(str, "F2S") == 0) { return 92;}
	else if(strcmp(str, "G2") == 0) { return 98;}
	else if(strcmp(str, "G2S") == 0) { return 104;}
	else if(strcmp(str, "A2") == 0) { return 110;}
	else if(strcmp(str, "A2S") == 0) { return 117;}
	else if(strcmp(str, "B2") == 0) { return 123;}
	else if(strcmp(str, "C3") == 0) { return 130;}
	else if(strcmp(str, "C3S") == 0) { return 139;}
	else if(strcmp(str, "D3") == 0) { return 147;}
	else if(strcmp(str, "D3S") == 0) { return 156;}
	else if(strcmp(str, "E3") == 0) { return 165;}
	else if(strcmp(str, "F3") == 0) { return 175;}
	else if(strcmp(str, "F3S") == 0) { return 185;}
	else if(strcmp(str, "G3") == 0) { return 196;}
	else if(strcmp(str, "G3S") == 0) { return 208;}
	else if(strcmp(str, "A3") == 0) { return 220;}
	else if(strcmp(str, "A3S") == 0) { return 233;}
	else if(strcmp(str, "B3") == 0) { return 247;}
	else if(strcmp(str, "C4") == 0) { return 261;}
	else if(strcmp(str, "C4S") == 0) { return 277;}
	else if(strcmp(str, "D4") == 0) { return 294;}
	else if(strcmp(str, "D4S") == 0) { return 311;}
	else if(strcmp(str, "E4") == 0) { return 330;}
	else if(strcmp(str, "F4") == 0) { return 349;}
	else if(strcmp(str, "F4S") == 0) { return 370;}
	else if(strcmp(str, "G4") == 0) { return 392;}
	else if(strcmp(str, "G4S") == 0) { return 415;}
	else if(strcmp(str, "A4") == 0) { return 440;}
	else if(strcmp(str, "A4S") == 0) { return 466;}
	else if(strcmp(str, "B4") == 0) { return 494;}
	else if(strcmp(str, "C5") == 0) { return 523;}
	else if(strcmp(str, "C5S") == 0) { return 554;}
	else if(strcmp(str, "D5") == 0) { return 587;}
	else if(strcmp(str, "D5S") == 0) { return 622;}
	else if(strcmp(str, "E5") == 0) { return 659;}
	else if(strcmp(str, "F5") == 0) { return 698 ;}
	else if(strcmp(str, "F5S") == 0) { return 740;}
	else if(strcmp(str, "G5") == 0) { return 784;}
	else if(strcmp(str, "G5S") == 0) { return 831;}
	else if(strcmp(str, "A5") == 0) { return 880 ;}
	else if(strcmp(str, "A5S") == 0) { return 932;}
	else if(strcmp(str, "B5") == 0) { return 988;}
	else if(strcmp(str, "C6") == 0) { return 1047;}
	else if(strcmp(str, "C6S") == 0) { return 1109;}
	else if(strcmp(str, "D6") == 0) { return 1174;}
	else if(strcmp(str, "D6S") == 0) { return 1245;}
	else if(strcmp(str, "E6") == 0) { return 1319;}
	else if(strcmp(str, "F6") == 0) { return 1397;}
	else if(strcmp(str, "F6S") == 0) { return 1480;}
	else if(strcmp(str, "G6") == 0) { return 1568;}
	else if(strcmp(str, "G6S") == 0) { return 1661;}
	else if(strcmp(str, "A6") == 0) { return 1760;}
	else if(strcmp(str, "A6S") == 0) { return 1865;}
	else if(strcmp(str, "B6") == 0) { return 1980;}
	else if(strcmp(str, "C7") == 0) { return 2093;}
	else if(strcmp(str, "C7S") == 0) { return 2218;}
	else if(strcmp(str, "D7") == 0) { return 2349;}
	else if(strcmp(str, "D7S") == 0) { return 2489;}
	else if(strcmp(str, "E7") == 0) { return 2637;}
	else if(strcmp(str, "F7") == 0) { return 2794;}
	else if(strcmp(str, "F7S") == 0) { return 2960;}
	else if(strcmp(str, "G7") == 0) { return 3136;}
	else if(strcmp(str, "G7S") == 0) { return 3322;}
	else if(strcmp(str, "A7") == 0) { return 3520;}
	else if(strcmp(str, "A7S") == 0) { return 3729;}
	else if(strcmp(str, "B7") == 0) { return 3951;}
	else if(strcmp(str, "C8") == 0) { return 4186;}
	else if(strcmp(str, "A") == 0) { return 440;}
	else if(strcmp(str, "B") == 0) { return 494;}
	else if(strcmp(str, "D") == 0) { return 588;}
	else if(strcmp(str, "G") == 0) { return 392;}	
	return -1;	
}

//setTempString
//sets global tempString with global temperature
//input: json string from openweathermap
//output: none
void setTempString(char *json) {
				temperaturePtr = strstr(json, "\"temp\":");
			
				strncpy(tempString, temperaturePtr, 13);
				//temp starts on pointer 7
				if(tempString[7] == '-') { tempString[10] = '\0'; }
				else { tempString[9] = '\0'; }
}

//getID
//parses json to get weather ID for icon display
//input: json string from openweathermap
//output: int with weather ID from OWM
uint32_t getID(char *json) {
				char *temperaturePtr;
				temperaturePtr = strstr(json, "\"id\":");
				
				char tempString[9];
				strncpy(tempString, temperaturePtr, 9);
				
				uint32_t id = 0;
				id += (tempString[5] - '0') * 100;
				id += (tempString[6] - '0') * 10;
				id += (tempString[7] - '0');
				ST7735_SetCursor(0,0);
				return id;
}

/*
 * Application's entry point
 */
// 1) change Austin Texas to your city
// 2) you can change metric to imperial if you want temperature in F
#define REQUEST "GET /data/2.5/weather?q=                                     &APPID=bf1ad9efaa27c46909ab234d0304f705&units=metric               HTTP/1.1\r\nUser-Agent: Keil\r\nHost:api.openweathermap.org\r\nAccept: */*\r\n\r\n"

#define LOCREQ "GET /json/ HTTP/1.1\r\nUser-Agent: Keil\r\nHost: ip-api.com\r\n\r\n"

#define YAHOOFORECAST "GET /v1/public/yql?q=select%20item.forecast%20from%20weather.forecast%20where%20u%3D%22c%22%20and%20woeid%20in%20(select%20woeid%20from%20geo.places(1)%20where%20text%3D%22                                    HTTP/1.1\r\nUser-Agent: Keil\r\nHost:query.yahooapis.com\r\nAccept: */*\r\n\r\n"

#define SONGLIST "GET /list HTTP/1.1\r\nUser-Agent: Keil\r\nHost:ee445l-shz96.appspot.com\r\nAccept: */*\r\n\r\n"

#define GETSONG "GET /song?id=         HTTP/1.1\r\nUser-Agent: Keil\r\nHost:ee445l-shz96.appspot.com\r\nAccept: */*\r\n\r\n"


static int32_t retVal;  
static SlSecParams_t secParams;
static char *pConfig = NULL; 
static INT32 ASize = 0; 
static SlSockAddrIn_t  Addr;

//connectWifi
//connects to wifi and maintains connect
//input: none
//output: none
void connectWifi(void) {
	retVal = configureSimpleLinkToDefaultState(pConfig); // set policies
  if(retVal < 0)Crash(4000000);
  retVal = sl_Start(0, pConfig, 0);
  if((retVal < 0) || (ROLE_STA != retVal) ) Crash(8000000);
  secParams.Key = PASSKEY;
  secParams.KeyLen = strlen(PASSKEY);
  secParams.Type = SEC_TYPE; // OPEN, WPA, or WEP
  sl_WlanConnect(SSID_NAME, strlen(SSID_NAME), 0, &secParams, 0);
  while((0 == (g_Status&CONNECTED)) || (0 == (g_Status&IP_AQUIRED))){
    _SlNonOsMainLoopTask();
  }
}

//getWeather
//sets global variables that store weatherID and temperature
//also sets global time
//inputs: none
//outputs: none
void getWeather(void) {

		strcpy(HostName,"ip-api.com");
		retVal = sl_NetAppDnsGetHostByName(HostName,
             strlen(HostName),&DestinationIP, SL_AF_INET);
		if(retVal ==0) {
				Addr.sin_family = SL_AF_INET;
				Addr.sin_port = sl_Htons(80);
				Addr.sin_addr.s_addr = sl_Htonl(DestinationIP);// IP to big endian 
				ASize = sizeof(SlSockAddrIn_t);
				SockID = sl_Socket(SL_AF_INET,SL_SOCK_STREAM, 0);
				if( SockID >= 0 ){
					retVal = sl_Connect(SockID, ( SlSockAddr_t *)&Addr, ASize);
				}
				if((SockID >= 0)&&(retVal >= 0)){
					strcpy(SendBuff,LOCREQ); 
					sl_Send(SockID, SendBuff, strlen(SendBuff), 0);// Send the HTTP GET 
					sl_Recv(SockID, Recvbuff, MAX_RECV_BUFF_SIZE, 0);// Receive response 
					sl_Close(SockID);
					
					strcpy(SendBuff,REQUEST);

					//put adc value into SendBuff
					char *qPtr;
					qPtr = strstr(SendBuff, "?q=");
					qPtr += 3;
					
					char *cityPtr;
					cityPtr = strstr(Recvbuff, "city\":\"");
					cityPtr += 7;
					while(*cityPtr != '\"') {
						*qPtr = *cityPtr;
						qPtr++;
						cityPtr++;
					}
					
					if(!SIUnits) {
						qPtr = strstr(SendBuff, "units=");
						qPtr += 6;
						*qPtr = 'i'; qPtr++;
						*qPtr = 'm'; qPtr++;
						*qPtr = 'p'; qPtr++;
						*qPtr = 'e'; qPtr++;
						*qPtr = 'r'; qPtr++;
						*qPtr = 'i'; qPtr++;
						*qPtr = 'a'; qPtr++;
						*qPtr = 'l'; qPtr++;
						
					}
			}
				
    }		
	
	
	//get weather
	strcpy(HostName,"api.openweathermap.org"); // works 9/2016
  retVal = sl_NetAppDnsGetHostByName(HostName,
             strlen(HostName),&DestinationIP, SL_AF_INET);
  if(retVal == 0){
    Addr.sin_family = SL_AF_INET;
    Addr.sin_port = sl_Htons(80);
    Addr.sin_addr.s_addr = sl_Htonl(DestinationIP);// IP to big endian 
    ASize = sizeof(SlSockAddrIn_t);
    SockID = sl_Socket(SL_AF_INET,SL_SOCK_STREAM, 0);
    if( SockID >= 0 ){
      retVal = sl_Connect(SockID, ( SlSockAddr_t *)&Addr, ASize);
    }
		if((SockID >= 0)&&(retVal >= 0)){
      sl_Send(SockID, SendBuff, strlen(SendBuff), 0);// Send the HTTP GET 
      sl_Recv(SockID, Recvbuff, MAX_RECV_BUFF_SIZE, 0);// Receive response 
      sl_Close(SockID);
			
			setTime(Recvbuff); //set global time
			setTempString(Recvbuff);
			weatherID = getID(Recvbuff);
		}
	}
}

//getWeatherForecast
//gets weather from yahoo api
//input: none
//output: none
void getWeatherForecast(void) {
	//get 4 day weather forecast
	strcpy(HostName,"ip-api.com");
		retVal = sl_NetAppDnsGetHostByName(HostName,
             strlen(HostName),&DestinationIP, SL_AF_INET);
		if(retVal ==0) {
				Addr.sin_family = SL_AF_INET;
				Addr.sin_port = sl_Htons(80);
				Addr.sin_addr.s_addr = sl_Htonl(DestinationIP);// IP to big endian 
				ASize = sizeof(SlSockAddrIn_t);
				SockID = sl_Socket(SL_AF_INET,SL_SOCK_STREAM, 0);
				if( SockID >= 0 ){
					retVal = sl_Connect(SockID, ( SlSockAddr_t *)&Addr, ASize);
				}
				if((SockID >= 0)&&(retVal >= 0)){
					strcpy(SendBuff,LOCREQ); 
					sl_Send(SockID, SendBuff, strlen(SendBuff), 0);// Send the HTTP GET 
					sl_Recv(SockID, Recvbuff, MAX_RECV_BUFF_SIZE, 0);// Receive response 
					sl_Close(SockID);
					
					strcpy(SendBuff,YAHOOFORECAST);
					

					char *qPtr = strstr(SendBuff, "text%3D%22");
					qPtr += 10;
					
					char *cityPtr = strstr(Recvbuff, "city\":\"");
					cityPtr += 7;
					while(*cityPtr != '\"') {
						if(*cityPtr == ' ') {
							*qPtr = '%';
							qPtr++;
							*qPtr = '2';
							qPtr++;
							*qPtr = '0';
							qPtr++;
							cityPtr++;
						}
						else {
							*qPtr = *cityPtr;
							qPtr++;
							cityPtr++;
						}
					}
					
					char *tail = "%22)&format=json";
					while(*tail != '\0') {
						*qPtr = *tail;
						qPtr++;
						tail++;
					}
									
					
					if(!SIUnits) {
						qPtr = strstr(SendBuff, "u%3D%22");
						qPtr += 7;
						*qPtr = 'f';
					}
					}
					

	
			}
			
				
			strcpy(HostName,"query.yahooapis.com");
		retVal = sl_NetAppDnsGetHostByName(HostName,
             strlen(HostName),&DestinationIP, SL_AF_INET);
		if(retVal ==0) {
				Addr.sin_family = SL_AF_INET;
				Addr.sin_port = sl_Htons(80);
				Addr.sin_addr.s_addr = sl_Htonl(DestinationIP);// IP to big endian 
				ASize = sizeof(SlSockAddrIn_t);
				SockID = sl_Socket(SL_AF_INET,SL_SOCK_STREAM, 0);
				if( SockID >= 0 ){
					retVal = sl_Connect(SockID, ( SlSockAddr_t *)&Addr, ASize);
				}
				if((SockID >= 0)&&(retVal >= 0)){
					//strcpy(SendBuff,YAHOOFORECAST);
					sl_Send(SockID, SendBuff, strlen(SendBuff), 0);// Send the HTTP GET 
					sl_Recv(SockID, Recvbuff, MAX_RECV_BUFF_SIZE, 0);// Receive response 
					sl_Close(SockID);

					
					char* json = strstr(Recvbuff, "forecast\"");
					json = strstr(Recvbuff, "day\":\"");
					json+= 6;
										
					//displayWeatherScreen();
					
					
					int forecastCt = 0;
					
					//ST7735_SetCursor(0,2);
					for(int i=0; i<5;i++) {
						while(*json != '\"') {
							forecast[forecastCt] = *json;
							forecastCt++;
							//ST7735_OutChar(*json);
							json++;
							
						}
						json +=10;
						
						forecast[forecastCt] = ' ';
						forecastCt++;
						forecast[forecastCt] = 'h';
						forecastCt++;
						forecast[forecastCt] = 'i';
						forecastCt++;
						forecast[forecastCt] = ':';
						forecastCt++;
						
						//ST7735_OutString(" hi:");
						
						while(*json != '\"') {
							forecast[forecastCt] = *json;
							forecastCt++;
							//ST7735_OutChar(*json);
							json++;
						}
						json +=9;
						
						if(SIUnits) {
							forecast[forecastCt] = 'C';
							forecastCt++;
							//ST7735_OutChar('C');
						}
						else {
							forecast[forecastCt] = 'F';
							forecastCt++;
							//ST7735_OutChar('F');
						}

						forecast[forecastCt] = ' ';
						forecastCt++;
						forecast[forecastCt] = 'l';
						forecastCt++;
						forecast[forecastCt] = 'o';
						forecastCt++;
						forecast[forecastCt] = ':';
						forecastCt++;
						//ST7735_OutString(" lo:");

						while(*json != '\"') {
							forecast[forecastCt] = *json;
							forecastCt++;
							//ST7735_OutChar(*json);
							json++;
						}
						
						if(SIUnits) {
							forecast[forecastCt] = 'C';
							forecastCt++;
							//ST7735_OutChar('C');
						}
						else {
							forecast[forecastCt] = 'F';
							forecastCt++;
							//ST7735_OutChar('F');
						}
						json +=10;
						
						forecast[forecastCt] = '\n';
						forecastCt++;
						forecast[forecastCt] = ' ';
						forecastCt++;
						forecast[forecastCt] = 'c';
						forecastCt++;
						forecast[forecastCt] = 'o';
						forecastCt++;
						forecast[forecastCt] = 'n';
						forecastCt++;
						forecast[forecastCt] = 'd';
						forecastCt++;
						forecast[forecastCt] = ':';
						forecastCt++;
						//ST7735_OutString("\n cond:");

						while(*json != '\"') {
							forecast[forecastCt] = *json;
							forecastCt++;
							//ST7735_OutChar(*json);
							json++;
						}
						
						forecast[forecastCt] = '\n';
						forecastCt++;
						//ST7735_OutString("\n");
						
						json = strstr(json, "day\":\"");
						json+= 6;
					}
					forecast[forecastCt] = '\0';
				}
			}
		}

//getSongList
//gets list of songs and puts them into global buffer for display at later time
//input: none
//output: none
void getSongList(void) {

		strcpy(HostName,"ee445l-shz96.appspot.com");
		retVal = sl_NetAppDnsGetHostByName(HostName,
             strlen(HostName),&DestinationIP, SL_AF_INET);
		if(retVal ==0) {
				Addr.sin_family = SL_AF_INET;
				Addr.sin_port = sl_Htons(80);
				Addr.sin_addr.s_addr = sl_Htonl(DestinationIP);// IP to big endian 
				ASize = sizeof(SlSockAddrIn_t);
				SockID = sl_Socket(SL_AF_INET,SL_SOCK_STREAM, 0);
				if( SockID >= 0 ){
					retVal = sl_Connect(SockID, ( SlSockAddr_t *)&Addr, ASize);
				}
				if((SockID >= 0)&&(retVal >= 0)){
					strcpy(SendBuff,SONGLIST); 
					sl_Send(SockID, SendBuff, strlen(SendBuff), 0);// Send the HTTP GET 
					sl_Recv(SockID, Recvbuff, MAX_RECV_BUFF_SIZE, 0);// Receive response 
					sl_Close(SockID);
					
					
					//parse through song from received response
					char *songPtr;
					char songName[14];
					uint8_t id;
					songPtr = strstr(Recvbuff, "\"ID = ");
					
					for(int i = 0; i < 4; i++) {
						songPtr += 6;
						id = (*songPtr)-'0'-1;
						
						songPtr += 10;
						int j = 0;
						while(*songPtr != '\"') {
								songName[j] = *songPtr;
								songPtr++;
								j++;
						}
						songName[j] = '\0';
						
						strcpy(songList[id],songName);
						
						songPtr = strstr(songPtr, "\"ID = ");
					}
				}
					
			}		
		
}	


//getSong
//gets song from global ID variable from personal server
//input: none
//output: none
void getSong(void) {
	
		//put song ID in
		strcpy(SendBuff,GETSONG);
		char *songPtr = strstr(SendBuff, "id=");
		songPtr += 3;
		*songPtr = songID + '0';

		strcpy(HostName,"ee445l-shz96.appspot.com");
		retVal = sl_NetAppDnsGetHostByName(HostName,
             strlen(HostName),&DestinationIP, SL_AF_INET);
		if(retVal ==0) {
				Addr.sin_family = SL_AF_INET;
				Addr.sin_port = sl_Htons(80);
				Addr.sin_addr.s_addr = sl_Htonl(DestinationIP);// IP to big endian 
				ASize = sizeof(SlSockAddrIn_t);
				SockID = sl_Socket(SL_AF_INET,SL_SOCK_STREAM, 0);
				if( SockID >= 0 ){
					retVal = sl_Connect(SockID, ( SlSockAddr_t *)&Addr, ASize);
				}
				if((SockID >= 0)&&(retVal >= 0)){
					sl_Send(SockID, SendBuff, strlen(SendBuff), 0);// Send the HTTP GET 
					sl_Recv(SockID, Recvbuff, MAX_RECV_BUFF_SIZE, 0);// Receive response 
					sl_Close(SockID);
					
					int note_Num = 1;
					
					char noteString[5];
					int noteStringCounter = 0;
					
					songPtr = strstr(Recvbuff, "{\"0:4,");
					songPtr+= 6;
					Note note = {0, 4};
					song_Buffer[0] = note;
					
					//process song buffer from server (assumes well formed)
					while(*songPtr != 'X') {
						
						noteStringCounter = 0;
						while(*songPtr != ':') {
							noteString[noteStringCounter] = *songPtr;
							noteStringCounter++;
							songPtr++;
						}
						noteString[noteStringCounter] = '\0';
						songPtr++;
						
						int charCounter = 0;
						while(*songPtr != ',') {
							charCounter++;
							songPtr++;
						}
						if(charCounter != 1) {
							Note note = {getNoteFreqFromString(noteString), 16 };
							song_Buffer[note_Num] = note;
						}
						else {
							Note note = {getNoteFreqFromString(noteString), (*(songPtr-1) - '0') };
							song_Buffer[note_Num] = note;
						}
						note_Num++;
						songPtr++;
					}
					global_Song = (Song){note_Num, song_Buffer};
				}
			}

}	
		
		
//displaytest
int main1(void) {
	LED_Init();
	ST7735_InitR(INITR_REDTAB);
	ST7735_SetCursor(0,0);
	displayAlarmBack();
}

int main(void){
	DisableInterrupts();
	initClk();        // PLL 50 MHz and enable FPU
	PLL_Init(Bus80MHz);
  LED_Init();       // initialize LaunchPad I/O
	DAC_Init(0x1000);
	SysTick_Init();
	Timer1_Init(&ProcessNote,F16HZ);
	ST7735_InitR(INITR_REDTAB);
	displayLoading();
	Switches_Init();
	connectWifi();
	ST7735_SetCursor(0,0);
	ST7735_OutString("\nWifi Connected...");
	getWeather();
	getSongList();
	//getSong();
	ST7735_OutString("\nHELLO!");
	interrupt_Flag = 1;
	current_State = 1;
	time_Interrupt = 1;
	ST7735_SetCursor(0,0);
	Timer2_Init();
	EnableInterrupts();
	
	while(1) {
		
		if(updateTime) {
			displayLoading();
			getWeather();
			updateTime = 0;
		}
		
		if(time_Interrupt) {
			if(current_State >= 1 && current_State <= 9) {
				displayDigits(global_Hour, global_Min);
			}
			else {
				displayJustTime();
			}
			time_Interrupt = 0;
		}
		if(interrupt_Flag) {
			if(alarm_Lock) {
				if(!playing) {	Start_Tone(); }
				switch(current_State) {
					case ALARMSNOOZE:
						displayAlarmLockSnooze("ALARM");
						break;
					case ALARMSTOP:
						displayAlarmLockStop("ALARM");
						break;
					case SNOOZESNOOZE:
						displayAlarmLockSnooze("SNOOZE");
						break;
					case SNOOZESTOP:
						displayAlarmLockStop("SNOOZE");
						break;
					case TIMERLOCK:
						displayTimerNotif();
						break;
				}
			}
			else {
				Stop_Tone();
				if(units_Changed) {
					displayLoading();
					getWeather();
					units_Changed = 0;
				}
				if(songChanged) {
					displayLoadingSong();
					getSong();
					songChanged = 0;
				}
				
				switch(current_State) {
					case MAINBLANK:
						displayMain("");
						break;
					case MAINWEATHER:
						displayMain("Weather");
						break;
					case MAINALARMS:
						displayMain("Alarms");
						break;
					case MAINSTOPWATCH:
						displayMain("Stopwatch");
						break;
					case MAINTIMER:
						displayMain("Timer");
						break;
					case MAINSETTINGS:
						displayMain("Settings");
						break;
					case SWTOG:
						displaySWSRS();
						break;
					case SWRESET:
						displaySWReset();
						break;
					case SWBACK:
						displaySWBack();
						break;
					case WEATHERLOAD:
						displayWeatherLoad();
						getWeatherForecast();
						displayWeatherScreen();
						current_State = WEATHERBACK;
						break;
					case WEATHERBACK:
						displayWeatherScreen();
						break;
					case TIMERSETH:
						displayTimerHourSelect();
						break;
					case TIMERSETM:
						displayTimerMinSelect();
						break;
					case TIMERSETS:
						displayTimerSecSelect();
						break;
					case TIMERHHOVER:
						displayTimerHour();
						break;
					case TIMERMHOVER:
						displayTimerMin();
						break;
					case TIMERSHOVER:
						displayTimerSec();
						break;
					case TIMERTOG:
						displayTimerEnable();
						break;
					case TIMERBACK:
						displayTimerBack();
						break;
					case TIMERRESET:
						displayTimerReset();
						break;
					case SETTINGS1:
						displaySettings(1);
						break;
					case SETTINGS2:
						displaySettings(2);
						break;
					case SETTINGS3:
						displaySettings(3);
						break;
					case SETTINGSBACK:
						displaySettings(-1);
						break;
					case SETTINGS1BACK:
						displayS1Songs(-1);
						break;
					case SETTINGS1SONG1:
						displayS1Songs(1);
						break;
					case SETTINGS1SONG2:
						displayS1Songs(2);
						break;
					case SETTINGS1SONG3:
						displayS1Songs(3);
						break;
					case SETTINGS1SONG4:
						displayS1Songs(4);
						break;
					case SETTINGS2UNITS:
						displayS2Units();
						break;
					case SETTINGS2BACK:
						displayS2Back();
						break;
					case SETTINGS3NIGHT:
						displayS3Night();
						break;
					case SETTINGS3BACK:
						displayS3Back();
						break;
					case ALARMSELECT1:
						displayAlarmSelect1();
						break;
					case ALARMSELECT2:
						displayAlarmSelect2();
						break;
					case ALARMSELECT1TOG:
						displayAlarmSelect1Tog();
						break;
					case ALARMSELECT2TOG:
						displayAlarmSelect2Tog();
						break;
					case ALARMBACK:
						displayAlarmBack();
						break;
					case A1HHOVER:
						displayAlarm1HourSelect();
						break;
					case A1MHOVER:
						displayAlarm1MinSelect();
						break;
					case A1H:
						displayAlarm1Hour();
						break;
					case A1M:
						displayAlarm1Min();
						break;
					case A1BACK:
						displayAlarm1Back();
						break;
					case A1ENABLE:
						displayAlarm1Enable();
						break;
					case A2HHOVER:
						displayAlarm2HourSelect();
						break;
					case A2MHOVER:
						displayAlarm2MinSelect();
						break;
					case A2H:
						displayAlarm2Hour();
						break;
					case A2M:
						displayAlarm2Min();
						break;
					case A2BACK:
						displayAlarm2Back();
						break;
					case A2ENABLE:
						displayAlarm2Enable();
						break;
					
					default:
						ST7735_SetCursor(0,0);
						ST7735_OutString("You should not be here");
				}
			}
			interrupt_Flag = 0;
		}		
		while(!interrupt_Flag && !time_Interrupt) {
			ST7735_SetCursor(0,0);
		}//wait for interrupt flag
		
		
	}
}

/*!
    \brief This function puts the device in its default state. It:
           - Set the mode to STATION
           - Configures connection policy to Auto and AutoSmartConfig
           - Deletes all the stored profiles
           - Enables DHCP
           - Disables Scan policy
           - Sets Tx power to maximum
           - Sets power policy to normal
           - Unregister mDNS services

    \param[in]      none

    \return         On success, zero is returned. On error, negative is returned
*/
static int32_t configureSimpleLinkToDefaultState(char *pConfig){
  SlVersionFull   ver = {0};
  UINT8           val = 1;
  UINT8           configOpt = 0;
  UINT8           configLen = 0;
  UINT8           power = 0;

  INT32           retVal = -1;
  INT32           mode = -1;

  mode = sl_Start(0, pConfig, 0);


    /* If the device is not in station-mode, try putting it in station-mode */
  if (ROLE_STA != mode){
    if (ROLE_AP == mode){
            /* If the device is in AP mode, we need to wait for this event before doing anything */
      while(!IS_IP_AQUIRED(g_Status));
    }

        /* Switch to STA role and restart */
    retVal = sl_WlanSetMode(ROLE_STA);

    retVal = sl_Stop(0xFF);

    retVal = sl_Start(0, pConfig, 0);

        /* Check if the device is in station again */
    if (ROLE_STA != retVal){
            /* We don't want to proceed if the device is not coming up in station-mode */
      return DEVICE_NOT_IN_STATION_MODE;
    }
  }
    /* Get the device's version-information */
  configOpt = SL_DEVICE_GENERAL_VERSION;
  configLen = sizeof(ver);
  retVal = sl_DevGet(SL_DEVICE_GENERAL_CONFIGURATION, &configOpt, &configLen, (unsigned char *)(&ver));

    /* Set connection policy to Auto + SmartConfig (Device's default connection policy) */
  retVal = sl_WlanPolicySet(SL_POLICY_CONNECTION, SL_CONNECTION_POLICY(1, 0, 0, 0, 1), NULL, 0);

    /* Remove all profiles */
  retVal = sl_WlanProfileDel(0xFF);

    /*
     * Device in station-mode. Disconnect previous connection if any
     * The function returns 0 if 'Disconnected done', negative number if already disconnected
     * Wait for 'disconnection' event if 0 is returned, Ignore other return-codes
     */
  retVal = sl_WlanDisconnect();
  if(0 == retVal){
        /* Wait */
     while(IS_CONNECTED(g_Status));
  }

    /* Enable DHCP client*/
  retVal = sl_NetCfgSet(SL_IPV4_STA_P2P_CL_DHCP_ENABLE,1,1,&val);

    /* Disable scan */
  configOpt = SL_SCAN_POLICY(0);
  retVal = sl_WlanPolicySet(SL_POLICY_SCAN , configOpt, NULL, 0);

    /* Set Tx power level for station mode
       Number between 0-15, as dB offset from max power - 0 will set maximum power */
  power = 0;
  retVal = sl_WlanSet(SL_WLAN_CFG_GENERAL_PARAM_ID, WLAN_GENERAL_PARAM_OPT_STA_TX_POWER, 1, (unsigned char *)&power);

    /* Set PM policy to normal */
  retVal = sl_WlanPolicySet(SL_POLICY_PM , SL_NORMAL_POLICY, NULL, 0);

    /* TBD - Unregister mDNS services */
  retVal = sl_NetAppMDNSUnRegisterService(0, 0);


  retVal = sl_Stop(0xFF);


  g_Status = 0;
  memset(&Recvbuff,0,MAX_RECV_BUFF_SIZE);
  memset(&SendBuff,0,MAX_SEND_BUFF_SIZE);
  memset(&HostName,0,MAX_HOSTNAME_SIZE);
  DestinationIP = 0;;
  SockID = 0;


  return retVal; /* Success */
}




/*
 * * ASYNCHRONOUS EVENT HANDLERS -- Start
 */

/*!
    \brief This function handles WLAN events

    \param[in]      pWlanEvent is the event passed to the handler

    \return         None

    \note

    \warning
*/
void SimpleLinkWlanEventHandler(SlWlanEvent_t *pWlanEvent){
  switch(pWlanEvent->Event){
    case SL_WLAN_CONNECT_EVENT:
    {
      SET_STATUS_BIT(g_Status, STATUS_BIT_CONNECTION);

            /*
             * Information about the connected AP (like name, MAC etc) will be
             * available in 'sl_protocol_wlanConnectAsyncResponse_t' - Applications
             * can use it if required
             *
             * sl_protocol_wlanConnectAsyncResponse_t *pEventData = NULL;
             * pEventData = &pWlanEvent->EventData.STAandP2PModeWlanConnected;
             *
             */
    }
    break;

    case SL_WLAN_DISCONNECT_EVENT:
    {
      sl_protocol_wlanConnectAsyncResponse_t*  pEventData = NULL;

      CLR_STATUS_BIT(g_Status, STATUS_BIT_CONNECTION);
      CLR_STATUS_BIT(g_Status, STATUS_BIT_IP_AQUIRED);

      pEventData = &pWlanEvent->EventData.STAandP2PModeDisconnected;

            /* If the user has initiated 'Disconnect' request, 'reason_code' is SL_USER_INITIATED_DISCONNECTION */
      if(SL_USER_INITIATED_DISCONNECTION == pEventData->reason_code){
        UARTprintf(" Device disconnected from the AP on application's request \r\n");
      }
      else{
        UARTprintf(" Device disconnected from the AP on an ERROR..!! \r\n");
      }
    }
    break;

    default:
    {
      UARTprintf(" [WLAN EVENT] Unexpected event \r\n");
    }
    break;
  }
}

/*!
    \brief This function handles events for IP address acquisition via DHCP
           indication

    \param[in]      pNetAppEvent is the event passed to the handler

    \return         None

    \note

    \warning
*/
void SimpleLinkNetAppEventHandler(SlNetAppEvent_t *pNetAppEvent){
  switch(pNetAppEvent->Event)
  {
    case SL_NETAPP_IPV4_ACQUIRED:
    {

      SET_STATUS_BIT(g_Status, STATUS_BIT_IP_AQUIRED);
        /*
             * Information about the connected AP's ip, gateway, DNS etc
             * will be available in 'SlIpV4AcquiredAsync_t' - Applications
             * can use it if required
             *
             * SlIpV4AcquiredAsync_t *pEventData = NULL;
             * pEventData = &pNetAppEvent->EventData.ipAcquiredV4;
             * <gateway_ip> = pEventData->gateway;
             *
             */

    }
    break;

    default:
    {
            UARTprintf(" [NETAPP EVENT] Unexpected event \r\n");
    }
    break;
  }
}

/*!
    \brief This function handles callback for the HTTP server events

    \param[in]      pServerEvent - Contains the relevant event information
    \param[in]      pServerResponse - Should be filled by the user with the
                    relevant response information

    \return         None

    \note

    \warning
*/
void SimpleLinkHttpServerCallback(SlHttpServerEvent_t *pHttpEvent,
                                  SlHttpServerResponse_t *pHttpResponse){
    /*
     * This application doesn't work with HTTP server - Hence these
     * events are not handled here
     */
  UARTprintf(" [HTTP EVENT] Unexpected event \r\n");
}

/*!
    \brief This function handles general error events indication

    \param[in]      pDevEvent is the event passed to the handler

    \return         None
*/
void SimpleLinkGeneralEventHandler(SlDeviceEvent_t *pDevEvent){
    /*
     * Most of the general errors are not FATAL are are to be handled
     * appropriately by the application
     */
  UARTprintf(" [GENERAL EVENT] \r\n");
}

/*!
    \brief This function handles socket events indication

    \param[in]      pSock is the event passed to the handler

    \return         None
*/
void SimpleLinkSockEventHandler(SlSockEvent_t *pSock){
  switch( pSock->Event )
  {
    case SL_NETAPP_SOCKET_TX_FAILED:
    {
            /*
            * TX Failed
            *
            * Information about the socket descriptor and status will be
            * available in 'SlSockEventData_t' - Applications can use it if
            * required
            *
            * SlSockEventData_t *pEventData = NULL;
            * pEventData = & pSock->EventData;
            */
      switch( pSock->EventData.status )
      {
        case SL_ECLOSE:
          UARTprintf(" [SOCK EVENT] Close socket operation failed to transmit all queued packets\r\n");
          break;


        default:
          UARTprintf(" [SOCK EVENT] Unexpected event \r\n");
          break;
      }
    }
    break;

    default:
      UARTprintf(" [SOCK EVENT] Unexpected event \r\n");
    break;
  }
}
/*
 * * ASYNCHRONOUS EVENT HANDLERS -- End
 */


