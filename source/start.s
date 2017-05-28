.section ".init"

.globl start
.global BOOTUP
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
    
    /* enabling the ACT-Pin */
    mov r0, #16
    mov r1, #1
    bl gpio_pinmode
    
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


blinkloop:
    mov r0, #47
    mov r1, #1
    bl gpio_write
    
    mov r0, #1
    bl wait
    
    mov r0, #47
    mov r1, #0
    bl gpio_write
    
    mov r0, #1
    bl wait
    b blinkloop  

gpio_address:
    ldr r0, =0x20200000;
    mov pc, lr;

/**
 * \brief writes a value to the GPIO-Pin (i.e. writes 1 to set or 1 to clear)
 * \param r0 GPIO-Pin
 * \param r1 output-value (1 for on, 0 for off)
 **/
gpio_write:
    cmp r0, #53
    movhi pc, lr

    //cmp r1, #1
    //movhi pc, lr
    
    mov r2, r0
    push {lr}
    bl gpio_address
    // r0 -> GPIO-base address
    // r1 -> output value
    // r2 -> GPIO-Pin
    
    //! still r1 is referenced here
    cmp r1, #0
    addhi r0, #28
    addls r0, #40
    
    cmp r2, #31
    subhi r2, #32
    addhi r0, #4

    mov r3, #1
    lsl r3, r2
    
    str r3, [r0]

    pop {pc}

/**
 * \brief sets the PIN into the mode we want
 * \param r0 GPIO-Pin
 * \param r1 GPIO-Mode
 * \return NONE
 **/
gpio_pinmode:
    /** if (r0>53 || r1>7) return **/
    cmp r0, #53
    cmpls r1, #7
    movhi pc, lr
    
    push {lr};
    
    // save r0 (the pinnumber)
    mov r2, r0
    bl gpio_address;
    // r0 => GPIO_address
    // r1 => mode
    // r2 => pin
    
    pincalc:
        cmp r2, #9
        subhi r2, #10
        addhi r0, #4
        bhi pincalc
        
    // r0 => correct address for the pin
    // r1 => mode (0-7)
    // r2 => pin-offset (0-9)
    
    // r2 = r2*3, because we need to shift r1 by r2
    add r2, r2, lsl #1
    lsl r1, r2

    push {r4,r6}
    mov r6, #7 // 000...00111
    lsl r6, r2
    mvn r6, r6    

    // get the currently set GPIO-register-values 
    ldr r4, [r0,r2]
    // mask the according values to 0
    and r4, r4, r6
    // add the mode to the mode-vector
    orr r4, r4, r1
    // store the mode-vector back to the GPIO-memory
    str r4, [r0]
    
    pop {r4,r6}
    // return
    pop {pc}

BOOTUP:
    mov pc, r0
    b halt

.globl GET32
.globl PUT32
.globl dummy
.globl BOOTUP

PUT32:
    str r1, [r0]
    mov pc, lr

GET32:
    ldr r0, [r0]
    mov pc, lr

dummy:
    mov pc, lr

/** 
 * enables IRQs in the ARM-core (not FIQs)
 **/
pios_irq_enable:
    mrs r0, cpsr
    bic r0, r0, #0x80
    msr cpsr_c, r0
    
    mov pc, lr     
