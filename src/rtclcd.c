//rtclcd.c
#include <lpc21xx.h>

#define PCLK 15000000

#define PREINT_VALUE  ((PCLK/32768)-1)
#define PREFRAC_VALUE (PCLK-((PREINT_VALUE+1)*32768))

void RTC_Init(void)
{
    CCR = (1<<1);          // Reset RTC

    PREINT  = PREINT_VALUE;
    PREFRAC = PREFRAC_VALUE;

    CCR = 0x00;            // Clear reset

    CCR = (1<<0);          // Enable RTC
}
