/************** ECE2049 DEMO CODE ******************/
/**************  13 March 2019   ******************/
/***************************************************/

#include <msp430.h>
#include "peripherals.h"

// Function Prototypes
void swDelay(char numLoops);

// Declare globals here
enum GAME_STATE {welcome = 0, songStart = 1, song = 2};

// Main
void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;

    // Useful code starts here
    initLeds();
    initButtons();

    configDisplay();
    configKeypad();

    unsigned char currKey = 0;
    unsigned int currButton = 0;
    unsigned int m;
    enum GAME_STATE state = welcome;


    while (1)    // Forever loop
    {
        switch(state){
        case welcome: // Start Screen
            // Write some text to the display
            Graphics_drawStringCentered(&g_sContext, "MSP430", AUTO_STRING_LENGTH, 48, 25, TRANSPARENT_TEXT);
            Graphics_drawStringCentered(&g_sContext, "HERO", AUTO_STRING_LENGTH, 48, 35, TRANSPARENT_TEXT);
            Graphics_drawStringCentered(&g_sContext, "Press *", AUTO_STRING_LENGTH, 48, 65, TRANSPARENT_TEXT);
            Graphics_drawStringCentered(&g_sContext, "to start", AUTO_STRING_LENGTH, 48, 75, TRANSPARENT_TEXT);

            // Refresh the display so it shows the new data
            Graphics_flushBuffer(&g_sContext);

            while(currKey != '*'){
                currKey = getKey();
            }
            if(currKey == '*'){
                Graphics_clearDisplay(&g_sContext); // Clear the display
                state = songStart;
            }
            break;

        case songStart:
            m = 20000;
            while(1){
                m--;
                if (m > 15000)
                    Graphics_drawStringCentered(&g_sContext, "GET READY", AUTO_STRING_LENGTH, 48, 25, TRANSPARENT_TEXT);
                else if ((m > 10000) && (m <= 15000)){
                    Graphics_drawStringCentered(&g_sContext, "3...", AUTO_STRING_LENGTH, 24, 35, TRANSPARENT_TEXT);
                    configUserLED('1'-'0');
                }
                else if ((m > 5000) && (m <= 15000)){
                    Graphics_drawStringCentered(&g_sContext, "2...", AUTO_STRING_LENGTH, 48, 35, TRANSPARENT_TEXT);
                    configUserLED('2'-'0');
                }
                else if ((m > 0) && (m <= 5000)){
                    Graphics_drawStringCentered(&g_sContext, "1...", AUTO_STRING_LENGTH, 72, 35, TRANSPARENT_TEXT);
                    configUserLED('3'-'0');
                }
                else if (m == 0){
                    Graphics_drawStringCentered(&g_sContext, "Go!", AUTO_STRING_LENGTH, 48, 15, TRANSPARENT_TEXT);
                    configUserLED('0');
                    return 0;

                }
            }
            Graphics_clearDisplay(&g_sContext);
            state = song;
            break;



        case song:
            currKey = getKey();
            currButton = readButtons() + '0';
            if(currButton != '0'){
                setLeds(currButton - 0x30);
            }
            else{
                setLeds(0);
            }
            configUserLED('1' - '0');

            if(currKey == '#'){
                Graphics_clearDisplay(&g_sContext);
                state = welcome;
            }

            break;

        }

    }  // end while (1)
}

//void heroButtons(void){
//    int currButton = readButtons();
//    currButton += '0';
//    if(currButton != '0'){
//        setLeds(currButton - 0x30);
//    }
//    else{
//        setLeds(0);
//    }
//}


void swDelay(char numLoops)
{
	// This function is a software delay. It performs
	// useless loops to waste a bit of time
	//
	// Input: numLoops = number of delay loops to execute
	// Output: none
	//
	// smj, ECE2049, 25 Aug 2013

	volatile unsigned int i,j;	// volatile to prevent removal in optimization
			                    // by compiler. Functionally this is useless code

	for (j=0; j<numLoops; j++)
    {
    	i = 50000 ;					// SW Delay
   	    while (i > 0)				// could also have used while (i)
	       i--;
    }
}
