//
// COMP-GENG 421 - Tom Lupfer
//
// ADC (Analog-to-Digital Converter) module header file
//

#ifndef ADC_H_
#define ADC_H_

#define ADC_RDG_MIN		0
#define ADC_RDG_MAX		4095	// 12-bit unsigned ADC reading


void AdcInit(void);
UInt AdcLeftReadingGet(void);
UInt AdcRightReadingGet(void);

#endif // ADC_H_
