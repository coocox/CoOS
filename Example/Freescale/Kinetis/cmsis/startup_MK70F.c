/**
 * @file 	startup_MK70F.c
 * @version V1.0
 * @date    24. Aug 2013
 * @author	Jozef Maslik (maslo@binarylemon.com)
 * @note	Based on Coocox startup_Cortex_M4.c
 * @brief  	MK70Fxxx (Cortex M4F) startup code.
 *          This module performs:
 *              - Set the initial SP
 *              - Set the vector table entries with the exceptions ISR address
 *              - Initialize data and bss
 *          	- Call the application's entry point.
 *          After Reset the Cortex-M4F processor is in Thread mode,
 *        	priority is Privileged, and the Stack is set to Main.
 */


/*----------Stack Configuration-----------------------------------------------*/
#define STACK_SIZE       0x00000100      /*!< Stack size (in Words) */
__attribute__ ((section(".co_stack")))
unsigned long pulStack[STACK_SIZE];


/*----------Macro definition--------------------------------------------------*/
#define WEAK __attribute__ ((weak))


/*----------Declaration of the default fault handlers-------------------------*/
/* System exception vector handler */
__attribute__ ((used))
void WEAK Reset_Handler(void);
void WEAK NMI_Handler(void);
void WEAK HardFault_Handler(void);
void WEAK MemManage_Handler(void);
void WEAK BusFault_Handler(void);
void WEAK UsageFault_Handler(void);
void WEAK SVC_Handler(void);
void WEAK DebugMon_Handler(void);
void WEAK PendSV_Handler(void);
void WEAK SysTick_Handler(void);

/*----------External Exceptions-----------------------------------------------*/
void WEAK DMA0_DMA16_IRQHandler(void);
void WEAK DMA1_DMA17_IRQHandler(void);
void WEAK DMA2_DMA18_IRQHandler(void);
void WEAK DMA3_DMA19_IRQHandler(void);
void WEAK DMA4_DMA20_IRQHandler(void);
void WEAK DMA5_DMA21_IRQHandler(void);
void WEAK DMA6_DMA22_IRQHandler(void);
void WEAK DMA7_DMA23_IRQHandler(void);
void WEAK DMA8_DMA24_IRQHandler(void);
void WEAK DMA9_DMA25_IRQHandler(void);
void WEAK DMA10_DMA26_IRQHandler(void);
void WEAK DMA11_DMA27_IRQHandler(void);
void WEAK DMA12_DMA28_IRQHandler(void);
void WEAK DMA13_DMA29_IRQHandler(void);
void WEAK DMA14_DMA30_IRQHandler(void);
void WEAK DMA15_DMA31_IRQHandler(void);
void WEAK DMA_Error_IRQHandler(void);
void WEAK MCM_IRQHandler(void);
void WEAK FTFE_IRQHandler(void);
void WEAK Read_Collision_IRQHandler(void);
void WEAK LVD_LVW_IRQHandler(void);
void WEAK LLW_IRQHandler(void);
void WEAK Watchdog_IRQHandler(void);
void WEAK RNG_IRQHandler(void);
void WEAK I2C0_IRQHandler(void);
void WEAK I2C1_IRQHandler(void);
void WEAK SPI0_IRQHandler(void);
void WEAK SPI1_IRQHandler(void);
void WEAK SPI2_IRQHandler(void);
void WEAK CAN0_ORed_Message_buffer_IRQHandler(void);
void WEAK CAN0_Bus_Off_IRQHandler(void);
void WEAK CAN0_Error_IRQHandler(void);
void WEAK CAN0_Tx_Warning_IRQHandler(void);
void WEAK CAN0_Rx_Warning_IRQHandler(void);
void WEAK CAN0_Wake_Up_IRQHandler(void);
void WEAK I2S0_Tx_IRQHandler(void);
void WEAK I2S0_Rx_IRQHandler(void);
void WEAK CAN1_ORed_Message_buffer_IRQHandler(void);
void WEAK CAN1_Bus_Off_IRQHandler(void);
void WEAK CAN1_Error_IRQHandler(void);
void WEAK CAN1_Tx_Warning_IRQHandler(void);
void WEAK CAN1_Rx_Warning_IRQHandler(void);
void WEAK CAN1_Wake_Up_IRQHandler(void);
void WEAK Reserved59_IRQHandler(void);
void WEAK UART0_LON_IRQHandler(void);
void WEAK UART0_RX_TX_IRQHandler(void);
void WEAK UART0_ERR_IRQHandler(void);
void WEAK UART1_RX_TX_IRQHandler(void);
void WEAK UART1_ERR_IRQHandler(void);
void WEAK UART2_RX_TX_IRQHandler(void);
void WEAK UART2_ERR_IRQHandler(void);
void WEAK UART3_RX_TX_IRQHandler(void);
void WEAK UART3_ERR_IRQHandler(void);
void WEAK UART4_RX_TX_IRQHandler(void);
void WEAK UART4_ERR_IRQHandler(void);
void WEAK UART5_RX_TX_IRQHandler(void);
void WEAK UART5_ERR_IRQHandler(void);
void WEAK ADC0_IRQHandler(void);
void WEAK ADC1_IRQHandler(void);
void WEAK CMP0_IRQHandler(void);
void WEAK CMP1_IRQHandler(void);
void WEAK CMP2_IRQHandler(void);
void WEAK FTM0_IRQHandler(void);
void WEAK FTM1_IRQHandler(void);
void WEAK FTM2_IRQHandler(void);
void WEAK CMT_IRQHandler(void);
void WEAK RTC_IRQHandler(void);
void WEAK RTC_Seconds_IRQHandler(void);
void WEAK PIT0_IRQHandler(void);
void WEAK PIT1_IRQHandler(void);
void WEAK PIT2_IRQHandler(void);
void WEAK PIT3_IRQHandler(void);
void WEAK PDB0_IRQHandler(void);
void WEAK USB0_IRQHandler(void);
void WEAK USBDCD_IRQHandler(void);
void WEAK ENET_1588_Timer_IRQHandler(void);
void WEAK ENET_Transmit_IRQHandler(void);
void WEAK ENET_Receive_IRQHandler(void);
void WEAK ENET_Error_IRQHandler(void);
void WEAK Reserved95_IRQHandler(void);
void WEAK SDHC_IRQHandler(void);
void WEAK DAC0_IRQHandler(void);
void WEAK DAC1_IRQHandler(void);
void WEAK TSI0_IRQHandler(void);
void WEAK MCG_IRQHandler(void);
void WEAK LPTimer_IRQHandler(void);
void WEAK Reserved102_IRQHandler(void);
void WEAK PORTA_IRQHandler(void);
void WEAK PORTB_IRQHandler(void);
void WEAK PORTC_IRQHandler(void);
void WEAK PORTD_IRQHandler(void);
void WEAK PORTE_IRQHandler(void);
void WEAK PORTF_IRQHandler(void);
void WEAK DDR_IRQHandler(void);
void WEAK SWI_IRQHandler(void);
void WEAK NFC_IRQHandler(void);
void WEAK USBHS_IRQHandler(void);
void WEAK LCD_IRQHandler(void);
void WEAK CMP3_IRQHandler(void);
void WEAK Reserved115_IRQHandler(void);
void WEAK Reserved116_IRQHandler(void);
void WEAK FTM3_IRQHandler(void);
void WEAK ADC2_IRQHandler(void);
void WEAK ADC3_IRQHandler(void);
void WEAK I2S1_Tx_IRQHandler(void);
void WEAK I2S1_Rx_IRQHandler(void);


/*----------Symbols defined in linker script----------------------------------*/
extern unsigned long _sidata;    /*!< Start address for the initialization
                                      values of the .data section.            */
extern unsigned long _sdata;     /*!< Start address for the .data section     */
extern unsigned long _edata;     /*!< End address for the .data section       */
extern unsigned long _sbss;      /*!< Start address for the .bss section      */
extern unsigned long _ebss;      /*!< End address for the .bss section        */
extern void _eram;               /*!< End address for ram                     */


/*----------Function prototypes-----------------------------------------------*/
extern int main(void);                  /*!< The entry point for the application. */
void Default_Reset_Handler(void);       /*!< Default reset handler */
static void Default_Handler(void);      /*!< Default exception handler */


/**
  *@brief The minimal vector table for a Cortex M4.  Note that the proper constructs
  *       must be placed on this to ensure that it ends up at physical address
  *       0x00000000.
  */
__attribute__ ((used, section(".isr_vector")))
void (* const g_pfnVectors[0x100])(void) = {
    /*----------Core Exceptions------------------------------------------------ */
    (void *)&pulStack[STACK_SIZE-1],     /*!< The initial stack pointer         */
    Reset_Handler,                      /*!< Reset Handler */
    NMI_Handler,                        /*!< NMI Handler */
    HardFault_Handler,                  /*!< Hard Fault Handler */
    MemManage_Handler,                  /*!< MPU Fault Handler */
    BusFault_Handler,                   /*!< Bus Fault Handler */
    UsageFault_Handler,                 /*!< Usage Fault Handler */
    0,0,0,0,                            /*!< Reserved */
    SVC_Handler,                        /*!< SVCall Handler */
    DebugMon_Handler,                   /*!< Debug Monitor Handler */
    0,                                  /*!< Reserved */
    PendSV_Handler,                     /*!< PendSV Handler */
    SysTick_Handler,                    /*!< SysTick Handler */

    /*----------External Exceptions-------------------------------------------*/
    DMA0_DMA16_IRQHandler,              /*!< DMA channel 0/16 transfer complete interrupt */
    DMA1_DMA17_IRQHandler,              /*!< DMA channel 1/17 transfer complete interrupt */
    DMA2_DMA18_IRQHandler,              /*!< DMA channel 2/18 transfer complete interrupt */
    DMA3_DMA19_IRQHandler,              /*!< DMA channel 3/19 transfer complete interrupt */
    DMA4_DMA20_IRQHandler,              /*!< DMA channel 4/20 transfer complete interrupt */
    DMA5_DMA21_IRQHandler,              /*!< DMA channel 5/21 transfer complete interrupt */
    DMA6_DMA22_IRQHandler,              /*!< DMA channel 6/22 transfer complete interrupt */
    DMA7_DMA23_IRQHandler,              /*!< DMA channel 7/23 transfer complete interrupt */
    DMA8_DMA24_IRQHandler,              /*!< DMA channel 8/24 transfer complete interrupt */
    DMA9_DMA25_IRQHandler,              /*!< DMA channel 9/25 transfer complete interrupt */
    DMA10_DMA26_IRQHandler,             /*!< DMA channel 10/26 transfer complete interrupt */
    DMA11_DMA27_IRQHandler,             /*!< DMA channel 11/27 transfer complete interrupt */
    DMA12_DMA28_IRQHandler,             /*!< DMA channel 12/28 transfer complete interrupt */
    DMA13_DMA29_IRQHandler,             /*!< DMA channel 13/29 transfer complete interrupt */
    DMA14_DMA30_IRQHandler,             /*!< DMA channel 14/30 transfer complete interrupt */
    DMA15_DMA31_IRQHandler,             /*!< DMA channel 15/31 transfer complete interrupt */
    DMA_Error_IRQHandler,               /*!< DMA error interrupt */
    MCM_IRQHandler,                     /*!< Normal interrupt */
    FTFE_IRQHandler,                    /*!< FTFE interrupt */
    Read_Collision_IRQHandler,          /*!< Read collision interrupt */
    LVD_LVW_IRQHandler,                 /*!< Low Voltage Detect, Low Voltage Warning */
    LLW_IRQHandler,                     /*!< Low Leakage Wakeup */
    Watchdog_IRQHandler,                /*!< WDOG interrupt */
    RNG_IRQHandler,                     /*!< RNGA interrupt */
    I2C0_IRQHandler,                    /*!< I2C0 interrupt */
    I2C1_IRQHandler,                    /*!< I2C1 interrupt */
    SPI0_IRQHandler,                    /*!< SPI0 interrupt */
    SPI1_IRQHandler,                    /*!< SPI1 interrupt */
    SPI2_IRQHandler,                    /*!< SPI2 interrupt */
    CAN0_ORed_Message_buffer_IRQHandler, /*!< CAN0 OR'd message buffers interrupt */
    CAN0_Bus_Off_IRQHandler,            /*!< CAN0 bus off interrupt */
    CAN0_Error_IRQHandler,              /*!< CAN0 error interrupt */
    CAN0_Tx_Warning_IRQHandler,         /*!< CAN0 Tx warning interrupt */
    CAN0_Rx_Warning_IRQHandler,         /*!< CAN0 Rx warning interrupt */
    CAN0_Wake_Up_IRQHandler,            /*!< CAN0 wake up interrupt */
    I2S0_Tx_IRQHandler,                 /*!< I2S0 transmit interrupt */
    I2S0_Rx_IRQHandler,                 /*!< I2S0 receive interrupt */
    CAN1_ORed_Message_buffer_IRQHandler, /*!< CAN1 OR'd message buffers interrupt */
    CAN1_Bus_Off_IRQHandler,            /*!< CAN1 bus off interrupt */
    CAN1_Error_IRQHandler,              /*!< CAN1 error interrupt */
    CAN1_Tx_Warning_IRQHandler,         /*!< CAN1 Tx warning interrupt */
    CAN1_Rx_Warning_IRQHandler,         /*!< CAN1 Rx warning interrupt */
    CAN1_Wake_Up_IRQHandler,            /*!< CAN1 wake up interrupt */
    Reserved59_IRQHandler,              /*!< Reserved interrupt 59 */
    UART0_LON_IRQHandler,               /*!< UART0 LON interrupt */
    UART0_RX_TX_IRQHandler,             /*!< UART0 receive/transmit interrupt */
    UART0_ERR_IRQHandler,               /*!< UART0 error interrupt */
    UART1_RX_TX_IRQHandler,             /*!< UART1 receive/transmit interrupt */
    UART1_ERR_IRQHandler,               /*!< UART1 error interrupt */
    UART2_RX_TX_IRQHandler,             /*!< UART2 receive/transmit interrupt */
    UART2_ERR_IRQHandler,               /*!< UART2 error interrupt */
    UART3_RX_TX_IRQHandler,             /*!< UART3 receive/transmit interrupt */
    UART3_ERR_IRQHandler,               /*!< UART3 error interrupt */
    UART4_RX_TX_IRQHandler,             /*!< UART4 receive/transmit interrupt */
    UART4_ERR_IRQHandler,               /*!< UART4 error interrupt */
    UART5_RX_TX_IRQHandler,             /*!< UART5 receive/transmit interrupt */
    UART5_ERR_IRQHandler,               /*!< UART5 error interrupt */
    ADC0_IRQHandler,                    /*!< ADC0 interrupt */
    ADC1_IRQHandler,                    /*!< ADC1 interrupt */
    CMP0_IRQHandler,                    /*!< CMP0 interrupt */
    CMP1_IRQHandler,                    /*!< CMP1 interrupt */
    CMP2_IRQHandler,                    /*!< CMP2 interrupt */
    FTM0_IRQHandler,                    /*!< FTM0 fault, overflow and channels interrupt */
    FTM1_IRQHandler,                    /*!< FTM1 fault, overflow and channels interrupt */
    FTM2_IRQHandler,                    /*!< FTM2 fault, overflow and channels interrupt */
    CMT_IRQHandler,                     /*!< CMT interrupt */
    RTC_IRQHandler,                     /*!< RTC interrupt */
    RTC_Seconds_IRQHandler,             /*!< RTC seconds interrupt */
    PIT0_IRQHandler,                    /*!< PIT timer channel 0 interrupt */
    PIT1_IRQHandler,                    /*!< PIT timer channel 1 interrupt */
    PIT2_IRQHandler,                    /*!< PIT timer channel 2 interrupt */
    PIT3_IRQHandler,                    /*!< PIT timer channel 3 interrupt */
    PDB0_IRQHandler,                    /*!< PDB0 interrupt */
    USB0_IRQHandler,                    /*!< USB0 interrupt */
    USBDCD_IRQHandler,                  /*!< USBDCD interrupt */
    ENET_1588_Timer_IRQHandler,         /*!< Ethernet MAC IEEE 1588 timer interrupt */
    ENET_Transmit_IRQHandler,           /*!< Ethernet MAC transmit interrupt */
    ENET_Receive_IRQHandler,            /*!< Ethernet MAC receive interrupt */
    ENET_Error_IRQHandler,              /*!< Ethernet MAC error and miscelaneous interrupt */
    Reserved95_IRQHandler,              /*!< Reserved interrupt 95 */
    SDHC_IRQHandler,                    /*!< SDHC interrupt */
    DAC0_IRQHandler,                    /*!< DAC0 interrupt */
    DAC1_IRQHandler,                    /*!< DAC1 interrupt */
    TSI0_IRQHandler,                    /*!< TSI0 interrupt */
    MCG_IRQHandler,                     /*!< MCG interrupt */
    LPTimer_IRQHandler,                 /*!< LPTimer interrupt */
    Reserved102_IRQHandler,             /*!< Reserved interrupt 102 */
    PORTA_IRQHandler,                   /*!< Port A interrupt */
    PORTB_IRQHandler,                   /*!< Port B interrupt */
    PORTC_IRQHandler,                   /*!< Port C interrupt */
    PORTD_IRQHandler,                   /*!< Port D interrupt */
    PORTE_IRQHandler,                   /*!< Port E interrupt */
    PORTF_IRQHandler,                   /*!< Port F interrupt */
    DDR_IRQHandler,                     /*!< DDR interrupt */
    SWI_IRQHandler,                     /*!< Software interrupt */
    NFC_IRQHandler,                     /*!< NAND flash controller interrupt */
    USBHS_IRQHandler,                   /*!< USB high speed OTG interrupt */
    LCD_IRQHandler,                     /*!< Graphical LCD interrupt */
    CMP3_IRQHandler,                    /*!< CMP3 interrupt */
    Reserved115_IRQHandler,             /*!< Reserved interrupt 115 */
    Reserved116_IRQHandler,             /*!< Reserved interrupt 116 */
    FTM3_IRQHandler,                    /*!< FTM3 fault, overflow and channels interrupt */
    ADC2_IRQHandler,                    /*!< ADC2 interrupt */
    ADC3_IRQHandler,                    /*!< ADC3 interrupt */
    I2S1_Tx_IRQHandler,                 /*!< I2S1 transmit interrupt */
    I2S1_Rx_IRQHandler,                 /*!< I2S1 receive interrupt */
};

__attribute__ ((used, section(".isr_vector.cfmprot"))) long cfmprot[4] = {
	0xffffffff,
    0xffffffff,
    0xffffffff,
    0xfffffffe,
};


/**
  * @brief  This is the code that gets called when the processor first
  *         starts execution following a reset event. Only the absolutely
  *         necessary set is performed, after which the application
  *         supplied main() routine is called.
  * @param  None
  * @retval None
  */
void Default_Reset_Handler(void) {
    /* Initialize data and bss */
    unsigned long *pulSrc, *pulDest;
    extern void SystemInit (void);

    SystemInit();

    /* Copy the data segment initializers from flash to SRAM */
    pulSrc = &_sidata;

    for (pulDest = &_sdata; pulDest < &_edata;) {
        *(pulDest++) = *(pulSrc++);
    }

    /* Zero fill the bss segment.  This is done with inline assembly since this
    will clear the value of pulDest if it is not kept in a register. */
    __asm("  ldr     r0, =_sbss\n"
          "  ldr     r1, =_ebss\n"
          "  mov     r2, #0\n"
          "  .thumb_func\n"
          "zero_loop:\n"
          "    cmp     r0, r1\n"
          "    it      lt\n"
          "    strlt   r2, [r0], #4\n"
          "    blt     zero_loop");

    /* Call the application's entry point.*/
    main();
}


/**
  *@brief Provide weak aliases for each Exception handler to the Default_Handler.
  *       As they are weak aliases, any function with the same name will override
  *       this definition.
  */
#pragma weak Reset_Handler = Default_Reset_Handler
#pragma weak NMI_Handler = Default_Handler
#pragma weak HardFault_Handler = Default_Handler
#pragma weak MemManage_Handler = Default_Handler
#pragma weak BusFault_Handler = Default_Handler
#pragma weak UsageFault_Handler = Default_Handler
#pragma weak SVC_Handler = Default_Handler
#pragma weak DebugMon_Handler = Default_Handler
#pragma weak PendSV_Handler = Default_Handler
#pragma weak SysTick_Handler = Default_Handler

/*----------External Exceptions-----------------------------------------------*/
#pragma weak DMA0_DMA16_IRQHandler = Default_Handler
#pragma weak DMA1_DMA17_IRQHandler = Default_Handler
#pragma weak DMA2_DMA18_IRQHandler = Default_Handler
#pragma weak DMA3_DMA19_IRQHandler = Default_Handler
#pragma weak DMA4_DMA20_IRQHandler = Default_Handler
#pragma weak DMA5_DMA21_IRQHandler = Default_Handler
#pragma weak DMA6_DMA22_IRQHandler = Default_Handler
#pragma weak DMA7_DMA23_IRQHandler = Default_Handler
#pragma weak DMA8_DMA24_IRQHandler = Default_Handler
#pragma weak DMA9_DMA25_IRQHandler = Default_Handler
#pragma weak DMA10_DMA26_IRQHandler = Default_Handler
#pragma weak DMA11_DMA27_IRQHandler = Default_Handler
#pragma weak DMA12_DMA28_IRQHandler = Default_Handler
#pragma weak DMA13_DMA29_IRQHandler = Default_Handler
#pragma weak DMA14_DMA30_IRQHandler = Default_Handler
#pragma weak DMA15_DMA31_IRQHandler = Default_Handler
#pragma weak DMA_Error_IRQHandler = Default_Handler
#pragma weak MCM_IRQHandler = Default_Handler
#pragma weak FTFE_IRQHandler = Default_Handler
#pragma weak Read_Collision_IRQHandler = Default_Handler
#pragma weak LVD_LVW_IRQHandler = Default_Handler
#pragma weak LLW_IRQHandler = Default_Handler
#pragma weak Watchdog_IRQHandler = Default_Handler
#pragma weak RNG_IRQHandler = Default_Handler
#pragma weak I2C0_IRQHandler = Default_Handler
#pragma weak I2C1_IRQHandler = Default_Handler
#pragma weak SPI0_IRQHandler = Default_Handler
#pragma weak SPI1_IRQHandler = Default_Handler
#pragma weak SPI2_IRQHandler = Default_Handler
#pragma weak CAN0_ORed_Message_buffer_IRQHandler = Default_Handler
#pragma weak CAN0_Bus_Off_IRQHandler = Default_Handler
#pragma weak CAN0_Error_IRQHandler = Default_Handler
#pragma weak CAN0_Tx_Warning_IRQHandler = Default_Handler
#pragma weak CAN0_Rx_Warning_IRQHandler = Default_Handler
#pragma weak CAN0_Wake_Up_IRQHandler = Default_Handler
#pragma weak I2S0_Tx_IRQHandler = Default_Handler
#pragma weak I2S0_Rx_IRQHandler = Default_Handler
#pragma weak CAN1_ORed_Message_buffer_IRQHandler = Default_Handler
#pragma weak CAN1_Bus_Off_IRQHandler = Default_Handler
#pragma weak CAN1_Error_IRQHandler = Default_Handler
#pragma weak CAN1_Tx_Warning_IRQHandler = Default_Handler
#pragma weak CAN1_Rx_Warning_IRQHandler = Default_Handler
#pragma weak CAN1_Wake_Up_IRQHandler = Default_Handler
#pragma weak Reserved59_IRQHandler = Default_Handler
#pragma weak UART0_LON_IRQHandler = Default_Handler
#pragma weak UART0_RX_TX_IRQHandler = Default_Handler
#pragma weak UART0_ERR_IRQHandler = Default_Handler
#pragma weak UART1_RX_TX_IRQHandler = Default_Handler
#pragma weak UART1_ERR_IRQHandler = Default_Handler
#pragma weak UART2_RX_TX_IRQHandler = Default_Handler
#pragma weak UART2_ERR_IRQHandler = Default_Handler
#pragma weak UART3_RX_TX_IRQHandler = Default_Handler
#pragma weak UART3_ERR_IRQHandler = Default_Handler
#pragma weak UART4_RX_TX_IRQHandler = Default_Handler
#pragma weak UART4_ERR_IRQHandler = Default_Handler
#pragma weak UART5_RX_TX_IRQHandler = Default_Handler
#pragma weak UART5_ERR_IRQHandler = Default_Handler
#pragma weak ADC0_IRQHandler = Default_Handler
#pragma weak ADC1_IRQHandler = Default_Handler
#pragma weak CMP0_IRQHandler = Default_Handler
#pragma weak CMP1_IRQHandler = Default_Handler
#pragma weak CMP2_IRQHandler = Default_Handler
#pragma weak FTM0_IRQHandler = Default_Handler
#pragma weak FTM1_IRQHandler = Default_Handler
#pragma weak FTM2_IRQHandler = Default_Handler
#pragma weak CMT_IRQHandler = Default_Handler
#pragma weak RTC_IRQHandler = Default_Handler
#pragma weak RTC_Seconds_IRQHandler = Default_Handler
#pragma weak PIT0_IRQHandler = Default_Handler
#pragma weak PIT1_IRQHandler = Default_Handler
#pragma weak PIT2_IRQHandler = Default_Handler
#pragma weak PIT3_IRQHandler = Default_Handler
#pragma weak PDB0_IRQHandler = Default_Handler
#pragma weak USB0_IRQHandler = Default_Handler
#pragma weak USBDCD_IRQHandler = Default_Handler
#pragma weak ENET_1588_Timer_IRQHandler = Default_Handler
#pragma weak ENET_Transmit_IRQHandler = Default_Handler
#pragma weak ENET_Receive_IRQHandler = Default_Handler
#pragma weak ENET_Error_IRQHandler = Default_Handler
#pragma weak Reserved95_IRQHandler = Default_Handler
#pragma weak SDHC_IRQHandler = Default_Handler
#pragma weak DAC0_IRQHandler = Default_Handler
#pragma weak DAC1_IRQHandler = Default_Handler
#pragma weak TSI0_IRQHandler = Default_Handler
#pragma weak MCG_IRQHandler = Default_Handler
#pragma weak LPTimer_IRQHandler = Default_Handler
#pragma weak Reserved102_IRQHandler = Default_Handler
#pragma weak PORTA_IRQHandler = Default_Handler
#pragma weak PORTB_IRQHandler = Default_Handler
#pragma weak PORTC_IRQHandler = Default_Handler
#pragma weak PORTD_IRQHandler = Default_Handler
#pragma weak PORTE_IRQHandler = Default_Handler
#pragma weak PORTF_IRQHandler = Default_Handler
#pragma weak DDR_IRQHandler = Default_Handler
#pragma weak SWI_IRQHandler = Default_Handler
#pragma weak NFC_IRQHandler = Default_Handler
#pragma weak USBHS_IRQHandler = Default_Handler
#pragma weak LCD_IRQHandler = Default_Handler
#pragma weak CMP3_IRQHandler = Default_Handler
#pragma weak Reserved115_IRQHandler = Default_Handler
#pragma weak Reserved116_IRQHandler = Default_Handler
#pragma weak FTM3_IRQHandler = Default_Handler
#pragma weak ADC2_IRQHandler = Default_Handler
#pragma weak ADC3_IRQHandler = Default_Handler
#pragma weak I2S1_Tx_IRQHandler = Default_Handler
#pragma weak I2S1_Rx_IRQHandler = Default_Handler


/**
  * @brief  This is the code that gets called when the processor receives an
  *         unexpected interrupt.  This simply enters an infinite loop,
  *         preserving the system state for examination by a debugger.
  * @param  None
  * @retval None
  */
static void Default_Handler(void) {
    /* Go into an infinite loop. */
    while (1);
}
