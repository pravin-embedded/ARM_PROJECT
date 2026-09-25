//LM35.c
#include "type.h"
#include "ADC.h"
#include "adc_defines.h"
#include "delay.h"
f32 Read_LM35degC(void)
{
	u32 dval;
	f32 eAR,sum=0;
	u8 i;
	for(i=0;i<20;i++)
	{
	ADC_Read(CH3,&dval,&eAR);
	sum += eAR * 100;
	delay_MS(2);
	}
	return sum;
}
f32 Read_LM35degf(void)
{
	u8 tempc;
	tempc=Read_LM35degC();
	return ((tempc*(1.8))+32);
	
}