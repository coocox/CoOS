/**
 *********************************************************************************************
 * @brief		CooCox OS demo test file	 		 
 * @file		main.c		
 * @version		V1.10	The first version		
 * @date		2009.06.11	
 *  	 
 * @details		This file is part of CooCox RTOS demo test.
 *********************************************************************************************
 * @copy
 *
 * <h2><center>&copy; COPYRIGHT 2009 CooCox </center></h2>
 *********************************************************************************************
 */

/*---------------------------- Inlcude ---------------------------------------*/
//#include <config.h>           /*!< CoOS configure header file.    */ 
#include <CoOS.h>	      /*!< CooCox CoOS header file.       */
#include <stm32f10x_lib.h>    /*!< STM32F10x library header file. */
#include "AppConfig.h"        /*!< User header file               */


/*---------------------------- Define  ---------------------------------------*/
#define TASK_STK_SIZE		128	/*!< Define stack size.	              */

#define	TIME_SET_PRI 		2       /*!< Priority of 'time_set' task.     */
#define LCD_BLINK_PRI		3	/*!< Priority of 'tcd_blink' task.    */
#define LCD_DISPLAY_PRI		4	/*!< Priority of 'display_adc' task.  */
#define UART_PRINT_PRI 		4	/*!< Priority of 'tart_print' task.   */
#define LED_DISPLAY_PRI		5	/*!< Priority of 'led_display' task.  */				    
#define TIME_DISRPLAY_PRI 	5	/*!< Priority of 'time_display' task. */

							    
/*---------------------------- Variable Define -------------------------------*/  
OS_STK   task_init_Stk[TASK_STK_SIZE];	     /*!< Stack of 'task_init' task.      */
OS_STK   lcd_display_adc_Stk[TASK_STK_SIZE]; /*!< Stack of 'lcd_display_adc' task.*/
OS_STK   uart_print_Stk[TASK_STK_SIZE];      /*!< Stack of 'uart_print' task.	  */
OS_STK   led_display_Stk[TASK_STK_SIZE];     /*!< Stack of 'led_display' task.	  */
OS_STK   time_display_Stk[TASK_STK_SIZE];    /*!< Stack of 'time_display' task.	  */   
OS_STK   time_set_Stk[TASK_STK_SIZE];	     /*!< Stack of 'time_set' task. 	  */
OS_STK   lcd_blink_Stk[TASK_STK_SIZE];	     /*!< Stack of 'lcd_blink' task.  	  */

OS_MutexID  mut_uart,mut_lcd;		/*!< Save id of mutex.			    */
OS_FlagID   button_sel_flg;		/*!< Flag id,related to 'EVT_BUTTON_SEL'.   */  
OS_FlagID   button_add_flg;		/*!< Flag id,related to 'EEVT_BUTTON_ADD'.  */ 
OS_FlagID   lcd_blink_flg;		/*!< Flag id,related to 'lcd_blink' task.   */
OS_FlagID   time_display_flg; 		/*!< Flag id,related to 'time_display' task.*/

U16 EVT_BUTTON_SEL = 0x0001;	 	/*!< Flag related to Select button.	    */
U16 EVT_BUTTON_ADD = 0x0002;		/*!< Flag related to add button.	    */

OS_TID		lcd_blink_id = 0; 	/*!< Save id of 'lcd_blink' task.	    */
OS_TID		time_display_id;	/*!< Save id of 'time_display' task.	    */
OS_EventID	mbox0;			/*!< Save id of mailbox.		    */
unsigned char 	errinfo[30];	   	/*!< Error information.			    */ 

unsigned char timeflag = 0;

unsigned char time[3] = {55,20,12};	/*!< Format: { second,minute,hour }	    */ 


char chart[9] = {'0','0',':','0','0',':','0','0','\0'};	 /*!< Time buffer 	    */

const char led[19] = {0xFF,0x7E,0x3C,0x18,
                      0x18,0x24,0x42,0x81,0x42,0x24,
					  0x18,0x24,0x42,0x81,
					  0x99,0xA5,0xC3,0xDB,0xE7};

extern unsigned short int  ADC_ConvertedValue;	/*!< The value which AD converted */			


/*---------------------------- Function declare ------------------------------*/
void lcd_blink      (void *pdata);  /*!< Lcd blink when setting time. 	      */
void lcd_display_adc(void *pdata);  /*!< Display value of adc convert in LCD. */
void uart_print	    (void *pdata);  /*!< Print value of adc convert by UART.  */
void led_blink	    (void *pdata);  /*!< LED blinky.			      */
void time_display   (void *pdata);  /*!< Display real time.		      */
void time_set	    (void *pdata);  /*!< Setting time.			      */
void task_init      (void *pdata);  /*!< Initialization task.		      */


void delay(unsigned int dly)
{
    unsigned int i,j; 
    for(i=0;i<5000;i++)
      for(j=dly;j>0;j--)
        ;
}

/**
 *******************************************************************************
 * @brief		lcd_blink task	  
 * @param[in] 	pdata	A pointer to parameter passed to task.	 
 * @param[out] 	None  
 * @retval		None
 *		 
 * @details	   	This task use to blink lcd when setting time.  
 *******************************************************************************
 */
void lcd_blink (void *pdata)
{
    char flag = 0;
    pdata = pdata;  
    for (;;)
    {
        CoWaitForSingleFlag(lcd_blink_flg,0);
        CoEnterMutexSection(mut_lcd);

        set_cursor(7, 0);
        lcd_print (chart);

        switch (timeflag) 
        {
            case 0:	
                    break;
            case 1:	
                    set_cursor (13, 0);
                    if (flag == 0) 
                    {
                        lcd_putchar (0);
                        lcd_putchar (0);					      				 
                        flag = 1;
                    }
                    else 
                    {
                        lcd_putchar (chart[6]);
                        lcd_putchar (chart[7]);
                        flag = 0;
                    }
                    break;

            case 2:	
                    set_cursor (10, 0);
                    if (flag == 0) 
                    {
                        lcd_putchar (0);
                        lcd_putchar (0);					      				 
                        flag = 1;
                    }
                    else 
                    {
                        lcd_putchar (chart[3]);
                        lcd_putchar (chart[4]);
                        flag = 0;
                    }
                    break;
            case 3:
                    set_cursor (7, 0);
                    if (flag == 0) 
                    {
                        lcd_putchar (0);
                        lcd_putchar (0);					      				 
                        flag = 1;
                    }
                    else 
                    {
                        lcd_putchar (chart[0]);
                        lcd_putchar (chart[1]);
                        flag = 0;
                    }
                    break;
            default: break;
        };
        CoLeaveMutexSection(mut_lcd);
        CoTickDelay (55);
    }	
}

/**
 *******************************************************************************
 * @brief		Task of display adc value in LCD.	  
 * @param[in] 	pdata	A pointer to parameter passed to task.	 
 * @param[out] 	None  
 * @retval		None
 *		 
 * @details	   	This task use to display adc value in lcd. 
 *******************************************************************************
 */
void lcd_display_adc(void *pdata)
{
    int AD_value 	 = 0;
    int AD_scaled_ex = 0;
    int AD_scaled 	 = 0;  
      
    pdata = pdata;
    for (;;) 
    { 
        AD_value  = ADC_ConvertedValue;         /*!< Read AD value. 			        */
        AD_scaled = AD_value / 52;              /*!< AD value scaled to 0 .. 78 (lines on LCD).	*/

        CoEnterMutexSection(mut_lcd);
        if (AD_scaled != AD_scaled_ex) {        /*!< If new AD value different than old.	*/
        AD_scaled_ex = AD_scaled;
        lcd_bargraphXY(0, 1, AD_scaled); 	 
        }
        CoLeaveMutexSection(mut_lcd);

        CoTickDelay(80);
    }		
}

/**
 *******************************************************************************
 * @brief		Print task.	  
 * @param[in] 	pdata	A pointer to parameter passed to task.	 
 * @param[out] 	None  
 * @retval		None
 *		 
 * @details	  	This task print adc value by UART.  
 *******************************************************************************
 */
void uart_print(void *pdata)
{
    int AD_value;	 
    pdata = pdata; 			
    for (;;) 
    {			
        CoEnterMutexSection(mut_uart);			/*!< Enter mutex area.*/
        
        AD_value = ADC_ConvertedValue;
        uart_printf ("\r AD value = 0x");
        uart_print_hex (AD_value,4);
        
        CoLeaveMutexSection(mut_uart); 			/*!< Leave mutex area.*/

        CoTickDelay(100);
    }		
}

/**
 *******************************************************************************
 * @brief		Task of blink led		  
 * @param[in] 	pdata	A pointer to parameter passed to task.	 
 * @param[out] 	None  
 * @retval		None
 *		 
 * @details	    This task use to blink led.
 *******************************************************************************
 */
void led_blink(void *pdata)
{
    char i;
    pdata = pdata;
    for (;;) 
    {
        for (i=0;i<19;i++)
        {
            GPIOB->ODR = (GPIOB->ODR & 0xFFFF00FF) | (led[i] << 8);

            CoTickDelay (20);
        }
    }		
}

/**
 *******************************************************************************
 * @brief		time_display task		  
 * @param[in] 	pdata	A pointer to parameter passed to task.	 
 * @param[out] 	None  
 * @retval		None
 *		 
 * @details	    This task use to display time in LCD.
 *******************************************************************************
 */
void time_display(void *pdata)
{
    static unsigned char tim[3] = {0,0,0};
    static unsigned char *ptr;

    pdata = pdata;
    for (;;)
    {
        CoWaitForSingleFlag(time_display_flg,0);
        ptr = (unsigned char *)CoPendMail(mbox0,0,&errinfo[30]);

        CoEnterMutexSection(mut_lcd);
        if (tim[0] != *(ptr+0)) 
        {
            tim[0]   = *(ptr+0);
            chart[6] = tim[0]/10 + '0';
            chart[7] = tim[0]%10 + '0';			
        }
        if (tim[1] != *(ptr+1)) 
        {
            tim[1] 	 = *(ptr+1);
            chart[3] = tim[1]/10 + '0';
            chart[4] = tim[1]%10 + '0';
        }
        if (tim[2] != *(ptr+2)) 
        {
            tim[2] = *(ptr+2);
            chart[0] = tim[2]/10 + '0';
            chart[1] = tim[2]%10 + '0';
        }		

        set_cursor(7, 0);
        lcd_print (chart);

        CoLeaveMutexSection(mut_lcd);
    }		
}

/**
 *******************************************************************************
 * @brief		Set time task		  
 * @param[in] 	pdata	A pointer to parameter passed to task.	 
 * @param[out] 	None  
 * @retval		None
 *		 
 * @details	  	This task use to set time.  
 *******************************************************************************
 */
void time_set(void *pdata)
{
    static unsigned char bct = 0;			/* Button calc 		*/
    U16 evtmp = 0;

    pdata = pdata;
    for (;;)
    {
          evtmp = CoWaitForMultipleFlags(EVT_BUTTON_SEL|EVT_BUTTON_ADD,OPT_WAIT_ANY,0,&errinfo[20]);
          if(errinfo[20] != E_OK)
                  uart_printf("\r Flag ERROR:\n");

          if (evtmp == EVT_BUTTON_SEL) 
          {
              bct++;
              switch (bct) 
              {	
                  case 1: 
                        timeflag = 1;
                        if(lcd_blink_id == 0)
                                lcd_blink_id = CoCreateTask (lcd_blink,(void *)0, LCD_BLINK_PRI ,&lcd_blink_Stk[TASK_STK_SIZE - 1], TASK_STK_SIZE );
                        
                        DisableRTCInterrupt();

                        CoClearFlag(time_display_flg);
                        CoSetFlag(lcd_blink_flg);
                        break;
                  case 2:
                        timeflag = 2;
                        break;
                  case 3:
                        timeflag = 3;
                        break;
                  case 4:	
                        bct = 0; 
                                
                        CoClearFlag(lcd_blink_flg);
                        CoSetFlag(time_display_flg);

                        EnableRTCInterrupt();								
                        break;
                  default: 
                        break;
              };
              CoTickDelay(40);
              CoClearFlag(button_sel_flg);				
          }
          else if(evtmp == EVT_BUTTON_ADD) 
          {			
              CoEnterMutexSection(mut_lcd);			
              switch (bct)
              {
                  case 1:
                          time[0]++;
                          if (time[0] >= 60) 
                                  time[0] = 0;
                          chart[6] = time[0]/10 + '0';
                          chart[7] = time[0]%10 + '0';
                          break;				
                  case 2:
                          time[1]++;
                          if (time[1] >= 60)
                                  time[1] = 0;
                          chart[3] = time[1]/10 + '0';
                          chart[4] = time[1]%10 + '0';
                          break;				

                  case 3:
                          time[2]++;
                          if (time[2] >= 24)
                                  time[2] = 0;
                          chart[0] = time[2]/10 + '0';
                          chart[1] = time[2]%10 + '0';
                          break;

                  default: break;
              }	
              set_cursor(7, 0);
              lcd_print (chart);
                              
              CoLeaveMutexSection(mut_lcd);
              CoTickDelay(40);
              CoClearFlag(button_add_flg);						
          }
                                  
    }		
}

/**
 *******************************************************************************
 * @brief		Initialization task	  
 * @param[in] 	pdata	A pointer to parameter passed to task.	 
 * @param[out] 	None  
 * @retval		None
 *		 
 * @details	    This task is called to initial hardware and created tasks.
 *******************************************************************************
 */
void task_init(void *pdata)
{
    uart_printf (" [OK]. \n\r\n\r");
    uart_printf ("\r \"task_init\" task enter.		\n\r\n\r ");
    pdata = pdata; 
    
    /* Initiate Time buffer for LCD display */
    chart[0] = time[2]/10 + '0';
    chart[1] = time[2]%10 + '0';
        
    chart[3] = time[1]/10 + '0';
    chart[4] = time[1]%10 + '0';
    
    chart[6] = time[0]/10 + '0';
    chart[7] = time[0]%10 + '0';	
    
    
    uart_printf ("\r Create the \"mut_uart\" mutex...      ");
    mut_uart = CoCreateMutex();
    if(mut_uart == E_CREATE_FAIL)
      uart_printf (" [Fail]. \n");		
    else
      uart_printf (" [OK]. \n");	 
    
    
    uart_printf ("\r Create the \"mut_lcd\" mutex...       ");
    mut_lcd  = CoCreateMutex(); 
    if(mut_lcd == E_CREATE_FAIL)
      uart_printf (" [Fail]. \n");		
    else
       uart_printf (" [OK]. \n");	
    
    
    uart_printf ("\r Create the \"button_sel_flg\" flag... ");
    button_sel_flg = CoCreateFlag(Co_FALSE,0); 	/*!< Manual reset flag,initial state:0	*/ 
    if(button_sel_flg == E_CREATE_FAIL)
      uart_printf (" [Fail]. \n");     	
    else
      uart_printf (" [OK]. \n");	   	
    
    
    uart_printf ("\r Create the \"button_add_flag\" flag...");
    button_add_flg = CoCreateFlag(Co_FALSE,0);	/*!< Manual reset flag,initial state:0	*/
    if(button_add_flg == E_CREATE_FAIL)
      uart_printf (" [Fail]. \n\n");	       		
    else
      uart_printf (" [OK]. \n\n");
           
    
    uart_printf ("\r Create the \"lcd_blink_flg\" flag...  ");
    lcd_blink_flg = CoCreateFlag(Co_FALSE,0);      /*!< Manual reset flag,initial state:0	*/ 
    if(lcd_blink_flg == E_CREATE_FAIL)
      uart_printf (" [Fail]. \n");     		
    else
      uart_printf (" [OK]. \n");
            
                                                              
    uart_printf ("\r Create the \"time_display_flg\" flag...");
    time_display_flg = CoCreateFlag(Co_FALSE,0);   /*!< Manual reset flag,initial state:0	*/ 
    if(time_display_flg == E_CREATE_FAIL)
      uart_printf (" [Fail]. \n");     	
    else
      uart_printf (" [OK]. \n");	
            
    CoSetFlag(time_display_flg);		/*!< Set flag to allow "time_display_flg" task run.	*/
            
    
    uart_printf ("\r Create the first mailbox...         ");
    mbox0 = CoCreateMbox(EVENT_SORT_TYPE_FIFO);
    if(mbox0 == E_CREATE_FAIL)
      uart_printf (" [Fail]. \n\n");      		
    else
      uart_printf (" [OK]. \n\n");
            	
    
    /* Configure Peripheral */
    uart_printf ("\r Initial hardware in Board :     \n\r");
    
    uart_printf ("\r ADC initial...                      ");
    ADC_Configuration  ();
    uart_printf (" [OK]. \n");
    
    uart_printf ("\r RTC initial...                      ");
    RTC_Configuration  ();
    uart_printf (" [OK]. \n");
    
    uart_printf ("\r GPIO initial...                     ");
    GPIO_Configuration ();	
    uart_printf (" [OK]. \n");
    
    uart_printf ("\r External interrupt initial...       ");
    EXIT_Configuration ();
    uart_printf (" [OK]. \n");	
    
    uart_printf ("\r LCD initial...                      ");
    LCD_Configuration  ();		
    uart_printf (" [OK]. \n\n");
    
    
    /* Create Tasks */
    CoCreateTask(lcd_display_adc, (void *)0, LCD_DISPLAY_PRI ,&lcd_display_adc_Stk[TASK_STK_SIZE - 1], TASK_STK_SIZE );
    CoCreateTask(     uart_print, (void *)0, UART_PRINT_PRI  ,     &uart_print_Stk[TASK_STK_SIZE - 1], TASK_STK_SIZE );
    CoCreateTask(      led_blink, (void *)0, LCD_BLINK_PRI   ,    &led_display_Stk[TASK_STK_SIZE - 1], TASK_STK_SIZE );
    time_display_id = CoCreateTask( time_display, (void *)0, TIME_DISRPLAY_PRI,   &time_display_Stk[TASK_STK_SIZE - 1], TASK_STK_SIZE );
    CoCreateTask(   	time_set, (void *)0, TIME_SET_PRI    ,       &time_set_Stk[TASK_STK_SIZE - 1], TASK_STK_SIZE );
    
    
    CoExitTask();	 /*!< Delete 'task_init' task. 	*/	
}



/**
 *******************************************************************************
 * @brief	RTC_IRQHandler(Vector :19, RTC)	  
 * @param[in] 	None	 	 
 * @param[out] 	None  
 * @retval	None
 *		 
 * @details     This is RTC interrupt handler,Use to post mailbox to 
 *              'time_display' task.  
 *******************************************************************************
 */
void RTC_IRQHandler (void)
{
    CoEnterISR();      /* Tell CooCox that we are starting an ISR. */
    
    time[0]++;
    if (time[0] == 60) 
    {
        time[0] = 0;
        time[1]++;
        if (time[1] == 60) 
        {
            time[1] = 0;
            time[2]++;
            if (time[2] == 24) 
                time[2] = 0;
        }
    } 
  
    isr_PostMail (mbox0,time);
  
    RTC->CRL &= ~(unsigned short)1<<0;	
    CoExitISR();   
}


/**
 *******************************************************************************
 * @brief	EXTI0_IRQHandler(Vector :22, PA0)	  
 * @param[in] 	None	 	 
 * @param[out] 	None  
 * @retval	None
 *		 
 * @details     This is external interrupt handler,use to select to set hour,minute,second.  
 *******************************************************************************
 */
void EXTI0_IRQHandler(void)
{
    CoEnterISR();                                       
    
    isr_SetFlag(button_sel_flg);
    EXTI_ClearITPendingBit(EXTI_Line0);
    
    CoExitISR();  
}

/**
 *******************************************************************************
 * @brief	EXTI15_10_IRQHandler(Vector :56, PC13)	  
 * @param[in] 	None	 	 
 * @param[out] 	None  
 * @retval      None
 *		 
 * @details     This is external interrupt handler,Use to add time when setting time.  
 *******************************************************************************
 */
void EXTI15_10_IRQHandler(void)
{
    CoEnterISR();                                       
    
    isr_SetFlag(button_add_flg);
    EXTI_ClearITPendingBit(EXTI_Line13);
    
    CoExitISR();
}



/**
 *******************************************************************************
 *    Main function
 *******************************************************************************
 */
int main()
{	
    RCC_Configuration();	/*!< Configure the system clocks	*/   
    NVIC_Configuration();	/*!< NVIC Configuration 		*/ 
    UART_Configuration ();	/*!< UART configuration			*/
    
    uart_printf ("\n\r\n\r");
    uart_printf ("\r System initial...                  ");
    uart_printf (" [OK]. \n");
    uart_printf ("\r System Clock have been configured as 60MHz!\n\n");
    
    uart_printf ("\r +------------------------------------------------+ \n");
    uart_printf ("\r | CooCox RTOS Demo for Cortex-M3 MCU(STM32F10x). |	\n");
    uart_printf ("\r | Demo in Keil MCBSTM32 Board.                   | \n");
    uart_printf ("\r +------------------------------------------------+ \n\n");
    
    
    uart_printf ("\r Initial CooCox RTOS...              ");
    CoInitOS();			/*!< Initial CooCox CoOS */
    uart_printf (" [OK]. \n");
    
    
    uart_printf ("\r Create a \"task_init\" task...        ");
    CoCreateTask(task_init, (void *)0, 10,&task_init_Stk[TASK_STK_SIZE-1], TASK_STK_SIZE);
    uart_printf (" [OK]. \n");
    
    
    uart_printf ("\r Start multi-task.                   ");
    CoStartOS();
    
    
    while (1);
}
