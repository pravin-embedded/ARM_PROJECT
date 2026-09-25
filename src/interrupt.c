//interrupt.c
#include "headerfile.h"

void EINT0_isr(void)__irq
{
	current_mode = 1;
	admin_mode = 1;

	EXTINT = 1<<0;
	VICVectAddr = 0;
}

void EINT1_isr(void)__irq
{
	pause_flag = !pause_flag;
	old_min = MIN;
	//IOSET0 = YELLOW_LED;

	EXTINT = 1<<1;
	VICVectAddr = 0;
}
