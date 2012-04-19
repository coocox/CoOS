

/* Includes ------------------------------------------------------------------*/
#include "AppConfig.h"
#include <stdio.h>

unsigned short int  ADC_ConvertedValue;

/*******************************************************************************
* Function Name  : SysTick_Configuration
* Description    : Configures the SysTick clocks.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SysTick_Configuration (void)
{
  /* SysTick event each 10 ms with input clock equal to 9MHz (HCLK/8)         */
  SysTick_SetReload(75000); 
  SysTick_ITConfig(ENABLE);             /* Enable SysTick interrupt         */	 
  SysTick_CounterCmd(SysTick_Counter_Enable);	 /* Enable the SysTick Counter    */
}

/*******************************************************************************
* Function Name  : RCC_Configuration
* Description    : Configures the different system clocks.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void RCC_Configuration(void)
{  
  RCC_DeInit ();                        /* RCC system reset(for debug purpose)*/
  RCC_HSEConfig (RCC_HSE_ON);           /* Enable HSE                         */

  /* Wait till HSE is ready                                                   */
  while (RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET);

  RCC_HCLKConfig   (RCC_SYSCLK_Div1);   /* HCLK   = SYSCLK                    */
  RCC_PCLK2Config  (RCC_HCLK_Div1);     /* PCLK2  = HCLK                      */
  RCC_PCLK1Config  (RCC_HCLK_Div2);     /* PCLK1  = HCLK/2                    */
  RCC_ADCCLKConfig (RCC_PCLK2_Div4);    /* ADCCLK = PCLK2/4                   */

  *(vu32 *)0x40022000 = 0x01;           /* Flash 2 wait state                 */

  /* PLLCLK = 4MHz * 15 = 60 MHz                                              */
  RCC_PLLConfig (RCC_PLLSource_HSE_Div2, RCC_PLLMul_15);

  RCC_PLLCmd (ENABLE);                  /* Enable PLL                         */

  /* Wait till PLL is ready                                                   */
  while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);

  /* Select PLL as system clock source                                        */
  RCC_SYSCLKConfig (RCC_SYSCLKSource_PLLCLK);

  /* Wait till PLL is used as system clock source                             */
  while (RCC_GetSYSCLKSource() != 0x08);

  /* Enable USART1 and GPIOA clock                                            */
  RCC_APB2PeriphClockCmd (RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA | 
  RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
}


/*******************************************************************************
* Function Name  : GPIO_Configuration
* Description    : Configures GPIOB 8 9 10.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void GPIO_Configuration (void) 
{
  GPIO_InitTypeDef  GPIO_InitStructure;

  /* Configure PB8..PB15 as outputs push-pull, max speed 50 MHz               */
  GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_8  | GPIO_Pin_9  | GPIO_Pin_10 | 
                                  GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 |
                                  GPIO_Pin_14 | GPIO_Pin_15 ;
  GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  /* Configure PA.00 as input */
  GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* Configure PC.13 as input */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
}


/*******************************************************************************
* Function Name  : UART_Configuration
* Description    : Configures USART as UART mode
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void UART_Configuration (void)  
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	/* Enable GPIOA clock                                                     */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	/* Configure USART1 Rx (PA10) as input floating                           */
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

  	/* Configure USART1 Tx (PA9) as alternate function push-pull              */
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate            = 115200;
	USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits            = USART_StopBits_1;
	USART_InitStructure.USART_Parity              = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStructure.USART_Clock               = USART_Clock_Disable;
	USART_InitStructure.USART_CPOL                = USART_CPOL_Low;
	USART_InitStructure.USART_CPHA                = USART_CPHA_2Edge;
	USART_InitStructure.USART_LastBit             = USART_LastBit_Disable;
	USART_Init(USART1, &USART_InitStructure);

	/* Enable USART1                      */
	USART_Cmd(USART1, ENABLE); 	            
}


/*******************************************************************************
* Function Name  : EXTI_Configuration
* Description    : Configures the used EXTI lines.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void EXIT_Configuration(void)
{
  EXTI_InitTypeDef EXTI_InitStructure;

  /* Connect EXTI Line0 to PA.00 */
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);
  
  EXTI_InitStructure.EXTI_Mode 		= EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger	= EXTI_Trigger_Rising;
  EXTI_InitStructure.EXTI_Line 		= EXTI_Line0;
  EXTI_InitStructure.EXTI_LineCmd 	= ENABLE;
  EXTI_Init(&EXTI_InitStructure);
  
  /* Connect EXTI Line9 to PC.13 */
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource13);
 
  EXTI_InitStructure.EXTI_Mode		= EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger	= EXTI_Trigger_Rising;
  EXTI_InitStructure.EXTI_Line 		= EXTI_Line13;
  EXTI_InitStructure.EXTI_LineCmd 	= ENABLE;
  EXTI_Init(&EXTI_InitStructure);
}


/*******************************************************************************
* Function Name  : fputc
* Description    : Retargets the C library printf function to the USART.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
//
//int fputc(int ch, FILE *f)
// {
//  /* Place your implementation of fputc here */
//  /* e.g. write a character to the USART */
//  USART_SendData(USART1, (u8) ch);
//
//  /* Loop until the end of transmission */
//  while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
//  {
//  }

 // return ch;
//}
//

void LCD_Configuration (void)
{
	lcd_init  ();
	lcd_clear ();
	set_cursor(0, 0);
	lcd_print ("Time : 00:00:00");
}

/*******************************************************************************
* Function Name  : ADC_Configuration
* Description    : Initialises the Analog/Digital converter
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void ADC_Configuration (void) {

//  GPIOA->CRL &= ~0x0000000F;                    // set PIN1 as analog input (see stm32_Init.c)

  RCC->AHBENR |= (1<<0);                          // enable periperal clock for DMA

  DMA_Channel1->CMAR  = (u32)&ADC_ConvertedValue;// set channel1 memory address
  DMA_Channel1->CPAR  = (u32)&(ADC1->DR);        // set channel1 peripheral address
  DMA_Channel1->CNDTR = 1;                       // transmit 1 word
  DMA_Channel1->CCR   = 0x00002520;              // configure DMA channel
  DMA_Channel1->CCR  |= (1 << 0);                // DMA Channel 1 enable


  RCC->APB2ENR |= (1<<9);                         // enable periperal clock for ADC1

  ADC1->SQR1  = 0x00000000;                       // only one conversion
  ADC1->SMPR2 = 0x00000028;                       // set sample time channel1 (55,5 cycles)
  ADC1->SQR3  = 0x00000001;                       // set channel1 as 1st conversion

  ADC1->CR1   = 0x00000100;                       // use independant mode, SCAN mode
  ADC1->CR2   = 0x000E0103;                       // use data align right,continuous conversion
                                                  // EXTSEL = SWSTART 
                                                  // enable ADC, DMA mode, no external Trigger
  ADC1->CR2  |=  0x00500000;					  // start SW conversion
}

/*******************************************************************************
* Function Name  : RTC_Configuration
* Description    : Configures RTC as second period
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void RTC_Configuration (void) 
{

	RCC_APB1PeriphClockCmd (RCC_APB1Periph_PWR, ENABLE);          // enable clock for Power interface
	
	PWR->CR      |= 0x00000100;                                   // enable access to RTC, BDC registers

	RCC_LSEConfig (RCC_LSE_ON);									  // enable LSE
	while ((RCC->BDCR & 0x02) == 0);                 			  // Wait for LSERDY = 1 (LSE is ready)
	
	RTC_EnterConfigMode ();	

	RCC_RTCCLKConfig (RCC_RTCCLKSource_LSE);					  // set RTC clock source
	RCC_RTCCLKCmd (ENABLE);                                       // enable RTC clock

  	RTC_SetCounter(0);
	RTC_WaitForLastTask ();
	RTC->CRH |= 1<<0;
																		  
	RTC_ExitConfigMode ();
  	RTC_WaitForLastTask ();				                          // wait until write is finished

 	PWR->CR   &= ~0x00000100;                                     // disable access to RTC registers
}

void NVIC_Configuration (void)
{
  	NVIC_InitTypeDef NVIC_InitStructure;
	/* Configure one bit for preemption priority */
  	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  
  	/* Enable the EXTI0 Interrupt */
  	NVIC_InitStructure.NVIC_IRQChannel 						= EXTI0_IRQChannel;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 	= 0;
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority 			= 0;
  	NVIC_InitStructure.NVIC_IRQChannelCmd					= ENABLE;
  	NVIC_Init(&NVIC_InitStructure);

	/* Enable the EXTI15_10 Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel 						= EXTI15_10_IRQChannel;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 	= 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority			= 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd 					= ENABLE;
	NVIC_Init(&NVIC_InitStructure);   	

	NVIC->Enable[0] = (1 << (RTC_IRQChannel & 0x1F)); 
	        
}

/*******************************************************************************
* Function Name  : SendChar
* Description    : Send a char by USART1
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
int SendChar (int ch)  					/* Write character to Serial Port     */
{                
  USART_SendData(USART1, (unsigned char) ch);
  while (!(USART1->SR & USART_FLAG_TXE));
  return (ch);
}

/*******************************************************************************
* Function Name  : uart_printf
* Description    : Send a string by USART1
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void uart_printf (char *ptr)
{
	while (*ptr) {
		SendChar (*ptr);
		ptr++;	
	}								
}


/**
 *********************************************************************************************
 * @brief		Send a string by USART 
 *   
 * @param[in] 	dat		Data which be sent.		
 * @param[in] 	ct		How long will be print.	 
 * @param[out] 	None  
 * @retval		None	 
 * @details		This function is called to print string with 'ct' bit hex format by USART.   
 *********************************************************************************************
 */
void uart_print_hex (unsigned int dat,char ct)
{
	char i,count;
	unsigned int ddat;
	char ptr[8],tmp;
	ddat = dat;
	count = 0;
	while (ddat != 0) 
	{
		tmp = ddat%16;
		ptr[count] = tmp>9 ? ('A'+tmp-10) : (tmp+'0');
		ddat = ddat/16;						
		count++;
	}
	
	if (count>=ct) 
		count = ct;
	else 
	{
		for (i=0;i<ct-count;i++) 
		{
			SendChar ('0');
		}
	}

	for (i=count;i>0;i--) 
	{
		SendChar(ptr[i-1]);
	}
}


/**
 *********************************************************************************************
 * @brief		Disable RTC interrupt	   
 * @param[in]	None 	 
 * @param[out]	None  
 * @retval		None	 
 * @details	 	This fucntion is called to disable RTC interrupt.
 *********************************************************************************************
 */
void DisableRTCInterrupt(void)
{
	NVIC->Disable[0] = (1<< (RTC_IRQChannel & 0x1F));
}


/**
 *********************************************************************************************
 * @brief		Enable RTC interrupt	   
 * @param[in]	None 	 
 * @param[out]	None  
 * @retval		None	 
 * @details	 	This fucntion is called to enable RTC interrupt.
 *********************************************************************************************
 */
void EnableRTCInterrupt(void)
{
	NVIC->Enable[0] = (1<< (RTC_IRQChannel & 0x1F));
}
