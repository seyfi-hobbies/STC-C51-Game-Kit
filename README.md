# STC15F2K60S2 Flappy Bird Clone

Hacking STC C51 8x16 Led Game Kit

If you bought a DIY 8x16 Led game kit as the one before, you can easily convert it to full colored 128x160 LCD game console!

## Screenshot:
![In-Game](https://raw.githubusercontent.com/seyfi-hobbies/STC-C51-Game-Kit/main/images/conversion.png)

This game kit is very chip and some of them comes with STC15F2K60S2.
If your chip is different the code examples might not work. 
I have another kit with STC15W408S which doesn't have enough flash memory for graphical programs.

STC15F2K60S2 comes with, 
* 60K Flash Memory
* 2K SRAM
* 2 Timers
* Hardware SPI

Which has plenty of memory to write graphical games. After I complete the hardware changes I ported 
Themistokle Benetatos' Arduino Flappy Bird game which uses the same LCD driver (ST7735S)
Check it out here: https://github.com/mrt-prodz/ATmega328-Flappy-Bird-Clone


## Warning!!!!!!!!!!!!!
There is no way to read the original firmware that's in the chip so you can't revert back to it. 
If you want to keep the original operation of the kit, you should put the original chip aside and buy new blank chips for experimenting. 

## Tools:

STC_ISP software
http://www.stcmicro.com/rjxz.html

I use version 6.86. It comes as stc-isp-15xx-v6.86E where 'E' specifies English.


SDCC Small Device C Compiler
https://sdcc.sourceforge.net/ 

I use version 4.2.0
