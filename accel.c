//
// COMP-GENG 421 - Tom Lupfer
//
// Accelerometer module
//

#include "main.h"
#include "accel.h"
#include "i2c.h"


#define ALERT_BITMASK			0x40		// bad reading alert bitmask

#define I2C_ADDR_ACCEL			0x98		// I2C address of accelerometer

#define MODE_REG_MODE			0x01		// mode reg MODE bit

#define REG_ADDR_XOUT			0x00		// x accel output reg addr
#define REG_ADDR_YOUT			0x01		// y accel output reg addr
#define REG_ADDR_ZOUT			0x02		// z accel output reg addr
#define REG_ADDR_TILT			0x03		// tilt status reg addr
#define REG_ADDR_SRST			0x04		// sampling rate status reg addr
#define REG_ADDR_SPCNT			0x05		// sleep count reg addr
#define REG_ADDR_INTSU			0x06		// interrupt setup reg addr
#define REG_ADDR_MODE			0x07		// mode reg addr
#define REG_ADDR_SR				0x08		// sample rate reg addr
#define REG_ADDR_PDET			0x09		// tap detection reg addr
#define REG_ADDR_PD				0x0a		// tap debounce count reg addr

#define SAMPLE_BYTES			0x03		// number of bytes per sample
#define SAMPLE_TICKS			100			// sample period (msec)

#define SHAKE_THRESHOLD			1400		// shake detection threshold

#define TILT_TH_TRUE			12			// X,Y accel threshold for tilt state to go true
#define TILT_TH_FALSE			7			// X,Y accel threshold for tilt state to go false

#define TILT_UP_ST_FALSE		0			// tilt up state is false
#define TILT_UP_ST_TRUE			1			// tilt up state is true

#define TILT_DOWN_ST_FALSE		0			// tilt down state is false
#define TILT_DOWN_ST_TRUE		1			// tilt down state is true

#define TILT_LEFT_ST_FALSE		0			// tilt left state is false
#define TILT_LEFT_ST_TRUE		1			// tilt left state is true

#define TILT_RIGHT_ST_FALSE		0			// tilt right state is false
#define TILT_RIGHT_ST_TRUE		1			// tilt right state is true


static Bool	IsTiltedUp = FALSE;				// board tilted up?
static Bool	IsTiltedDown = FALSE;			// board tilted down?
static Bool	IsTiltedLeft = FALSE;			// board tilted left?
static Bool	IsTiltedRight = FALSE;			// board tilted right?
static Bool IsShaken = FALSE;				// board shaken?


static void AccelSample(int * Ax, int * Ay, int * Az);


void AccelInit(void)
{
	UInt8 XmtBuf[1];

	// Clear the control registers
	XmtBuf[0] = 0x00;
	I2cWrite(I2C_ADDR_ACCEL, REG_ADDR_MODE,  0x01, XmtBuf);	// clear TON bit to get out of test mode
	I2cWrite(I2C_ADDR_ACCEL, REG_ADDR_INTSU, 0x01, XmtBuf);
	I2cWrite(I2C_ADDR_ACCEL, REG_ADDR_SR,    0x01, XmtBuf);
	I2cWrite(I2C_ADDR_ACCEL, REG_ADDR_PDET,  0x01, XmtBuf);
	I2cWrite(I2C_ADDR_ACCEL, REG_ADDR_PD,    0x01, XmtBuf);

	// Enable the active mode
	XmtBuf[0] = MODE_REG_MODE;
	I2cWrite(I2C_ADDR_ACCEL, REG_ADDR_MODE,  0x01, XmtBuf);
}


void AccelClock(void)
{
	static int	SampleTickCnt = SAMPLE_TICKS;

	static int	TiltUpState = TILT_UP_ST_FALSE;
	static int	TiltDownState = TILT_DOWN_ST_FALSE;
	static int	TiltLeftState = TILT_LEFT_ST_FALSE;
	static int	TiltRightState = TILT_RIGHT_ST_FALSE;

	int		AccelX;
	int 	AccelY;
	int 	AccelZ;

	// Read the accelerometer
	AccelSample(&AccelX, &AccelY, &AccelZ);

	// The sample period provides debouncing for tilt detection
	if (--SampleTickCnt > 0)
	{
		return;
	}
	SampleTickCnt = SAMPLE_TICKS;

	// As board is tilted up, AccelX goes negative
	switch(TiltUpState)
	{
	case TILT_UP_ST_FALSE:
		if (AccelX < -TILT_TH_TRUE)
		{
			IsTiltedUp = TRUE;
			TiltUpState = TILT_UP_ST_TRUE;
		}
		break;

	case TILT_UP_ST_TRUE:
		if (AccelX > -TILT_TH_FALSE)
		{
			IsTiltedUp = FALSE;
			TiltUpState = TILT_UP_ST_FALSE;
		}
		break;
	}

	// As board is tilted down, AccelX goes positive
	switch(TiltDownState)
	{
	case TILT_DOWN_ST_FALSE:
		if (AccelX > TILT_TH_TRUE)
		{
			IsTiltedDown = TRUE;
			TiltDownState = TILT_DOWN_ST_TRUE;
		}
		break;

	case TILT_DOWN_ST_TRUE:
		if (AccelX < TILT_TH_FALSE)
		{
			IsTiltedDown = FALSE;
			TiltDownState = TILT_DOWN_ST_FALSE;
		}
		break;
	}

	// As board is tilted left, AccelY goes positive
	switch(TiltLeftState)
	{
	case TILT_LEFT_ST_FALSE:
		if (AccelY > TILT_TH_TRUE)
		{
			IsTiltedLeft = TRUE;
			TiltLeftState = TILT_LEFT_ST_TRUE;
		}
		break;

	case TILT_LEFT_ST_TRUE:
		if (AccelY < TILT_TH_FALSE)
		{
			IsTiltedLeft = FALSE;
			TiltLeftState = TILT_LEFT_ST_FALSE;
		}
		break;
	}

	// As board is tilted right, AccelY goes negative
	switch(TiltRightState)
	{
	case TILT_RIGHT_ST_FALSE:
		if (AccelY < -TILT_TH_TRUE)
		{
			IsTiltedRight = TRUE;
			TiltRightState = TILT_RIGHT_ST_TRUE;
		}
		break;

	case TILT_RIGHT_ST_TRUE:
		if (AccelY > -TILT_TH_FALSE)
		{
			IsTiltedRight = FALSE;
			TiltRightState = TILT_RIGHT_ST_FALSE;
		}
		break;
	}
}


Bool AccelIsTiltedUp(void)
{
	return IsTiltedUp;
}


void AccelClearIsTiltedUp(void)
{
	IsTiltedUp = FALSE;
}


Bool AccelIsTiltedDown(void)
{
	return IsTiltedDown;
}


void AccelClearIsTiltedDown(void)
{
	IsTiltedDown = FALSE;
}


Bool AccelIsTiltedLeft(void)
{
	return IsTiltedLeft;
}


void AccelClearIsTiltedLeft(void)
{
	IsTiltedLeft = FALSE;
}


Bool AccelIsTiltedRight(void)
{
	return IsTiltedRight;
}


void AccelClearIsTiltedRight(void)
{
	IsTiltedRight = FALSE;
}


Bool AccelIsShaken(void)
{
	return IsShaken;
}


void AccelClearIsShaken(void)
{
	IsShaken = FALSE;
}


static void AccelSample(int * Ax, int * Ay, int * Az)
{
	UInt8 RcvBuf[SAMPLE_BYTES];

	// Read acceleration values until no alert bits are set
	do
	{
		I2cRead(I2C_ADDR_ACCEL, REG_ADDR_XOUT, SAMPLE_BYTES, RcvBuf);
	}
	while ((RcvBuf[0] | RcvBuf[1] | RcvBuf[2]) & ALERT_BITMASK);

	// Sign-extend the 6-bit readings
	*Ax = (int) ((RcvBuf[0] << 26) >> 26);
	*Ay = (int) ((RcvBuf[1] << 26) >> 26);
	*Az = (int) ((RcvBuf[2] << 26) >> 26);

	IsShaken = ((((*Ax * *Ax) + (*Ay * *Ay)) > SHAKE_THRESHOLD) ||
				(((*Ay * *Ay) + (*Az * *Az)) > SHAKE_THRESHOLD) ||
				(((*Az * *Az) + (*Ax * *Ax)) > SHAKE_THRESHOLD));
}
