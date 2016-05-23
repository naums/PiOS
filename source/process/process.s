.globl dispatch
.globl yield
.globl initProcessStack
.global startProcess

yield:
    // return is not needed    
    b dispatch


dispatch:
    push {lr}
    push {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11, r12}
    ldr r2, =running
    // dereferencing the ptr
    ldr r2, [r2]
    str sp, [r2, #28]
    
    // schedule next process
    ldr r0, =schedule_lastUsed
    ldr r1, =schedule_lastUsed_winning
    bl schedule
    
    cmp r0, #0
    beq blinkloop
    
    // store new running process
    ldr r2, =running
    str r0, [r2]
    
    ldr sp, [r0, #28]
    pop {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11, r12}

    pop {pc}

startProcess:
    ldr r2, =running
    ldr r2, [r2]
    ldr sp, [r2, #28]
    
    pop {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11, r12}
    pop {pc}
    

idle:
    b idle
    
// r0 -> new stack ptr    
// r1 -> new code address
initProcessStack:
    mov r3, sp

    mov r2, #0
    mov sp, r0
    // put new address onto stack
    push {r1}
    // put zeroes onto stack as register-values
    push {r2} 
    push {r2} 
    push {r2} 
    push {r2} 
    push {r2} 
    push {r2} 
    push {r2} 
    push {r2} 
    push {r2} 
    push {r2} 
    push {r2} 
    push {r2} 
    push {r2}   
    
    mov r0, sp
    mov sp, r3
    mov pc, lr
    
    
    
    
    
    
    
