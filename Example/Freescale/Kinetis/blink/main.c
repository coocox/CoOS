/**
 * @file main.c
 * @date 25 Jul 2013
 * @brief Very simple demo which blink with LED on TWR-K60N512 board
 *
 * @author Jozef Maslik (maslo@binarylemon.com)
 *
 */ 

#include <CoOS.h>

#include <MK60D.h>

#define TASK_LED_STACK_SIZE     128
#define TASK_LED_PRI            3

OS_STK task_led_stack[TASK_LED_STACK_SIZE];

static void task_led(void *pdata) {
    (void)pdata;
    
    PORTA_PCR11 = PORT_PCR_MUX(1);      // set PORTA.11 as GPIO
    GPIOA_PDDR = (1 << 11);             // PORTA.11 output
    
    for (;;) {
        GPIOA_PTOR = (1 << 11);         // toggle LED1
        CoTickDelay (100);
    }		
}

int main() {
    CoInitOS();

    CoCreateTask(task_led, (void *)0, TASK_LED_PRI, &task_led_stack[TASK_LED_STACK_SIZE - 1], TASK_LED_STACK_SIZE);

    CoStartOS();
}
