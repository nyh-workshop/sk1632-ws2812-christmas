#ifndef  _WS2812_PIC32MX_H
#define _WS2812_PIC32MX_H

#define WS2812_NO_OF_BITS 24        // Number of bits in the color. (24-bits color)
#define WS2812_T_RST_BITS 50        // T-reset time in bits.

void ws2812_init_DMA(void);
void ws2812_setColorStrip(unsigned int numOfPixels, unsigned int* inputArray);
void ws2812_scan(unsigned int numOfPixels, unsigned int* inputArray, unsigned int speed_in_ms);










#endif