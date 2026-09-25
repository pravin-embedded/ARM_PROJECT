//adc_defines.c
#include <lpc21xx.h>
#include "delay.h"
#include "adc_defines.h"
void ADC_Init(void)
{
	PINSEL1|0x154000001;
	ADCR = PDN_BIT|CLKDIV_VALUE;
}
void ADC_Read(u32 CHN3, u32 *AdcDval,f32* eAR)
{
	ADCR&=~(255<<0);
	ADCR|=CHN3|START_CONV;
	delay_US(10);
	while(((ADDR>>DONE_BIT)&1)==0);
	ADCR&=~(START_CONV);
	*AdcDval=((ADDR >> RESULT)&1023);
	*eAR=(3.3*(*AdcDval*100))/1023;
}
