//
// COMP-GENG 421 - Tom Lupfer
//
// Analog-to-Digital Converter (ADC) module
//

#include "main.h"
#include "adc.h"


void AdcInit(void)
{
	// Enable the GPIOA port clock
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);

	// Configure PA0 as ADC_IN0 (left knob), PA1 as ADC_IN1 (right knob)
	LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_0, LL_GPIO_MODE_ANALOG);
	LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_1, LL_GPIO_MODE_ANALOG);

	// Enable the clock for ADC1
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_ADC1);

	// Set the conversion trigger source to software start
	LL_ADC_REG_SetTriggerSource(ADC1, LL_ADC_REG_TRIG_SOFTWARE);

	// Configure for single conversion mode of a single channel
	LL_ADC_REG_SetContinuousMode(ADC1, LL_ADC_REG_CONV_SINGLE);
	LL_ADC_REG_SetSequencerLength(ADC1, LL_ADC_REG_SEQ_SCAN_DISABLE);
	LL_ADC_REG_SetFlagEndOfConversion(ADC1, LL_ADC_REG_FLAG_EOC_UNITARY_CONV);
}


UInt AdcLeftReadingGet(void)
{
	// Set the ADC channel
	LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_1, LL_ADC_CHANNEL_0);

	// Enable the ADC, wait for it to be ready
	LL_ADC_Enable(ADC1);
	while (!LL_ADC_IsActiveFlag_ADRDY(ADC1))
		;

	// Start the conversion, wait for it to complete
	LL_ADC_REG_StartConversionSWStart(ADC1);
	while (!LL_ADC_IsActiveFlag_EOCS(ADC1))
		;

	// Disable the ADC
	LL_ADC_Disable(ADC1);

	return LL_ADC_REG_ReadConversionData12(ADC1);
}


UInt AdcRightReadingGet(void)
{
	// Set the ADC channel
	LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_1, LL_ADC_CHANNEL_1);

	// Enable the ADC, wait for it to be ready
	LL_ADC_Enable(ADC1);
	while (!LL_ADC_IsActiveFlag_ADRDY(ADC1))
		;

	// Start the conversion, wait for it to complete
	LL_ADC_REG_StartConversionSWStart(ADC1);
	while (!LL_ADC_IsActiveFlag_EOCS(ADC1))
		;

	// Disable the ADC
	LL_ADC_Disable(ADC1);

	return LL_ADC_REG_ReadConversionData12(ADC1);
}
