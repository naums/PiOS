/**
 * \brief implements access to the GPIO-Pins
 * \file gpio.s
 * \author Stefan Naumann
 **/

.globl wait
.globl gpio_address
.globl gpio_write
.globl gpio_pinmode

/**
 * \brief waits for about r0 seconds
 * \param r0 amount of time the pi should wait (in seconds)
 * \return NONE
 **/
wait:
    cmp r0, #0
    beq wait_back
    sub r0, r0, #1
    
    mov r2, #0x800000
innerloop:
    sub r2,#1
    cmp r2,#0
    bne innerloop;
    b wait
wait_back:    
    /* jump back */
    mov pc, lr

/********************/
/** GPIO - SECTION **/
/********************/

/**
 * \brief returns the GPIO-base address in r0
 * \return r0 GPIO-base address
 **/
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
    
