#include<xc.h>
#include"microoven.h"
#include"clcd.h"
#include"matrix_keypad.h"
#include"main.h"

unsigned char sec,min,pre_heat_time ;
extern unsigned char operational_flag;

void power_on_screen(void)
{
    for(unsigned int i=0;i<16;i++)
    {
        clcd_putch(BAR,LINE1(i));
    }

    clcd_print("Powering On", LINE2(2));
    clcd_print("Microwave Oven", LINE3(1));
    
    for (unsigned int i=0;i<16;i++)
    {
        clcd_putch(BAR, LINE4(i));  // Ensure LINE4(i) is defined properly
    }
    __delay_ms(5000);
}

  void clear_screen(void)
{
    clcd_write(CLEAR_DISP_SCREEN, INST_MODE);
    __delay_us(500);
}

void display_menu_screen(void)
{
    clcd_print("1.Micro", LINE1(0));
    clcd_print("2.Grill", LINE2(0));
    clcd_print("3.Convection", LINE3(0));  // Fixed spelling from "Convention" to "Convection"
    clcd_print("4.Start", LINE4(0));
}

void set_time(unsigned char key,unsigned char reset_flag)
{
    static unsigned char blink_pos=0, key_count=0;
    
    if(reset_flag == RESET_MODE)
    {
        blink_pos=0;
        key_count=0;
        key=ALL_RELEASED;
        min=0;
        sec=0;
        reset_flag=RESET_NOTHING;
    }
     
    clcd_print("SET TIME (MM:SS)",LINE1(0));
    clcd_print("TIME-",LINE2(0));
    clcd_print("*:CLEAR  #:ENTER",LINE4(0));
    
    if(key!='*'&&key!='#'&&key!=ALL_RELEASED)
    {
       key_count++; 
       if(key_count<=2)
       {
           sec=sec*10+key;
           blink_pos=0;
       }
       else if(key_count<=4)
       {
           min=min*10+key;
           blink_pos=1;
       }
    }
    else if(key=='*')
    {   
        if(blink_pos)
        { 
            min=0;
            key_count=2;
        }
        else
        {
            sec=0;
            key_count=0;
        }
        
    }
    else if(key=='#')
    {
        FAN=1;
        
        TMR2ON=1;
        
        operational_flag=DISPLAY_TIME;
        clear_screen();
    }
    
    
    if(blink_pos==0)
    {
        clcd_putch(' ',LINE2(9));
        clcd_putch(' ',LINE2(10));
        
    }
    else
    {
        clcd_putch(' ',LINE2(6));
        clcd_putch(' ',LINE2(7));
    }
    for(unsigned char wait=50;wait--;);
    
    clcd_putch((sec/10+'0'),LINE2(9));
    clcd_putch((sec%10+'0'),LINE2(10));
    clcd_putch(':',LINE2(8));
    clcd_putch((min/10+'0'),LINE2(6));
    clcd_putch((min%10+'0'),LINE2(7));
}

void time_display(void)
{
    clcd_print("TIME = ",LINE1(1));
    clcd_putch((sec/10+'0'),LINE1(12));
    clcd_putch((sec%10+'0'),LINE1(13));
    clcd_putch(':',LINE1(11));
    clcd_putch((min/10+'0'),LINE1(9));
    clcd_putch((min%10+'0'),LINE1(10));
    clcd_print("4.START/RESUME",LINE2(1));
    clcd_print("5.PAUSE",LINE3(1));
    clcd_print("6.STOP",LINE4(1));
    
    if(min==0&&sec==0)
    {
        clear_screen();
        
        TMR2ON=0;
        FAN=0;
        clcd_print("TIME UP!!!",LINE2(4));
        clcd_print("Enjoy your meal",LINE3(0));
        
        //buzzer
        BUZZER=1;
        __delay_ms(1000);
        BUZZER=0;
        //switch back to menu screen
        operational_flag=MENU_SCREEN;
        
    }
    
}
void set_temp(unsigned char key,unsigned char reset_flag)
{
    static unsigned char key_count;
    static unsigned int temp;
    if(reset_flag==RESET_MODE)
    {   
        key=ALL_RELEASED;
        temp=0;
        key_count=0;
        
    }
    
    clcd_print("SET TEMP <*C>",LINE1(0));
    clcd_print("TEMP -",LINE2(0));
    clcd_print("*:CLEAR  #:ENTER",LINE4(0));
    
     //blinking the temperature to indicate we are reading temperature
    clcd_putch(' ',LINE2(8));
    clcd_putch(' ',LINE2(9));
    clcd_putch(' ',LINE2(10));
    
    for(unsigned char wait=50;wait--;);
     
    //convert decimal temperature into ascii format
    clcd_putch((temp/100+'0'),LINE2(8));
    clcd_putch(((temp/10)%10+'0'),LINE2(9));
    clcd_putch((temp%10+'0'),LINE2(10));

    
    //matrix keypad keys 0 to 9 are used to read the temperature
    if(key!='*'&&key!='#'&&key!=ALL_RELEASED)
    {
        if(key_count<3)
        {
            temp=temp*10+key;
            key_count++;
        }
    }
        
    //* clear temp
    else if(key=='*')
    {
        temp=0;
        key_count=0;
    }
    //# confirm temp
    else if(key=='#')
    {
       if(temp>180)
           temp=180;
           clcd_putch((temp/100+'0'),LINE2(8));
           clcd_putch(((temp/10)%10+'0'),LINE2(9));
           clcd_putch((temp%10+'0'),LINE2(10));
   
           __delay_ms(500);  // Small delay to allow user to see the corrected value
   
           
            
       
       clear_screen();
       clcd_print("Pre-Heating",LINE1(2));
       clcd_print("Time Rem :",LINE3(0));
       clcd_print("sec",LINE3(13));
       pre_heat_time=60;
       TMR2ON=1;
       
       //display preheat screen till pre_heat_time is 0
       while(pre_heat_time!=0)
       {
           //convert decimal to ascii
           clcd_putch((pre_heat_time/100+'0'),LINE3(10));
           clcd_putch(((pre_heat_time/10)%10+'0'),LINE3(11));
           clcd_putch((pre_heat_time%10+'0'),LINE3(12));
           
           
        }
       //turn on the timer and change operational_flag
       TMR2ON=0;
       operational_flag=MICRO_MODE;
    }
}
    void heat_food(void)
    {
        //start cooking with 30sec
        sec=30;
        min=0;
        FAN=1;
        TMR2ON=1;
        
        operational_flag=DISPLAY_TIME;
    }
    
    
    
