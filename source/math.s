/**
 * \file math.s
 * \author Stefan Naumann
 * \brief implements unsigned div and signed div
 **/

.globl udiv
.globl sdiv

/**
 * \brief divides r0 / r1 -> remainder: r1
 * \param r0 numerator
 * \param r1 denumerator
 * \return r0 -> result, r1 -> remainder
 * \note based on wikipedia (https://en.wikipedia.org/wiki/Division_algorithm#Integer_division_.28unsigned.29_with_remainder)
 **/ 
udiv:
    mov r2, #0  // quotient
    mov r3, #0  // remainder
    mov r4, #32
    
    push {r5, r6}
    mov r6, #1
    $loopudiv:
        sub r4, #1
        lsl r3, #1
        and r5, r0, r6, lsl r4
        cmp r5, #0
        addhi r3, #1
        
        cmp r1, r3
        subls r3, r3, r1
        orrls r2, r2, r6, lsl r4
    
        cmp r4, #0
        bhi $loopudiv
    
    pop {r5, r6}
    
    mov r0, r2
    mov r1, r3    
    mov pc, lr

/**
 * \brief signed integer division
 * \param r0 numerator
 * \param r1 denumerator
 * \return r0 -> result, r1 -> remainder
 * \note based on wikipedia (https://en.wikipedia.org/wiki/Division_algorithm#Integer_division_.28unsigned.29_with_remainder)
 **/
sdiv:
    push {lr}
    ldr r2, =0x7fffffff
    push {r0,r1}
    // absolute value of r0 and r1
    and r0, r2
    and r1, r2
    
    pop {r3, r4}
    cmp r0, r3
    cmpeq r1, r4
    beq unsigned
    
    cmp r0, r3
    cmpne r1, r4
    bne unsigned
    
    cmp r3, #0
    blt signed_rem    
    
    b signed
    
unsigned:
    bl udiv
    pop {pc}
    
signed:
    bl udiv
    ldr r2, =0x80000000
    orr r0, r2
    pop {pc}

signed_rem:
    bl udiv
    ldr r2, =0x80000000
    orr r0, r2
    orr r1, r2
    pop {pc}
    
    

