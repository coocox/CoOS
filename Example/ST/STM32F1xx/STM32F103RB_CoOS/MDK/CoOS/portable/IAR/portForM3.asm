;*******************************************************************************
;                         PUBLIC FUNCTIONS
;*******************************************************************************
		
	; Functions declared in this file
	PUBLIC  Inc8              ; U8   Inc8(U8 *data);
	PUBLIC  Dec8              ; U8   Dec8(U8 *data); 
	PUBLIC  IRQ_ENABLE_RESTORE; void IRQ_ENABLE_RESTORE(void) ;
  PUBLIC  IRQ_DISABLE_SAVE  ; void IRQ_DISABLE_SAVE(void) ; 
	PUBLIC  SwitchContext     ; void SwitchContext(void); 
	PUBLIC  PendSV_Handler    ; void PendSV_Handler(void);
	PUBLIC	SetEnvironment	  ; void SetEnvironment(void);

	EXTERN  TCBRunning
	EXTERN  TCBNext
	EXTERN  OSSchedLock

;*******************************************************************************
;                              EQUATES
;*******************************************************************************	
NVIC_INT_CTRL   EQU     0xE000ED04      ; Interrupt control state register
NVIC_SYSPRI2    EQU     0xE000ED22      ; System priority register (PendSV 14)
NVIC_PENDSV_PRI EQU           0xFF      ; PendSV priority value (Lowest)
NVIC_PENDSVSET  EQU     0x10000000      ; Value to trigger PendSV exception


    RSEG    CODE:CODE(2)
    thumb


;U8 Inc8(U8 *data);
Inc8
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


;U8 Dec8 (U8 *data);
Dec8
    PUSH    {R1}
    CPSID   I
    LDRB    R1,[R0]
    SUBS    R1,#1
    STRB    R1,[R0]
    CPSIE   I
    MOVS    R0,R1
    POP     {R1}
    BX      LR

IRQ_ENABLE_RESTORE
    CPSIE   I
    BX      LR
	
IRQ_DISABLE_SAVE	
	  CPSID   I
    BX      LR
    
SetEnvironment
    SUBS    R0, #28
    MSR     PSP, R0
    BX      LR	
 
 	
; void SwitchContext(void)
SwitchContext
    LDR     R0, =NVIC_INT_CTRL  ; Trigger the PendSV exception (causes context switch)
    LDR     R1, =NVIC_PENDSVSET
    STR     R1, [R0]
    BX	    LR


; void PendSV_Handler(void)
PendSV_Handler
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
      
    STR     R2, [R3]            ; TCBRunning  = TCBNext;
    LDR     R0, [R2]            ; Get SP of task that be switch into.
    LDMIA   R0!,{R4-R11}        ; POP {R4-R11},R0 += regCnt * 4
    MSR     PSP, R0             ; Mov new stack point to PSP
  
exitPendSV  
    LDR     R3,=OSSchedLock
    MOVS    R0, #0x0
    STRB    R0, [R3]
    ORR     LR, LR, #0x04       ; Ensure exception return uses process stack
    BX      LR                  ; Exit interrupt
    
    END	
	