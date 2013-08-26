/**
 * @file main.c
 * @date 25 Jul 2013
 * @brief Very simple demo which blink with LED on TWR-K60N512 board
 *
 * @author Jozef Maslik (maslo@binarylemon.com)
 *
 */

//#include <CoOS.h>
#include <AppConfig.h>
#include <coocox.h>

#include <string.h>

#define TASK_LED_STACK_SIZE     1000
#define TASK_LED_PRI            3

#define TASK_FPU_STACK_SIZE     1000
#define TASK_FPU_PRI            3

OS_STK task_led_stack[TASK_LED_STACK_SIZE];
OS_STK task_fpu0_stack[TASK_FPU_STACK_SIZE];
OS_STK task_fpu1_stack[TASK_FPU_STACK_SIZE];
OS_STK task_fpu2_stack[TASK_FPU_STACK_SIZE];

OS_TID led_id;

CO_FLOAT_ISR(PIT0_IRQHandler) {
    uint32_t tmp;
    CoEnterISR();

    PIT_TFLG0 = PIT_TFLG_TIF_MASK;

    tmp = 0x12345678;

    asm volatile (
            "vldr s0, [%0] \n"
            "vldr s10, [%0] \n"
            "vldr s20, [%0] \n"
            "vldr s30, [%0] \n" : : "r"(&tmp));

    CoExitISR();
}

static void task_led(void *pdata) {
    (void)pdata;

    // init pit0
    PIT_MCR &= ~PIT_MCR_MDIS_MASK;
    PIT_MCR |= PIT_MCR_FRZ_MASK;
    PIT_LDVAL0 = PIT_LDVAL_TSV(CFG_CPU_FREQ / 100);
    PIT_TCTRL0 = PIT_TCTRL_TEN_MASK | PIT_TCTRL_TIE_MASK;

    NVIC_SetPriority(PIT0_IRQn, 6);
    NVIC_EnableIRQ(PIT0_IRQn);

    // led
    PORTA_PCR10 = PORT_PCR_MUX(1);      // set PORTA.10 as GPIO
    GPIOA_PDDR = (1 << 10);             // PORTA.10 output

    for (;;) {
        GPIOA_PTOR = (1 << 10);         // toggle LED
        CoTickDelay (100);
    }
}

static float tst[3][32], templ[3][32];

static void task_fpu(void *pdata) {
    float *p = templ[(int)pdata];
    int i;

//    CoSchedLock();
    __disable_irq();

    for (i = 0; i < 32; i++) {
        templ[(int)pdata][i] = 10 * ((int)pdata + 1);
    }

    // initialize test content
    asm volatile ("mov r0, %0 \n"
                  "vldmia r0!, {s0 - s15} \n"
                  "vldmia r0!, {s16 - s31} \n" : : "r"(p) : "r0");
//    CoSchedUnlock();
    __enable_irq();


    while (1) {
//        CoSchedLock();
        __disable_irq();
        p = tst[(int)pdata];
        asm volatile ("mov r0, %0 \n"
                      "vstmia r0!, {s0 - s15} \n"
                      "vstmia r0!, {s16 - s31} \n" : : "r"(p) : "r0");
//        CoSchedUnlock();
        __enable_irq();

        if (memcmp(&templ[(int)pdata][0], &tst[(int)pdata][0], sizeof(templ[0]))) {
            // error, content is different
            CoDelTask(led_id);
            while (1);
        }
        CoTickDelay((int)pdata * 3);
    }
}

int main() {
    CoInitOS();

    led_id = CoCreateTask(task_led, (void*)0, TASK_LED_PRI, &task_led_stack[TASK_LED_STACK_SIZE - 1], TASK_LED_STACK_SIZE);
    CoCreateTask(task_fpu, (void*)0, TASK_FPU_PRI, &task_fpu0_stack[TASK_FPU_STACK_SIZE - 1], TASK_FPU_STACK_SIZE);
    CoCreateTask(task_fpu, (void*)1, TASK_FPU_PRI, &task_fpu1_stack[TASK_FPU_STACK_SIZE - 1], TASK_FPU_STACK_SIZE);
    CoCreateTask(task_fpu, (void*)2, TASK_FPU_PRI, &task_fpu2_stack[TASK_FPU_STACK_SIZE - 1], TASK_FPU_STACK_SIZE);

    CoStartOS();

    return 0;
}
