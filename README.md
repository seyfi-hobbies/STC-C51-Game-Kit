# STC15F2K60S2 Flappy Bird Clone

Hacking STC C51 8x16 Led Game Kit

If you bought a DIY 8x16 Led game kit as seen below, you can easily convert it to full colored 128x160 LCD game console!

## Screenshot:
![In-Game](https://raw.githubusercontent.com/seyfi-hobbies/STC-C51-Game-Kit/main/images/conversion.png)

See it in action : https://www.youtube.com/watch?v=FxkKD0-nTUY

This game kit is very cheap and some of them comes with STC15F2K60S2.

Features of the chip: 
* 60K Flash Memory
* 2K SRAM
* 2 Timers
* Hardware SPI

The MCU has plenty of memory to write graphical games. After I completed the hardware changes I ported 
Themistokle Benetatos' Arduino Flappy Bird game which uses the same LCD driver (ST7735S)
Check it out here: https://github.com/mrt-prodz/ATmega328-Flappy-Bird-Clone

*Note that if your chip is different the code examples will not work. 
I have another kit with STC15W408S which doesn't have enough flash memory for graphical programs.(8K flash, 5K eeprom, and 512 bytes of RAM). You can keep this one on the side and buy a blank STC15F2K60S2 and replace it. They are pin compatible*

Since free Keil IDE doesn't allow to create hex files more than 2K, I used SDCC to compile the C source code. You can find compile script for Windows in the source folder. You only need to recompile the code if you need to change some port numbers or something about the game.

If you match the exact schematic and LCD you can just upload the flappyBird.hex file in binary folder to start playing the game. I calibrated the game according to 24 Mhz clock frequency so please make sure you set this frequency when you program the chip in STC_ISP software.

This chip can be programmed using an FTDI USB to Serial Adapter with only 4 pins. To learn how to use STC_ISP programmer I watched the video of another STC hacker Rick100 here: https://www.youtube.com/watch?v=uuZZEVhCWIg
The upload process is described after 21:00 minutes of this video. More info about Rick100's project is here: https://github.com/Rick-100/STC-hourglass-kit/wiki

## Warning!!!!!!!!!!!!!
There is no way to read the original firmware that's in the chip so you can't revert back to it. 
If you want to keep the original operation of the kit, you should put the original chip aside and buy new blank chips for experimenting. 

## Hardware changes
Removing the Leds after the soldering is very difficult, so I prefered buying a new kit and not soldering Leds and the 7-segment display at all.
After getting rid of the original displays MCU will have about 23 GPIOs available to use!! More than enough for a TFT LCD screen.

## 128x160 TFT LCD
This is a cheapest LCD you can buy online.(1.8" TFT LCD Screen SPI ST7735S 128x160)
It has 8 pins and driver is ST7735S. Pin header and mapping can be seen below. LCD examples are in http://www.lcdwiki.com

![LCD](https://raw.githubusercontent.com/seyfi-hobbies/STC-C51-Game-Kit/main/images/LCD_image.png)
![LCD](https://raw.githubusercontent.com/seyfi-hobbies/STC-C51-Game-Kit/main/images/Pin-Mapping.png)

## Tools:

- STC_ISP uploading software
- 
  When your Hex file is ready this is the tool to flash the chip. You can also select clock frequency here before uploading the code. 
  (I programmed at 24 Mhz)
http://www.stcmicro.com/rjxz.html

I use version 6.86. It comes as stc-isp-15xx-v6.86E where 'E' specifies English.


- SDCC Small Device C Compiler
https://sdcc.sourceforge.net/ 

C compiler for microcontrollers/CPUs. I use version 4.2.0

- Keil to SDCC
https://github.com/ywaby/keil2sdcc
This is a good tool to convert Keil C code to SDCC style.

If you write to much code in Keil and way passed 2K limit you might want to convert your code to SDCC (there are some differences)

