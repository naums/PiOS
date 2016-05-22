.globl dispatch
.globl yield

yield:
    // return is not needed
    b dispatch
    mov pc, lr


dispatch:
    push {lr}
    push {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11, r12}
    ldr r2, =running
    // dereferencing the ptr
    ldr r2, [r2]
    str sp, [r2, #28]
    
    ldr r0, =schedule_lastUsed
    ldr r1, =schedule_lastUsed_winning
    bl schedule
    
    tst r0, #0
    beq idle
    
    ldr r2, =running
    str r0, [r2]
    ldr sp, [r0, #28]
    
    pop {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11, r12}
    pop {pc}
    
idle:
    b idle
    
    
    
    
    
    
    
    
    
    
    
