//
// COMP-GENG 421 - Quinn Bigane
//
// Etch A Sketch Emulator module
// Example code for Final Project
//

#include "main.h"
#include "adc.h"
#include "etch.h"
#include "lcd.h"
#include "nav.h"
#include "rgb.h"
#include "accel.h"

#define ETCH_ST_INIT		0		// state to initialize the system
#define ETCH_ST_CURSOR_1X1	1		// state to move the point without drawing 1x1 cursor (cursor)
#define ETCH_ST_STYLUS_1X1	2		// state to move the point and draw 1x1 cursor (stylus)
#define ETCH_ST_CURSOR_2X2	3		// state to move the point without drawing 2x2 cursor (cursor)
#define ETCH_ST_STYLUS_2X2	4		// state to move the point and draw 2x2 cursor (stylus)

#define HOR_DIV			((ADC_RDG_MAX + 1) / LCD_PIXEL_WIDTH)	// PosX ADC reading divisor
#define VER_DIV			((ADC_RDG_MAX + 1) / LCD_PIXEL_HEIGHT)	// PosY ADC reading divisor

#define OBJECT_TYPE		LCD_OBJ_POINT_2X2	// object to be rendered


void EtchClock(void)
{
	static int EtchState = ETCH_ST_INIT;
	static int PrevPosX;
	static int PrevPosY;

	int CurPosX = -1;	// ensure not equal to initial position
	int CurPosY = -1;	// ensure not equal to initial position

	// Get the current X,Y position, invert Y so clockwise rotation of right knob => up
	CurPosX = AdcLeftReadingGet() / HOR_DIV;
	CurPosY = (LCD_PIXEL_HEIGHT - 1 ) - (AdcRightReadingGet() / VER_DIV);

	LcdReverse(NavSelectIsPressed());
	if(AccelIsShaken())
	{
		LcdClear();
		EtchState = ETCH_ST_INIT;
	}

	switch (EtchState)
	{
	case ETCH_ST_INIT:
		LcdRenderObject(LCD_OBJ_POINT_2X2, CurPosX, CurPosY, LCD_MODE_CURSOR);			// render the cursor in its initial position
		RGB_RED_ENA();
		EtchState = ETCH_ST_CURSOR_2X2;
		break;

	case ETCH_ST_CURSOR_1X1:
		LcdRenderObject(LCD_OBJ_POINT_1X1, PrevPosX, PrevPosY, LCD_MODE_CURSOR);		// erase the previous cursor
		if(NavRightIsPressed())
		{
			LcdRenderObject(LCD_OBJ_POINT_2X2, CurPosX, CurPosY, LCD_MODE_CURSOR);		// render the new cursor
			EtchState = ETCH_ST_CURSOR_2X2;
		}
		else if(NavDownIsPressed())
		{
			LcdRenderObject(LCD_OBJ_POINT_1X1, CurPosX, CurPosY, LCD_MODE_STYLUS);		// render the new cursor
			RGB_RED_DIS();
			RGB_GRN_ENA();
			EtchState = ETCH_ST_STYLUS_1X1;
		}
		else
		{
			LcdRenderObject(LCD_OBJ_POINT_1X1, CurPosX, CurPosY, LCD_MODE_CURSOR);		// render the new cursor
		}
		break;

	case ETCH_ST_STYLUS_1X1:
		if(NavRightIsPressed())
		{
			LcdRenderObject(LCD_OBJ_POINT_2X2, CurPosX, CurPosY, LCD_MODE_STYLUS);		// render the new cursor
			EtchState = ETCH_ST_STYLUS_2X2;
		}
		else if(NavUpIsPressed())
		{
			LcdRenderObject(LCD_OBJ_POINT_1X1, CurPosX, CurPosY, LCD_MODE_CURSOR);		// render the new cursor
			RGB_GRN_DIS();
			RGB_RED_ENA();
			EtchState = ETCH_ST_CURSOR_1X1;
		}
		else
		{
			if ((CurPosX == PrevPosX) && (CurPosY == PrevPosY))
			{
				return;
			}
			LcdRenderObject(LCD_OBJ_POINT_1X1, CurPosX, CurPosY, LCD_MODE_STYLUS);		// render the stylus
		}
		break;
	case ETCH_ST_CURSOR_2X2:
		LcdRenderObject(LCD_OBJ_POINT_2X2, PrevPosX, PrevPosY, LCD_MODE_CURSOR);		// erase the previous cursor
		if(NavLeftIsPressed())
		{
			LcdRenderObject(LCD_OBJ_POINT_1X1, CurPosX, CurPosY, LCD_MODE_CURSOR);		// render the new cursor
			EtchState = ETCH_ST_CURSOR_1X1;
		}
		else if(NavDownIsPressed())
		{
			LcdRenderObject(LCD_OBJ_POINT_2X2, CurPosX, CurPosY, LCD_MODE_STYLUS);		// render the new cursor
			RGB_RED_DIS();
			RGB_GRN_ENA();
			EtchState = ETCH_ST_STYLUS_2X2;
		}
		else
		{
			LcdRenderObject(LCD_OBJ_POINT_2X2, CurPosX, CurPosY, LCD_MODE_CURSOR);		// render the stylus
		}
		break;
	case ETCH_ST_STYLUS_2X2:
		if(NavLeftIsPressed())
		{
			LcdRenderObject(LCD_OBJ_POINT_1X1, CurPosX, CurPosY, LCD_MODE_STYLUS);		// render the new cursor
			EtchState = ETCH_ST_STYLUS_1X1;
		}
		else if(NavUpIsPressed())
		{
			LcdRenderObject(LCD_OBJ_POINT_2X2, CurPosX, CurPosY, LCD_MODE_CURSOR);		// render the new cursor
			RGB_GRN_DIS();
			RGB_RED_ENA();
			EtchState = ETCH_ST_CURSOR_2X2;
		}
		else
		{
			if ((CurPosX == PrevPosX) && (CurPosY == PrevPosY))
			{
				return;
			}
			LcdRenderObject(LCD_OBJ_POINT_2X2, CurPosX, CurPosY, LCD_MODE_STYLUS);		// render the stylus
		}
		break;
	}

	// Save the current position
	PrevPosX = CurPosX;
	PrevPosY = CurPosY;
}
