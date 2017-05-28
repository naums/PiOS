.section ".init"

.globl start
.globl blinkloop

start:
    ldr pc, _reset_h
    ldr pc, _undefined_instruction_vector_h
    ldr pc, _software_interrupt_vector_h
    ldr pc, _prefetch_abort_vector_h
    ldr pc, _data_abort_vector_h
    ldr pc, _unused_handler_h
    ldr pc, _interrupt_vector_h
    ldr pc, _fast_interrupt_vector_h

_reset_h:                           .word   _reset_
_undefined_instruction_vector_h:    .word   undef_vector
_software_interrupt_vector_h:       .word   swi_vector
_prefetch_abort_vector_h:           .word   abort_vector
_data_abort_vector_h:               .word   abort_vector
_unused_handler_h:                  .word   _reset_
_interrupt_vector_h:                .word   irq_vector
_fast_interrupt_vector_h:           .word   fiq_vector

_reset_:
	// Setup the stack.
	ldr	sp, =0x8000
    
    // enable FPU in coprocessor enable register - this gives everybody access to both locations of coprocessor.
    /*ldr r0, =(0xF << 20)
    mcr p15, 0, r0, c1, c0, 2
    
    // enable FPU in FP exception register
    MOV r3, #0x40000000
    #VMSR FPEXC, r3   
    .long 0xeee83a10*/
    
    mov     r0, #0x8000
    mov     r1, #0x0000
    ldmia   r0!,{r2, r3, r4, r5, r6, r7, r8, r9}    /** load 32 Byte worth of data **/
    stmia   r1!,{r2, r3, r4, r5, r6, r7, r8, r9}    /** store 32 Byte **/
    ldmia   r0!,{r2, r3, r4, r5, r6, r7, r8, r9}
    stmia   r1!,{r2, r3, r4, r5, r6, r7, r8, r9}
    
    //bl pios_irq_enable
    
    /* I don't really need a framebuffer right now
    ldr r0, =1920
    ldr r1, =1080
    mov r2, #24
    bl framebufferInit*/
    // returnvalue is frameinfo-address
	
	bl main
	
halt:
	//wfe
	b halt

BOOTUP:
    mov pc, r0
    b halt

/** 
 * enables IRQs in the ARM-core (not FIQs)
 **/
pios_irq_enable:
    mrs r0, cpsr
    bic r0, r0, #0x80
    msr cpsr_c, r0
    
    mov pc, lr     
