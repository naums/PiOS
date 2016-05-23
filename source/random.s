/**
 * \file random.s
 * \author Stefan Naumann
 * \brief implements a random-numbers generator. The next random-number will be created wenn calling seed
 **/

.globl srand
.globl init
.globl rand
.globl seed

/**
 * \brief inits the constant parts of the equation; sets them in the memory
 * \param r0 a (needs to be even)
 * \param r1 c (needs to be odd)
 **/
init:
    mov r2, r0
    lsr r0, #1
    lsl r0, #1
    cmp r2, r0
    movne pc, lr
    
    mov r2, r1
    lsr r1, #1
    lsl r1, #1
    cmp r1, r2
    moveq pc, lr
    
    //mov r2, r0 MOD #4
    add r2, #1
    
    ldr r3, =rand_a
    
    str r0, [r3]        // a
    str r1, [r3, #8]    // b
    str r2, [r3, #4]    // c
    
    mov pc,lr


/**
 * \brief initiates the random value with a preset value (eg. timestamp)
 * \param r0 the value you want to set your random value
 * \note will be seeded once before the function returns
 **/
srand:
    ldr r1, =current
    str r0, [r1]
    push {lr}
    bl seed
    pop {pc}

/**
 * \brief returns the current random value and reseeds
 * \return a random value in [0, 2^32-1]
 **/
rand:
    ldr r0, =current
    push {r5, lr}
    ldr r5, [r0]
    bl seed
    mov r0, r5
    pop {r5, pc}

/**
 * \brief (re)seeds the random value
 * \note uses the equation x_{n+1} = a * x_n ^ 2 + b * n + c mod 2^32
 **/
seed:
    push {r0, r1, r2, r3, r4, r5}
    ldr r5, =rand_a
    ldr r1, [r5]
    ldr r2, [r5, #4]
    ldr r3, [r5, #8]
    ldr r4, [r5, #12]
    
    mul r0, r4, r4
    mul r0, r1
    
    mul r1, r2, r4
    add r0, r1
    add r0, r3
    
    str r0, [r5, #12]
    
    pop {r0, r1, r2, r3, r4, r5}
    mov pc, lr


.section .data
.align 1
rand_a:
    .int 0x0000ef00
    .int 0x00000001
    .int 73
current:
    .int 0

