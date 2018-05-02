 /* ******** Display.c ************** 
 * Samuel Zhang, Zi Zhou Wang
 * 4/2/2018
 * images and data points
 */

#include <stdint.h>
#include "ST7735.h"
#include "string.h"
#include "Globals.h"
#include "main.h"



//*************drawHour********************************************
//  Draws four-pixel wide line on the ST7735 color LCD (Hour hand)
//  Inputs: (x1,y1) is the start point
//          (x2,y2) is the end point
// x1,x2 are horizontal positions, columns from the left edge
//               must be less than 128
//               0 is on the left, 126 is near the right
// y1,y2 are vertical positions, rows from the top edge
//               must be less than 160
//               159 is near the wires, 0 is the side opposite the wires
//        color 16-bit color, which can be produced by ST7735_Color565() 
// Output: none
void drawHour(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color) {
    float slope = 0;
    if (x2 == x1) { //if vertical line
        if(y2 >= y1) {
            for(uint16_t yPt = y1; yPt <= y2; yPt++) {
                ST7735_DrawPixel(x1, yPt, color);
                ST7735_DrawPixel(x1+1, yPt, color);
                ST7735_DrawPixel(x1, yPt+1, color);
                ST7735_DrawPixel(x1+1, yPt+1, color);
            }
        }
        else {
            for(uint16_t yPt = y2; yPt <= y1; yPt++) {
                ST7735_DrawPixel(x2, yPt, color);
                ST7735_DrawPixel(x2+1, yPt, color);
                ST7735_DrawPixel(x2, yPt+1, color);
                ST7735_DrawPixel(x2+1, yPt+1, color);
            }
        }
    }
    else if (y2 == y1) { //if horizontal line
        if(x2 >= x1) {
            for(uint16_t xPt = x1; xPt <= x2; xPt++) {
                ST7735_DrawPixel(xPt, y1, color);
                ST7735_DrawPixel(xPt+1, y1, color);
                ST7735_DrawPixel(xPt, y1+1, color);
                ST7735_DrawPixel(xPt+1, y1+1, color);
            }
        }
        else {
            for(uint16_t xPt = x2; xPt <= x1; xPt++) {
                ST7735_DrawPixel(xPt, y2, color);
                ST7735_DrawPixel(xPt+1, y2, color);
                ST7735_DrawPixel(xPt, y2+1, color);
                ST7735_DrawPixel(xPt+1, y2+1, color);
            }
        }
    }
    
    //all other cases have to do with relative positions of x1 and x2
    else if(x2 > x1 && y2 > y1) {
        if((x2 - x1) >= (y2 - y1)) {
            slope = ((float)y2-(float)y1)/((float)x2-(float)x1);    //calculate slope
            for(uint16_t xPt = x1; xPt <= x2; xPt++) {
                ST7735_DrawPixel(xPt, (y1 + ((slope)*((float)(xPt-x1)))),color);    //slope point formula
                ST7735_DrawPixel(xPt+1, (y1 + ((slope)*((float)(xPt-x1)))),color);
                ST7735_DrawPixel(xPt, (y1 + ((slope)*((float)(xPt-x1))))+1,color);
                ST7735_DrawPixel(xPt+1, (y1 + ((slope)*((float)(xPt-x1))))+1,color);
            }
        }
        else {
            slope = ((float)x2-(float)x1)/((float)y2-(float)y1);
            for(uint16_t yPt = y1; yPt <= y2; yPt++) {
                ST7735_DrawPixel(x1 + ((slope)*((float)(yPt-y1))), yPt, color);
                ST7735_DrawPixel(x1 + ((slope)*((float)(yPt-y1)))+1, yPt, color);
                ST7735_DrawPixel(x1 + ((slope)*((float)(yPt-y1))), yPt+1, color);
                ST7735_DrawPixel(x1 + ((slope)*((float)(yPt-y1)))+1, yPt+1, color);
            }
        }
    }
    else if (x2 > x1 && y1 > y2) {
        if((x2 - x1) >= (y1 - y2)) {
            slope = ((float)y2-(float)y1)/((float)x2-(float)x1);    //calculate slope
            for(uint16_t xPt = x1; xPt <= x2; xPt++) {
                ST7735_DrawPixel(xPt, (y1 + ((slope)*((float)(xPt-x1)))),color); //slope point formula
                ST7735_DrawPixel(xPt+1, (y1 + ((slope)*((float)(xPt-x1)))),color);
                ST7735_DrawPixel(xPt, (y1 + ((slope)*((float)(xPt-x1))))+1,color);
                ST7735_DrawPixel(xPt+1, (y1 + ((slope)*((float)(xPt-x1))))+1,color);
            }
        }
        else {
            slope = ((float)x1-(float)x2)/((float)y1-(float)y2);
            for(uint16_t yPt = y2; yPt <= y1; yPt++) {
                ST7735_DrawPixel(x2 + ((slope)*((float)(yPt-y2))), yPt, color);
                ST7735_DrawPixel(x2 + ((slope)*((float)(yPt-y2)))+1, yPt, color);
                ST7735_DrawPixel(x2 + ((slope)*((float)(yPt-y2))), yPt+1, color);
                ST7735_DrawPixel(x2 + ((slope)*((float)(yPt-y2)))+1, yPt+1, color);            
            }
        }
    }
    else if (x1 > x2 && y2 > y1) {
        if((x1 - x2) >= (y2 - y1)) {
            slope = ((float)y1-(float)y2)/((float)x1-(float)x2);    //calculate slope
            for(uint16_t xPt = x2; xPt <= x1; xPt++) {
                ST7735_DrawPixel(xPt, (y2 + ((slope)*((float)(xPt-x2)))),color); //slope point formula
                ST7735_DrawPixel(xPt+1, (y2 +((slope)*((float)(xPt-x2)))),color);
                ST7735_DrawPixel(xPt, (y2 + ((slope)*((float)(xPt-x2))))+1,color);
                ST7735_DrawPixel(xPt+1, (y2 + ((slope)*((float)(xPt-x2))))+1,color);
            }
        }
        else {
            slope = ((float)x2-(float)x1)/((float)y2-(float)y1);
            for(uint16_t yPt = y1; yPt <= y2; yPt++) {
                ST7735_DrawPixel(x1 + ((slope)*((float)(yPt-y1))), yPt, color);
                ST7735_DrawPixel(x1 + ((slope)*((float)(yPt-y1)))+1, yPt, color);
                ST7735_DrawPixel(x1 + ((slope)*((float)(yPt-y1))), yPt+1, color);
                ST7735_DrawPixel(x1 + ((slope)*((float)(yPt-y1)))+1, yPt+1, color);
            }
        }
    }
    else if (x1 > x2 && y1 > y2){
        if((x1 - x2) >= (y1 - y2)) {    
            slope = ((float)y1-(float)y2)/((float)x1-(float)x2);    //calculate slope
            for(uint16_t xPt = x2; xPt <= x1; xPt++) {
                ST7735_DrawPixel(xPt, (y2 + (uint16_t)((slope)*((float)(xPt-x2)))),color); //slope point formula
                ST7735_DrawPixel(xPt+1, (y2 + (uint16_t)((slope)*((float)(xPt-x2)))),color);
                ST7735_DrawPixel(xPt, (y2 + (uint16_t)((slope)*((float)(xPt-x2))))+1,color);
                ST7735_DrawPixel(xPt+1, (y2 + (uint16_t)((slope)*((float)(xPt-x2))))+1,color);
            }
        }
        else {
            slope = ((float)x1-(float)x2)/((float)y1-(float)y2);
            for(uint16_t yPt = y2; yPt <= y1; yPt++) {
                ST7735_DrawPixel(x2 + ((slope)*((float)(yPt-y2))), yPt, color);
                ST7735_DrawPixel(x2 + ((slope)*((float)(yPt-y2)))+1, yPt, color);
                ST7735_DrawPixel(x2 + ((slope)*((float)(yPt-y2))), yPt+1, color);
                ST7735_DrawPixel(x2 + ((slope)*((float)(yPt-y2)))+1, yPt+1, color);            
            }
        }
    }

}


//*************displayLoading********************************************
// display loading
// Input: none
//  Output: none
void displayLoading(void) {
	
	ST7735_SetCursor(0,0);
	if(dayTime && !forceNight){ 
		ST7735_FillScreen(ST7735_WHITE);
		ST7735_SetTextColor(ST7735_BLACK);
	}
	else {
		ST7735_FillScreen(ST7735_BLACK);
		ST7735_SetTextColor(ST7735_WHITE);
	}
		ST7735_OutString("LOADING...");
	
}

//*************displayLoadingSong********************************************
// display loading
// Input: none
//  Output: none
void displayLoadingSong(void) {
	
	ST7735_SetCursor(0,0);
	if(dayTime && !forceNight){ 
		ST7735_FillScreen(ST7735_WHITE);
		ST7735_SetTextColor(ST7735_BLACK);
	}
	else {
		ST7735_FillScreen(ST7735_BLACK);
		ST7735_SetTextColor(ST7735_WHITE);
	}
		ST7735_OutString("FETCHING SONG...");
	
}

//displayTime
//output time in top left relative to night/day mode
//input: none
//output: none
void displayTime(void) {
	ST7735_SetCursor(0,0);
	if(dayTime && !forceNight) {
		ST7735_FillScreen(ST7735_WHITE);
		ST7735_SetTextColor(ST7735_BLACK);
	}
	else {
		
		ST7735_FillScreen(ST7735_BLACK);
		ST7735_SetTextColor(ST7735_WHITE);
	}
	if(global_Hour <10) {
		ST7735_OutUDec(0);
	}
	ST7735_OutUDec(global_Hour);
	ST7735_OutChar(':');
	if(global_Min<10) {
		ST7735_OutUDec(0);
	}
	ST7735_OutUDec(global_Min);
}

//displayJustTime
//output time in top left relative to night/day mode
//input: none
//output: none
void displayJustTime(void) {
	ST7735_SetCursor(0,0);
	if(dayTime && !forceNight) {
		ST7735_SetTextColor(ST7735_BLACK);
	}
	else {
		ST7735_SetTextColor(ST7735_WHITE);
	}
	if(global_Hour <10) {
		ST7735_OutUDec(0);
	}
	ST7735_OutUDec(global_Hour);
	ST7735_OutChar(':');
	if(global_Min<10) {
		ST7735_OutUDec(0);
	}
	ST7735_OutUDec(global_Min);
}

/****** displayWeatherIcon ***********
 * Display weather icon from corresponding weather code
 * Inputs: an integer from the weather api that corresponds to weather status
 * Output: None
 */
void displayWeatherIcon(int icon){
	if((icon/100) == 2){
		if(dayTime && !forceNight){
			ST7735_DrawBitmap(112, 15, icon38B, 16, 16);
		}
		else{
			ST7735_DrawBitmap(112, 15, icon38W, 16, 16);
		}
	}
	else if((icon/100) == 3){
		if(dayTime && !forceNight){
			ST7735_DrawBitmap(112, 15, icon8B, 16, 16);
		}
		else{
			ST7735_DrawBitmap(112, 15, icon8W, 16, 16);
		}
	}
	else if((icon/100) == 5){
		if(dayTime && !forceNight){
			ST7735_DrawBitmap(112, 15, icon12B, 16, 16);
		}
		else{
			ST7735_DrawBitmap(112, 15, icon12W, 16, 16);
		}
	}
	else if((icon/100) == 6){
		if(dayTime && !forceNight){
			ST7735_DrawBitmap(112, 15, icon7B, 16, 16);
		}
		else{
			ST7735_DrawBitmap(112, 15, icon7W, 16, 16);
		}
	}
	else if((icon/100) == 7){
		if(dayTime && !forceNight){
			ST7735_DrawBitmap(112, 15, icon20B, 16, 16);
		}
		else{
			ST7735_DrawBitmap(112, 15, icon20W, 16, 16);
		}
	}
	else if(icon == 800){
		if(dayTime){
			if(!forceNight) {
				ST7735_DrawBitmap(112, 15, icon32B, 16, 16);
			}
			else {
				ST7735_DrawBitmap(112, 15, icon32W, 16, 16);
			}
		}
		else{
			ST7735_DrawBitmap(112, 15, icon31W, 16, 16);
		}
	}
	else if((icon/100) == 8){
		if(dayTime){
			if(!forceNight) {
				ST7735_DrawBitmap(112, 15, icon28B, 16, 16);
			}
			else {
				ST7735_DrawBitmap(112, 15, icon28W, 16, 16);
			}
		}
		else{
			ST7735_DrawBitmap(112, 15, icon27W, 16, 16);
		}
	}
	else if(icon == 900){
		if(dayTime && !forceNight){
			ST7735_DrawBitmap(112, 15, icon0B, 16, 16);
		}
		else{
			ST7735_DrawBitmap(112, 15, icon0W, 16, 16);
		}
	}
	else if(icon == 901 || icon == 902){
		if(dayTime && !forceNight){
			ST7735_DrawBitmap(112, 15, icon1B, 16, 16);
		}
		else{
			ST7735_DrawBitmap(112, 15, icon1W, 16, 16);
		}
	}
	else if(icon == 903){
		if(dayTime && !forceNight){
			ST7735_DrawBitmap(112, 15, icon13B, 16, 16);
		}
		else{
			ST7735_DrawBitmap(112, 15, icon13W, 16, 16);
		}
	}
	else if(icon == 904){
		if(dayTime && !forceNight){
			ST7735_DrawBitmap(112, 15, icon25B, 16, 16);
		}
		else{
			ST7735_DrawBitmap(112, 15, icon25W, 16, 16);
		}
	}
	else if(icon == 905){
		if(dayTime && !forceNight){
			ST7735_DrawBitmap(112, 15, icon22B, 16, 16);
		}
		else{
			ST7735_DrawBitmap(112, 15, icon22W, 16, 16);
		}
	}
	else if(icon == 906){
		if(dayTime && !forceNight){
			ST7735_DrawBitmap(112, 15, icon19B, 16, 16);
		}
		else{
			ST7735_DrawBitmap(112, 15, icon19W, 16, 16);
		}
	}
	else if((icon/10) == 95){
		if(dayTime && !forceNight){
			ST7735_DrawBitmap(112, 15, icon23B, 16, 16);
		}
		else{
			ST7735_DrawBitmap(112, 15, icon23W, 16, 16);
		}
	}
	else if((icon/10) == 96){
		if(dayTime && !forceNight){
			ST7735_DrawBitmap(112, 15, icon1B, 16, 16);
		}
		else{
			ST7735_DrawBitmap(112, 15, icon1W, 16, 16);
		}
	}
	else{
		if(dayTime && !forceNight){
			ST7735_DrawBitmap(112, 15, naB, 16, 16);
		}
		else{
			ST7735_DrawBitmap(112, 15, naW, 16, 16);
		}
	}
}

//Draws digits to display time
/****displayDigits *********
* Print out digits
* Inputs: hour and minute numbers
* Outputs: None
*/
	void displayDigits(int hour, int min){
	if(dayTime && !forceNight) {
		if(hour < 10){
			ST7735_DrawBitmap(0, 105, zeroW, 25, 50);
		}
		else if(hour < 20){
			ST7735_DrawBitmap(0, 105, oneW, 25, 50);
		}
		else{
			ST7735_DrawBitmap(0, 105, twoW, 25, 50);
		}
		int digit = (hour % 10);
		if(digit == 0){
			ST7735_DrawBitmap(25, 105, zeroW, 25, 50);
		}
		else if(digit == 1){
			ST7735_DrawBitmap(25, 105, oneW, 25, 50);
		}
		else if(digit == 2){
			ST7735_DrawBitmap(25, 105, twoW, 25, 50);
		}
		else if(digit == 3){
			ST7735_DrawBitmap(25, 105, threeW, 25, 50);
		}
		else if(digit == 4){
			ST7735_DrawBitmap(25, 105, fourW, 25, 50);
		}
		else if(digit == 5){
			ST7735_DrawBitmap(25, 105, fiveW, 25, 50);
		}
		else if(digit == 6){
			ST7735_DrawBitmap(25, 105, sixW, 25, 50);
		}
		else if(digit == 7){
			ST7735_DrawBitmap(25, 105, sevenW, 25, 50);
		}
		else if(digit == 8){
			ST7735_DrawBitmap(25, 105, eightW, 25, 50);
		}
		else if(digit == 9){
			ST7735_DrawBitmap(25, 105, nineW, 25, 50);
		}
		ST7735_DrawBitmap(59, 105, colonW, 10, 50);
		if(min < 10){
			ST7735_DrawBitmap(77, 105, zeroW, 25, 50);
		}
		else if(min < 20){
			ST7735_DrawBitmap(77, 105, oneW, 25, 50);
		}
		else if(min < 30){
			ST7735_DrawBitmap(77, 105, twoW, 25, 50);
		}
		else if(min < 40){
			ST7735_DrawBitmap(77, 105, threeW, 25, 50);
		}
		else if(min < 50){
			ST7735_DrawBitmap(77, 105, fourW, 25, 50);
		}
		else{
			ST7735_DrawBitmap(77, 105, fiveW, 25, 50);
		}
		digit = (min % 10);
		if(digit == 0){
			ST7735_DrawBitmap(102, 105, zeroW, 25, 50);
		}
		else if(digit == 1){
			ST7735_DrawBitmap(102, 105, oneW, 25, 50);
		}
		else if(digit == 2){
			ST7735_DrawBitmap(102, 105, twoW, 25, 50);
		}
		else if(digit == 3){
			ST7735_DrawBitmap(102, 105, threeW, 25, 50);
		}
		else if(digit == 4){
			ST7735_DrawBitmap(102, 105, fourW, 25, 50);
		}
		else if(digit == 5){
			ST7735_DrawBitmap(102, 105, fiveW, 25, 50);
		}
		else if(digit == 6){
			ST7735_DrawBitmap(102, 105, sixW, 25, 50);
		}
		else if(digit == 7){
			ST7735_DrawBitmap(102, 105, sevenW, 25, 50);
		}
		else if(digit == 8){
			ST7735_DrawBitmap(102, 105, eightW, 25, 50);
		}
		else if(digit == 9){
			ST7735_DrawBitmap(102, 105, nineW , 25, 50);
		}
	}
	else {
		if(hour < 10){
			ST7735_DrawBitmap(0, 105, zero, 25, 50);
		}
		else if(hour < 20){
			ST7735_DrawBitmap(0, 105, one, 25, 50);
		}
		else{
			ST7735_DrawBitmap(0, 105, two, 25, 50);
		}
		int digit = (hour % 10);
		if(digit == 0){
			ST7735_DrawBitmap(25, 105, zero, 25, 50);
		}
		else if(digit == 1){
			ST7735_DrawBitmap(25, 105, one, 25, 50);
		}
		else if(digit == 2){
			ST7735_DrawBitmap(25, 105, two, 25, 50);
		}
		else if(digit == 3){
			ST7735_DrawBitmap(25, 105, three, 25, 50);
		}
		else if(digit == 4){
			ST7735_DrawBitmap(25, 105, four, 25, 50);
		}
		else if(digit == 5){
			ST7735_DrawBitmap(25, 105, five, 25, 50);
		}
		else if(digit == 6){
			ST7735_DrawBitmap(25, 105, six, 25, 50);
		}
		else if(digit == 7){
			ST7735_DrawBitmap(25, 105, seven, 25, 50);
		}
		else if(digit == 8){
			ST7735_DrawBitmap(25, 105, eight, 25, 50);
		}
		else if(digit == 9){
			ST7735_DrawBitmap(25, 105, nine, 25, 50);
		}
		ST7735_DrawBitmap(59, 105, colon, 10, 50);
		if(min < 10){
			ST7735_DrawBitmap(77, 105, zero, 25, 50);
		}
		else if(min < 20){
			ST7735_DrawBitmap(77, 105, one, 25, 50);
		}
		else if(min < 30){
			ST7735_DrawBitmap(77, 105, two, 25, 50);
		}
		else if(min < 40){
			ST7735_DrawBitmap(77, 105, three, 25, 50);
		}
		else if(min < 50){
			ST7735_DrawBitmap(77, 105, four, 25, 50);
		}
		else{
			ST7735_DrawBitmap(77, 105, five, 25, 50);
		}
		digit = (min % 10);
		if(digit == 0){
			ST7735_DrawBitmap(102, 105, zero, 25, 50);
		}
		else if(digit == 1){
			ST7735_DrawBitmap(102, 105, one, 25, 50);
		}
		else if(digit == 2){
			ST7735_DrawBitmap(102, 105, two, 25, 50);
		}
		else if(digit == 3){
			ST7735_DrawBitmap(102, 105, three, 25, 50);
		}
		else if(digit == 4){
			ST7735_DrawBitmap(102, 105, four, 25, 50);
		}
		else if(digit == 5){
			ST7735_DrawBitmap(102, 105, five, 25, 50);
		}
		else if(digit == 6){
			ST7735_DrawBitmap(102, 105, six, 25, 50);
		}
		else if(digit == 7){
			ST7735_DrawBitmap(102, 105, seven, 25, 50);
		}
		else if(digit == 8){
			ST7735_DrawBitmap(102, 105, eight, 25, 50);
		}
		else if(digit == 9){
			ST7735_DrawBitmap(102, 105, nine, 25, 50);
		}
	}

}

//*************displayMain********************************************
// Draws the Main Screen
// Input: string, the mode to display at bottom center of screen
//  Output: none
void displayMain(char* string) {
	if(dayTime && !forceNight) {
		ST7735_FillScreen(ST7735_WHITE);
		ST7735_SetTextColor(ST7735_BLACK);
		displayDigits(global_Hour, global_Min);
		displayWeatherIcon(weatherID);

		//display temperature
		int x = 0; //value for placing cursor
		if(tempString[7] == '-') { x=17; }
		else { x=18; }
		ST7735_DrawString(x,2,&(tempString[7]), ST7735_BLACK);
		if(SIUnits) {
			ST7735_DrawStringWhiteBg(20,2,"C", ST7735_BLACK);
		}
		else {
			ST7735_DrawStringWhiteBg(20,2,"F", ST7735_BLACK);	
		}

		//Prints "Home in top left
		//ST7735_DrawString(0, 0, "Home", ST7735_WHITE);
		
		//Prints mode in bottom center
		if(strlen(string) < 8){
			ST7735_DrawStringWhiteBg(7, 15, string, ST7735_BLACK);
		}
		else{
			ST7735_DrawStringWhiteBg(6, 15, string, ST7735_BLACK);
		}
	}
	else {
		ST7735_FillScreen(ST7735_BLACK);
		ST7735_SetTextColor(ST7735_WHITE);
		displayDigits(global_Hour, global_Min);
		displayWeatherIcon(weatherID);
		
		//display temperature
		if(tempString[7] == '-') { ST7735_SetCursor(17,2); }
		else { ST7735_SetCursor(18,2); }
		ST7735_OutString(&(tempString[7]));
		if(SIUnits) {
			ST7735_DrawString(20,2,"C", ST7735_WHITE);
		}
		else {
			ST7735_DrawString(20,2,"F", ST7735_WHITE);	
		}

		//Prints "Home in top left
		//ST7735_DrawString(0, 0, "Home", ST7735_WHITE);
		
		//Prints mode in bottom center
		if(strlen(string) < 8){
			ST7735_DrawString(7, 15, string, ST7735_WHITE);
		}
		else{
			ST7735_DrawString(6, 15, string, ST7735_WHITE);
		}
	}
}

//*************displaySW********************************************
//  Draws stopwatch screen
//  Inputs: none
//  Output: none
void displaySW(){
	//Prints time in top left
	displayTime();
	if(dayTime && !forceNight) {
		ST7735_DrawStringWhiteBg(11, 0, "Stop Watch", ST7735_BLACK);
		ST7735_DrawStringWhiteBg(17, 15, "Back", ST7735_BLACK);
		if(stopwatch_Enable) {
			ST7735_DrawStringWhiteBg(0, 14, "Stop ", ST7735_BLACK);
		}
		else {
			ST7735_DrawStringWhiteBg(0, 14, "Start", ST7735_BLACK);
		}
		
		ST7735_DrawStringWhiteBg(0, 15, "Reset", ST7735_BLACK);

	}
	else {
		ST7735_DrawString(11, 0, "Stop Watch", 0xFFFF);
		ST7735_DrawString(17, 15, "Back", 0xFFFF);
		if(stopwatch_Enable) {
			ST7735_DrawString(0, 14, "Stop ", 0xFFFF);
		}
		else {
			ST7735_DrawString(0, 14, "Start", 0xFFFF);
		}
		
		ST7735_DrawString(0, 15, "Reset", 0xFFFF);
	}
	//Print Stop Watch Time
	ST7735_SetCursor(7,6);
	if(stopwatch_Hour < 10){
		ST7735_OutUDec(0);
		ST7735_OutUDec(stopwatch_Hour);
	}
	else{
		ST7735_OutUDec(stopwatch_Hour);
	}
	ST7735_OutString(":");
	if(stopwatch_Min < 10){
		ST7735_OutUDec(0);
		ST7735_OutUDec(stopwatch_Min);
	}
	else{
		ST7735_OutUDec(stopwatch_Min);
	}
	ST7735_OutString(":");
	if(stopwatch_Sec < 10){
		ST7735_OutUDec(0);
		ST7735_OutUDec(stopwatch_Sec);
	}
	else{
		ST7735_OutUDec(stopwatch_Sec);
	}
}
	

//*************displaySWBack********************************************
//  Draws stopwatch back screen
//  Inputs: none
//  Output: none
void displaySWBack(){
	displaySW();
	
	ST7735_DrawString(17, 15, "Back", ST7735_YELLOW);
}

//*************displaySWSRS********************************************
//  Draws stopwatch start/stop screen
//  Inputs: none
//  Output: none
void displaySWSRS(){
	displaySW();
	if(stopwatch_Enable) {
		ST7735_DrawString(0, 14, "Stop ", ST7735_YELLOW);
	}
	else {
		ST7735_DrawString(0, 14, "Start", ST7735_YELLOW);
	}
}

//*************displaySWReset********************************************
//  Draws stopwatch reset screen
//  Inputs: none
//  Output: none
void displaySWReset(){
	displaySW();
	ST7735_DrawString(0, 15, "Reset", ST7735_YELLOW);
}

//*************displayWeatherLoad********************************************
//  Draws loading weather with back option
//  Inputs: none
//  Output: none
void displayWeatherLoad() {
	displayLoading();
	ST7735_DrawString(17, 15, "Back", ST7735_YELLOW);
}

//*************displayWeatherScreen********************************************
//  Displays weather information
//  Inputs: none
//  Output: none
void displayWeatherScreen() {
	displayTime();
	if(dayTime && !forceNight) {
		ST7735_DrawStringWhiteBg(13, 0, "Forecast", ST7735_BLACK);
		ST7735_SetCursor(0,2);
		ST7735_OutString(forecast);
		//ST7735_DrawStringWhiteBg(0,2,forecast, ST7735_BLACK);
	}
	else {
		ST7735_DrawString(13, 0, "Forecast", ST7735_WHITE);
		ST7735_SetCursor(0,2);
		ST7735_OutString(forecast);
		//ST7735_DrawString(0,2,forecast, ST7735_WHITE);
	}

	ST7735_DrawString(17, 15, "Back", ST7735_YELLOW);
}




//*************displayAlarmDefault********************************************
// Draws alarm screen
// Input: none
//  Output: none
void displayAlarmDefault() {
	displayTime();
	
	if(dayTime && !forceNight) {
		ST7735_DrawStringWhiteBg(16,0,"Alarms",ST7735_BLACK);
		ST7735_DrawStringWhiteBg(0,3,"Alarm 1",ST7735_BLACK);
		
		if(alarm1_Enable) {
			ST7735_DrawStringWhiteBg(19,3,"On ",ST7735_BLACK);
		}
		else {
			ST7735_DrawStringWhiteBg(18,3,"Off",ST7735_BLACK);
		}
		
		ST7735_DrawStringWhiteBg(0,6,"Alarm 2",ST7735_BLACK);
	
		if(alarm2_Enable) {
			ST7735_DrawStringWhiteBg(19,6,"On ",ST7735_BLACK);
		}
		else {
			ST7735_DrawStringWhiteBg(18,6,"Off",ST7735_BLACK);
		}

		ST7735_DrawStringWhiteBg(17,15,"Back",ST7735_BLACK);
	}
	else {
		ST7735_DrawString(16,0,"Alarms",ST7735_WHITE);
		ST7735_DrawString(0,3,"Alarm 1",ST7735_WHITE);
		
		if(alarm1_Enable) {
			ST7735_DrawString(19,3,"On ",ST7735_WHITE);
		}
		else {
			ST7735_DrawString(18,3,"Off",ST7735_WHITE);
		}
		
		ST7735_DrawString(0,6,"Alarm 2",ST7735_WHITE);
	
		if(alarm2_Enable) {
			ST7735_DrawString(19,6,"On ",ST7735_WHITE);
		}
		else {
			ST7735_DrawString(18,6,"Off",ST7735_WHITE);
		}

		ST7735_DrawString(17,15,"Back",ST7735_WHITE);
	}
	
	ST7735_SetCursor(10,3);
	if(alarm1_Hour <10) {
		ST7735_OutUDec(0);
	}
	ST7735_OutUDec(alarm1_Hour);
	ST7735_OutChar(':');
	if(alarm1_Min<10) {
		ST7735_OutUDec(0);
	}
	ST7735_OutUDec(alarm1_Min);
	
	
	
	ST7735_SetCursor(10,6);
	if(alarm2_Hour <10) {
		ST7735_OutUDec(0);
	}
	ST7735_OutUDec(alarm2_Hour);
	ST7735_OutChar(':');
	if(alarm2_Min<10) {
		ST7735_OutUDec(0);
	}
	ST7735_OutUDec(alarm2_Min);
}

//*************displayAlarmSelect1********************************************
// Select Alarm 1
// Input: none
//  Output: none
void displayAlarmSelect1() {
	displayAlarmDefault();
	ST7735_DrawString(0, 3, "Alarm 1", ST7735_YELLOW);
}

//*************displayAlarmSelect2********************************************
// Select Alarm 2
// Input: none
//  Output: none
void displayAlarmSelect2() {
	displayAlarmDefault();
	ST7735_DrawString(0, 6, "Alarm 2", ST7735_YELLOW);
}

//*************displayAlarmSelect1Tog********************************************
// Select Alarm 1 Toggle
// Input: none
//  Output: none
void displayAlarmSelect1Tog() {
	displayAlarmDefault();
		if(alarm1_Enable) {
		ST7735_DrawString(19,3,"On ",ST7735_YELLOW);
	}
	else {
		ST7735_DrawString(18,3,"Off",ST7735_YELLOW);
	}
}

//*************displayAlarmSelect1Tog********************************************
// Select Alarm 2 Toggle
// Input: none
//  Output: none
void displayAlarmSelect2Tog() {
	displayAlarmDefault();
		if(alarm2_Enable) {
		ST7735_DrawString(19,6,"On ",ST7735_YELLOW);
	}
	else {
		ST7735_DrawString(18,6,"Off",ST7735_YELLOW);
	}
}

//*************displayAlarmBack********************************************
// Select Alarm Back
// Input: none
//  Output: none
void displayAlarmBack() {
	displayAlarmDefault();
	ST7735_DrawString(17,15,"Back",ST7735_YELLOW);
}


//*************displayAlarm1Default********************************************
// Draws the alarm 1 default template
// Input: none
//  Output: none
void displayAlarm1Default() {
	ST7735_SetCursor(0,0);
	displayTime();
	
	if(dayTime && !forceNight) { 
		ST7735_DrawStringWhiteBg(14,0,"Alarm 1",ST7735_BLACK);
		if(alarm1_Enable) {
			ST7735_DrawStringWhiteBg(0,15,"Disable",ST7735_BLACK);
		}
		else {
			ST7735_DrawStringWhiteBg(0,15,"Enable ",ST7735_BLACK);
		}
		ST7735_DrawStringWhiteBg(17,15,"Back",ST7735_BLACK);
	}
	else {
		ST7735_DrawString(14,0,"Alarm 1",ST7735_WHITE);
		if(alarm1_Enable) {
			ST7735_DrawString(0,15,"Disable",ST7735_WHITE);
		}
		else {
			ST7735_DrawString(0,15,"Enable ",ST7735_WHITE);
		}
		ST7735_DrawString(17,15,"Back",ST7735_WHITE);
	}
	ST7735_SetCursor(8,6);
	if(alarm1_Hour <10) {
		ST7735_OutUDec(0);
	}
	ST7735_OutUDec(alarm1_Hour);
	ST7735_OutChar(':');
	if(alarm1_Min<10) {
		ST7735_OutUDec(0);
	}
	ST7735_OutUDec(alarm1_Min);
}

//*************displayAlarm1HourSelect********************************************
// Draws the alarm 1 hour select screen
// Input: none
//  Output: none
void displayAlarm1HourSelect() {
	displayAlarm1Default();
	
	
	
	//set hour to yellow
	ST7735_SetTextColor(ST7735_YELLOW);
	ST7735_SetCursor(8,6);
	if(alarm1_Hour <10) {
		ST7735_OutUDec(0);
	}
	ST7735_OutUDec(alarm1_Hour);
}

//*************displayAlarm1MinSelect********************************************
// Draws the alarm 1 min select screen
// Input: none
//  Output: none
void displayAlarm1MinSelect() {
	displayAlarm1Default();
	
	
	//set minute to yellow
	ST7735_SetTextColor(ST7735_YELLOW);
	ST7735_SetCursor(11,6);
	if(alarm1_Min <10) {
		ST7735_OutUDec(0);
	}
	ST7735_OutUDec(alarm1_Min);
}

//*************displayAlarm1Hour********************************************
// Draws the alarm 1 hour screen
// Input: none
//  Output: none
void displayAlarm1Hour() {
	displayAlarm1Default();
	
	
	//draw X indicating selected
	drawHour(49,50,55,55,ST7735_RED);
	drawHour(55,50,49,55,ST7735_RED);
	
	//set hour to yellow
	ST7735_SetTextColor(ST7735_YELLOW);
	ST7735_SetCursor(8,6);
	if(alarm1_Hour <10) {
		ST7735_OutUDec(0);
	}
	ST7735_OutUDec(alarm1_Hour);
	
}

//*************displayAlarm1Min********************************************
// Draws the alarm 1 min screen
// Input: none
//  Output: none
void displayAlarm1Min() {
	displayAlarm1Default();
	
	//draw X indicating selected
	drawHour(67,50,73,55,ST7735_RED);
	drawHour(73,50,67,55,ST7735_RED);

	
	//set minute to yellow
	ST7735_SetTextColor(ST7735_YELLOW);
	ST7735_SetCursor(11,6);
	if(alarm1_Min <10) {
		ST7735_OutUDec(0);
	}
	ST7735_OutUDec(alarm1_Min);
	
}

//*************displayAlarm1Enable********************************************
// Draws the alarm 1 enable screen
// Input: none
//  Output: none
void displayAlarm1Enable() {
	displayAlarm1Default();
	if(alarm1_Enable) {
		ST7735_DrawString(0,15,"Disable",ST7735_YELLOW);
	}
	else {
		ST7735_DrawString(0,15,"Enable ",ST7735_YELLOW);
	}
}

//*************displayAlarm1Back********************************************
// Draws the alarm 1 back screen
// Input: none
//  Output: none
void displayAlarm1Back() {
	displayAlarm1Default();
	ST7735_DrawString(17,15,"Back",ST7735_YELLOW);
}

//*************displayAlarm2Default********************************************
// Draws the alarm 2 default template
// Input: none
//  Output: none
void displayAlarm2Default() {
	displayTime();
	
	if(dayTime && !forceNight) {
		ST7735_DrawStringWhiteBg(14,0,"Alarm 2",ST7735_BLACK);
		if(alarm2_Enable) {
			ST7735_DrawStringWhiteBg(0,15,"Disable",ST7735_BLACK);
		}
		else {
			ST7735_DrawStringWhiteBg(0,15,"Enable ",ST7735_BLACK);
		}
		ST7735_DrawStringWhiteBg(17,15,"Back",ST7735_BLACK);
	}
	else {
		ST7735_DrawString(14,0,"Alarm 2",ST7735_WHITE);
		if(alarm2_Enable) {
			ST7735_DrawString(0,15,"Disable",ST7735_WHITE);
		}
		else {
			ST7735_DrawString(0,15,"Enable ",ST7735_WHITE);
		}
		ST7735_DrawString(17,15,"Back",ST7735_WHITE);
	}
	
	ST7735_SetCursor(8,6);
	if(alarm2_Hour <10) {
		ST7735_OutUDec(0);
	}
	ST7735_OutUDec(alarm2_Hour);
	ST7735_OutChar(':');
	if(alarm2_Min<10) {
		ST7735_OutUDec(0);
	}
	ST7735_OutUDec(alarm2_Min);
}

//*************displayAlarm2HourSelect********************************************
// Draws the alarm 2 hour select screen
// Input: none
//  Output: none
void displayAlarm2HourSelect() {
	displayAlarm2Default();
	
	
	
	//set hour to yellow
	ST7735_SetTextColor(ST7735_YELLOW);
	ST7735_SetCursor(8,6);
	if(alarm2_Hour <10) {
		ST7735_OutUDec(0);
	}
	ST7735_OutUDec(alarm2_Hour);
}

//*************displayAlarm2MinSelect********************************************
// Draws the alarm 2 min select screen
// Input: none
//  Output: none
void displayAlarm2MinSelect() {
	displayAlarm2Default();
	
	

	//set minute to yellow
	ST7735_SetTextColor(ST7735_YELLOW);
	ST7735_SetCursor(11,6);
	if(alarm2_Min <10) {
		ST7735_OutUDec(0);
	}
	ST7735_OutUDec(alarm2_Min);
}

//*************displayAlarm2Hour********************************************
// Draws the alarm 2 hour screen
// Input: none
//  Output: none
void displayAlarm2Hour() {
	displayAlarm2Default();
	
	//draw X indicating selected
	drawHour(49,50,55,55,ST7735_RED);
	drawHour(55,50,49,55,ST7735_RED);
	
	//set hour to yellow
	ST7735_SetTextColor(ST7735_YELLOW);
	ST7735_SetCursor(8,6);
	if(alarm2_Hour <10) {
		ST7735_OutUDec(0);
	}
	ST7735_OutUDec(alarm2_Hour);
	
}

//*************displayAlarm2Min********************************************
// Draws the alarm 2 min screen
// Input: none
//  Output: none
void displayAlarm2Min() {
	displayAlarm2Default();
	
	//draw X indicating selected
	drawHour(67,50,73,55,ST7735_RED);
	drawHour(73,50,67,55,ST7735_RED);
	
	//set minute to yellow
	ST7735_SetTextColor(ST7735_YELLOW);
	ST7735_SetCursor(11,6);
	if(alarm2_Min <10) {
		ST7735_OutUDec(0);
	}
	ST7735_OutUDec(alarm2_Min);
	
}

//*************displayAlarm2Enable********************************************
// Draws the alarm 2 enable screen
// Input: none
//  Output: none
void displayAlarm2Enable() {
	displayAlarm2Default();
	if(alarm2_Enable) {
		ST7735_DrawString(0,15,"Disable",ST7735_YELLOW);
	}
	else {
		ST7735_DrawString(0,15,"Enable ",ST7735_YELLOW);
	}
}

//*************displayAlarm2Back********************************************
// Draws the alarm 2 back screen
// Input: none
//  Output: none
void displayAlarm2Back() {
	displayAlarm2Default();
	ST7735_DrawString(17,15,"Back",ST7735_YELLOW);
}

//*************displaySettings********************************************
// display settings menu
// Input: integer for option, -1 is selecting back button
//  Output: none
void displaySettings(int i){
	displayTime();
	
	if(dayTime && !forceNight){
		ST7735_DrawStringWhiteBg(13, 0, "Settings", ST7735_BLACK);
		ST7735_DrawStringWhiteBg(0, 2, "1. Set Tone", ST7735_BLACK);
		ST7735_DrawStringWhiteBg(0, 3, "2. Weather Options", ST7735_BLACK);
		ST7735_DrawStringWhiteBg(0, 4, "3. Nightmode", ST7735_BLACK);
		ST7735_DrawStringWhiteBg(17,15,"Back", ST7735_BLACK);
	}
	else {
		ST7735_DrawString(13, 0, "Settings", ST7735_WHITE);
		ST7735_DrawString(0, 2, "1. Set Tone", ST7735_WHITE);
		ST7735_DrawString(0, 3, "2. Weather Options", ST7735_WHITE);
		ST7735_DrawString(0, 4, "3. Nightmode", ST7735_WHITE);
		ST7735_DrawString(17,15,"Back", ST7735_WHITE);
	}
	
	if(i == 1) { ST7735_DrawString(0, 2, "1. Set Tone", ST7735_YELLOW);	}
	else if(i == 2) { ST7735_DrawString(0, 3, "2. Weather Options", ST7735_YELLOW); }
	else if(i == 3) { ST7735_DrawString(0, 4, "3. Nightmode", ST7735_YELLOW); }
	else if(i == -1) { ST7735_DrawString(17,15,"Back",ST7735_YELLOW); }
}

//*************displayTimerDefault********************************************
// Draws the timer default template
// Input: none
//  Output: none
void displayTimerDefault() {

	displayTime();
	
	if(dayTime && !forceNight) {
		ST7735_DrawStringWhiteBg(16,0,"Timer",ST7735_BLACK);
		if(timer_Enable) {
			ST7735_DrawStringWhiteBg(0,14,"Stop ",ST7735_BLACK);
		}
		else {
			ST7735_DrawStringWhiteBg(0,14,"Start",ST7735_BLACK);
		}
		ST7735_DrawStringWhiteBg(0, 15, "Reset", ST7735_BLACK);
		ST7735_DrawStringWhiteBg(17,15,"Back",ST7735_BLACK);	
	}
	else {
		ST7735_DrawString(16,0,"Timer",ST7735_WHITE);
		if(timer_Enable) {
			ST7735_DrawString(0,14,"Stop ",ST7735_WHITE);
		}
		else {
			ST7735_DrawString(0,14,"Start",ST7735_WHITE);
		}
		ST7735_DrawString(0, 15, "Reset", ST7735_WHITE);
		ST7735_DrawString(17,15,"Back",ST7735_WHITE);
	}
	
	ST7735_SetCursor(7,6);
	if(timer_Hour <10) {
		ST7735_OutUDec(0);
	}
	ST7735_OutUDec(timer_Hour);
	ST7735_OutChar(':');
	if(timer_Min<10) {
		ST7735_OutUDec(0);
	}
	ST7735_OutUDec(timer_Min);
	ST7735_OutChar(':');
	if(timer_Sec<10) {
		ST7735_OutUDec(0);
	}
	ST7735_OutUDec(timer_Sec);
}

//*************displayTimerHourSelect********************************************
// Draws the timer hour select screen
// Input: none
//  Output: none
void displayTimerHourSelect() {
	displayTimerDefault();
	
	//draw X indicating selected
	//drawHour(49,50,55,55,ST7735_RED);
	//drawHour(55,50,49,55,ST7735_RED);
	drawHour(43,50,49,55,ST7735_RED);
	drawHour(49,50,43,55,ST7735_RED);
	
	//set hour to yellow
	ST7735_SetTextColor(ST7735_YELLOW);
	ST7735_SetCursor(7,6);
	if(timer_Hour <10) {
		ST7735_OutUDec(0);
	}
	ST7735_OutUDec(timer_Hour);
}


//*************displayTimerMinSelect********************************************
// Draws the timer min select screen
// Input: none
//  Output: none
void displayTimerMinSelect() {
	displayTimerDefault();
	
	//draw X indicating selected
	drawHour(61,50,67,55,ST7735_RED);
	drawHour(67,50,61,55,ST7735_RED);

	//set minute to yellow
	ST7735_SetTextColor(ST7735_YELLOW);
	ST7735_SetCursor(10,6);
	if(timer_Min <10) {
		ST7735_OutUDec(0);
	}
	ST7735_OutUDec(timer_Min);
}

//*************displayTimerSecSelect********************************************
// Draws the timer min select screen
// Input: none
//  Output: none
void displayTimerSecSelect() {
	displayTimerDefault();
	
	//draw X indicating selected
	drawHour(79,50,85,55,ST7735_RED);
	drawHour(85,50,79,55,ST7735_RED);

	//set minute to yellow
	ST7735_SetTextColor(ST7735_YELLOW);
	ST7735_SetCursor(13,6);
	if(timer_Sec <10) {
		ST7735_OutUDec(0);
	}
	ST7735_OutUDec(timer_Sec);
}

//*************displayTimerHour********************************************
// Draws the timer hour screen
// Input: none
//  Output: none
void displayTimerHour() {
	displayTimerDefault();
	
	//set hour to yellow
	ST7735_SetTextColor(ST7735_YELLOW);
	ST7735_SetCursor(7,6);
	if(timer_Hour <10) {
		ST7735_OutUDec(0);
	}
	ST7735_OutUDec(timer_Hour);
	
}

//*************displayTimerMin********************************************
// Draws the alarm 1 min screen
// Input: none
//  Output: none
void displayTimerMin() {
	displayTimerDefault();
	
	//set minute to yellow
	ST7735_SetTextColor(ST7735_YELLOW);
	ST7735_SetCursor(10,6);
	if(timer_Min <10) {
		ST7735_OutUDec(0);
	}
	ST7735_OutUDec(timer_Min);
	
}

//*************displayTimerSec********************************************
// Draws the alarm 1 min screen
// Input: none
//  Output: none
void displayTimerSec() {
	displayTimerDefault();
	
	//set minute to yellow
	ST7735_SetTextColor(ST7735_YELLOW);
	ST7735_SetCursor(13,6);
	if(timer_Sec <10) {
		ST7735_OutUDec(0);
	}
	ST7735_OutUDec(timer_Sec);
	
}

//*************displayTimerEnable********************************************
// Draws the timer enable screen
// Input: none
//  Output: none
void displayTimerEnable() {
	displayTimerDefault();
	if(timer_Enable) {
		ST7735_DrawString(0,14,"Stop ",ST7735_YELLOW);
	}
	else {
		ST7735_DrawString(0,14,"Start",ST7735_YELLOW);
	}
}

//*************displayTimerReset*******************************************
// Draws the timer enable screen
// Input: none
//  Output: none
void displayTimerReset() {
	displayTimerDefault();
	ST7735_DrawString(0,15,"Reset",ST7735_YELLOW);
}

//*************displayTimerBack********************************************
// Draws the timer back screen
// Input: none
//  Output: none
void displayTimerBack() {
	displayTimerDefault();
	ST7735_DrawString(17,15,"Back",ST7735_YELLOW);
}

//*************displayS2Units********************************************
// Draws weather settings units
// Input: none
//  Output: none
void displayS2Units() {
	displayTime();
	
	if(dayTime && !forceNight) {
		ST7735_DrawStringWhiteBg(7, 0, "Weather Options", ST7735_BLACK);
		ST7735_DrawStringWhiteBg(0,3, "Weather Units =", ST7735_BLACK);
		ST7735_DrawStringWhiteBg(17,15,"Back",ST7735_BLACK);		
	}
	else {
		ST7735_DrawString(7, 0, "Weather Options", ST7735_WHITE);
		ST7735_DrawString(0,3, "Weather Units =", ST7735_WHITE);
		ST7735_DrawString(17,15,"Back",ST7735_WHITE);
	}
	if(SIUnits) {
		ST7735_DrawString(16,3, "C", ST7735_YELLOW);
	}
	else {
		ST7735_DrawString(16,3, "F", ST7735_YELLOW);
	}
	
}

//*************displayS2back********************************************
// Draws weather settings back
// Input: none
//  Output: none
void displayS2Back() {
	displayTime();
	
	if(dayTime && !forceNight) {
		ST7735_DrawStringWhiteBg(7, 0, "Weather Options", ST7735_BLACK);
		ST7735_DrawStringWhiteBg(0,3, "Weather Units =", ST7735_BLACK);
		if(SIUnits) {
			ST7735_DrawStringWhiteBg(16,3, "C", ST7735_BLACK);
		}
		else {
			ST7735_DrawStringWhiteBg(16,3, "F", ST7735_BLACK);
		}
	}
	else {
		ST7735_DrawString(7, 0, "Weather Options", ST7735_WHITE);
		ST7735_DrawString(0,3, "Weather Units =", ST7735_WHITE);
		if(SIUnits) {
			ST7735_DrawString(16,3, "C", ST7735_WHITE);
		}
		else {
			ST7735_DrawString(16,3, "F", ST7735_WHITE);
		}

	}
	
	ST7735_DrawString(17,15,"Back",ST7735_YELLOW);
}


//*************displayS3Night********************************************
// Draws nightmode settings select
// Input: none
//  Output: none
void displayS3Night() {
	displayTime();
	
	if(dayTime && !forceNight) {
		ST7735_DrawStringWhiteBg(12, 0, "NightMode", ST7735_BLACK);
		ST7735_DrawStringWhiteBg(0,3, "Force NightMode =", ST7735_BLACK);
		ST7735_DrawStringWhiteBg(17,15,"Back",ST7735_BLACK);		
	}
	else {
		ST7735_DrawString(12, 0, "NightMode", ST7735_WHITE);
		ST7735_DrawString(0,3, "Force NightMode =", ST7735_WHITE);
		ST7735_DrawString(17,15,"Back",ST7735_WHITE);
	}
	if(forceNight) {
		ST7735_DrawString(18,3, "Y", ST7735_YELLOW);
	}
	else {
		ST7735_DrawString(18,3, "N", ST7735_YELLOW);
	}
	
}

//*************displayS3Back********************************************
// Draws nightmode settings back
// Input: none
//  Output: none
void displayS3Back() {
	displayTime();
	
	if(dayTime && !forceNight) {
		ST7735_DrawStringWhiteBg(12, 0, "NightMode", ST7735_BLACK);
		ST7735_DrawStringWhiteBg(0,3, "Force NightMode =", ST7735_BLACK);
		if(forceNight) {
			ST7735_DrawStringWhiteBg(18,3, "Y", ST7735_BLACK);
		}
		else {
			ST7735_DrawStringWhiteBg(18,3, "N", ST7735_BLACK);
		}
	}
	else {
		ST7735_DrawString(12, 0, "NightMode", ST7735_WHITE);
		ST7735_DrawString(0,3, "Force NightMode =", ST7735_WHITE);
		if(SIUnits) {
			ST7735_DrawString(18,3, "Y", ST7735_WHITE);
		}
		else {
			ST7735_DrawString(18,3, "N", ST7735_WHITE);
		}

	}
	ST7735_DrawString(17,15,"Back",ST7735_YELLOW);
}

//*************displayS3Back********************************************
// display song list with song ID selected
// Input: integer with song ID selected (-1 for back)
//  Output: none
void displayS1Songs(int i) {
	displayTime();
	

	
	
	
	if(dayTime && !forceNight) {
		ST7735_DrawStringWhiteBg(12, 0, "Song List", ST7735_BLACK);
		ST7735_DrawStringWhiteBg(0,3, "1. ", ST7735_BLACK);
		ST7735_DrawStringWhiteBg(3,3, songList[0], ST7735_BLACK);
		ST7735_DrawStringWhiteBg(0,4, "2. ", ST7735_BLACK);
		ST7735_DrawStringWhiteBg(3,4, songList[1], ST7735_BLACK);
		ST7735_DrawStringWhiteBg(0,5, "3. ", ST7735_BLACK);
		ST7735_DrawStringWhiteBg(3,5, songList[2], ST7735_BLACK);
		ST7735_DrawStringWhiteBg(0,6, "4. ", ST7735_BLACK);
		ST7735_DrawStringWhiteBg(3,6, songList[3], ST7735_BLACK);
		ST7735_DrawStringWhiteBg(17,15,"Back",ST7735_BLACK);
		
		if(songID == 1) {
			ST7735_DrawStringWhiteBg(3+strlen(songList[0]),3, " (Current)", ST7735_BLACK);
		}
		else if(songID == 2) {
			ST7735_DrawStringWhiteBg(3+strlen(songList[1]),4, " (Current)", ST7735_BLACK);
		}
		else if(songID == 3) {
			ST7735_DrawStringWhiteBg(3+strlen(songList[2]),5, " (Current)", ST7735_BLACK);
		}
		else if(songID == 4) {
			ST7735_DrawStringWhiteBg(3+strlen(songList[3]),6, " (Current)", ST7735_BLACK);
		}
		
		
		
	}
	else {
		ST7735_DrawString(12, 0, "NightMode", ST7735_WHITE);
		ST7735_DrawString(0,3, "1. ", ST7735_WHITE);
		ST7735_DrawString(3,3, songList[0], ST7735_WHITE);
		ST7735_DrawString(0,4, "2. ", ST7735_WHITE);
		ST7735_DrawString(3,4, songList[1], ST7735_WHITE);
		ST7735_DrawString(0,5, "3. ", ST7735_WHITE);
		ST7735_DrawString(3,5, songList[2], ST7735_WHITE);
		ST7735_DrawString(0,6, "4. ", ST7735_WHITE);
		ST7735_DrawString(3,6, songList[3], ST7735_WHITE);
		ST7735_DrawString(17,15,"Back",ST7735_WHITE);
		if(songID == 1) {
			ST7735_DrawString(3+strlen(songList[0]),3, " (Current)", ST7735_WHITE);
		}
		else if(songID == 2) {
			ST7735_DrawString(3+strlen(songList[1]),4, " (Current)", ST7735_WHITE);
		}
		else if(songID == 3) {
			ST7735_DrawString(3+strlen(songList[2]),5, " (Current)", ST7735_WHITE);
		}
		else if(songID == 4) {
			ST7735_DrawString(3+strlen(songList[3]),6, " (Current)", ST7735_WHITE);
		}
	}
	
	if(i == 1 || i == 0) {	
		ST7735_DrawString(0,3, "1. ", ST7735_YELLOW);
		ST7735_DrawString(3,3, songList[0], ST7735_YELLOW);
		if(songID == 1) {
			ST7735_DrawString(3+strlen(songList[0]),3, " (Current)", ST7735_YELLOW);
		}
	}
	else if (i == 2) { 
		ST7735_DrawString(0,4, "2. ", ST7735_YELLOW);
		ST7735_DrawString(3,4, songList[1], ST7735_YELLOW);
		if(songID == 2) {
			ST7735_DrawString(3+strlen(songList[1]),4, " (Current)", ST7735_YELLOW);
		}
	}
	else if (i == 3) { 
		ST7735_DrawString(0,5, "3. ", ST7735_YELLOW);
		ST7735_DrawString(3,5, songList[2], ST7735_YELLOW);
		if(songID == 3) {
			ST7735_DrawString(3+strlen(songList[2]),5, " (Current)", ST7735_YELLOW);
		}
	}
	else if (i == 4) { 
		ST7735_DrawString(0,6, "4. ", ST7735_YELLOW);
		ST7735_DrawString(3,6, songList[3], ST7735_YELLOW);
		if(songID == 4) {
			ST7735_DrawString(3+strlen(songList[3]),6, " (Current)", ST7735_YELLOW);
		}
	}
	else if (i == -1) {
		ST7735_DrawString(17,15,"Back",ST7735_YELLOW);	
	}
}

//*************displayAlarmLock********************************************
// Draws the alarm lock template
// Input: string to output for mode
//  Output: none
void displayAlarmLock(char* str){
	displayTime();
	if(dayTime && !forceNight) {
		ST7735_DrawStringWhiteBg(7, 6, str, ST7735_BLACK);
		ST7735_DrawStringWhiteBg(0, 15, "Snooze", ST7735_BLACK);
		ST7735_DrawStringWhiteBg(17, 15, "Stop", ST7735_BLACK);
	}
	else {
		ST7735_DrawString(7, 6, str, ST7735_WHITE);
		ST7735_DrawString(0, 15, "Snooze", ST7735_WHITE);
		ST7735_DrawString(17, 15, "Stop", ST7735_WHITE);
	}
}

//*************displayAlarmLockSnooze********************************************
// Draws the alarm lock snooze screen
// Input: string to output for mode
//  Output: none
void displayAlarmLockSnooze(char* str){
	displayAlarmLock(str);
	ST7735_DrawString(0, 15, "Snooze", ST7735_YELLOW);
}

//*************displayAlarmLockStop********************************************
// Draws the alarm lock stop screen
// Input: string to output for mode
//  Output: none
void displayAlarmLockStop(char* str){
	displayAlarmLock(str);
	ST7735_DrawString(17, 15, "Stop", ST7735_YELLOW);
}

//*************displayTimerNotif********************************************
// Draws the timer going off
// Input: none
//  Output: none
void displayTimerNotif(void) {
	displayTime();
	if(dayTime && !forceNight) {
		ST7735_DrawStringWhiteBg(7, 6, "TIMER", ST7735_BLACK);
		
	}
	else {
		ST7735_DrawString(7, 6, "TIMER", ST7735_WHITE);
	}
	
	ST7735_DrawString(8, 15, "Stop", ST7735_YELLOW);
}
