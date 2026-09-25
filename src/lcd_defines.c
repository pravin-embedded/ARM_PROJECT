//lcd_defines.c
#include <lpc21xx.h>
#include "delay.h"
#include "lcd.h"
#define EN (1<<9)
#define RS (1<<8)
void LCD_Init()
{
    IODIR0 |= 0xff;
    IODIR0 |= RS;
    IODIR0 |= EN;

    delay_MS(15);
		LCD_Cmd(0x30);
		delay_MS(5);
		LCD_Cmd(0x30);
		delay_US(100);
		LCD_Cmd(0x30);
    LCD_Cmd(0x38);
    LCD_Cmd(0x0F);
    LCD_Cmd(0x01);
		LCD_Cmd(0x06);
    
}
void LCD_Cmd(unsigned int cmd)
{
    IOCLR0 = 0xff | RS | EN;

    IOSET0 = (cmd);

    IOSET0 = EN;

    delay_US(1);

    IOCLR0 = EN;

    delay_MS(2);
}
void LCD_Char(unsigned char data)
{
	IOCLR0 = 0xff;
	IOSET0 = RS;
	IOSET0 = (data);
	IOSET0 = EN;
	delay_US(1);
	IOCLR0 = EN;
	delay_MS(2);
}
void LCD_str( char *str)
{
	while(*str)
	{
		LCD_Char(*str);
		str++;
	}
}
void LCD_U32(unsigned int num)
{
		int arr[10];
		int i=0,t=num;
		if(num==0)
		{
			LCD_Char('0');
			return;
		}
		while(t)
		{
			arr[i++] = t%10;
			t=t/10;
		}
		for(i=i-1;i>=0;i--)
			{
			LCD_Char((arr[i] + 48));
			}
}
void LCD_F32(float f)
{
    int ipart;
    int frac;

    ipart = (int)f;

    frac = (f - ipart) * 100;

    LCD_U32(ipart);

    LCD_Char('.');

    LCD_U32(frac);
}

void LCD_2Digit(unsigned int n)
{
    if(n < 10)
        LCD_Char('0');

    LCD_U32(n);

}
void LCD_Num(u32 num)
{
    char buf[10];
    int i=0,j;

    if(num==0)
    {
        LCD_Char('0');
        return;
    }

    while(num)
    {
        buf[i++] = (num%10)+'0';
        num/=10;
    }

    for(j=i-1;j>=0;j--)
        LCD_Char(buf[j]);
}
