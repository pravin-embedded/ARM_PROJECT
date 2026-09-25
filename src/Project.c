//Project.c
#include "headerfile.h"
#include "Project_defines.h"

int main()
{
    Hardware_Init();
    RTC_Time_Setup();
		

    while(1)
    {	
        Countdown_Update();
        Pause_Display();
        Normal_Display();
        Exam_Over_Check();
        Show_Result();
        Admin_Menu();
    }
}

