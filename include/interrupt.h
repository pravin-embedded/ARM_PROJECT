//interrupt.h
#include <lpc21xx.h>
#include "type.h"

extern int admin_mode;
extern int current_mode;
extern int pause_flag;
extern u32 old_min;

void EINT0_isr(void)__irq;
void EINT1_isr(void)__irq;
