//
// COMP-GENG 421 - Tom Lupfer
//
// RGB LED color effects module
//

#include "main.h"
#include "colors.h"
#include "rgb.h"


#define RED_UP	 0			// red LED fade up state
#define RED_DN	 1			// red LED fade down state
#define GRN_UP	 2			// green LED fade up state
#define GRN_DN	 3			// green LED fade down state
#define BLU_UP	 4			// blue LED fade up state
#define BLU_DN	 5			// blue LED fade down state
#define RG_UP	 6			// red+green LED fade up state
#define RG_DN	 7			// red+green LED fade down state
#define GB_UP	 8			// green+blue LED fade up state
#define GB_DN	 9			// green+blue LED fade down state
#define BR_UP	10			// blue+red LED fade up state
#define BR_DN	11			// blue+red LED fade down state
#define RGB_UP	12			// red+green+blue LED fade up state
#define RGB_DN	13			// red+green+blue LED fade down state

#define DLY_100MSEC	100		// number of ticks in 100 msec


void ColorsClock(void)
{
	static int	ColorsState = RED_UP;
	static int	DelayCount = DLY_100MSEC;
	static int	PwmSetting = RGB_PWM_OFF;

	// Delay 100 msec between clocks of the state machine
	if (--DelayCount > 0)
	{
		return;
	}
	DelayCount = DLY_100MSEC;

	switch(ColorsState)
	{
	case RED_UP:
		RgbRedPwmSet(PwmSetting++);
		if (PwmSetting > RGB_PWM_MAX)
		{
			PwmSetting = RGB_PWM_MAX - 1;
			ColorsState = RED_DN;
		}
		break;

	case RED_DN:
		RgbRedPwmSet(PwmSetting--);
		if (PwmSetting < RGB_PWM_OFF)
		{
			PwmSetting = RGB_PWM_OFF + 1;
			ColorsState = GRN_UP;
		}
		break;
	case GRN_UP:
		RgbGrnPwmSet(PwmSetting++);
		if (PwmSetting > RGB_PWM_MAX)
		{
			PwmSetting = RGB_PWM_MAX - 1;
			ColorsState = GRN_DN;
		}
		break;

	case GRN_DN:
		RgbGrnPwmSet(PwmSetting--);
		if (PwmSetting < RGB_PWM_OFF)
		{
			PwmSetting = RGB_PWM_OFF + 1;
			ColorsState = BLU_UP;
		}
		break;
	case BLU_UP:
		RgbBluPwmSet(PwmSetting++);
		if (PwmSetting > RGB_PWM_MAX)
		{
			PwmSetting = RGB_PWM_MAX - 1;
			ColorsState = BLU_DN;
		}
		break;

	case BLU_DN:
		RgbBluPwmSet(PwmSetting--);
		if (PwmSetting < RGB_PWM_OFF)
		{
			PwmSetting = RGB_PWM_OFF + 1;
			ColorsState = RG_UP;
		}
		break;
	case RG_UP:
		RgbRedPwmSet(PwmSetting);
		RgbGrnPwmSet(PwmSetting++);
		if (PwmSetting > RGB_PWM_MAX)
		{
			PwmSetting = RGB_PWM_MAX - 1;
			ColorsState = RG_DN;
		}
		break;

	case RG_DN:
		RgbRedPwmSet(PwmSetting);
		RgbGrnPwmSet(PwmSetting--);
		if (PwmSetting < RGB_PWM_OFF)
		{
			PwmSetting = RGB_PWM_OFF + 1;
			ColorsState = GB_UP;
		}
		break;
	case GB_UP:
		RgbGrnPwmSet(PwmSetting);
		RgbBluPwmSet(PwmSetting++);
		if (PwmSetting > RGB_PWM_MAX)
		{
			PwmSetting = RGB_PWM_MAX - 1;
			ColorsState = GB_DN;
		}
		break;

	case GB_DN:
		RgbGrnPwmSet(PwmSetting);
		RgbBluPwmSet(PwmSetting--);
		if (PwmSetting < RGB_PWM_OFF)
		{
			PwmSetting = RGB_PWM_OFF + 1;
			ColorsState = BR_UP;
		}
		break;
	case BR_UP:
		RgbBluPwmSet(PwmSetting);
		RgbRedPwmSet(PwmSetting++);
		if (PwmSetting > RGB_PWM_MAX)
		{
			PwmSetting = RGB_PWM_MAX - 1;
			ColorsState = BR_DN;
		}
		break;

	case BR_DN:
		RgbBluPwmSet(PwmSetting);
		RgbRedPwmSet(PwmSetting--);
		if (PwmSetting < RGB_PWM_OFF)
		{
			PwmSetting = RGB_PWM_OFF + 1;
			ColorsState = RGB_UP;
		}
		break;
	case RGB_UP:
		RgbRedPwmSet(PwmSetting);
		RgbGrnPwmSet(PwmSetting);
		RgbBluPwmSet(PwmSetting++);
		if (PwmSetting > RGB_PWM_MAX)
		{
			PwmSetting = RGB_PWM_MAX - 1;
			ColorsState = RGB_DN;
		}
		break;

	case RGB_DN:
		RgbRedPwmSet(PwmSetting);
		RgbGrnPwmSet(PwmSetting);
		RgbBluPwmSet(PwmSetting--);
		if (PwmSetting < RGB_PWM_OFF)
		{
			PwmSetting = RGB_PWM_OFF + 1;
			ColorsState = RED_UP;
		}
		break;
	}
}
