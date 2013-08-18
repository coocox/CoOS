/******************************************************************************
* File:    isr.h
* Purpose: Define interrupt service routines referenced by the vector table.
* Note: Only "vectors.c" should include this header file.
******************************************************************************/

#ifndef _ISR_H
#define _ISR_H

void PendSV_Handler(void);
void SysTick_Handler(void);
void ethernetif_isr(void);

#undef  VECTOR_014
#define VECTOR_014 PendSV_Handler

#undef  VECTOR_015
#define VECTOR_015 SysTick_Handler

#endif  //_ISR_H
