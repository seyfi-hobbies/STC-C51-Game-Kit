#ifndef __GRAPH_H__
#define __GRAPH_H__
#include "sys.h"

void GR_drawPoint(u16 x,u16 y,u16 color);
void LCD_fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 color);
void LCD_drawLine(u16 x1, u16 y1, u16 x2, u16 y2);
void LCD_rectangle(u16 x1, u16 y1, u16 x2, u16 y2);
void LCD_fillRect(u16 x1, u16 y1, u16 x2, u16 y2, u16 color);
void LCD_drawFastHLine(u16 x1, u16 y1, u16 x2, u16 col);
void LCD_drawFastVLine(u16 x1, u16 y1, u16 y2, u16 col);
void Show_Str(u16 x, u16 y, u16 fc, u16 bc, u8 *str,u8 size,u8 mode);
void Gui_StrCenter(u16 x, u16 y, u16 fc, u16 bc, u8 *str,u8 size,u8 mode);
void LCD_ShowChar(u16 x,u16 y,u16 fc, u16 bc, u8 num,u8 size,u8 mode);
void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len,u8 size);

#endif
