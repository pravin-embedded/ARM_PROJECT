//Project_defines.c
#include <lpc21xx.h>
#include "type.h"
#include "Project.h"
#include "headerfile.h"
#include "Project_defines.h"
static u8 screen=0;
u32 password = 1234;

s32 admin_mode = 0;
s32 current_mode = 0;
s32 pause_flag = 1;
u8 screen_time = 0;

u32 old_min = 0;
u32 old_sec = 0;

u32 start_hour = 0;
u32 start_min = 0;
u32 start_sec = 0;

u32 end_hour = 0;
u32 end_min = 0;
u32 end_sec = 0;
u32 display_time=0;
u32 mux_time = 0;

u32 exam_start_hour=0;
u32 exam_start_min=0;
u32 exam_start_sec=0;
u32 exam_running=0;

u32 i = 0;
s8 key ='1';

u32 pass = 0;
u8 option = '0';

u32 exam_time = 0;
u32 exam_time_max = 0;

f32 tempc = 0;

s32 exam_over_flag = 0;
s32 result_displayed = 0;
s32 pause_screen_shown = 0;

const unsigned char SegLUT[10] =
{
    0xC0, //0
    0xF9, //1
    0xA4, //2
    0xB0, //3
    0x99, //4
    0x92, //5
    0x82, //6
    0xF8, //7
    0x80, //8
    0x90  //9
};

  	void Display_2Digit(u32 display_time)
    {
         u32 tens, ones;
         volatile int d;

         tens = display_time / 10;
         ones = display_time % 10;

      /* ---------- ONES DIGIT ---------- */

           /* Disable both digits */
         IOCLR0 = (1<<10) | (1<<25);

           /* Turn OFF all segments */
		     IOCLR0 = (0x7F<<17);

           /* Load segment data */
         IOSET0 = (SegLUT[ones] << 17);

           /* Enable ones digit */
         IOSET0 = (1<<25);

         for(d=0; d<50000; d++);

        /* Disable ones digit */
         IOCLR0 = (1<<25);

       /* ---------- TENS DIGIT ---------- */

       /* Turn OFF all segments */
         IOCLR0 = (0x7F<<17);

       /* Load segment data */
         IOSET0 = (SegLUT[tens] << 17);

        /* Enable tens digit */
         IOSET0 = (1<<10);

         for(d=0; d<50000; d++);

        /* Disable tens digit */
         IOCLR0 = (1<<10);
    }
		void Hardware_Init(void)
		{
			LCD_Init();
			ADC_Init();
			RTC_Init();
		//Key_pad matrix config.
			IODIR1 |= ROW;     
			IODIR1 &= ~COL;   
			PINSEL1 = 0x15400001;
			VICIntSelect = 0<<EINT0_CH;
			VICIntEnable = 1<<EINT0_CH;
			VICVectAddr0 = (unsigned int)EINT0_isr;
			VICVectCntl0 = (1<<5)| EINT0_CH;
			EXTMODE = 1<<0;
			
			// Configure P0.14 as EINT1
			PINSEL0 &= ~(3<<28);
			PINSEL0 |=  (2<<28);
			// Configure VIC
			VICIntSelect &= ~(1<<EINT1_CH);
			VICIntEnable |=  (1<<EINT1_CH);
			VICVectAddr1 = (unsigned int)EINT1_isr;
			VICVectCntl1 = (1<<5) | EINT1_CH;
			// Edge Triggered
			EXTMODE |= (1<<1);
			// Falling Edge
			EXTPOLAR &= ~(1<<1);
			IODIR0 |= (0x7F<<17);
			IODIR0 |= (1<<10)|(1<<25);
			PINSEL1 &= ~((3<<22)|(3<<24)|(3<<26)|(1<<28));
			PINSEL1 |=(1<<28);
			IODIR0|= ((GREEN_LED) | (YELLOW_LED) | (RED_LED));
			IODIR1 |= BUZZER;
			IOCLR1 = BUZZER;
		}
		void RTC_Time_Setup(void)
		{
				LCD_Cmd(0x01);
				LCD_str("Enter Date");
				LCD_Cmd(0xC0);
				DOM = ReadNum();

				LCD_Cmd(0x01);
				LCD_str("Enter Month");
				LCD_Cmd(0xC0);
				MONTH = ReadNum();

				LCD_Cmd(0x01);
				LCD_str("Enter Year");
				LCD_Cmd(0xC0);
				YEAR = ReadNum();

				LCD_Cmd(0x01);
				LCD_str("Enter Hour");
				LCD_Cmd(0xC0);
				HOUR = ReadNum();

				LCD_Cmd(0x01);
				LCD_str("Enter Min");
				LCD_Cmd(0xC0);
				MIN = ReadNum();

				LCD_Cmd(0x01);
				LCD_str("Enter Sec");
				LCD_Cmd(0xC0);
				SEC = ReadNum();

				old_min = MIN;
		}
		
		void Countdown_Update(void)
		{
				for(i=0;i<200;i++)
				{
						if(exam_running)
						{
								/* Display countdown on 7-segment */
								Display_2Digit(mux_time);
						}
						else
						{
								/* Turn OFF both digit enables */
								IOCLR0 = (1<<24) | (1<<25);

								/* Turn OFF all segments */
								IOCLR0 = (0x7F<<17);
						}
				}
		}
		void Pause_Display(void)
		{
			if(pause_flag==0)
			{
					if(pause_screen_shown==0)
					{
							LCD_Cmd(0x01);
							LCD_Cmd(0x85);
							LCD_str("PAUSE");

							pause_screen_shown = 1;
					}
			}
		else
		{
				pause_screen_shown = 0;
		}
		}
		void Normal_Display(void)
		{
			if(pause_flag == 0)
				return;
			if(SEC != old_sec)
			{
					old_sec = SEC;
					screen_time++;
					tempc = Read_LM35degC();
					if(screen == 0)
					{
							if(screen_time >= 10)
							{
									screen = 1;
									screen_time = 0;
									LCD_Cmd(0x01);
							}
					}
					else
					{
							if(screen_time >= 3)
							{
									screen = 0;
									screen_time = 0;
									LCD_Cmd(0x01);
							}
					}
			}
				if(!exam_running)
				{
								if((HOUR==exam_start_hour) &&
									 (MIN==exam_start_min) &&
									 (SEC==exam_start_sec))
								{
										exam_running=1;

										start_hour=HOUR;
										start_min=MIN;
										start_sec=SEC;
										old_min = MIN;

										LCD_Cmd(0x01);
										LCD_str("Exam Started");
										delay_MS(1000);
								}
				}
				if(exam_running)
				{
				if((MIN != old_min) && (pause_flag==1))
				{
						old_min = MIN;

						if(exam_time > 0)
						{
								exam_time--;

								if(mux_time > 0)
										mux_time--;
						}

						if(mux_time==0 && exam_time>0)
						{
								if(exam_time>=60)
										mux_time=60;
								else
										mux_time=exam_time;
							}
						}
				}
				
				
				if(screen == 0)
				{
					LCD_Cmd(0x80);
					LCD_str("Time:");
					LCD_2Digit(HOUR);
					LCD_Char(':');
					LCD_2Digit(MIN);
					LCD_Char(':');
					LCD_2Digit(SEC);

					tempc = Read_LM35degC();

					LCD_Cmd(0xC0);
					LCD_str("Temp:");
					LCD_F32(tempc);
					LCD_Char(0xDF);
					LCD_Char('C');
					LCD_str("     ");
				}
				else
				{
					LCD_Cmd(0x80);

					LCD_str("Date:");
					LCD_2Digit(DOM);
					LCD_Char('/');
					LCD_2Digit(MONTH);
					LCD_Char('/');
					LCD_2Digit(YEAR%100);

					LCD_Cmd(0xC0);

					if(exam_running)
					{
							LCD_str("Exam:");
							LCD_Num(exam_time);
					}
					else if(exam_time_max != 0)
					{
							LCD_str("Start:");
							LCD_2Digit(exam_start_hour);
							LCD_Char(':');
							LCD_2Digit(exam_start_min);
							LCD_Char(':');
							LCD_2Digit(exam_start_sec);
					}
					else
					{
							LCD_str("Ready");
					}

					LCD_str("    ");
			}	
		}
		
		void Exam_Over_Check(void)
		{
			if(exam_running)
			{
			if(exam_time > (exam_time_max*70)/100)
				{
						IOSET0 = GREEN_LED;
						IOCLR0 = YELLOW_LED | RED_LED;
				}
				else if(exam_time > (exam_time_max*30)/100)
				{
						IOSET0 = YELLOW_LED;
						IOCLR0 = GREEN_LED | RED_LED;
				}
				else
				{
						IOSET0 = RED_LED;
						IOCLR0 = GREEN_LED | YELLOW_LED;
				}
			}
				
			if(current_mode == 0)
			{
					if(exam_time == 0 && exam_time_max != 0 && exam_over_flag == 0)
					{
							IOSET1 = BUZZER;
							LCD_Cmd(0x01);
							LCD_str("EXAM OVER");
							delay_MS(2000);
							end_hour = HOUR;
							end_min  = MIN;
							end_sec  = SEC;

							exam_over_flag = 1;
							exam_running = 0;
							exam_start_hour = 0;
							exam_start_min  = 0;
							exam_start_sec  = 0;
							exam_running = 0; 
					}
					else
					{
							IOCLR1 = BUZZER;
					}
			}
			else
			{
					IOCLR1 = BUZZER;
			}
			}
		void Show_Result(void)
		{
			if(exam_over_flag && result_displayed == 0)
			{
					LCD_Cmd(0x01);

					LCD_str("S:");
					LCD_2Digit(start_hour);
					LCD_Char(':');
					LCD_2Digit(start_min);

					LCD_Cmd(0xC0);

					LCD_str("E:");
					LCD_2Digit(end_hour);
					LCD_Char(':');
					LCD_2Digit(end_min);

					delay_MS(6000);
					result_displayed = 1;
					exam_time = 0;
					exam_time_max = 0;
					mux_time = 0;
			}
		
		}
		void Admin_Menu(void)
		{
			if(current_mode==1)
			{
				{
					admin_mode = 0;

					LCD_Cmd(0x01);
					LCD_str("ADMIN MODE");
					LCD_Cmd(0xC0);
					LCD_str("Enter Pass:");
					
					pass=ReadPassword();
					LCD_str("                   ");
					if(pass==password)
					{
						while(current_mode == 1)
						{
							LCD_Cmd(0x01);
							LCD_str("1.Set Exam");
							LCD_Cmd(0xC0);
							LCD_str("2.More");

							option = keyscan();

							if(option=='1')
							{
									LCD_Cmd(0x01);
									LCD_str("Set Exam Time");
									LCD_Cmd(0x01);
									LCD_str("Start Hr");
									LCD_Cmd(0xC0);
									exam_start_hour = ReadNum();

									LCD_Cmd(0x01);
									LCD_str("Start Min");
									LCD_Cmd(0xC0);
									exam_start_min = ReadNum();

									LCD_Cmd(0x01);
									LCD_str("Start Sec");
									LCD_Cmd(0xC0);
									exam_start_sec = ReadNum();

									if(HOUR > exam_start_hour)
									{
											LCD_Cmd(0x01);
											LCD_str("Invalid Time");
											delay_MS(1000);
											continue;
									}
									else if(HOUR == exam_start_hour && MIN > exam_start_min)
									{
											LCD_Cmd(0x01);
											LCD_str("Invalid Time");
											delay_MS(1000);
											continue;
									}
									else if(HOUR == exam_start_hour &&
													MIN == exam_start_min &&
													SEC > exam_start_sec)
									{
											LCD_Cmd(0x01);
											LCD_str("Invalid Time");
											delay_MS(1000);
											continue;
									}
									LCD_Cmd(0x01);
									LCD_str("Duration");
									LCD_Cmd(0xC0);
									exam_time = ReadNum();

									exam_time_max = exam_time;

									if(exam_time>=60)
											mux_time=60;
									else
											mux_time=exam_time;

									exam_over_flag = 0;
									result_displayed = 0;

									end_hour = 0;
									end_min = 0;
									end_sec = 0;

									old_min = MIN;
									old_sec = SEC;
									exam_running = 0;
									current_mode = 0;
							}

						else if(option=='2')
						{
								LCD_Cmd(0x01);
								LCD_str("1.View Log");
								LCD_Cmd(0xC0);
								LCD_str("2.Back");

								option = keyscan();
								if(option == 'C')
								{
										current_mode = 0;
										LCD_Cmd(0x01);
										return;
								}
								if(option=='1')
								{
										LCD_Cmd(0x01);
										LCD_str("S:");
										LCD_2Digit(start_hour);
										LCD_Char(':');
										LCD_2Digit(start_min);

										LCD_Cmd(0xC0);

										LCD_str("E:");
										LCD_2Digit(end_hour);
										LCD_Char(':');
										LCD_2Digit(end_min);
										if(start_hour==0 && end_hour==0)
										{
												LCD_Cmd(0x01);
												LCD_str("No Log Found");
										}
										keyscan();
										current_mode=0;
								}

								else if(option=='2')
								{
										continue;    // return to first menu
								}
								}
								}
		
						}
					
						else
						{
							LCD_Cmd(0x01);
							LCD_str("Access Denied");
							if(keyscan()=='C')
								current_mode=0;
						}
				}
		}
}

		