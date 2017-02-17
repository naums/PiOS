.globl delay_ms
.globl delay

/**
 * \brief returns the base address of the timer
 **/
counteraddr:
    ldr r0, =0x20003000
    mov pc, lr


/**
 * \brief sets a compare-register of the hardware-timer to a specific 32-bit statement
 * \param r0 -> value for the compare-register
 * \param r1 -> channel of the timer
 * \note will not reset the timer
 **/
pios_timer_cmpset:
    push {lr}
    mov r2, r0
    bl counteraddr
    add r0, r0, #0x0c
    add r0, r0, r1, LSL #4
    str r1, [r0]
    pop {pc}

pios_timer_read:
    push {lr}
    bl counteraddr
    ldrd r0, r1, [r0, #4]
    pop {pc}

pios_timer_offset:
    push {lr}
    mov r2, r0
    mov r3, r1
    
    bl pios_timer_read
    
    subs r0, r0, r2 // substract the new value from the older one with carry setting
    sbc r1, r1, r3  // substract the new value (higher 32Bits) from the older ones with carry.
    
    pop {pc}

/**
 * \brief waits for the given amount of seconds
 * \param r0 amount of seconds 
 * \note is maybe not that exact :/
 **/
delay:
    push {lr}
    push {r5}
    mov r5, r0
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

