;********************************************************************************************************
;                                               uC/OS-II
;                                         The Real-Time Kernel
;
*                                       LM3S5956 Specific code
*
*                                          CCS
*   Reference Doc:  Doc0:Compiler SPNU151H
*                   Doc1:Datasheet SPMS211L
*                   Doc2:Instruction Set
*                   Doc3:Driver code
;
; File         : OS_CPU_A.ASM
; By           : Stone903
;********************************************************************************************************

;********************************************************************************************************
;                                    PUBLIC and EXTERNAL REFERENCES
;********************************************************************************************************

    .global  OSRunning                                           ; External references
    .global  OSPrioCur
    .global  OSPrioHighRdy
    .global  OSTCBCur
    .global  OSTCBHighRdy
    .global  OSIntExit
    .global  OSTaskSwHook


    .global  OS_CPU_SR_Save                                      ; Functions declared in this file
    .global  OSStartHighRdy
    .global  OSCtxSw
    .global  OSIntCtxSw
    .global  OS_CPU_PendSVHandler
    
    .align  4
 

||CPU_OSRunning||:     .field  OSRunning,32
||CPU_OSPrioCur||:     .field  OSPrioCur,32
||CPU_OSPrioHighRdy||: .field  OSPrioHighRdy,32
||CPU_OSTCBCur||:     .field  OSTCBCur,32
||CPU_OSTCBHighRdy||:  .field  OSTCBHighRdy,32


;********************************************************************************************************
;                                                EQUATES
;********************************************************************************************************

NVIC_INT_CTRL   .word     0xE000ED04                              ; Interrupt control state register.
NVIC_SYSPRI14   .word     0xE000ED22                              ; System priority register (priority 14).
NVIC_PENDSV_PRI .word           0xFF                              ; PendSV priority value (lowest).
NVIC_PENDSVSET  .word     0x10000000    
   
            
OS_CPUcpsie	
				MSR PRIMASK,R0			;
				CPSIE	i
				BX LR 
      
;*********************************************************************************************************
;                                          START MULTITASKING
;                                       void OSStartHighRdy(void)
;
; The stack frame is assumed to look as follows:
;
; OSTCBHighRdy->OSTCBStkPtr --> R4                               (Low memory)
;                               R5
;                               R6
;                               R7
;                               R8
;                               R9
;                               R10
;                               R11
;                               R0
;                               R1
;                               R2
;                               R3
;                               R12
;                               LR
;                               PC
;                               XPSR                             (High memory)
;                                         
;
; Note : OSStartHighRdy() MUST:
;           a) Call OSTaskSwHook() then,
;           b) Set OSRunning to TRUE,
;           c) Switch to the highest priority task.
;*********************************************************************************************************

OSStartHighRdy            
            
            LDR R0,NVIC_SYSPRI14               ;Set PendSV's priority to lowest
            LDR R1,NVIC_PENDSV_PRI
            STR R1,[R0]
            
            MOVS R0, #0                         ; Set the PSP to 0 for initial context switch call
            MSR  PSP,R0
            
            MOV R0,#0X1
            LDR R1,CPU_OSRunning                   ;Load the address of _OSRunning
            STR R0,[R1]                         ;Store R0 to the [R1]'S addresss Memory unit
            
            LDR R0,NVIC_INT_CTRL                ;Load the address of INTCTRL
            LDR R1,NVIC_PENDSVSET               ;changes the PendSV exception state to pending                              
            STR R1,[R0]                          ;Store configuration to INTCTRL
                              ;Interrupt happen Goto PendSV handler
            
            CPSIE I                              ;Enable the interrupt.
OSStartHang
    B       OSStartHang                                         ; Should never get here           
            
;********************************************************************************************************
;                                         HANDLE PendSV EXCEPTION
;                                     void OS_CPU_PendSVHandler(void)
;
; Note(s) : 1) PendSV is used to cause a context switch.  This is a recommended method for performing
;              context switches with Cortex-M3.  This is because the Cortex-M3 auto-saves half of the
;              processor context on any exception, and restores same on return from exception.  So only
;              saving of R4-R11 is required and fixing up the stack pointers.  Using the PendSV exception
;              this way means that context saving and restoring is identical whether it is initiated from
;              a thread or occurs due to an interrupt or exception.
;
;           2) Pseudo-code is:
;              a) Get the process SP, if 0 then skip (goto d) the saving part (first context switch);
;              b) Save remaining regs r4-r11 on process stack;
;              c) Save the process SP in its TCB, OSTCBCur->OSTCBStkPtr = SP;
;              d) Call OSTaskSwHook();
;              e) Get current high priority, OSPrioCur = OSPrioHighRdy;
;              f) Get current ready thread TCB, OSTCBCur = OSTCBHighRdy;
;              g) Get new process SP from TCB, SP = OSTCBHighRdy->OSTCBStkPtr;
;              h) Restore R4-R11 from new process stack;
;              i) Perform exception return which will restore remaining context.
;
;           3) On entry into PendSV handler:
;              a) The following have been saved on the process stack (by processor):
;                 xPSR, PC, LR, R12, R0-R3
;              b) Processor mode is switched to Handler mode (from Thread mode)
;              c) Stack is Main stack (switched from Process stack)
;              d) OSTCBCur      points to the OS_TCB of the task to suspend
;                 OSTCBHighRdy  points to the OS_TCB of the task to resume
;
;           4) Since PendSV is set to lowest priority in the system (by OSStartHighRdy() above), we
;              know that it will only be run when no other exception or interrupt is active, and
;              therefore safe to assume that context being switched out was using the process stack (PSP).
;********************************************************************************************************
            
OS_CPU_PendSVHandler
    CPSID   I
    MRS     R0,PSP
    CBZ     R0,OS_CPU_PendSVHandler_nosave          ;skip for first time, it is ready when stack init
    
          ;Initial the stack, save the context
    STMDB R0!,{R4-R11}                              ;Store R4-R11 to  stack contex                                              
    LDR   R1,CPU_OSTCBCur                              ;Load the address of _OSTCBCur
    LDR   R1,[R1]                                   ;Load the value of _OSTCBCur(the address of SP)
    STR   R0,[R1]                                   ;Store the new SP
          ;Save the normal register into Stack Done
          
OS_CPU_PendSVHandler_nosave
                ;Call Hook function
    PUSH  {R14}                           ;Save LR exc_return value
    B     OSTaskSwHook                   ; Call user defined task switch hook
    POP   {R14}
                ;OSPrioCur = OSPrioHighRdy;
    LDR   R0,CPU_OSPrioCur                    ;Load the address of OSPrioCur
    LDR   R1,CPU_OSPrioHighRdy                ;Load the address of OSPrioHighRdy
    LDR   R2,[R1]                         ;Load the value of OSPrioHighRdy
    STR   R2,[R0]                         ;OSPrioCur = OSPrioHighRdy;
                ;OSTCBCur  = OSTCBHighRdy;
    LDR   R0,CPU_OSTCBCur                   
    LDR   R1,CPU_OSTCBHighRdy
    LDR   R2,[R1]
    STR   R2,[R0]                        ;R2 is the address of new SP
                ;recovery the context
    LDR   R0,[R2]                        ;New value of New SP
    LDMIA R0!,{R4-R11}                   ;Load new value to R4-R11
    MSR   PSP,R0                         ;Save new SP    
    ORR   LR, LR, #0x04                ; Ensure exception return uses process stack
    CPSIE I
    
    BX    LR

    
    
;********************************************************************************************************
;                               PERFORM A CONTEXT SWITCH (From task level)
;                                           void OSCtxSw(void)
;
; Note(s) : 1) OSCtxSw() is called when OS wants to perform a task context switch.  This function
;              triggers the PendSV exception which is where the real work is done.
;********************************************************************************************************

OSCtxSw
    LDR     R0, NVIC_INT_CTRL                                  ; Trigger the PendSV exception (causes context switch)
    LDR     R1, NVIC_PENDSVSET
    STR     R1, [R0]
    BX      LR

;********************************************************************************************************
;                             PERFORM A CONTEXT SWITCH (From interrupt level)
;                                         void OSIntCtxSw(void)
;
; Notes:    1) OSIntCtxSw() is called by OSIntExit() when it determines a context switch is needed as
;              the result of an interrupt.  This function simply triggers a PendSV exception which will
;              be handled when there are no more interrupts active and interrupts are enabled.
;********************************************************************************************************

OSIntCtxSw
    LDR     R0, NVIC_INT_CTRL                                  ; Trigger the PendSV exception (causes context switch)
    LDR     R1, NVIC_PENDSVSET
    STR     R1, [R0]
    BX      LR

   
   .end