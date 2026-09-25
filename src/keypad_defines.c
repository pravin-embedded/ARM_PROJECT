//keypad_defines.c
#include <lpc21xx.h>
#include "keypadfunc.h"
#include "type.h"
#include "delay.h"
#include "lcd.h"
#define ROW 0x000F0000
#define COL 0x00F00000
u8 KpmLUT[4][4] =
{
 {'1','2','3','/'},
 {'4','5','6','*'},
 {'7','8','9','-'},
 {'C','0','=','+'}
};

int column(void)
{
    if(((IOPIN1 >> 20) & 0x0f) != 0x0f)
        return 0;      

    return 1;          
}

unsigned int row(void)
{
    u32 rno;

    for(rno=0; rno<4; rno++)
    {
        IOCLR1 = ROW;                          // all rows LOW
        IOSET1 = ((~(1<<(rno+16))) & ROW);     // only ROW bits affected

        if(column() == 0)
            break;
    }

    IOCLR1 = ROW;

    return rno;
}

unsigned int colcheck(void)
{
    u32 cno;

    for(cno=0; cno<4; cno++)
    {
        if(((IOPIN1 >> (cno+20)) & 1) == 0)
            break;
    }

    return cno;
}

unsigned int keyscan(void)
{
		u8 key;
    u32 rno,cno;

    while(column());

    delay_MS(20);

    rno = row();
    cno = colcheck();

    if(rno >= 4 || cno >= 4)
        return 0;

    key = KpmLUT[rno][cno];

    while(!column());

    delay_MS(20);

    return key;
}
u32 ReadNum(void)
{
	u8 key;
	u32 sum=0;
 while(1)
    {
        key = keyscan();
				if(key >= '0' && key <= '9')
				{
						LCD_Char(key);
						sum = (sum*10)+(key-'0');
				}
				else if(key=='C')
				{
						sum = sum/10;      // remove last digit

						LCD_Cmd(0x10);    // cursor left
						LCD_Char(' ');    // erase char
						LCD_Cmd(0x10);    // cursor left again
				}
				else if(key=='=')
				{
						break;
				}
    }
		return sum;
}
u32 ReadPassword(void)
{
    u32 pass=0;
    u8 key;
		u8 count=0;

    while(1)
    {
        key = keyscan();

       if(key>='0' && key<='9')
				{
						pass = pass*10 + (key-'0');
						LCD_Char('*');
						count++;
				}
			 else if(key=='C')
				{
						 if(count > 0)
            {
                pass = pass/10;
                count--;

                LCD_Cmd(0x10);
                LCD_Char(' ');
                LCD_Cmd(0x10);
            }
				}
			 else if(key=='=')
				{
						break;
				}
				else if(key=='*')
				{
						return 0;
				}
    }

    return pass;
}
