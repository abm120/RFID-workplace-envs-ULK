#include "macros.h"
#include <ulk.h>
struct rtc_struct rtc_struct1;
int main(void) PROGRAM_ENTRY;
#define GLCD_ROWS 240
#define GLCD_COLUMNS 320
#define LINE_WIDTH 5
#define GLCD_MEMORY 0x80500000
#define WHITE_COLOR 0x00ffffff
#define BLACK_COLOR 0X00000000
unsigned int offset=0;
int length,breadth,entry_count,exit_count;
unsigned long *ptr = GLCD_MEMORY;
int order_entry[20],entry_hour[20],entry_min[20],entry_sec[20],order_exit[20],exit_hour[20],exit_min[20],exit_sec[20];
struct PIXEL
{
	unsigned int x;
	unsigned int y;
};
extern struct PIXEL pixel;
extern struct PIXEL ulk_proc_touch_spi_enable(void);
extern struct PIXEL ulk_proc_touch_spi_poll(void);
unsigned char tag_id[12]="";
int buzz=0;
int count=0x0000;
int min;
void min_update()
{
	if(min>=57)
	{
		min=min-57;
	}
	else
	{
		min=min+3;
	}
}
void time_print()
{
	ulk_proc_rtc_get_time (&rtc_struct1);
	ulk_cpanel_printf(" TIME - %d:%d:%d \n", rtc_struct1.tm_hour, rtc_struct1.tm_min, rtc_struct1.tm_sec);
	ulk_cpanel_printf(" DATE - %d/%d/%d \n", rtc_struct1.tm_day, rtc_struct1.tm_mon, rtc_struct1.tm_year);	
}
void authorize()
{
	ulk_cpanel_printf(" AUTHORISED TAG-ID\n");
	ulk_fpga_clcd_display_string(" ACCESS GRANTED");
	ulk_cpanel_printf(" ACCESS GRANTED\n");
}
void tag_read_test()
{
	uint8 led_count=0;
	for(led_count=0;led_count<MAX_COUNT;led_count++)
	{
		ulk_proc_led_drive(led_count, 1);
	}
	ulk_proc_delay(ULK_MSEC(500));
	for(led_count=0;led_count<MAX_COUNT;led_count++)
	{
		ulk_proc_led_drive(led_count, 0);
	}
}
void access_grant(int enter)
{
	buzz=0;
	ulk_proc_gpio_set_data_out(140,buzz);
	//For screen to be painted GREEN
	for(length=163;length<315;length++)
	{
		for(breadth=4;breadth<237;breadth++)
		{
			offset=length+breadth*GLCD_COLUMNS;
			*(ptr+offset)=0x100ff010;
		}
	}
	//ulk_proc_delay(ULK_MSEC(1300));
	if(enter%2==1)
	{
		count++;
		ulk_cpanel_printf(" ENTRY\n");
	}
	else
	{
		count--;
		ulk_cpanel_printf(" EXIT\n");
	}
}
void entry_details(int emp_no)
{
     order_entry[entry_count] = emp_no;       
     timeprint();
     entry_hour[entry_count] = rtc_struct1.tm_hour;
     entry_min[entry_count] = rtc_struct1.tm_min;
     entry_sec[entry_count] = rtc_struct1.tm_sec;
     entry_count++;                 
}
void exit_details(int emp_no)
{
     order_exit[exit_count] = emp_no;                
     timeprint();
     exit_hour[exit_count] = rtc_struct1.tm_hour;
     exit_min[exit_count] = rtc_struct1.tm_min;
     exit_sec[exit_count] = rtc_struct1.tm_sec;       
     exit_count++;
}
     
void E1()
{
     ulk_cpanel_printf("ANIRUDH SHARMA \n");
}
void E2()
{
	ulk_cpanel_printf("MD. TAHIR \n");
}
void E3()
{
     ulk_cpanel_printf("BENEDICT GERSHOM \n");
}
void E4()
{
     ulk_cpanel_printf("ANWESHA GUPTA \n");
}
void E5()
{
     ulk_cpanel_printf("SHRUTI HASSAN \n");
}
void E6()
{
     ulk_cpanel_printf("JESSICA SIMPSON \n");
}
void EMPLOYEE1()
{
	ulk_cpanel_printf(" NAME- ANIRUDH SHARMA \n");
	ulk_cpanel_printf(" ADDRESS- KORMANGALA \n");
	ulk_cpanel_printf(" DESIGNATION- TECHNICAL MANAGER \n");
	ulk_cpanel_printf(" CONTACT NO.- 8867855731 \n");
}
void EMPLOYEE2()
{
	ulk_cpanel_printf(" NAME- MD. TAHIR \n");
	ulk_cpanel_printf(" ADDRESS- JAYANAGAR \n");
	ulk_cpanel_printf(" DESIGNATION- HEAD OF RESEARCH \n");
	ulk_cpanel_printf(" CONTACT NO.- 8867855732 \n");
}
void EMPLOYEE3()
{
	ulk_cpanel_printf(" NAME- BENEDICT GERSHOM \n");
	ulk_cpanel_printf(" ADDRESS- INDRANAGAR \n");
	ulk_cpanel_printf(" DESIGNATION- MANAGING DIRECTOR \n");
	ulk_cpanel_printf(" CONTACT NO.- 8867855733 \n");
}
void EMPLOYEE4()
{
	ulk_cpanel_printf(" NAME- ANWESHA GUPTA \n");
	ulk_cpanel_printf(" ADDRESS- BANASHANKARI \n");
	ulk_cpanel_printf(" DESIGNATION- HR MANAGER \n");
	ulk_cpanel_printf(" CONTACT NO.- 8867855734 \n");
}
void EMPLOYEE5()
{
	ulk_cpanel_printf(" NAME- SHRUTI HASSAN \n");
	ulk_cpanel_printf(" ADDRESS- J.P.NAGAR \n");
	ulk_cpanel_printf(" DESIGNATION- HEAD OF FINANCE \n");
	ulk_cpanel_printf(" CONTACT NO.- 8867855735 \n");
}
void EMPLOYEE6()
{
	ulk_cpanel_printf(" NAME- JESSICA SIMPSON \n");
	ulk_cpanel_printf(" ADDRESS- SHANTHI NAGAR \n");
	ulk_cpanel_printf(" DESIGNATION- CHIEF ANALYST \n");
	ulk_cpanel_printf(" CONTACT NO.- 8867855736 \n");
}
int add_comp(unsigned char add[12])
{
	int i=0;
	int res=1;
	for(i=0;i<12;i++)
	{
		if(tag_id[i]==add[i])
		{
			res=0;
		}
		else
		{
			res=1;
			break;
		}
	}
	return res;
}
void db_security_check()
{
     int w=0;
     for(w=0;w<20;w++)
     {                     
         if (order_entry[w] == 0 && order_exit[w] != 0)
         {
                ulk_cpanel_printf("Database corrupted : Data Unreliable");
                goto end_of_program;
         }
     }        
}
int main()
{
	int i=0,j=0;
	count=0x0000;
	int ent_emp1=0,ent_emp2=0,ent_emp3=0,ent_emp4=0,ent_emp5=0,ent_emp6=0;
	int emp1=5,emp2=5,emp3=5,emp4=5,emp5=5,emp6=5;
	int exit_flag=0,entry_flag=0;
	int grid_rows=1,grid_columns=2;
	ulk_proc_gpio_init();
	ulk_proc_enable_rtc();
	ulk_proc_rtc_get_time (&rtc_struct1);
	min=rtc_struct1.tm_min;
	min_update();
	ulk_fpga_clcd_init();
	ulk_fpga_7seg_led_enable();
	ulk_fpga_7seg_led_write(count);
	ulk_fpga_uart_init(9600,3);
	ulk_proc_gpio_set_dir(140,0);
	ulk_proc_gpio_set_data_out(140,0);
	ulk_fpga_clcd_display_clear();
	entry_count=0;
	exit_count=0;
	for (i=0;i<20;i++)
	{
        order_entry[i] = entry_hour[i] = entry_min[i] = entry_sec[i] = order_exit[i] = exit_hour[i] = exit_min[i] = exit_sec[i] = 0;
    }
	while(1)
	{
		ulk_fpga_uart_init(9600,3);
		unsigned char tag1[12]="6F0054E5F628";
		unsigned char tag2[12]="6F0055E29E46";
		unsigned char tag3[12]="6F006033A29E";
		unsigned char tag4[12]="6F0055FB5796";
		unsigned char tag5[12]="6F0055F8DE1C";
		unsigned char tag6[12]="6F0055FB8D4C";
		// To make the complete screen painted WHITE
		db_security_check();
		for(length=0;length<GLCD_COLUMNS*GLCD_ROWS;length++)
		{
			*(ptr+length)= WHITE_COLOR;
		}
		ulk_fpga_clcd_display_string("  WELCOME TO G1");
		for(i=0;i<13;i++)
		{
			tag_id[i]=ulk_fpga_uart_getc();
		}
		for(i=0;i<12;i++)
		{
			tag_id[i]=tag_id[i+1];
		}
		tag_id[i]='\0';
		tag_read_test();
		ulk_proc_delay(ULK_MSEC(400));
		tag_read_test();
		ulk_cpanel_printf(" TAG ID- ");
		ulk_cpanel_printf(tag_id);
		ulk_cpanel_printf("\n");
		emp1=add_comp(tag1);
		emp2=add_comp(tag2);
		emp3=add_comp(tag3);
		emp4=add_comp(tag4);
		emp5=add_comp(tag5);
		emp6=add_comp(tag6);
		if(emp1==0)
		{
			authorize();
			EMPLOYEE1();
			ent_emp1++;
			access_grant(ent_emp1);
			if(ent_emp1 % 2 == 1)
			{
                 entry_details(1);
            }
            else
            {
                 exit_details(1);
            }                
		}
		else if(emp2==0)
		{
			authorize();
			EMPLOYEE2();
			ent_emp2++;
			access_grant(ent_emp2);
			if(ent_emp2 % 2 == 1)
			{
                 entry_details(2);
            }
            else
            {
                 exit_details(2);
            }                
		}
		else if(emp3==0)
		{
			authorize();
			EMPLOYEE3();
			ent_emp3++;
			access_grant(ent_emp3);
			if(ent_emp3 % 2 == 1)
			{
                 entry_details(3);
            }
            else
            {
                 exit_details(3);
            }                
		}
		else if(emp4==0)
		{
			authorize();
			EMPLOYEE4();
			ent_emp4++;
			access_grant(ent_emp4);
			if(ent_emp4 % 2 == 1)
			{
                 entry_details(4);
            }
            else
            {
                 exit_details(4);
            }                
		}
		else if(emp5==0)
		{
			authorize();
			EMPLOYEE5();
			ent_emp5++;
			access_grant(ent_emp5);
			if(ent_emp5 % 2 == 1)
			{
                 entry_details(5);
            }
            else
            {
                 exit_details(5);
            }                
		}
		else if(emp6==0)
		{
			authorize();
			EMPLOYEE6();
			ent_emp6++;
			access_grant(ent_emp6);
			if(ent_emp6 % 2 == 1)
			{
                 entry_details(6);
            }
            else
            {
                 exit_details(6);
            }                
		}
		else
		{
			buzz=1;
			ulk_cpanel_printf(" UNAUTHORISED TAG-ID\n");
			ulk_fpga_clcd_display_string(" ACCESS DENIED");
			ulk_cpanel_printf(" ACCESS DENIED \n");
        	ulk_proc_rtc_get_time (&rtc_struct1);
        	ulk_cpanel_printf(" TIME - %d:%d:%d \n", rtc_struct1.tm_hour, rtc_struct1.tm_min, rtc_struct1.tm_sec);
        	ulk_cpanel_printf(" DATE - %d/%d/%d \n", rtc_struct1.tm_day, rtc_struct1.tm_mon, rtc_struct1.tm_year);
			//For the screen to be painted RED
			for(length=3;length<157;length++)
			{
				for(breadth=4;breadth<237;breadth++)
				{
					offset=length+breadth*GLCD_COLUMNS;
					*(ptr+offset)=0x00ff0000;
				}
			}
			ulk_proc_gpio_set_data_out(140,buzz);
			ulk_proc_delay(ULK_MSEC(500));
			ulk_proc_gpio_set_data_out(140,0);
			ulk_proc_delay(ULK_MSEC(300));
			ulk_proc_gpio_set_data_out(140,buzz);
			ulk_proc_delay(ULK_MSEC(500));
			ulk_proc_gpio_set_data_out(140,0);
		}
		entry_count++;
		ulk_fpga_7seg_led_write(count);
		exit_flag=0;
		ulk_proc_rtc_get_time (&rtc_struct1);
		int check_min=rtc_struct1.tm_min;
		if(check_min==min)
		{
			min_update();
            i=0;
            while(order_entry[i]!=0)
            {
                 switch(order_entry[i])
                 {
                       case 1: E1();
                               break;                                      			
                       case 2: E2();
                               break;                                      			
                       case 3: E3();
                               break;                                      			
                       case 4: E4();
                               break;                                      			
                       case 5: E5();
                               break;                                      			
                       case 6: E6();
                               break;                                      			
                       case 0: ulk_cpanel_printf("No one has entered this gate !");
                               break;                                      		
                       default:	ulk_cpanel_printf("Error - Needs software checkup !");
                                break;
                 }
                 //ulk_cpanel_printf("\t");
                 ulk_cpanel_printf("\t%d:%d:%d", entry_hour, entry_min, entry_sec);
                 j=i;
                 //exit_flag = 0;
                 while(order_exit[j] != 0)
                 {
                       if(order_exit[j] == order_entry[i])
                       {
                              ulk_cpanel_printf("\t%d:%d:%d\n", exit_hour, exit_min, exit_sec);
                              exit_flag = 1;
                              break;
                       }
                       else
                              exit_flag = 0;
                       j++;
                 }
                 if(j==i && order_exit[j] == 0)
                       ulk_cpanel_printf("\t--------\n");
                 if (exit_flag==0)
                 {
                       ulk_cpanel_printf("\nNo one has exited this gate !\n");
                       exit_flag = 1;
                 }
                 else
                       exit_flag = 1;
                 i++;
            }
            if (order_entry[0] == 0)
                  ulk_cpanel_printf("\n No one has entered this gate !\n");
		}
		if(order_entry[19] != 0)
        {
              ulk_cpanel_printf("\n DATABASE RESET\n");
              for (i=0;i<20;i++)
        	  {
                  order_entry[i] = entry_hour[i] = entry_min[i] = entry_sec[i] = order_exit[i] = exit_hour[i] = exit_min[i] = exit_sec[i] = 0;
              }
        }              
	}
end_of_program:	return 0;
}
