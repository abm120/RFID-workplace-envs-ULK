/**
   V1.03 Code for RFID BASED DATABASE LOGGER SYSTEM
   created on 29th July 2013
   @Authors: Aniket Mitra, Anil B Murthy, Arpit Lunawat
**/
#include "macros.h"
#include <ulk.h>                      //Required Header files
struct rtc_struct rtc_struct1;        //Declaring an RTC structure variable
//struct rtc_struct rtc_struct2;
int main(void) PROGRAM_ENTRY;         //main function Signature 

//Definition of Constants required for Graphic LCD module
#define GLCD_ROWS 240
#define GLCD_COLUMNS 320
#define LINE_WIDTH 5
#define GLCD_MEMORY 0x80500000
#define WHITE_COLOR 0x00ffffff
#define BLACK_COLOR 0X00000000

//Declaration of variables for Graphic LCD
unsigned int offset=0;
int length,breadth; 
unsigned long *ptr = GLCD_MEMORY;
//Declaration of variables for database access
int entry_count,exit_count;
int order_entry[20],entry_hour[20],entry_min[20],entry_sec[20],order_exit[20],exit_hour[20],exit_min[20],exit_sec[20];
//Declaration of variable to store tag address
unsigned char tag_id[12]="";
//Declaration of variables for buzzer module, total count of employees and updated minute
int buzz=0;
int count=0x0000;
int min;
//common variable for undefined use throughout program code
int i=0;

//Declaring structure and its variables to operate Graphic LCD
struct PIXEL
{
	unsigned int x;
	unsigned int y;
};
extern struct PIXEL pixel;
extern struct PIXEL ulk_proc_touch_spi_enable(void);
extern struct PIXEL ulk_proc_touch_spi_poll(void);

//min_update function definition
void min_update()
{
     /*
     Required Parameters : None
     Return value : None
     Updates the time to +3 minutes after which to display the employees accessing the respective access gate on the host PC
     */
	if(min>=57)
	{
		min=min-57;
	}
	else
	{
		min=min+3;
	}
}

//time_print function definition
void time_print()
{
    /*
     Required Parameters : None
     Return value : None
     Displays, on the host PC, the current time of the Real Time Clock(RTC) running on the System
     */ 
	ulk_proc_rtc_get_time (&rtc_struct1);                    //gets time from RTC clock
	ulk_cpanel_printf(" TIME - %d:%d:%d \n", rtc_struct1.tm_hour, rtc_struct1.tm_min, rtc_struct1.tm_sec);
	ulk_cpanel_printf(" DATE - %d/%d/%d \n", rtc_struct1.tm_day, rtc_struct1.tm_mon, rtc_struct1.tm_year);	
}

//authorize function definition
void authorize()
{
    /*
    Required Parameters : None
    Return value : None
    For authorized employee tags, displays "authorized tag-id","access granted" message on Access gate display and Host PC
    */
	ulk_cpanel_printf(" AUTHORISED TAG-ID\n");
	ulk_fpga_clcd_display_string(" ACCESS GRANTED");
	ulk_cpanel_printf(" ACCESS GRANTED\n");
}

//tag_read_test function definition
void tag_read_test()
{
    /*
    Required Parameters : None
    Return value : None
    Provides a visual signal of a series of glowing green LED's if a tag address is read
    */
	uint8 led_count=0;                                //denotes the LED number being used
	for(led_count=0;led_count<MAX_COUNT;led_count++)
	{
		ulk_proc_led_drive(led_count, 1);             //swithces on respective LED
	}
	ulk_proc_delay(ULK_MSEC(500));                    //provides a 500 ms delay
	for(led_count=0;led_count<MAX_COUNT;led_count++)
	{
		ulk_proc_led_drive(led_count, 0);             //switches off respective LED
	}
}

//access_grant function definition
void access_grant(int enter)
{
    /*
    Required Parameters : enter - entry/exit variable
    Return value : None
    Displays a visual signal for "access granted" cases and classifies accesses as entry or exit
    */
	buzz=0;
	ulk_proc_gpio_set_data_out(140,buzz);             //Drives the Buzzer power pin to logic LOW    
	//For screen to be painted GREEN
	for(length=163;length<315;length++)
	{
		for(breadth=4;breadth<237;breadth++)
		{
			offset=length+breadth*GLCD_COLUMNS;       //Selection of specific region on GLCD
			*(ptr+offset)=0x100ff010;                 //Painting the region GREEN
		}
	}
	//ulk_proc_delay(ULK_MSEC(1300));
	//Classification of entry and exit cases and keeping count of employees currently accessing region
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

//entry_details function definition
void entry_details(int emp_no)
{
     /*
     Required Parameters : emp_no - employee number
     Return value : None
     Storage of entry details such as entry time & employee number as a record onto database
     */
     order_entry[entry_count] = emp_no;               //storing order of entry of employees based on employee number               
     time_print();                                    //function call to print time
     ulk_proc_rtc_get_time (&rtc_struct1);            //gets time from RTC clock
     //storing entry time as hour, minute and seconds as separate modules.
     entry_hour[entry_count] = rtc_struct1.tm_hour;   
     entry_min[entry_count] = rtc_struct1.tm_min;
     entry_sec[entry_count] = rtc_struct1.tm_sec;
     entry_count++;                                   //total entry count increment
}

//exit_details function definition
void exit_details(int emp_no, int position)
{
     /*
     Required Parameters : emp_no - employee number, position - position of order_entry array
     Return value : None
     Storage of exit details such as exit time & employee number as a record onto database
     */
     order_exit[position] = emp_no;                   //storing exit of employees based on employee number                     
     time_print();                                    //function call to print time
     ulk_proc_rtc_get_time (&rtc_struct1);            //gets time from RTC clock
     //storing exit time as hour, minute and seconds as separate modules.
     exit_hour[position] = rtc_struct1.tm_hour;
     exit_min[position] = rtc_struct1.tm_min;
     exit_sec[position] = rtc_struct1.tm_sec;       
     exit_count++;                                    //total exit count increment
}

//E1 function definition     
void E1()
{
     /*
     Required Parameters : None
     Return value : None
     Displays the name of employee 1
     */
     ulk_cpanel_printf(" ANIRUDH SHARMA");
}
//E2 function definition     
void E2()
{
     /*
     Required Parameters : None
     Return value : None
     Displays the name of employee 2
     */
	 ulk_cpanel_printf(" MD. TAHIR");
}
//E3 function definition     
void E3()
{
     /*
     Required Parameters : None
     Return value : None
     Displays the name of employee 3
     */
     ulk_cpanel_printf(" BENEDICT GERSHOM");
}
//E4 function definition     
void E4()
{
     /*
     Required Parameters : None
     Return value : None
     Displays the name of employee 4
     */
     ulk_cpanel_printf(" ANWESHA GUPTA");
}
//E5 function definition     
void E5()
{
     /*
     Required Parameters : None
     Return value : None
     Displays the name of employee 5
     */
     ulk_cpanel_printf(" SHRUTI HASSAN");
}
//E6 function definition     
void E6()
{
     /*
     Required Parameters : None
     Return value : None
     Displays the name of employee 6
     */
     ulk_cpanel_printf(" JESSICA SIMPSON");
}

//EMPLOYEE1 function definition
void EMPLOYEE1()
{
    /*
    Required Parameters : None
    Return value : None
    Displays the details of employee 1
    */ 
	ulk_cpanel_printf(" NAME- ANIRUDH SHARMA \n");
	ulk_cpanel_printf(" ADDRESS- KORMANGALA \n");
	ulk_cpanel_printf(" DESIGNATION- TECHNICAL MANAGER \n");
	ulk_cpanel_printf(" CONTACT NO.- 8867855731 \n");
}
//EMPLOYEE2 function definition
void EMPLOYEE2()
{
    /*
    Required Parameters : None
    Return value : None
    Displays the details of employee 2
    */  
	ulk_cpanel_printf(" NAME- MD. TAHIR \n");
	ulk_cpanel_printf(" ADDRESS- JAYANAGAR \n");
	ulk_cpanel_printf(" DESIGNATION- HEAD OF RESEARCH \n");
	ulk_cpanel_printf(" CONTACT NO.- 8867855732 \n");
}
//EMPLOYEE3 function definition
void EMPLOYEE3()
{
    /*
    Required Parameters : None
    Return value : None
    Displays the details of employee 3
    */  
	ulk_cpanel_printf(" NAME- BENEDICT GERSHOM \n");
	ulk_cpanel_printf(" ADDRESS- INDRANAGAR \n");
	ulk_cpanel_printf(" DESIGNATION- MANAGING DIRECTOR \n");
	ulk_cpanel_printf(" CONTACT NO.- 8867855733 \n");
}
//EMPLOYEE4 function definition
void EMPLOYEE4()
{
    /*
    Required Parameters : None
    Return value : None
    Displays the details of employee 4
    */  
	ulk_cpanel_printf(" NAME- ANWESHA GUPTA \n");
	ulk_cpanel_printf(" ADDRESS- BANASHANKARI \n");
	ulk_cpanel_printf(" DESIGNATION- HR MANAGER \n");
	ulk_cpanel_printf(" CONTACT NO.- 8867855734 \n");
}
//EMPLOYEE5 function definition
void EMPLOYEE5()
{
    /*
    Required Parameters : None
    Return value : None
    Displays the details of employee 5
    */  
	ulk_cpanel_printf(" NAME- SHRUTI HASSAN \n");
	ulk_cpanel_printf(" ADDRESS- J.P.NAGAR \n");
	ulk_cpanel_printf(" DESIGNATION- HEAD OF FINANCE \n");
	ulk_cpanel_printf(" CONTACT NO.- 8867855735 \n");
}
//EMPLOYEE6 function definition
void EMPLOYEE6()
{
    /*
    Required Parameters : None
    Return value : None
    Displays the details of employee 6
    */  
	ulk_cpanel_printf(" NAME- JESSICA SIMPSON \n");
	ulk_cpanel_printf(" ADDRESS- SHANTHI NAGAR \n");
	ulk_cpanel_printf(" DESIGNATION- CHIEF ANALYST \n");
	ulk_cpanel_printf(" CONTACT NO.- 8867855736 \n");
}

//add_comp function definition
int add_comp(unsigned char add[12])
{
    /*
    Required Parameters : add[12] - tag address
    Return value : Int - result of comparison
    Compares the tag address with the addresses stored in the database*/
	int i=0;
	int result_flag=1;                                //a flag to aid assumption that the tag address does not match any ID
	//comparison of the full tag address characters 4 bits each
	for(i=0;i<12;i++)
	{
		if(tag_id[i]==add[i])
		{
			result_flag=0;
		}
		else
		{
			result_flag=1;
			break;
		}
	}
	return result_flag;                               //returning result of comparison - 0 for a match, 1 otherwise
}

//db_security_check function definition
void db_security_check()
{
     /*
     Required Parameters : None
     Return value : None
     Checks for any corruption in the database and creates an alert if any is detected.
     */
     int w=0;
     i=0;                                         //declaring local common variable                                                    
     for(w=0;w<20;w++)                            //sufing through the database
     {                     
         for(i=w+1;i<20;i++)                      //Check database for any multiple entries without corresponding (single) exit
         {
              if(order_entry[i]==order_entry[w])
              {
                    if(order_entry[w] != order_exit[w] && order_exit[w] == 0)
                          ulk_cpanel_printf("Database corrupted : Data Unreliable");                  //dasplaying an alert message on host PC                                                 
              }
         }
         //if (order_entry[w] == 0 && order_exit[w] != 0)
         //{
           //     ulk_cpanel_printf("Database corrupted : Data Unreliable");
                //goto end_of_program;
         //}
         
     }
     if(entry_count < exit_count)                 //checking for security condition - more number of exits than entries 
     {
            ulk_cpanel_printf("Database corrupted : Data Unreliable");                            //dasplaying an alert message on host PC                                     
     }        
}

//check_entry_or_exit function definition
void check_entry_or_exit(int check, int emp_no)
{
    /*
    Required Parameters : check - entry/exit variable, emp_no - employee number
    Return value : None
    Checks if the employee is entering or exiting the access point
    */ 
    if(check % 2 == 1)
	{
         entry_details(emp_no);                       //function call to store entry details  
    }
    else
    {
         i=0;
         for(i=19;i>=0;i--)                           //searching for the record position of exiting employee by employee number                 
         {
             if(order_entry[i] == emp_no)
             {
                  exit_details(emp_no,i);             //function call to store exit details
                  break;
             }
         }             
    }  
}    

//main function definition
int main()
{
    /*
    Required Parameters : None
    Return value : int - 0
    Initiates the system modules and overlooks and initiates the operation of other program modules
    */
	int i=0,j=0;                                      //declaring local common variables
	count=0x0000;                                     //initializing count to before initializing modules nad system start up
	//Declaring employee related variables
	int ent_emp1=0,ent_emp2=0,ent_emp3=0,ent_emp4=0,ent_emp5=0,ent_emp6=0;
	int emp1=5,emp2=5,emp3=5,emp4=5,emp5=5,emp6=5;
    //Declaring flags for entry/exit
	int exit_flag=0,entry_flag=0;
    	
##	int grid_rows=1,grid_columns=2;
	//initializing GPIO and RTC modules of ULK
    ulk_proc_gpio_init();
	ulk_proc_enable_rtc();
	ulk_proc_rtc_get_time (&rtc_struct1);             //get current time from RTC                   
	min=rtc_struct1.tm_min;                           //storing minutes
	min_update();                                     //updating time
	time_print();                                     //displaying current time
	//Initializing CLCD and 7-segment displays
	ulk_fpga_clcd_init();
	ulk_fpga_7seg_led_enable();
	ulk_fpga_7seg_led_write(count);                   //displaying initial count 
	ulk_fpga_uart_init(9600,3);                       //initiation of Baudrate, port number
    //setting pin direction and value
	ulk_proc_gpio_set_dir(140,0);                     
	ulk_proc_gpio_set_data_out(140,0);
	ulk_fpga_clcd_display_clear();                    //clearing CLCD display
    //initializing entry/exit related variables to 0, before start of logging
	entry_count = exit_count = 0;                     
	for (i=0;i<20;i++)
	{
        order_entry[i] = entry_hour[i] = entry_min[i] = entry_sec[i] = order_exit[i] = exit_hour[i] = exit_min[i] = exit_sec[i] = 0;
    }
    //Module to read tags and log onto database as long as system is powered up
	while(1)
	{
		ulk_fpga_uart_init(9600,3);                   //initiation of Baudrate, port number
        //storage of employee tag ID numbers            
		unsigned char tag1[12]="6F0054E5F628";
		unsigned char tag2[12]="6F0055E29E46";
		unsigned char tag3[12]="6F006033A29E";
		unsigned char tag4[12]="6F0055FB5796";
		unsigned char tag5[12]="6F0055F8DE1C";
		unsigned char tag6[12]="6F0055FB8D4C";
		//Call to perform a database security check
		db_security_check();
		// To make the complete screen painted WHITE		
		for(length=0;length<GLCD_COLUMNS*GLCD_ROWS;length++)
		{
			*(ptr+length)= WHITE_COLOR;
		}
		ulk_fpga_clcd_display_string("  WELCOME TO G1");                             //Display respective access point welcome message
		//obtaining tag ID from Reader
		for(i=0;i<13;i++)
		{
			tag_id[i]=ulk_fpga_uart_getc();
		}
		//Storing tag - ID's after clean-up in specific format
		for(i=0;i<12;i++)
		{
			tag_id[i]=tag_id[i+1];
		}
		tag_id[i]='\0';
		//performing read_test signal display procedure
		tag_read_test();
		ulk_proc_delay(ULK_MSEC(400));
		tag_read_test();
		//Displaying initial Read details
		ulk_cpanel_printf("\n");
		ulk_cpanel_printf(" TAG ID- ");
		ulk_cpanel_printf(tag_id);
		ulk_cpanel_printf("\n");
		//Performing Address comparison procedure
		emp1=add_comp(tag1);
		emp2=add_comp(tag2);
		emp3=add_comp(tag3);
		emp4=add_comp(tag4);
		emp5=add_comp(tag5);
		emp6=add_comp(tag6);		       
		//Checking for an address match from the database
		if(emp1==0)
		{//Performing Employee 1 check access status and information display procedure       
            authorize();
			EMPLOYEE1();
			ent_emp1++;
			access_grant(ent_emp1);
            check_entry_or_exit(ent_emp1, 1);               
		}
		else if(emp2==0)
		{//Performing Employee 2 check access status and information display procedure       
            authorize();
			EMPLOYEE2();
			ent_emp2++;
			access_grant(ent_emp2);
            check_entry_or_exit(ent_emp2, 2);                               
		}
		else if(emp3==0)
		{//Performing Employee 3 check access status and information display procedure                   
			authorize();
			EMPLOYEE3();
			ent_emp3++;
			access_grant(ent_emp3);
            check_entry_or_exit(ent_emp3, 3);                               
		}
		else if(emp4==0)
		{//Performing Employee 4 check access status and information display procedure       
			authorize();
			EMPLOYEE4();
			ent_emp4++;
			access_grant(ent_emp4);
            check_entry_or_exit(ent_emp4,4);                               
		}
		else if(emp5==0)
		{//Performing Employee 5 check access status and information display procedure       
			authorize();
			EMPLOYEE5();
			ent_emp5++;
			access_grant(ent_emp5);
            check_entry_or_exit(ent_emp5, 5);                               
		}
		else if(emp6==0)
		{//Performing Employee 6 check access status and information display procedure       
			authorize();
			EMPLOYEE6();
			ent_emp6++;
			access_grant(ent_emp6);
            check_entry_or_exit(ent_emp6, 6);
		}
		else
		{//Performing Unauthorized tag access procedure
			buzz=1;                                   //setting off buzzer value to logic HIGH
            //Displaying access status 
			ulk_cpanel_printf(" UNAUTHORISED TAG-ID\n");
			ulk_fpga_clcd_display_string(" ACCESS DENIED");
			ulk_cpanel_printf(" ACCESS DENIED \n");
        	/*ulk_proc_rtc_get_time (&rtc_struct1);
        	ulk_cpanel_printf(" TIME - %d:%d:%d \n", rtc_struct1.tm_hour, rtc_struct1.tm_min, rtc_struct1.tm_sec);
        	ulk_cpanel_printf(" DATE - %d/%d/%d \n", rtc_struct1.tm_day, rtc_struct1.tm_mon, rtc_struct1.tm_year);*/
        	time_print();                             //function call to print current time                   
			//For the screen to be painted RED
			for(length=3;length<157;length++)
			{
				for(breadth=4;breadth<237;breadth++)
				{
					offset=length+breadth*GLCD_COLUMNS;                                //Selection of specific region on GLCD
					*(ptr+offset)=0x00ff0000;                                          //Painting the region RED
				}
			}
			//Setting of Auditory signal via Sound Alarm for Unauthorized Access
			ulk_proc_gpio_set_data_out(140,buzz);
			ulk_proc_delay(ULK_MSEC(500));
			ulk_proc_gpio_set_data_out(140,0);
			ulk_proc_delay(ULK_MSEC(300));
			ulk_proc_gpio_set_data_out(140,buzz);
			ulk_proc_delay(ULK_MSEC(500));
			ulk_proc_gpio_set_data_out(140,0);
		}
		//entry_count++;
		ulk_fpga_7seg_led_write(count);               //Displaying total count of employees currently accessing  a region
		exit_flag=0;                                  
		ulk_proc_rtc_get_time (&rtc_struct1);         //get current time from RTC clock
		int check_min=rtc_struct1.tm_min;             //initialize check time value for updated Display purpose
		if(check_min==min)                            //check for running time equivalence check time condition
		{
			min_update();                             //call to update time
            //Database display procedure 
			ulk_cpanel_printf("NAME                 ENTRY TIME     EXIT TIME\n");       
            int r=0;                                  //declaring local common variable    
            while(order_entry[r]!=0)                  //entry check
            {
                 switch(order_entry[r])               //Branching based on employee number check to display employee details
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
                       default:	ulk_cpanel_printf("Error - Needs software checkup !");           //Database authenticity check condition
                                db_security_check();
                                break;
                 }
                 //ulk_cpanel_printf("\t");
                 //Display of entry time
                 ulk_cpanel_printf("      %d:%d:%d       ", entry_hour[r], entry_min[r], entry_sec[r]);
                 //j=i;
                 //exit_flag = 0;
                 //while(order_exit[j] != 0)
                 //{
                 //if(order_exit[i] == order_entry[i])
                 //{
                 //Display of exit time                                    
                 if(exit_hour[r] == 0 && exit_min[r]==0 && exit_sec[r] == 0 && order_exit[r] == 0)
                        ulk_cpanel_printf("--------");         
                 else                   
                 {
                        ulk_cpanel_printf("%d:%d:%d", exit_hour[r], exit_min[r], exit_sec[r]);
                        exit_flag = 1;
                 }
                        //break;
                 //}
                 //else
                 //       exit_flag = 0;
                       //j++;
                 //}
                 /*if(order_exit[r] == 0)
                       ulk_cpanel_printf("\t--------\n");*/
                 /*if (exit_flag==0)
                 {
                       ulk_cpanel_printf("\nNo one has exited this gate !\n");
                       exit_flag = 1;
                 }
                 else
                       exit_flag = 1;*/
                 r++;
            }
            if (order_entry[0] == 0)
                  ulk_cpanel_printf("\n No one has entered this gate !\n");
		}
		//Reset of database when database memory is full
		if(order_entry[19] != 0)
        {
              ulk_cpanel_printf("\n DATABASE RESET\n");
              for (i=0;i<20;i++)
        	  {
                  order_entry[i] = entry_hour[i] = entry_min[i] = entry_sec[i] = order_exit[i] = exit_hour[i] = exit_min[i] = exit_sec[i] = 0;
              }
        }              
	}
/*end_of_program:*/	return 0;
}
//END OF PROGRAM CODE
