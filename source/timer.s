.globl delay_ms
.globl delay

/**
 * \brief returns the base address of the timer
 **/
counteraddr:
    ldr r0, =0x20003000
    mov pc, lr

/**
 * \brief waits for the given amount of seconds
 * \param r0 amount of seconds 
 * \note is maybe not that exact :/
 **/
delay:
    push {lr}
    push {r5}
    mov r5, r0
    // using ldr, not 'mov r0, #1000000' because immediate
    // is not big enough to hold decimal 1000000
delay_inner:
    ldr r0, =1000000
    cmp r5, #0    
    bls delay_out;
    
    sub r5, #1
    bl delay_ms
    b delay_inner

delay_out:
    pop {r5}
    pop {pc}
    
/**
 * \brief waits for the given amount of microseconds
 * \param r0 amount of microseconds
 **/
delay_ms:
    push {lr}
    push {r6,r7}
    mov r4, r0
    bl counteraddr
    ldr r2, [r0, #4]
    ldr r3, [r0, #8]
    add r1, r2, r4
    addcs r3, #1
    
delay_ms_inner:    
    ldr r6, [r0, #4]
    ldr r7, [r0, #8]
    cmp r1, r6
    cmpls r3, r7
    bhi delay_ms_inner
    pop {r6, r7}
    pop {pc}
 
