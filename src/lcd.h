//////////////////////////////////////////////////////////////////////////////////   
//LCD 1.8inch SPI_Module_ST7735S library for STC C51
// copied from www.lcdwiki.com
//http://www.lcdwiki.com
//**************************************************************************************************/   
#ifndef __LCD_H
#define __LCD_H
#include "sys.h"

/////////////////////////////////////user configuration area///////////////////////////////////    
//Support fast definition switching between horizontal and vertical screens
//Define the clockwise rotation direction of the LCD screen 0-0 degree rotation, 1-90 degree rotation, 2-180 degree rotation, 3-270 degree rotation
#define USE_HORIZONTAL 2  //set this to 0 if you connected LCD differently

//Define the size of the LCD
#define LCD_W 128
#define LCD_H 160


//8x16 Game board pins
#define LCD_RS P1_6  //rx, 
#define LCD_SDI P1_3 //sdi, mosi
//#define LCD_SDO P1_4	//sdo, miso, not used on this LCD
#define LCD_CS P1_7    //CS
#define LCD_CLK P1_5   // sclk
#define LCD_RESET P1_2  //reset


//LCD text color and background color   
extern u16 POINT_COLOR;
extern u16 BACK_COLOR;
//LCD important parameter set
typedef struct
{
    u16 width;//LCD width
    u16 height;//LCD height
    u16 id;//LCD ID
    u8 dir;//Horizontal or vertical screen control: 0, vertical screen; 1, horizontal screen. 
    u16 wramcmd;//Start writing gram instruction
    u16 setxcmd;//Set x coordinate command
    u16 setycmd;//Set y coordinate command	
}_lcd_dev;

//timer0 values for STC15F2K60S2

#define MCU_STC15
#define MAIN_Fosc 24000000L  //Chip should be programmed at 24Mhz
#ifdef MCU_STC15
__sfr __at (0xA2) AUXR1;
#endif
//-----------------------------SPI bus configuration--------------------------------------//


//STC microcontroller hardware SPI register definition
__sfr __at (0xCD) SPSTAT;//STC15F2K60S2
__sfr __at (0xCE) SPCTL;//STC15F2K60S2
__sfr __at (0xCF) SPDAT;//STC15F2K60S2

//   7       6       5       4       3       2       1       0      Reset Value
//  SSIG    SPEN    DORD    MSTR    CPOL    CPHA    SPR1    SPR0        0x00
#define SSIG 1 //1: Ignore the SS pin, the master or slave is determined by the MSTR bit 0: The SS pin is used to determine the master and slave.
#define SPEN 1 //1: Allow SPI, 0: Disable SPI, all SPI pins are normal IO
#define DORD 0 //1: LSB first, 0: MSB first
#define MSTR 1 //1: set as master 0: set as slave
#define CPOL 1 //1: SCLK is high when idle, 0: SCLK is low when idle
#define CPHA 1 //
#define SPR1 0 //SPR1,SPR0   00: fosc/4,     01: fosc/16
#define SPR0 0 //            10: fosc/64,    11: fosc/128
#define SPEED_4 0 // fosc/4
#define SPEED_16 1 // fosc/16
#define SPEED_64 2 // fosc/64
#define SPEED_128 3 // fosc/128

//   7       6      5   4   3   2   1   0       Reset Value
//  SPIF    WCOL    -   -   -   -   -   -
#define SPIF 0x80//SPI transfer completion flag. Write 1 to clear 0.
#define WCOL 0x40//SPI write conflict flag. Write 1 to clear 0.

extern _lcd_dev lcddev;

void LCD_Init(void);
void LCD_Clear(u16 Color);
void LCD_WR_DATA(u8 Data);
void LCD_WR_REG(u8 Reg);
void LCD_SetCursor(u16 Xpos, u16 Ypos);
void LCD_SetWindows(u16 xStar, u16 yStar,u16 xEnd,u16 yEnd);
void LCD_DrawPoint(u16 x,u16 y);
void LCD_WriteRAM_Prepare(void);
void LCD_direction(u8 direction );
void LCD_WR_DATA_16Bit(u16 Data);


#define WHITE 0xFFFF
#define BLACK 0x0000
#define BLUE 0x001F
#define BRED 0XF81F
#define GRED 0XFFE0
#define GBLUE 0X07FF
#define RED 0xF800
#define MAGENTA 0xF81F
#define GREEN 0x07E0
#define CYAN 0x7FFF
#define YELLOW 0xFFE0
#define BROWN 0XBC40
#define BRRED 0XFC07
#define GRAY 0X8430

#define DARKBLUE 0X01CF
#define LIGHTBLUE 0X7D7C
#define GRAYBLUE 0X5458
 
#define LIGHTGREEN 0X841F
#define LGRAY 0XC618

#define LGRAYBLUE 0XA651
#define LBBLUE 0X2B12


                             
#endif
     
     



