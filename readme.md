# Basic WS2812/PL9823 driver for PIC32MX with custom effects

Those addressable LEDs are all over the place and they are gonna be a dime-a-dozen soon. Plus, it's gonna be Christmas! That is the best time for that sort of thing!

Tough part is, driving those little stuff. You need tight and precise timing! However, all is not lost as those 32-bit microcontrollers are equipped with DMA so that you can push those pulses without (a lot of) CPU intervention!

In here, we are using a PIC32MX250F128B with the Cytron SK1632 board. The WS2812's one and zero bits do look like PWM pulses with a different duty cycle, and the firmware replicates those by pushing the PWM into the WS2812's input by using DMA.

The custom effects are from WS2812FX library and Adafruit NeoPixel Library:
- https://github.com/kitesurfer1404/WS2812FX
- https://github.com/adafruit/Adafruit_NeoPixel

Youtube links:
- https://youtu.be/GamRjbBVx68
- https://youtu.be/Dt6opyrJ8xI

There is a small difference between WS2812 and the PL9823 - the color arrangement for WS2812 is GRB, while for PL9823 is RGB.

**You can change the LED type and the numbers of LED in the main.h file.**

Requirements:
- MPLAB Harmony 1.07.01
- MPLAB XC32 1.44

Description are in the code - I will write a small guide on here if I can make time for it! :)

Extra note: Pinouts are in the Harmony Configurator too.
