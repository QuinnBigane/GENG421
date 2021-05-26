//
// COMP-GENG 421 - Tom Lupfer
//
// Session 28 - Example code for Final Project
//

#include "main.h"
#include "accel.h"
#include "adc.h"
#include "clock.h"
#include "colors.h"
#include "dbg.h"
#include "etch.h"
#include "i2c.h"
#include "lcd.h"
#include "nav.h"
#include "rgb.h"
#include "uart.h"


int main(void)
{
	// Initialize the MCU hardware required for debugging first
	ClockInit();
	DbgInit();
	UartInit();

	// Initialize the remaining MCU hardware
	AdcInit();
	I2cInit();

	// Initialize the external peripherals
	AccelInit();
	LcdInit();
	NavInit();
	RgbInit();

	// Main loop
	while(TRUE)
	{
		// Wait for tick timer
		ClockWaitForTick();

		// Clock the state machines
DBG_SET();
		AccelClock();
		NavClock();
		RgbClock();
		EtchClock();
DBG_CLR();

	}	// End - main loop
}
