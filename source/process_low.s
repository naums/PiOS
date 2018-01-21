.global pios_setup_stack;
.global pios_resume_do;
.global pios_switch
.global pios_retrieve_registers

.section ".text"
pios_setup_stack:
    /* registers:
        r0: new pc
        r1: new stack
        r2: zeroes
        r3: old stack
        r4-r15 not touched
    */
         
    mov r3, sp
    mov sp, r1
    push {r0}
    mov r2, #0
    // put zeroes onto stack as new register-values
    stmdb sp, {r2,r2,r2,r2,r2,r2,r2,r2,r2,r2,r2,r2,r2} 
    mrs r0, cpsr
    push {r0}
    mov sp, r3
    mov pc, lr    

pios_resume_do:
    mov sp, r0
    pop {r0}
    msr cpsr, r0
    pop {r0, r1, r2, r3, r4,r5, r6, r7, r8, r9, r10, r11, r12}
    pop {pc}
    
pios_switch:
    push {lr};
    push {r0, r1, r2, r3, r4,r5, r6, r7, r8, r9, r10, r11, r12}
    mrs r0, cpsr
    push {r0}
    b pios_yield
    
pios_retrieve_registers:
    stmia r0, {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr,sp}
    mrs r1, cpsr
    str r1, [r0]
    mov pc,lr
    
# vi:syntax=c
