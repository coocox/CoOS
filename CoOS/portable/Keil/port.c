/**
 *******************************************************************************
 * @file       prot.c
 * @version   V1.1.4    
 * @date      2011.04.20
 * @brief      Compiler adapter for CooCox CoOS kernel.	
 *******************************************************************************
 * @copy
 *
 * INTERNAL FILE,DON'T PUBLIC.
 * 
 * <h2><center>&copy; COPYRIGHT 2010 CooCox </center></h2>
 *******************************************************************************
 */ 

/*---------------------------- Include ---------------------------------------*/
#include <coocox.h>


/**
 ******************************************************************************
 * @brief      Plus a byte integers and Saved into memory cell
 * @param[in]  data    byte integers.	 
 * @param[out] None  
 * @retval     Returns Original value.		 
 *
 * @par Description
 * @details    This function is called to Plus a byte integers 
 *             and Saved into memory cell.
 ******************************************************************************
 */
__asm U8  Inc8 (volatile U8 *data)
{
    PUSH    {R1}
    CPSID   I
    LDRB    R1,[R0]
    ADDS    R1,#1
    STRB    R1,[R0]
    CPSIE   I
    SUBS    R1,#1
    MOVS    R0,R1
    POP     {R1}
    BX      LR	
    ALIGN
}


/**
 ******************************************************************************
 * @brief      Decrease a byte integers and Saved into memory cell
 * @param[in]  data    byte integers.	 
 * @param[out] None  
 * @retval     Returns Original value.		 
 *
 * @par Description
 * @details    This function is called to Decrease a byte integers 
 *             and Saved into memory cell.
 ******************************************************************************
 */
__asm U8 Dec8 (volatile U8 *data)
{
    PUSH    {R1}
    CPSID   I
    LDRB    R1,[R0]
    SUBS    R1,#1
    STRB    R1,[R0]
    CPSIE   I
    MOVS    R0,R1
    POP     {R1}
    BX      LR
    ALIGN
}

/**
 ******************************************************************************
 * @brief      ENABLE Interrupt
 * @param[in]  None	 
 * @param[out] None  
 * @retval     None		 
 *
 * @par Description
 * @details    This function is called to ENABLE Interrupt.
 ******************************************************************************
 */
__asm void IRQ_ENABLE_RESTORE(void)
{   CPSIE   I
    BX      LR
}

/**
 ******************************************************************************
 * @brief      Close Interrupt
 * @param[in]  None	 
 * @param[out] None  
 * @retval     None		 
 *
 * @par Description
 * @details    This function is called to close Interrupt.
 ******************************************************************************
 */
__asm void IRQ_DISABLE_SAVE(void)
{   CPSID   I
    BX      LR
}

/**
 ******************************************************************************
 * @brief      Set environment	for Coocox OS running
 * @param[in]  pstk    stack pointer	 
 * @param[out] None  
 * @retval     None.		 
 *
 * @par Description
 * @details    This function is called to Set environment
 *              for Coocox OS running.
 ******************************************************************************
 */
__asm  void SetEnvironment(OS_STK *pstk)
{
    SUBS    R0,#28
    MSR     PSP, R0             ; Mov new stack point to PSP
    BX      LR	
    ALIGN
}


/**
 ******************************************************************************
 * @brief      Do ready work to Switch Context for task change
 * @param[in]  None	 
 * @param[out] None  
 * @retval     None.		 
 *
 * @par Description
 * @details    This function is called to Do ready work to 
 *              Switch Context for task change
 ******************************************************************************
 */
__asm void SwitchContext(void)
{
    LDR     R0, =0xE000ED04  ; Trigger the PendSV exception (causes context switch)
    LDR     R1, =0x10000000
    STR     R1, [R0]
    BX      LR
    ALIGN	
}


/**
 ******************************************************************************
 * @brief      Switch Context for task change
 * @param[in]  None	 
 * @param[out] None  
 * @retval     None.		 
 *
 * @par Description
 * @details    This function is called to Switch Context for task change.
 ******************************************************************************
 */
#if CFG_CHIP_TYPE == 2
__asm  void PendSV_Handler()
{
    IMPORT  TCBRunning
    IMPORT  TCBNext
    IMPORT  OSSchedLock
    LDR     R3,=TCBRunning
    LDR     R1,[R3]             ; R1 == running tcb
    LDR     R2,=TCBNext
    LDR     R2,[R2]             ; R2 == next tcb

    CMP     R1,R2
    BEQ     exitPendSV                
    MRS     R0, PSP             ; Get PSP point (can not use PUSH,in ISR,SP is MSP )
  
    SUBS    R0,R0,#32
    STR     R0,[R1]             ; Save orig PSP
								; Store r4-r11,r0 -= regCnt * 4,r0 is new stack 
                                ; top point (addr h->l r11,r10,...,r5,r4)
    STMIA   R0!,{R4-R7}         ; Save old context (R4-R7)
    MOV     R4,R8
    MOV     R5,R9
    MOV     R6,R10
    MOV     R7,R11
    STMIA   R0!,{R4-R7}         ; Save old context (R8-R11)


popStk  
    STR     R2, [R3]            ; TCBRunning  = TCBNext;
    LDR     R0, [R2]            ; Get SP of task that be switch into.
		
    ADDS    R0,	R0,#16
    LDMIA   R0!,{R4-R7}         ; Restore new Context (R8-R11)
    MOV     R8,R4
    MOV     R9,R5
    MOV     R10,R6
    MOV     R11,R7
    SUBS    R0,R0,#32
    LDMIA   R0!,{R4-R7}         ; Restore new Context (R4-R7)
    ADDS    R0,	R0,#16
    MSR     PSP, R0             ; Mov new stack point to PSP

exitPendSV    
   LDR     R3,=OSSchedLock
   MOVS    R0, #0x0
   STRB    R0, [R3]
   MOVS    R0,#4
   RSBS    R0,#0               ; =0xFFFFFFFC,Ensure exception return uses process stack
   BX      R0                  ; Exit interrupt
   ALIGN
}
#endif


#if CFG_CHIP_TYPE == 1
__asm  void PendSV_Handler()
{
    IMPORT  TCBRunning
    IMPORT  TCBNext
    IMPORT  OSSchedLock
    LDR     R3,=TCBRunning
    LDR     R1,[R3]             ; R1 == running tcb
    LDR     R2,=TCBNext
    LDR     R2,[R2]             ; R2 == next tcb

    CMP     R1,R2
    BEQ     exitPendSV                
    MRS     R0, PSP             ; Get PSP point (can not use PUSH,in ISR,SP is MSP )
    STMDB   R0!,{R4-R11}        ; Store r4-r11,r0 -= regCnt * 4,r0 is new stack 
                                ; top point (addr h->l r11,r10,...,r5,r4)
    STR     R0,[R1]             ; Save orig PSP
popStk  
    STR     R2, [R3]            ; TCBRunning  = TCBNext;
    LDR     R0, [R2]            ; Get SP of task that be switch into.
    LDMIA   R0!,{R4-R11}        ; POP (R4-R11),R0 += regCnt * 4
    MSR     PSP, R0             ; Mov new stack point to PSP

exitPendSV    
    LDR     R3,=OSSchedLock
    MOVS    R0, #0x0
    STRB    R0, [R3]
    ORR     LR, LR, #0x04       ; Ensure exception return uses process stack
    BX      LR                  ; Exit interrupt
    
    ALIGN
}
#endif


