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
    str sp, [r2, #28]
    
    ldr r0, =schedule_lastUsed
    ldr r1, =schedule_lastUsed_winning
    bl schedule
    
    ldr sp, [r0, #28]
    
    pop {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11, r12}
    pop {pc}
    
    
    
    
    
    
    
    
    
    
    
    
