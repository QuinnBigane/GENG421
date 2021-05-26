//
// COMP-GENG 421 - Tom Lupfer
//
// Accelerometer module header file
//

#ifndef ACCEL_H_
#define ACCEL_H_

void AccelInit(void);
void AccelClock(void);
Bool AccelIsTiltedUp(void);
void AccelClearIsTiltedUp(void);
Bool AccelIsTiltedDown(void);
void AccelClearIsTiltedDown(void);
Bool AccelIsTiltedLeft(void);
void AccelClearIsTiltedLeft(void);
Bool AccelIsTiltedRight(void);
void AccelClearIsTiltedRight(void);
Bool AccelIsShaken(void);
void AccelClearIsShaken(void);

#endif // ACCEL_H_
