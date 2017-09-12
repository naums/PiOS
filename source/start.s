.section ".init"

.globl start

irq_vector_start:
_start:
    ldr pc, _reset_h
    ldr pc, _undefined_instruction_vector_h
    ldr pc, _software_interrupt_vector_h
    ldr pc, _prefetch_abort_vector_h
    ldr pc, _data_abort_vector_h
    ldr pc, _unused_handler_h
    ldr pc, _interrupt_vector_h
    ldr pc, _fast_interrupt_vector_h

_reset_h:                           .word   _reset_
_undefined_instruction_vector_h:    .word   pios_exception_undef
_software_interrupt_vector_h:       .word   pios_exception_swi
_prefetch_abort_vector_h:           .word   pios_exception_abort
_data_abort_vector_h:               .word   pios_exception_abort
_unused_handler_h:                  .word   _reset_
_interrupt_vector_h:                .word   pios_exception_irq
_fast_interrupt_vector_h:           .word   pios_exception_fiq

_reset_:
    ldr sp, =stack_top
    
    /// copies interrupt vectors to address 0x0000 0000 
    /*mov     r0, #0x00
    ldr     r1, =irq_vector_start
    ldmia   r0!,{r2, r3, r4, r5, r6, r7, r8, r9}
    stmia   r1!,{r2, r3, r4, r5, r6, r7, r8, r9}
    ldmia   r0!,{r2, r3, r4, r5, r6, r7, r8, r9}
    stmia   r1!,{r2, r3, r4, r5, r6, r7, r8, r9}
    
    mrs r0, cpsr
    bic r1, r0, #0x1f
    orr r1, r1, #0x12   // IRQ mode
    msr cpsr, r1
    ldr sp, =irq_stack_top
    
    bic r1, r1, #0x02   // FIQ mode
    orr r1, r1, #0x01
    msr cpsr, r1
    ldr sp, =fiq_stack_top
    
    bic r0, r0, #0x80       /// enable interrupts and return to supervisor mode
    msr cpsr, r0*/
    
    /// load C15 control register c1-c1,0,0 (c1 Control Register) into r0
    /*mrc p15, 0, r0, c1, c0, 0
    ldr r1, =0x00400000     /// set bit 22 (U-bit)
    orr r0, r1
    mcr p15, 0, r0, c1, c0, 0*/
	
	bl main
	
halt:
	//wfe
	b halt
