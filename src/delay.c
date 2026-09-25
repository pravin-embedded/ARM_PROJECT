#include"type.h"
void delay_US(u32 delayUS)
{
	delayUS*=12;
	while(delayUS--);
}
void delay_MS(u32 delayMS)
{
	delayMS*=12000;
	while(delayMS--);
}
void delay_S(u32 delayS)
{
	delayS=12000000;
	while(delayS--);
}
