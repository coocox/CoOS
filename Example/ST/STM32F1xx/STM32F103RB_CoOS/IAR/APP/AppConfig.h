
#ifndef _APP_CONFIG_H
#define _APP_CONFIG_H

#include "stm32f10x_lib.h"


//#define OS_EVT U16

void RCC_Configuration  (void);
void GPIO_Configuration (void);
void UART_Configuration (void);
void LCD_Configuration  (void);
void ADC_Configuration  (void);
void RTC_Configuration  (void);
void EXIT_Configuration (void);
void NVIC_Configuration (void);
void SysTick_Configuration (void);

void DisableRTCInterrupt(void);
void EnableRTCInterrupt	(void);

void uart_printf (char *ptr);
void uart_print_hex (unsigned int dat,char ct);


extern void lcd_init       (void);
extern void lcd_clear      (void);
extern void lcd_putchar    (char c);
extern void set_cursor     (int column, int line);
extern void lcd_print      (char *string);
extern void lcd_bargraph   (int value, int size);
extern void lcd_bargraphXY (int pos_x, int pos_y, int value);


#endif
