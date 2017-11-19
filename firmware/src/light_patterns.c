#include <xc.h>
#include <string.h>
#include "main.h"
#include "ws2812_pic32mx.h"
#include "sys/kmem.h"                   // KVA_TO_PA macro
#include "system/common/sys_module.h"   // SYS function prototypes
#include "math.h"

// Those pattern functions are for convenience - just put those function ptn-effects (where effects are example: alternateTwoColors)
// in the while loop and put the numbers into the parameters.
// Template if you wanna write your pattern function:
//void ptn_customPattern(unsigned int* param1, unsigned int* param2, ...) {
//    
//    start_timer45();
//    while(!isSwNextPattern) {
//        
//        Your custom function here. Define them in other C file, or just append it
//        into the ws2812_pic32mx.c!
//        Each of the function is separated by a delay. The lighting pattern in the function
//        moves each step. So one call to the function = one frame.
//    }
//    reset_timer45();      
//}




extern void delay_ms(unsigned int count);
extern unsigned char isSwNextPattern;

void ptn_alternateTwoColors(unsigned int* inputColor1, unsigned int* inputColor2, unsigned int inputDelay) {
    
    start_timer45();
    while(!isSwNextPattern) {
        ws2812_setColorStrip(NO_OF_WS2812, (unsigned int*)inputColor1);
        delay_ms(inputDelay);
        ws2812_setColorStrip(NO_OF_WS2812, (unsigned int*)inputColor2);
        delay_ms(inputDelay);
    }
    reset_timer45();      
}

void ptn_rainbow(unsigned int inputDelay) {
    start_timer45();
    while (!isSwNextPattern) {
        rainbowCycle();
        delay_ms(inputDelay);
    }
    reset_timer45();
}

void ptn_fade(unsigned int inputColor, unsigned int inputFadeSpeed, unsigned int inputDelay) {
    start_timer45();
    while (!isSwNextPattern) {
        mode_fade(inputColor, inputFadeSpeed);
        delay_ms(inputDelay);
    }
    reset_timer45();
}

void ptn_runningLights(unsigned int inputColor, unsigned int inputShift, unsigned int inputDelay) {
    start_timer45();
    while (!isSwNextPattern) {
        mode_running_lights(inputColor, inputShift);
        delay_ms(inputDelay);
    }
    reset_timer45();
}

void ptn_fadeRG(unsigned char fadeSpeed, unsigned int inputDelay) {
    start_timer45();
    while (!isSwNextPattern) {
        mode_fade_rg(fadeSpeed);
        delay_ms(inputDelay);
    }
    reset_timer45();
}

void ptn_fadeRB(unsigned char fadeSpeed, unsigned int inputDelay) {
    start_timer45();
    while (!isSwNextPattern) {
        mode_fade_rb(fadeSpeed);
        delay_ms(inputDelay);
    }
    reset_timer45();
}

void ptn_fadeBG(unsigned char fadeSpeed, unsigned int inputDelay) {
    start_timer45();
    while (!isSwNextPattern) {
        mode_fade_bg(fadeSpeed);
        delay_ms(inputDelay);
    }
    reset_timer45();
}