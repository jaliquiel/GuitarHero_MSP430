/************** ECE2049 DEMO CODE ******************/
/**************  13 March 2019   ******************/
/***************************************************/

/**
 * - When are we supposed to start timer A2
 */



#include <msp430.h>
#include "peripherals.h"

// Function Prototypes
void swDelay(char numLoops);

// Declare globals here
enum GAME_STATE {welcome = 0, songStart = 1, song = 2};
long unsigned int timer_cnt = 0;
//char songNotes[28] = {'A', 'B', 'C', 'D'};
char songNotes[28] = {'C', 'C', 'D', 'E','C','E','D','C','C','D','E','C','B','C','C','D','E','F','E','D','C'}; // 21 notes
int i;


// Main
void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;
    __bis_SR_register(GIE); // Global INterrupt enable


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
            m = 30;
            while(1){
                m--;
                if (m > 15){
                    Graphics_drawStringCentered(&g_sContext, "GET READY", AUTO_STRING_LENGTH, 48, 25, TRANSPARENT_TEXT);
                }
                else if ((m > 10) && (m <= 15)){
                    Graphics_drawStringCentered(&g_sContext, "3...", AUTO_STRING_LENGTH, 24, 35, TRANSPARENT_TEXT);
                    configUserLED('1'-'0');
                }
                else if ((m > 5) && (m <= 10)){
                    Graphics_drawStringCentered(&g_sContext, "2...", AUTO_STRING_LENGTH, 48, 35, TRANSPARENT_TEXT);
                    configUserLED('2'-'0');
                }
                else if ((m > 0) && (m <= 5)){
                    Graphics_drawStringCentered(&g_sContext, "1...", AUTO_STRING_LENGTH, 72, 35, TRANSPARENT_TEXT);
                    configUserLED('3'-'0');
                }
                else if (m == 0){
                    Graphics_drawStringCentered(&g_sContext, "Go!", AUTO_STRING_LENGTH, 48, 15, TRANSPARENT_TEXT);
                    configUserLED('0');
                    Graphics_flushBuffer(&g_sContext);
                    break;
                }
                Graphics_flushBuffer(&g_sContext);
            }
            Graphics_clearDisplay(&g_sContext);
            state = song;
            break;

        case song:

            i = 0;
            while(i < 21){
                runTimerA2();

                while(timer_cnt < 200){
                    playNote(songNotes[i]);
                }
                while(timer_cnt<215){
                    BuzzerOff();
                }
                i++;
                stopTimerA2(1);
            }
            BuzzerOff();
            runTimerA2(0);

            setLeds('0' - 0x30);
            currKey = 0;
            state = welcome;
//
////            for(i = 0; i< 4; i++){
////                playNote(songNotes[i]);
////                swDelay(2);
////            }
//
//
//
//            while(1){
//                currButton = readButtons();
//
//            }
//
//            currKey = getKey();
//
//            currButton = readButtons() + '0';
//            if(currButton != '0'){
//                setLeds(currButton - 0x30);
//            }
//            else{
//                setLeds(0);
//            }
//            configUserLED('1' - '0');
//
//            if(currKey == '#'){
//                Graphics_clearDisplay(&g_sContext);
//                state = welcome;
//            }
//
//            break;

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

// given a note, the curresponding LED lights up and the corresponding buzzer frequency is sent
void playNote(char note){
    unsigned int frequency;

    switch(note){
    case 'A':
        frequency = 440;
        setLeds('8' - 0x30);
        break;
    case 'B':
        frequency = 494;
        setLeds('4' - 0x30);
        break;
    case 'C':
        frequency = 523;
        setLeds('2' - 0x30);
        break;
    case 'D':
        frequency = 587;
        setLeds('1' - 0x30);
        break;

    case 'E':
        frequency = 659;
        setLeds('8' - 0x30);
        break;
    case 'F':
        frequency = 698;
        setLeds('4' - 0x30);
        break;
    case 'G':
        frequency = 784;
        setLeds('2' - 0x30);
        break;

    }
    BuzzerNote(frequency);

}

void runTimerA2(){
    TA2CTL = TASSEL_1 + ID_0 + MC_1;
    TA2CCR0 = 163; // interrupt every 0.005
//    TA2CCR0 = 327; // interrupt every 0.005
    TA2CCTL0 = CCIE;
}

void stopTimerA2(int reset){
    TA2CTL = MC_0; // stop timer
    TA2CCTL0 &= ~CCIE; // TA2CCR0 interrupt disabled
    if(reset)
        timer_cnt=0;
}


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


//------------------------------------------------------------------------------
// Timer2 A2 Interrupt Service Routine
//------------------------------------------------------------------------------
#pragma vector=TIMER2_A0_VECTOR
__interrupt void TIMER_A2_ISR (void)
{
    // Display is using Timer A1
    // Not sure where Timer A1 is configured?
//    Sharp96x96_SendToggleVCOMCommand();  // display needs this toggle < 1 per sec
    timer_cnt++;

//                    if (timer_cnt % 200 == 0){
//                        i++;
//                    }
//                    playNote(songNotes[i]);



//    if (timer_cnt == 600000){
//        timer_cnt = 0;
//    }


}
