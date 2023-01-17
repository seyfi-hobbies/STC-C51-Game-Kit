//////////////////////////////////////////////////////////////////////////////////   
//LCD 1.8inch SPI_Module_ST7735S library for STC C51
// copied from www.lcdwiki.com
//http://www.lcdwiki.com
//**************************************************************************************************/   
#include "sys.h"
#include "lcd.h"

//LCD text color and background color
u16 POINT_COLOR = 0x0000;
u16 BACK_COLOR = 0xFFFF; 
//Manage important LCD parameters
//The default is vertical screen
_lcd_dev lcddev;

void delay_ms_new(unsigned char ms)
{
     unsigned int i;
     do{
          i = MAIN_Fosc / 14000;
          while(--i);//14T per loop
     }while(--ms);
}

// init hardware spi for STC15F2K60S2
void SPI_init(void)
{
    SPCTL = (SSIG << 7)+(SPEN << 6)+(DORD << 5)+(MSTR << 4)+(CPOL << 3)+(CPHA << 2)+ SPEED_4;
}

/*****************************************************************************
 * @name       :void SPI_WriteByte(u8 byte)
 * @date       :2018-08-09 
 * @function   :Write a byte of data using C51's Hardware SPI
 * @parameters :d:Data to be written
 * @retvalue   :None
******************************************************************************/
void SPI_WriteByte(u8 byte)
{
            SPDAT = byte;
            while((SPSTAT & SPIF) == 0);//Wait for the send to complete
            SPSTAT = SPIF+WCOL;//Clear the SPIF and WCOL flags
}

/*****************************************************************************
 * @name       :void LCD_WR_REG(u8 data)
 * @date       :2018-08-09 
 * @function   :Write an 8-bit command to the LCD screen
 * @parameters :data:Command value to be written
 * @retvalue   :None
******************************************************************************/
void LCD_WR_REG(u8 Reg)
{
    LCD_CS = 0;
    LCD_RS = 0;
    SPI_WriteByte(Reg);
    LCD_CS = 1;
}

/*****************************************************************************
 * @name       :void LCD_WR_DATA(u8 data)
 * @date       :2018-08-09 
 * @function   :Write an 8-bit data to the LCD screen
 * @parameters :data:data value to be written
 * @retvalue   :None
******************************************************************************/
void LCD_WR_DATA(u8 Data)
{
    LCD_CS = 0;
    LCD_RS = 1;
    SPI_WriteByte(Data);
    LCD_CS = 1;
}

/*****************************************************************************
 * @name       :void LCD_WR_DATA_16Bit(u16 Data)
 * @date       :2018-08-09 
 * @function   :Write an 16-bit command to the LCD screen
 * @parameters :Data:Data to be written
 * @retvalue   :None
******************************************************************************/  
void LCD_WR_DATA_16Bit(u16 Data)
{
    LCD_CS = 0;
    LCD_RS = 1;
    SPI_WriteByte(Data>>8);
    SPI_WriteByte(Data);
    LCD_CS = 1;
}

/*****************************************************************************
 * @name       :void LCD_WriteReg(u8 LCD_Reg, u16 LCD_RegValue)
 * @date       :2018-08-09 
 * @function   :Write data into registers
 * @parameters :LCD_Reg:Register address
                LCD_RegValue:Data to be written
 * @retvalue   :None
******************************************************************************/
void LCD_WriteReg(u8 LCD_Reg, u8 LCD_RegValue)
{
  LCD_WR_REG(LCD_Reg);
    LCD_WR_DATA(LCD_RegValue);
}

/*****************************************************************************
 * @name       :void LCD_WriteRAM_Prepare(void)
 * @date       :2018-08-09 
 * @function   :Write GRAM
 * @parameters :None
 * @retvalue   :None
******************************************************************************/ 
void LCD_WriteRAM_Prepare(void)
{
    LCD_WR_REG(lcddev.wramcmd);
}

/*****************************************************************************
 * @name       :void LCD_Clear(u16 Color)
 * @date       :2018-08-09 
 * @function   :Full screen filled LCD screen
 * @parameters :color:Filled color
 * @retvalue   :None
******************************************************************************/ 
void LCD_Clear(u16 Color)
{
    u16 i,j;
    LCD_SetWindows(0,0,lcddev.width-1,lcddev.height-1);
    for(i = 0;i<lcddev.width;i++)
     {
      for (j = 0;j<lcddev.height;j++)
        {
             LCD_WR_DATA_16Bit(Color);
        }

      }
}

/*****************************************************************************
 * @name       :void LCD_DrawPoint(u16 x,u16 y)
 * @date       :2018-08-09 
 * @function   :Write a pixel data at a specified location
 * @parameters :x:the x coordinate of the pixel
                y:the y coordinate of the pixel
 * @retvalue   :None
******************************************************************************/ 
void LCD_DrawPoint(u16 x,u16 y)
{
    LCD_SetWindows(x,y,x,y);//set cursor position
    LCD_WR_DATA_16Bit(POINT_COLOR);
}

/*****************************************************************************
 * @name       :void LCD_Reset(void)
 * @date       :2018-08-09 
 * @function   :Reset LCD screen
 * @parameters :None
 * @retvalue   :None
******************************************************************************/ 
void LCD_Reset(void)
{
    LCD_RESET = 1;
    delay_ms_new(50);
    LCD_RESET = 0;
    delay_ms_new(50);
    LCD_RESET = 1;
    delay_ms_new(50);
}

/*****************************************************************************
 * @name       :void LCD_Init(void)
 * @date       :2018-08-09 
 * @function   :Initialization LCD screen
 * @parameters :None
 * @retvalue   :None
******************************************************************************/      
void LCD_Init(void)
{
    SPI_init();//initialize hardware SPI
    LCD_Reset();//Reset before initializing
//************* ST7735S initialization**********//  
    LCD_WR_REG(0x11);//Sleep exit 
    delay_ms (120);
    //ST7735R Frame Rate
    LCD_WR_REG(0xB1);
    LCD_WR_DATA(0x01);
    LCD_WR_DATA(0x2C);
    LCD_WR_DATA(0x2D);
    LCD_WR_REG(0xB2);
    LCD_WR_DATA(0x01);
    LCD_WR_DATA(0x2C);
    LCD_WR_DATA(0x2D);
    LCD_WR_REG(0xB3);
    LCD_WR_DATA(0x01);
    LCD_WR_DATA(0x2C);
    LCD_WR_DATA(0x2D);
    LCD_WR_DATA(0x01);
    LCD_WR_DATA(0x2C);
    LCD_WR_DATA(0x2D);
    LCD_WR_REG(0xB4);//Column inversion 
    LCD_WR_DATA(0x07);
    //ST7735R Power Sequence
    LCD_WR_REG(0xC0);
    LCD_WR_DATA(0xA2);
    LCD_WR_DATA(0x02);
    LCD_WR_DATA(0x84);
    LCD_WR_REG(0xC1);
    LCD_WR_DATA(0xC5);
    LCD_WR_REG(0xC2);
    LCD_WR_DATA(0x0A);
    LCD_WR_DATA(0x00);
    LCD_WR_REG(0xC3);
    LCD_WR_DATA(0x8A);
    LCD_WR_DATA(0x2A);
    LCD_WR_REG(0xC4);
    LCD_WR_DATA(0x8A);
    LCD_WR_DATA(0xEE);
    LCD_WR_REG(0xC5);//VCOM 
    LCD_WR_DATA(0x0E);
    LCD_WR_REG(0x36);//MX, MY, RGB mode 
    LCD_WR_DATA(0xC0);
    //ST7735R Gamma Sequence
    LCD_WR_REG(0xe0);
    LCD_WR_DATA(0x0f);
    LCD_WR_DATA(0x1a);
    LCD_WR_DATA(0x0f);
    LCD_WR_DATA(0x18);
    LCD_WR_DATA(0x2f);
    LCD_WR_DATA(0x28);
    LCD_WR_DATA(0x20);
    LCD_WR_DATA(0x22);
    LCD_WR_DATA(0x1f);
    LCD_WR_DATA(0x1b);
    LCD_WR_DATA(0x23);
    LCD_WR_DATA(0x37);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x07);
    LCD_WR_DATA(0x02);
    LCD_WR_DATA(0x10);
    LCD_WR_REG(0xe1);
    LCD_WR_DATA(0x0f);
    LCD_WR_DATA(0x1b);
    LCD_WR_DATA(0x0f);
    LCD_WR_DATA(0x17);
    LCD_WR_DATA(0x33);
    LCD_WR_DATA(0x2c);
    LCD_WR_DATA(0x29);
    LCD_WR_DATA(0x2e);
    LCD_WR_DATA(0x30);
    LCD_WR_DATA(0x30);
    LCD_WR_DATA(0x39);
    LCD_WR_DATA(0x3f);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x07);
    LCD_WR_DATA(0x03);
    LCD_WR_DATA(0x10);
    LCD_WR_REG(0x2a);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x7f);
    LCD_WR_REG(0x2b);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x9f);
    LCD_WR_REG(0xF0);//Enable test command  
    LCD_WR_DATA(0x01);
    LCD_WR_REG(0xF6);//Disable ram power save mode 
    LCD_WR_DATA(0x00);
    LCD_WR_REG(0x3A);//65k mode 
    LCD_WR_DATA(0x05);
    LCD_WR_REG(0x29);//display on  

    //Set LCD property parameters
    LCD_direction(USE_HORIZONTAL);//Set the LCD display direction
   // LCD_BL = 1;//Turn on the backlight if you use the pin
}
 
/*****************************************************************************
 * @name       :void LCD_SetWindows(u16 xStar, u16 yStar,u16 xEnd,u16 yEnd)
 * @date       :2018-08-09 
 * @function   :Setting LCD display window
 * @parameters :xStar:the beginning x coordinate of the LCD display window
                                yStar:the beginning y coordinate of the LCD display window
                                xEnd:the endning x coordinate of the LCD display window
                                yEnd:the endning y coordinate of the LCD display window
 * @retvalue   :None
******************************************************************************/ 
void LCD_SetWindows(u16 xStar, u16 yStar,u16 xEnd,u16 yEnd)
{
    LCD_WR_REG(lcddev.setxcmd);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(xStar);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(xEnd);

    LCD_WR_REG(lcddev.setycmd);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(yStar);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(yEnd);

    LCD_WriteRAM_Prepare();//Start writing to GRAM            
}

/*****************************************************************************
 * @name       :void LCD_SetCursor(u16 Xpos, u16 Ypos)
 * @date       :2018-08-09 
 * @function   :Set coordinate value
 * @parameters :Xpos:the  x coordinate of the pixel
                                Ypos:the  y coordinate of the pixel
 * @retvalue   :None
******************************************************************************/ 
void LCD_SetCursor(u16 Xpos, u16 Ypos)
{
    LCD_SetWindows(Xpos,Ypos,Xpos,Ypos);
}

/*****************************************************************************
 * @name       :void LCD_direction(u8 direction)
 * @date       :2018-08-09 
 * @function   :Setting the display direction of LCD screen
 * @parameters :direction:0-0 degree
                          1-90 degree
                                                    2-180 degree
                                                    3-270 degree
 * @retvalue   :None
******************************************************************************/ 
void LCD_direction(u8 direction)
{
            lcddev.setxcmd = 0x2A;
            lcddev.setycmd = 0x2B;
            lcddev.wramcmd = 0x2C;
    switch(direction){
        case 0:
            lcddev.width = LCD_W;
            lcddev.height = LCD_H;
            LCD_WriteReg(0x36,(0<<3)|(1<<6)|(1<<7));//BGR==1,MY==0,MX==0,MV==0
        break;
        case 1:
            lcddev.width = LCD_H;
            lcddev.height = LCD_W;
            LCD_WriteReg(0x36,(0<<3)|(1<<7)|(1<<5));//BGR==1,MY==1,MX==0,MV==1
        break;
        case 2:
            lcddev.width = LCD_W;
            lcddev.height = LCD_H;
            LCD_WriteReg(0x36,(0<<3)|(0<<6)|(0<<7));//BGR==1,MY==0,MX==0,MV==0
        break;
        case 3:
            lcddev.width = LCD_H;
            lcddev.height = LCD_W;
            LCD_WriteReg(0x36,(0<<3)|(0<<7)|(1<<6)|(1<<5));//BGR==1,MY==1,MX==0,MV==1
        break;
        default:break;
    }
}





