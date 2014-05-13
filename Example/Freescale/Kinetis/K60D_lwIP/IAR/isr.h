/**
 * @file AppConfig.h
 * @date 25 Jul 2013
 * @brief Define interrupt service routines referenced by the vector table.
 *
 * @note
 * Only "vectors.c" should include this header file.
 *
 */

#ifndef __ISR_H
#define __ISR_H

void PendSV_Handler(void);
void SysTick_Handler(void);
void ENET_Transmit_IRQHandler(void);
void ENET_Receive_IRQHandler(void);
void ENET_Error_IRQHandler(void);

#undef  VECTOR_014
#define VECTOR_014 PendSV_Handler

#undef  VECTOR_015
#define VECTOR_015 SysTick_Handler

#undef  VECTOR_092
#define VECTOR_092 ENET_Transmit_IRQHandler

#undef  VECTOR_093
#define VECTOR_093 ENET_Receive_IRQHandler

#undef  VECTOR_094
#define VECTOR_094 ENET_Error_IRQHandler

#endif  //__ISR_H
