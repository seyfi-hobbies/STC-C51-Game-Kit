//LCD 1.8inch SPI_Module_ST7735S library
//copied from www.lcdwiki.com
#include "graph.h"
#include "lcd.h"
#include "string.h"
#include "font.h"

#define drawPixel(x, y, c) LCD_SetWindows(x,y,x,y);LCD_WR_DATA_16Bit(c)

static void _swap(u16 *a, u16 *b)
{
  u16 tmp;
  tmp = *a;
  *a = *b;
  *b = tmp;
}

void LCD_ShowChar(u16 x,u16 y,u16 fc, u16 bc, u8 num,u8 size,u8 mode)
{
    u8 temp;
    u8 pos,t;
    u16 colortemp = POINT_COLOR;
           
    num = num-' ';//Get the offset value
    LCD_SetWindows(x,y,x+size/2-1,y+size-1);//Set a single text display window
    if(!mode)//non-overlapping method
    {
        
        for(pos = 0;pos<size;pos++)
        {
            if(size == 12)temp = asc2_1206[num][pos];//use 1206 fonts
            else temp = asc2_1608[num][pos];//use 1608 font
            for(t = 0;t<size/2;t++)
            {
                if(temp&0x01)LCD_WR_DATA_16Bit(fc);
                else LCD_WR_DATA_16Bit(bc);
                temp>>= 1;
                
            }
            
        }
    }else//Overlay method
    {
        for(pos = 0;pos<size;pos++)
        {
            if(size == 12)temp = asc2_1206[num][pos];
            else temp = asc2_1608[num][pos];
            for(t = 0;t<size/2;t++)
            {
                POINT_COLOR = fc;
                if(temp&0x01)LCD_DrawPoint(x+t,y+pos);//draw a point
                temp>>= 1;
            }
        }
    }
    POINT_COLOR = colortemp;
    LCD_SetWindows(0,0,lcddev.width-1,lcddev.height-1);//restore window to full screen                
}


void Gui_StrCenter(u16 x, u16 y, u16 fc, u16 bc, u8 *str,u8 size,u8 mode)
{
    u16 len = strlen((const char *)str);
    u16 x1 = (lcddev.width-len*8)/2;
    Show_Str(x+x1,y,fc,bc,str,size,mode);
}


void Show_Str(u16 x, u16 y, u16 fc, u16 bc, u8 *str,u8 size,u8 mode)
{
    u16 x0 = x;
    u8 bHz = 0;
    while(*str != 0)
    {
        if(!bHz)
        {
            if(x>(lcddev.width-size/2)||y>(lcddev.height-size))
            return;
            if(*str>0x80)
				bHz = 1;
            else
            {
                if(*str == 0x0D)//newline
                {
                    y+= size;
                    x = x0;
                    str++;
                }
                else
                {
                    if(size>16)//
                    {
                    LCD_ShowChar(x,y,fc,bc,*str,16,mode);
                    x+= 8; 
                    }
                    else
                    {
                    LCD_ShowChar(x,y,fc,bc,*str,size,mode);
                    x+= size/2; 
                    }
                }
                str++;
                
            }
        }
    }
}
               
void LCD_drawFastHLine(u16 x1, u16 y1, u16 w, u16 col)
{
    u16 i;

    
    for (i=0;i< w;i+=1) {
        drawPixel(i+x1, y1, col);
    }
        
}

void LCD_drawFastVLine(u16 x1, u16 y1, u16 w, u16 col)
{
    u16 i;

    for (i = 0;i< w;i+=1) {
        drawPixel(x1, i+y1, col);
    }
    
}

/*******************************************************************
 * @name       :void LCD_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 color)
 * @date       :2018-08-09 
 * @function   :fill the specified area
 * @parameters :sx:the beginning x coordinate of the specified area
                sy:the beginning y coordinate of the specified area
                                ex:the ending x coordinate of the specified area
                                ey:the ending y coordinate of the specified area
                                color:the filled color value
 * @retvalue   :None
********************************************************************/
void LCD_fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 color)
{
    u16 i,j;
    u16 width = ex-sx+1;
    u16 height = ey-sy+1;
	
    LCD_SetWindows(sx,sy,ex,ey);
    for(i = 0;i<height;i++)
    {
        for(j = 0;j<width;j++)
        LCD_WR_DATA_16Bit(color);
    }
    LCD_SetWindows(0,0,lcddev.width-1,lcddev.height-1);
}


/*****************************************************************************
 * @name       :void LCD_DrawFillRectangle(u16 x1, u16 y1, u16 x2, u16 y2)
 * @date       :2018-08-09 
 * @function   :Filled a rectangle
 * @parameters :x1:the beginning x coordinate of the filled rectangle
                y1:the beginning y coordinate of the filled rectangle
                                x2:the ending x coordinate of the filled rectangle
                                y2:the ending y coordinate of the filled rectangle
 * @retvalue   :None
******************************************************************************/  
void LCD_fillRect(u16 x1, u16 y1, u16 x2, u16 y2, u16 col)
{
    LCD_fill(x1,y1,x2,y2,col);
}
