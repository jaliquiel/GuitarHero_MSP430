/************** ECE2049 DEMO CODE ******************/
/**************  13 March 2019   ******************/
/***************************************************/

/**
 * - When are we supposed to start timer A2
 */


#include "stdio.h"
#include <msp430.h>
#include "peripherals.h"

// Function Prototypes
void swDelay(char numLoops);

// Declare globals here
enum GAME_STATE {welcome = 0, songStart = 1, song = 2, win=3, lose=4, reset =5};
long unsigned int timer_cnt = 0;
int i;
long unsigned int noteDuration;
long unsigned int noteStartTime;


// variables for displaying score
int correctNotes;
bool pressedCorrect;
bool pressedWrong;
int reactTimer = 100;


unsigned char dispFour[4] = {NULL, NULL, NULL, NULL};
unsigned char dispThree[4] = {NULL, NULL, NULL};
char score[3] = {NULL, NULL, NULL};

struct note{
    char name;
    unsigned int frequency;
    char ledValue;
    long unsigned int noteDuration;

};


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

    correctNotes = 0;

    int loseSongLength = 4;
    struct note loseSong[4]=
    {
     {'D', 587, '1' - 0x30, 200},
     {'C', 523, '2' - 0x30, 200},
     {'B', 494, '4' - 0x30, 200},
     {'A', 440, '8' - 0x30, 200},
    };

    int winSongLength = 4;
    struct note winSong[4]=
    {
     {'A', 440, '8' - 0x30, 200},
     {'B', 494, '4' - 0x30, 200},
     {'C', 523, '2' - 0x30, 200},
     {'D', 587, '1' - 0x30, 200},
    };


    int songNoteLength = 54;
    struct note songNotes[54] =
    {
     {'C', 523, '2' - 0x30, 100},
     {'C', 523, '2' - 0x30, 100},
     {'D', 587, '1' - 0x30, 100},
     {'E', 659, '8' - 0x30, 100},

     {'C', 523, '2' - 0x30, 100},
     {'E', 659, '8' - 0x30, 100},
     {'D', 587, '1' - 0x30, 200},

     {'C', 523, '2' - 0x30, 100},
     {'C', 523, '2' - 0x30, 100},
     {'D', 587, '1' - 0x30, 100},
     {'E', 659, '8' - 0x30, 100},

     {'C', 523, '2' - 0x30, 200},
     {'B', 494, '4' - 0x30, 200},

     {'C', 523, '2' - 0x30, 100},
     {'C', 523, '2' - 0x30, 100},
     {'D', 587, '1' - 0x30, 100},
     {'E', 659, '8' - 0x30, 100},

     {'F', 698, '1' - 0x30, 100},
     {'E', 659, '8' - 0x30, 100},
     {'D', 587, '1' - 0x30, 100},
     {'C', 523, '2' - 0x30, 100},

     {'B', 494, '4' - 0x30, 100},
     {'G', 784, '2' - 0x30, 100},
     {'A', 440, '8' - 0x30, 100},
     {'B', 494, '4' - 0x30, 100},

     {'C', 523, '2' - 0x30, 200},
     {'C', 523, '2' - 0x30, 200},

     {'A', 440, '8' - 0x30, 150},
     {'B', 494, '4' - 0x30, 50},
     {'A', 440, '8' - 0x30, 100},
     {'G', 784, '2' - 0x30, 100},

     {'A', 440, '8' - 0x30, 100},
     {'B', 494, '4' - 0x30, 100},
     {'C', 523, '2' - 0x30, 200},

     {'G', 784, '2' - 0x30, 150},
     {'A', 440, '8' - 0x30, 50},
     {'G', 784, '4' - 0x30, 100},
     {'F', 698, '1' - 0x30, 100},

     {'E', 659, '8' - 0x30, 200},
     {'G', 784, '4' - 0x30, 200},

     {'A', 440, '8' - 0x30, 150},
     {'B', 494, '4' - 0x30, 50},
     {'A', 440, '8' - 0x30, 100},
     {'G', 784, '4' - 0x30, 100},

     {'A', 440, '8' - 0x30, 100},
     {'B', 494, '4' - 0x30, 100},
     {'C', 523, '2' - 0x30, 100},
     {'A', 440, '8' - 0x30, 100},

     {'G', 784, '4' - 0x30, 100},
     {'C', 523, '2' - 0x30, 100},
     {'B', 494, '4' - 0x30, 100},
     {'D', 587, '1' - 0x30, 100},

     {'C', 523, '2' - 0x30, 200},
     {'C', 523, '2' - 0x30, 200},
    };
//


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
            runTimerA2();
            while(1){
                if (timer_cnt < 200){
                    Graphics_drawStringCentered(&g_sContext, "GET READY", AUTO_STRING_LENGTH, 48, 25, TRANSPARENT_TEXT);
                }
                else if (timer_cnt < 400){
                    Graphics_drawStringCentered(&g_sContext, "3...", AUTO_STRING_LENGTH, 24, 35, TRANSPARENT_TEXT);
                    configUserLED('1'-'0'); // left LED on red
                }
                else if (timer_cnt < 600){
                    Graphics_drawStringCentered(&g_sContext, "2...", AUTO_STRING_LENGTH, 48, 35, TRANSPARENT_TEXT);
                    configUserLED('2'-'0'); // right LED on green
                }
                else if (timer_cnt < 800){
                    Graphics_drawStringCentered(&g_sContext, "1...", AUTO_STRING_LENGTH, 72, 35, TRANSPARENT_TEXT);
                    configUserLED('3'-'0'); // both LED on
                }
                else if (timer_cnt > 800){
                    Graphics_drawStringCentered(&g_sContext, "Go!", AUTO_STRING_LENGTH, 48, 15, TRANSPARENT_TEXT);
                    configUserLED('0');
                    Graphics_flushBuffer(&g_sContext);
                    break;
                }
                Graphics_flushBuffer(&g_sContext);
            }
            stopTimerA2(1);
            Graphics_clearDisplay(&g_sContext);
            state = song;
            break;

        case song:

            i = 0;
            correctNotes = 0;
            runTimerA2();

            while(i < songNoteLength){
                struct note currentNote = songNotes[i];

                noteStartTime = timer_cnt;
                pressedCorrect = false;
                pressedWrong = false;

                while(timer_cnt < noteStartTime + reactTimer){ // half a second react time to correct note
                    setLeds(currentNote.ledValue);
                    configUserLED('3'-'0'); // both LED

                    currButton = readButtons();
                    if(correctPress(currentNote.ledValue,currButton)){
                        pressedCorrect = true;
                        break;
                    }else if(currButton == 0){
                        // did not press anything
                    }
                    else{
                        // pressed the wrong note
                        pressedWrong = true;
                        break;
                    }
                }

                setLeds(0);
                if(pressedCorrect){
                    while(timer_cnt < noteStartTime + currentNote.noteDuration + reactTimer){ // play note if pressed correct
//                        setLeds(currentNote.ledValue);
                        configUserLED('2'-'0'); // right LED on green
                        BuzzerNote(currentNote.frequency);
                    }
                }else if (pressedWrong){
                    while(timer_cnt < noteStartTime + currentNote.noteDuration + reactTimer){ // play note if pressed correct
//                        setLeds(currentNote.ledValue);
                        configUserLED('1'-'0'); // left LED on red
                        BuzzerNote(1000);
                        setLeds(15);
                    }
                }else{
                    while(timer_cnt < noteStartTime + currentNote.noteDuration + reactTimer){ // be silentif nothing pressed
                        // nothing
                        setLeds(15);
                    }
                }
                BuzzerOff();
                setLeds(0);
                configUserLED('0');
                i++;
                if(pressedCorrect)
                    correctNotes++;
            }
            BuzzerOff();

            // Calculate player's score, if player lost send to lose state, otherwise to win state
            state = checkScore(correctNotes, songNoteLength);

            setLeds('0' - 0x30);
            Graphics_clearDisplay(&g_sContext);
            currKey = 0;
            break;

        case win:
            Graphics_clearDisplay(&g_sContext);
            sprintf(score, "%d", correctNotes);
            for(i = 1; i < 10; i ++){
                if (i == 4)
                    continue;
                Graphics_drawStringCentered(&g_sContext, "YOU WON", AUTO_STRING_LENGTH, 50, i*10, TRANSPARENT_TEXT);
            }
            Graphics_drawStringCentered(&g_sContext, score , AUTO_STRING_LENGTH, 30, 40, TRANSPARENT_TEXT);
            Graphics_drawStringCentered(&g_sContext, "CORRECT" , AUTO_STRING_LENGTH, 60, 40, TRANSPARENT_TEXT);
            Graphics_flushBuffer(&g_sContext);

            //proper celebration w/ buzzer and leds
            for(i=0;i<winSongLength; i++){
                struct note note = winSong[i];
                BuzzerNote(note.frequency);
                swDelay(1);
            }
            BuzzerOff();

            while(currKey != '#'){
                currKey = getKey();
            }

            if(currKey == '#'){
                // Go to reset state
                state = reset;
            }
            break;


        case lose:
            // display celebration
            Graphics_clearDisplay(&g_sContext);
            sprintf(score, "%d", correctNotes);
            for(i = 1; i < 10; i ++){
                if (i == 4)
                    continue;
                Graphics_drawStringCentered(&g_sContext, "YOU LOST", AUTO_STRING_LENGTH, 50, i*10, TRANSPARENT_TEXT);
            }
            Graphics_drawStringCentered(&g_sContext, score , AUTO_STRING_LENGTH, 30, 40, TRANSPARENT_TEXT);
            Graphics_drawStringCentered(&g_sContext, "CORRECT" , AUTO_STRING_LENGTH, 60, 40, TRANSPARENT_TEXT);
            Graphics_flushBuffer(&g_sContext);

            //proper celebration w/ buzzer and leds
            for(i=0;i<loseSongLength; i++){
                struct note note = loseSong[i];
                BuzzerNote(note.frequency);
                swDelay(1);
            }
            BuzzerOff();


            while(currKey != '#'){
                currKey = getKey();
            }

            if(currKey == '#'){
                // Go to reset state
                state = reset;
            }
            break;


        case reset:
            setLeds('0' - 0x30);
            Graphics_clearDisplay(&g_sContext);
            currKey = 0;
            state = welcome;
            pressedCorrect = 0;
            for(i = 0; i< 3; i++){
                score[i] = NULL;
            }
            stopTimerA2(1);
            break;

        }// end switch

    }  // end while (1)
}


int correctNotes;
int songNoteLength = 54;

// false if player lost, true if player won
int checkScore(int correctNotes, int songNoteLength){
    if (songNoteLength*2/3 > correctNotes){
        return lose;
    }
    return win;
}


void celebration(void){
    // play buzzer song
}

void humiliation(void){
    // play buzzer song
}


// given a note, the curresponding LED lights up and the corresponding buzzer frequency is sent
void playNote(struct note note){

    setLeds(note.ledValue);
    BuzzerNote(note.frequency);

}

bool correctPress(unsigned char ledValue, unsigned int buttonPressed){
    if (ledValue & buttonPressed)
        return true;
    return false;
}


void runTimerA2(){
    TA2CTL = TASSEL_1 + ID_0 + MC_1;
    TA2CCR0 = 163; // interrupt every 0.005
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

}
