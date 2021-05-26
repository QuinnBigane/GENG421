//
// COMP-GENG 421 - Tom Lupfer
//
// DAC (Digital-to-Analog Converter) module
//

#include <math.h>

#include "main.h"
#include "dac.h"


#define RAMP_OUTPUTx
#define SIN_OUTPUTx
#define LUT_OUTPUT

#define DAC_VAL_MIN		0
#define DAC_VAL_MAX		4095
#define DAC_VAL_RANGE	(DAC_VAL_MAX - DAC_VAL_MIN + 1)
#define DAC_VAL_MID		(DAC_VAL_RANGE / 2)

#define PI				3.14159
#define DEG_2_RAD(DEG)	((PI * DEG) / 180.0)

#define SIN_MULT		10000


#ifdef LUT_OUTPUT
const static UInt SineArray[90] =
{
       0,  174,  348,  523,  697,
     871, 1045, 1218, 1391, 1564,
    1736, 1908, 2079, 2249, 2419,
    2588, 2756, 2923, 3090, 3255,
    3420, 3583, 3746, 3907, 4067,
    4226, 4383, 4539, 4694, 4848,
    4999, 5150, 5299, 5446, 5591,
    5735, 5877, 6018, 6156, 6293,
    6427, 6560, 6691, 6819, 6946,
    7071, 7193, 7313, 7431, 7547,
    7660, 7771, 7880, 7986, 8090,
    8191, 8290, 8386, 8480, 8571,
    8660, 8746, 8829, 8910, 8987,
    9063, 9135, 9205, 9271, 9335,
    9396, 9455, 9510, 9563, 9612,
    9659, 9702, 9743, 9781, 9816,
    9848, 9876, 9902, 9925, 9945,
    9961, 9975, 9986, 9993, 9998,
};
#endif	// LUT_OUTPUT

static void DacValueSet(UInt Val);

#ifdef LUT_OUTPUT
static UInt Deg2DacValue(UInt Degrees);
#endif	// LUT_OUTPUT


void DacInit(void)
{
	// Enable the GPIOA port clock
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);

	// Configure PA4 as DAC_OUT1
	LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_4, LL_GPIO_MODE_ANALOG);

	// Enable the clock for DAC1
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_DAC1);

	// Enable the output buffer for DAC1 channel 1
	LL_DAC_SetOutputBuffer(DAC1, LL_DAC_CHANNEL_1, LL_DAC_OUTPUT_BUFFER_ENABLE);

	// Enable DAC1 channel 1
	LL_DAC_Enable(DAC1, LL_DAC_CHANNEL_1);
}


void DacClock(void)
{
#ifdef RAMP_OUTPUT
	static UInt Incr = 0;

	DacValueSet(Incr++);
#endif	// RAMP_OUTPUT

#ifdef SIN_OUTPUT
	static UInt Degrees = 0;

	// Offset sin(x) by 1.0 to be in the range [0,2], multiply by half the range of DAC values
	DacValueSet((sin(DEG_2_RAD(Degrees++)) + 1.0) * (DAC_VAL_RANGE / 2));
#endif	// SIN_OUTPUT

#ifdef LUT_OUTPUT
	static UInt Degrees = 0;

	DacValueSet(Deg2DacValue(Degrees++));
#endif	// LUT_OUTPUT
}


static void DacValueSet(UInt Val)
{
	LL_DAC_ConvertData12RightAligned(DAC1, LL_DAC_CHANNEL_1, Val);
}


#ifdef LUT_OUTPUT
static UInt Deg2DacValue(UInt Degrees)
{
	Degrees %= 360;

	// First quadrant [0-89]
	if (Degrees < 90)
	{
		return DAC_VAL_MID + (DAC_VAL_MID * SineArray[Degrees] / SIN_MULT);
	}
	// Second quadrant [90-179]
	if (Degrees < 180)
	{
		return DAC_VAL_MID + (DAC_VAL_MID * SineArray[179 - Degrees] / SIN_MULT);
	}
	// Third quadrant [180-269]
	if (Degrees < 270)
	{
		return DAC_VAL_MID - (DAC_VAL_MID * SineArray[Degrees - 180] / SIN_MULT);
	}
	// Fourth quadrant [270-359]
	return DAC_VAL_MID - (DAC_VAL_MID * SineArray[359 - Degrees] / SIN_MULT);
}
#endif	// LUT_OUTPUT
