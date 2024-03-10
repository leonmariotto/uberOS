.text
.syntax unified
.thumb

.global HardFault_Handler
.global HardFault_Handler_C

.align 4
.type HardFault_Handler,%function
HardFault_Handler:
  tst lr, #4
  beq HardFault_Handler_msp
  mrs r0, psp 
HardFault_Handler_resume:
  mov r1, lr 
HardFault_Handler_end:
  ldr r2, =HardFault_Handler_C 
  bx r2
HardFault_Handler_msp:
  mrs r0, msp 
  b HardFault_Handler_resume

.text
.syntax unified
.thumb

.global SVC_Handler
.global SVC_Handler_C
.global __svc_exc_return  // EXC_RETURN use by SVC

.type SVC_Handler,%function
SVC_Handler:
  TST    LR, #4   // Extract stack frame location
  beq    SVC_Handler_msp
  MRS    R0, PSP
SVC_Handler_resume:
  LDR    R1,=__svc_exc_return // Save current EXC_RETURN
  STR    LR,[R1]	
  // ldr    r2 =SVC_Handler_C 
  bl     SVC_Handler_C
  LDR    R1,=__svc_exc_return // Load new EXC_RETURN
  LDR    LR,[R1]
  BX     LR
SVC_Handler_msp:
  MRS   R0, MSP
  b     SVC_Handler_resume
.align  4

.text
.syntax unified
.thumb

.global PendSV_Handler
.global PendSV_Handler_C
.global __svc_exc_return  // EXC_RETURN use by SVC
.global __StacksPSP  // Tasks PSP array
.global __curr_task  // current PSP
.global __next_task  // next PSP

.type PendSV_Handler,%function
PendSV_Handler:
  // Context switching code
  // Simple version - assume No floating point support
  // -------------------------
  // Save current context
  MRS    R0, PSP     // Get current process stack pointer value
  MOV    R2, LR
  MRS    R3, CONTROL
  STMDB  R0!,{R2-R11}// Save R2 to R11 in task stack (10 regs)
  LDR    R1,=__curr_task
  LDR    R2,[R1]     // Get current task ID
  LDR    R3,=__TasksPSP
  STR    R0,[R3, R2, LSL #2] // Save PSP value into PSP_array
  // -------------------------
  // Load next context
  LDR    R4,=__next_task
  LDR    R4,[R4]     // Get next task ID
  STR    R4,[R1]     // Set curr_task = next_task
  LDR    R0,[R3, R4, LSL #2] // Load PSP value from PSP_array
  LDMIA  R0!,{R2-R11}// Load R2 to R11 from task stack (10 regs)
  MOV    LR, R2
  MSR    CONTROL, R3
  MSR    PSP, R0     // Set PSP to next task
  BX     LR          // Return
.align  4

.end
