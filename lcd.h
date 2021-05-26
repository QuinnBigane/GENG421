//
// COMP-GENG 421 - Tom Lupfer
//
// LCD module header file
//

#ifndef LCD_H_
#define LCD_H_

#define LCD_PIXEL_WIDTH		128		// width of the LCD, in pixels
#define LCD_PIXEL_HEIGHT	32		// height of the LCD, in pixels

#define LCD_CHAR_HEIGHT		4		// height of the LCD, in characters
#define LCD_CHAR_WIDTH		21		// width of the LCD, in characters

#define LCD_OBJ_POINT_1X1	0		// 1x1 pixel square point
#define LCD_OBJ_POINT_2X2	1		// 2x2 pixel square point

#define LCD_MODE_CURSOR		0		// object renders as a cursor
#define LCD_MODE_STYLUS		1		// object renders as a stylus


void LcdInit(void);
void LcdClear(void);
void LcdReverse(Bool ReverseDisplay);
void LcdRenderObject(int Object, int PosX, int PosY, int Mode);
int LcdPrintf(int CharRow, int CharCol, char * FmtStrPtr, ...);

#endif	// LCD_H_
