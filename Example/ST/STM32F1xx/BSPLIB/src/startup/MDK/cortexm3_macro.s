;******************** (C) COPYRIGHT 2007 STMicroelectronics ********************
;* File Name          : cortexm3_macro.s
;* Author             : MCD Application Team
;* Date First Issued  : 09/29/2006
;* Description        : Instruction wrappers for special Cortex-M3 instructions.
;*******************************************************************************
; History:
; 04/02/2007: V0.2
; 02/05/2007: V0.1
; 09/29/2006: V0.01
;*******************************************************************************
; THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
; WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
; AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
; INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
; CONTENT OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
; INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
;*******************************************************************************
    
  THUMB
  REQUIRE8
  PRESERVE8
    
  AREA |.text|, CODE, READONLY, ALIGN=2

  ; Exported functions
  EXPORT __WFI
  EXPORT __WFE
  EXPORT __SEV
  EXPORT __ISB
  EXPORT __DSB
  EXPORT __DMB
  EXPORT __SVC
  EXPORT __MRS_CONTROL
  EXPORT __MSR_CONTROL
  EXPORT __SETPRIMASK
  EXPORT __RESETPRIMASK
  EXPORT __SETFAULTMASK
  EXPORT __RESETFAULTMASK
  EXPORT __BASEPRICONFIG
  EXPORT __GetBASEPRI

;*******************************************************************************
; Function Name  : __WFI
; Description    : Assembler function for the WFI instruction.
; Input          : None
; Return         : None
;*******************************************************************************
__WFI 
 
    WFI
    BX r14

;*******************************************************************************
; Function Name  : __WFE
; Description    : Assembler function for the WFE instruction.
; Input          : None
; Return         : None
;*******************************************************************************
__WFE

    WFE
    BX r14

;*******************************************************************************
; Function Name  : __SEV
; Description    : Assembler function for the SEV instruction.
; Input          : None
; Return         : None
;*******************************************************************************
__SEV

    SEV
    BX r14

;*******************************************************************************
; Function Name  : __ISB
; Description    : Assembler function for the ISB instruction.
; Input          : None
; Return         : None
;*******************************************************************************
__ISB

    ISB
    BX r14

;*******************************************************************************
; Function Name  : __DSB
; Description    : Assembler function for the DSB instruction.
; Input          : None
; Return         : None
;*******************************************************************************
__DSB

    DSB
    BX r14

;*******************************************************************************
; Function Name  : __DMB
; Description    : Assembler function for the DMB instruction.
; Input          : None
; Return         : None
;*******************************************************************************
__DMB

    DMB
    BX r14

;*******************************************************************************
; Function Name  : __SVC
; Description    : Assembler function for the SVC instruction.
; Input          : None
; Return         : None
;*******************************************************************************
__SVC

    SVC 0x01
    BX r14

;*******************************************************************************
; Function Name  : __MRS_CONTROL
; Description    : Assembler function for the MRS instruction.
; Input          : None
; Return         : - r0 : Cortex-M3 CONTROL register value.
;*******************************************************************************
__MRS_CONTROL

  MRS r0, CONTROL
  BX r14

;*******************************************************************************
; Function Name  : __MSR_CONTROL
; Description    : Assembler function for the MSR instruction.
; Input          : - R0 : Cortex-M3 CONTROL register new value.  
; Return         : None
;*******************************************************************************
__MSR_CONTROL

  MSR CONTROL, r0
  ISB
  BX r14

;*******************************************************************************
; Function Name  : __SETPRIMASK
; Description    : Assembler function to set the PRIMASK.
; Input          : None 
; Return         : None
;*******************************************************************************
__SETPRIMASK

  CPSID i
  BX r14

;*******************************************************************************
; Function Name  : __RESETPRIMASK
; Description    : Assembler function to reset the PRIMASK.
; Input          : None 
; Return         : None
;*******************************************************************************
__RESETPRIMASK

  CPSIE i
  BX r14

;*******************************************************************************
; Function Name  : __SETFAULTMASK
; Description    : Assembler function to set the FAULTMASK.
; Input          : None 
; Return         : None
;*******************************************************************************
__SETFAULTMASK

  CPSID f
  BX r14

;*******************************************************************************
; Function Name  : __RESETFAULTMASK
; Description    : Assembler function to reset the FAULTMASK.
; Input          : None 
; Return         : None
;*******************************************************************************
__RESETFAULTMASK

  CPSIE f
  BX r14

;*******************************************************************************
; Function Name  : __BASEPRICONFIG
; Description    : Assembler function to set the Base Priority.
; Input          : - R0 : Base Priority new value  
; Return         : None
;*******************************************************************************
__BASEPRICONFIG

  MSR BASEPRI, r0
  BX r14

;*******************************************************************************
; Function Name  : __GetBASEPRI
; Description    : Assembler function to get the Base Priority value.
; Input          : None 
; Return         : - r0 : Base Priority value 
;*******************************************************************************
__GetBASEPRI

  MRS r0, BASEPRI_MAX
  BX r14
  
  END
  
;******************* (C) COPYRIGHT 2007 STMicroelectronics *****END OF FILE*****
