#include <xc.h>
#include <string.h>
#include "main.h"
#include "ws2812_pic32mx.h"
#include "sys/kmem.h"                   // KVA_TO_PA macro
#include "system/common/sys_module.h"   // SYS function prototypes
#include "math.h"

uint8_t ws2812sendBit[1024];
uint32_t ws2812array[NO_OF_WS2812];

extern volatile unsigned char transfer_done;

extern void delay_ms(unsigned int count);


unsigned int colorRampUpDown[8] = { 0xFF0000, 0xFF6600, 0xFFFF0B, 0x1BFF0B, 0x0E16FF, 0x7800ff, 0xff66ff, 0 };

void ws2812_init_DMA(void) {
    
    memset(ws2812sendBit, 1024, 0x00);        // buffer for 24 individual bits of the WS2812 to be sent to the PWM module!
        
    IEC1bits.DMA0IE = 1;
    IFS1bits.DMA0IF = 0;
    DMACONSET  = 0x8000;                                       // enable DMA.
    DCH0CON = 0x0000;
    DCRCCON = 0x00;                                            //   
    DCH0INTCLR = 0xff00ff;                                     // clear DMA interrupts register.
    DCH0INTbits.CHBCIE = 1;                                    // DMA Interrupts when channel block transfer complete enabled.
    DCH0ECON = 0x00;
   
    DCH0SSA = KVA_TO_PA(&ws2812sendBit);     // DMA source address.
    DCH0DSA = KVA_TO_PA(&OC1RS);             // DMA destination address.
    DCH0SSIZ = WS2812_NO_OF_BITS + 1;        // DMA Source size (default).
    DCH0DSIZ = 1;                            // DMA destination size.
    DCH0CSIZ = 1;                            // DMA cell size.
    DCH0ECONbits.CHSIRQ = _TIMER_2_IRQ;      // DMA transfer triggered by which interrupt? (On PIC32MX - it is by _IRQ suffix!)
    DCH0ECONbits.AIRQEN = 0;                 // do not enable DMA transfer abort interrupt.
    DCH0ECONbits.SIRQEN = 1;                 // enable DMA transfer start interrupt.
    
    DCH0CONbits.CHAEN = 0;                  // DMA Channel 0 is always disabled right after the transfer.
    
    OC1CONSET = 0x8000;
    T2CONCLR  = 0x8000;
    PR2 = 25;
    
    ws2812_setColorStrip(NO_OF_WS2812, ws2812array);
}

void ws2812_setColorStrip(unsigned int numOfPixels, unsigned int* inputArray) {
    
    unsigned int i = 0;
    unsigned int j = 0;
    unsigned int color = 0;
    
    unsigned int green = 0;
    unsigned int red   = 0;
    unsigned int blue  = 0;
    
    DCH0SSIZ = (numOfPixels * WS2812_NO_OF_BITS) + WS2812_T_RST_BITS;        
      
    for (j = 0; j < numOfPixels; j++) {

        color = inputArray[j];
        
#ifdef WS2812_LED
        green = (color & 0xFF0000) >> 16;
        blue = (color & 0x00FF00) >> 8;
        red = (color & 0x0000FF);
        
        for (i = 0; i < 8; i++) {
            if (green & (1 << i))
                ws2812sendBit[(j*24) + (7 - i)] = 14;
            else
                ws2812sendBit[(j*24) +  (7 - i)] = 7;
        }

        for (i = 0; i < 8; i++) {
            if (red & (1 << i))
                ws2812sendBit[(j*24) + (15 - i)] = 14;
            else
                ws2812sendBit[(j*24) + (15 - i)] = 7;
        }

        for (i = 0; i < 8; i++) {
            if (blue & (1 << i))
                ws2812sendBit[(j*24) + (23 - i)] = 14;
            else
                ws2812sendBit[(j*24) + (23 - i)] = 7;
        }
#endif
        
#ifdef PL9823_LED
       red = (color & 0xFF0000) >> 16;
       green = (color & 0x00FF00) >> 8;
       blue = (color & 0x0000FF);
       
       for (i = 0; i < 8; i++) {
            if (red & (1 << i))
                ws2812sendBit[(j*24) + (7 - i)] = 14;
            else
                ws2812sendBit[(j*24) +  (7 - i)] = 7;
        }

        for (i = 0; i < 8; i++) {
            if (green & (1 << i))
                ws2812sendBit[(j*24) + (15 - i)] = 14;
            else
                ws2812sendBit[(j*24) + (15 - i)] = 7;
        }

        for (i = 0; i < 8; i++) {
            if (blue & (1 << i))
                ws2812sendBit[(j*24) + (23 - i)] = 14;
            else
                ws2812sendBit[(j*24) + (23 - i)] = 7;
        }
    }
#endif

        
    
    memset(ws2812sendBit + (numOfPixels * WS2812_NO_OF_BITS), WS2812_T_RST_BITS, 0x00);
    
    // Enable DMA and start streaming PWM data immediately!
    // WS2812's Data Transfer Time:
    // (all give and take 150nS!)
    // T0H = 0.35uS
    // T0L = 0.80uS
    // T1H = 0.70uS
    // T1L = 0.60uS
    // T0H+T0L = 1.15uS
    IFS0bits.T2IF = 0;
    DCH0CONbits.CHEN = 1;                   
    DCH0ECONbits.CFORCE = 1;
    TMR2 = 0x0000;
    T2CONSET = 0x8000;
    
    while(!transfer_done);
    transfer_done = 0;        
    
}

// Below are the example functions to display various color effects on a string of 
// WS2812/PL9823. Feel free to separate them to other C file if possible if you need to 
// reuse the code!

// Commented out the function because uh... It doesn't suit me after I wrote it. My bad! 
//void ws2812_scan(unsigned int numOfPixels, unsigned int* inputArray, unsigned int speed_in_ms) {
//    
//    //unsigned int colorRampUpDown[8] = { 31, 63, 127, 255, 127, 63, 31, 0};
//    unsigned int colorRampUpDown[8] = { 0xFF0000, 0xFF6600, 0xFFFF0B, 0x1BFF0B, 0x0E16FF, 0x7800ff, 0xff66ff, 0 };
//    
//    unsigned int i = 0;
//    
//    
//    for(i = 0; i < 8; i++) {        // start of buffer.
//        memcpy(inputArray, colorRampUpDown + (7-i), i*4);     
//        ws2812_setColorStrip(numOfPixels, inputArray);
//        delay_ms(speed_in_ms);
//        memset(inputArray, 0x00, 10*4);
//    }
//    
//    for(i = 0; i < 2; i++) {        // middle of buffer.
//        memcpy(inputArray + 1 + i, colorRampUpDown, 8*4);     
//        ws2812_setColorStrip(numOfPixels, inputArray);
//        delay_ms(speed_in_ms); 
//        memset(inputArray, 0x00, 10*4);
//    }
//    
//    for(i = 0; i < 8; i++) {        // end of buffer.
//        memcpy(inputArray + 3 + i, colorRampUpDown, (7-i)*4);     
//        ws2812_setColorStrip(numOfPixels, inputArray);
//        delay_ms(speed_in_ms); 
//        memset(inputArray, 0x00, 10*4);
//    }
//    
//}

// NOTE: Those fancy effects are ported from the WS2812FX and Adafruit NeoPixel library:
// https://github.com/kitesurfer1404/WS2812FX
// https://github.com/adafruit/Adafruit_NeoPixel
// All thanks to the writers of that code! :)

// Taken from Adafruit NeoPixel library to fit into C code!
void setPixelColor(unsigned int pixel_num, unsigned int color) {
    ws2812array[pixel_num] = color;
}

// Taken from Adafruit NeoPixel library to fit into C code!
uint32_t Color(uint8_t r, uint8_t g, uint8_t b) {
  return ((uint32_t)r << 16) | ((uint32_t)g <<  8) | b;
}


// Taken from Adafruit NeoPixel library to fit into C code!
// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(uint8_t WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

// Taken from Adafruit NeoPixel library to fit into C code!
// This function, when it is called, moves a color by one step.
unsigned int rainbowStep = 0;
void rainbowCycle() {
  uint16_t i, j;
  
  if(rainbowStep > 256*5)
      rainbowStep = 0;
  else
      rainbowStep = rainbowStep + 1;
  
  //for(j=0; j<256*5; j++) { // (256*5) 5 cycles of all colors on wheel
    for(i=0; i< NO_OF_WS2812; i++) {
      setPixelColor(i, Wheel(((i * 256 / NO_OF_WS2812) + rainbowStep) & 255));
   }
    //strip.show();
    ws2812_setColorStrip(NO_OF_WS2812, ws2812array);
    //delay_ms(wait);
  //}
}

// modified from WS2812FX library!
void mode_single_dynamic(void) {
  
  unsigned int randPos = 0;
  uint16_t i;
  
  for( i = 0; i <= (NO_OF_WS2812 - 1); i++) {
      setPixelColor(i, Wheel(rand()));
  }
  
  randPos = (rand() % 16) + 1;
    
  setPixelColor(randPos, Wheel(rand()));
  ws2812_setColorStrip(NO_OF_WS2812, ws2812array);

}

// Set pixel color from separate R,G,B components:
void setPixelColor_s(uint16_t n, uint8_t r, uint8_t g, uint8_t b) {
    ws2812array[n] = ((uint32_t)r << 16) | ((uint32_t)g <<  8) | b;  
}

const uint8_t sine_pattern[16] = {127,176,217,245,
254,245,217,176,
127,79,38,10,
0,10,38,79 };

// if shift_ctr is zero, the sine-wave light brightness is static.
uint8_t shift_ctr = 0;
void mode_running_lights(uint32_t startColor, uint8_t shift) {
  uint16_t i;  
  uint16_t s;
  
  uint8_t r = ((startColor >> 16) & 0xFF);
  uint8_t g = ((startColor >> 8) & 0xFF);
  uint8_t b = (startColor & 0xFF);

  if(shift)
      shift_ctr++;
  
  for(i = 0; i < NO_OF_WS2812; i++) {
      //s = (uint16_t)((sin(2.00*3.142*i/16.00) * 127.00) + 128.00);       // <- spread i to the number of pixels available!
      if(shift)
          s = sine_pattern[(i + shift_ctr) & 0x0f];
      else
          s = sine_pattern[i & 0x0f];
      
      setPixelColor_s(i, (((uint32_t)(r * s)) / 255), (((uint32_t)(g * s)) / 255), (((uint32_t)(b * s)) / 255));
  }
  
  ws2812_setColorStrip(NO_OF_WS2812, ws2812array);
}


// modified from WS2812FX library!
uint32_t fade_step = 0;
// fadeSpeed = 0-255 only!
void mode_fade(uint32_t startColor, uint32_t fadeSpeed) {
  uint32_t i = 0;
  uint32_t brightness; 
  
  fadeSpeed = fadeSpeed & 0xff;  
  fade_step = (fade_step + fadeSpeed) & 0xff;
  
  // https://stackoverflow.com/questions/1073606/is-there-a-one-line-function-that-generates-a-triangle-wave
  brightness = 127 - abs( (fade_step % (2*127)) - 127);
  
  uint8_t red = (startColor >> 16 & 0xFF);
  uint8_t green = (startColor >> 8 & 0xFF);
  uint8_t blue = (startColor & 0xFF);
  for(i = 0; i < NO_OF_WS2812; i++) {
       setPixelColor_s(i, ((uint32_t)(red * brightness))/255, ((uint32_t)(green * brightness))/255, ((uint32_t)(blue * brightness))/255);
  }
  
  ws2812_setColorStrip(NO_OF_WS2812, ws2812array);
}

uint32_t fade_step2 = 0;
// fadeSpeed = 0-255 only!
void mode_fade_rg(uint32_t fadeSpeed) {
  uint32_t i = 0;
  uint32_t startColor = 0xffff00;
  uint32_t brightness1; 
  uint32_t brightness2;
    
  fadeSpeed = fadeSpeed & 0xff;  
  fade_step2 = (fade_step2 + fadeSpeed) & 0xff;
  
  // https://stackoverflow.com/questions/1073606/is-there-a-one-line-function-that-generates-a-triangle-wave
  brightness1 = 127 - abs( (fade_step2 % (2*127)) - 127);
  brightness2 = abs( (fade_step2 % (2*127)) - 127);
  
  uint8_t red = (startColor >> 16 & 0xFF);
  uint8_t green = (startColor >> 8 & 0xFF);
  uint8_t blue = (startColor & 0xFF);
  
 for(i = 0; i < NO_OF_WS2812; i++) {
       setPixelColor_s(i, ((uint32_t)(red * brightness1))/255, ((uint32_t)(green * brightness2))/255, ((uint32_t)(blue * brightness1))/255);
  }
  
  ws2812_setColorStrip(NO_OF_WS2812, ws2812array);
}

uint32_t fade_step3 = 0;
// fadeSpeed = 0-255 only!
void mode_fade_rb(uint32_t fadeSpeed) {
  uint32_t i = 0;
  uint32_t startColor = 0xff00ff;
  uint32_t brightness1; 
  uint32_t brightness2;
    
  fadeSpeed = fadeSpeed & 0xff;  
  fade_step3 = (fade_step3 + fadeSpeed) & 0xff;
  
  // https://stackoverflow.com/questions/1073606/is-there-a-one-line-function-that-generates-a-triangle-wave
  brightness1 = 127 - abs( (fade_step3 % (2*127)) - 127);
  brightness2 = abs( (fade_step3 % (2*127)) - 127);
  
  uint8_t red = (startColor >> 16 & 0xFF);
  uint8_t green = (startColor >> 8 & 0xFF);
  uint8_t blue = (startColor & 0xFF);
  
  for(i = 0; i < NO_OF_WS2812; i++) {
       setPixelColor_s(i, ((uint32_t)(red * brightness1))/255, ((uint32_t)(green * brightness1))/255, ((uint32_t)(blue * brightness2))/255);
  }
  
  ws2812_setColorStrip(NO_OF_WS2812, ws2812array);
}

uint32_t fade_step4 = 0;
// fadeSpeed = 0-255 only!
void mode_fade_bg(uint32_t fadeSpeed) {
  uint32_t i = 0;
  uint32_t startColor = 0x00ffff;
  uint32_t brightness1; 
  uint32_t brightness2;
    
  fadeSpeed = fadeSpeed & 0xff;  
  fade_step4 = (fade_step4 + fadeSpeed) & 0xff;
  
  // https://stackoverflow.com/questions/1073606/is-there-a-one-line-function-that-generates-a-triangle-wave
  brightness1 = 127 - abs( (fade_step4 % (2*127)) - 127);
  brightness2 = abs( (fade_step4 % (2*127)) - 127);
  
  uint8_t red = (startColor >> 16 & 0xFF);
  uint8_t green = (startColor >> 8 & 0xFF);
  uint8_t blue = (startColor & 0xFF);
  
  for(i = 0; i < NO_OF_WS2812; i++) {
       setPixelColor_s(i, ((uint32_t)(red * brightness1))/255, ((uint32_t)(green * brightness1))/255, ((uint32_t)(blue * brightness2))/255);
  }
  
  ws2812_setColorStrip(NO_OF_WS2812, ws2812array);
}

