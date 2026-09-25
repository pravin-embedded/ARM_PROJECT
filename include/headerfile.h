//headerfile.h

#include <lpc21xx.h>
#include "ADC.h"
#include "delay.h"
#include "type.h"
#include "lcd.h"
#include "rtc.h"
#include "LM35.h"
#include "interrupt.h"
#include "keypadfunc.h"
#include "Project.h"
#define EINT1_CH 15
#define EINT0_CH 14
#define ROW 0x000F0000
#define COL 0x00F00000
#define GREEN_LED  (1<<27)
#define YELLOW_LED (1<<28)
#define RED_LED    (1<<29)
#define BUZZER (1<<24)

extern u32 password;
extern s32 admin_mode;
extern s32 current_mode;
extern s32 pause_flag;
extern u32 old_min;

extern u32 start_hour;
extern u32 start_min;
extern u32 start_sec;

extern u32 end_hour;
extern u32 end_min;
extern u32 end_sec;
