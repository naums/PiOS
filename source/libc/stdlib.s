/**
 * \file stdio/lib with printf, sprintf and a rudimentary malloc-command
 * \author Stefan Naumann
 * \date 30.01.2016
 **/

.globl printf
.globl sprintf
.globl malloc

/**
 * \brief prints a string onto screen
 * \param r0 format-string
 * \param ... pop from stack! the parameters to the format-string
 * \return number of characters written to screen
 **/ 
printf:
    ldr r2, =printf_registers
    str r5, [r2]
    str r6, [r2, #4]
    
    ldr r5, =printf_returnptr
    str lr, [r5]
    
    mov r6, r0

    ldr r0, =sprintf_buffer    
    mov r1, r6
    bl sprintf   
    
    mov r6, r0
    mov r0, r1
    // TODO: not fixed positions for printing the sodding string
    mov r1, #10
    mov r2, #10
    
    bl printString
    
    mov r0, r6
    mov r2, r5

    ldr r3, =printf_registers
    ldr r5, [r3]
    ldr r6, [r3, #4]

    ldr pc, [r2]
    
    
    
/**
 * \brief prints a string into a variable
 * \param r0 ptr to memory used for printing the format-string an values
 * \param r1 ptr to format-string
 * \param[in] ... pops from stack! the parameters to the format-string
 * \return r0 -> number of characters written to r0; r1 -> ptr to the printed string
 **/
sprintf:
    push {r5, r6, r7, r8, r9}
    mov r8, r0
    $sprintloop:
        ldrb r2, [r1]
        add r1, #1
        // nullbyte found -> return
        teq r2, #'\0'
        beq nullbyte
        
        // found a single % -> find out, what comes behind (d,x,c,s or %)
        teq r2, #'%'
        beq sprintfescape
        
        // otherwise print character into the characterstring
        strb r2, [r0] 
        add r0, #1
        
        b $sprintloop

nullbyte:
    mov r2, #'\0'
    strb r2, [r0]
    
    pop {r5, r6, r7, r8, r9}
    
    sub r0, r0, r8
    mov r1, r8
    
    mov pc, lr
    
// a % has been read, now determine, what has to be done next
sprintfescape:
    ldrb r2, [r1]
    add r1, #1
    
    // %%
    teq r2, #'%'
    streqb r2, [r0]
    addeq r0, #1
    beq $sprintloop
    
    // %d
    teq r2, #'d'
    beq sprintdecimal
    
    // %x
    teq r2, #'x'
    beq sprinthexa
    
    // %s
    teq r2, #'s'
    beq sprintstring
    
    // %c
    teq r2, #'c'
    beq sprintcharacter

    b $sprintloop
    
sprintdecimal:
    pop {r3}
    // save the current values
    mov r6, r0
    mov r7, r1

    // convert the integer number to string
    mov r0, r3
    bl itoa
    
    // count number of characters used
    mov r5, r0
    bl strlen 
    
    // copy the result to r9
    mov r9, r0
    
    // copy the integer string into our sprintf-target string
    mov r0, r6
    mov r1, r5
    bl strcpy
    
    // add number of characters to sprint-target-string potr
    mov r0, r6
    add r0, r9
    mov r1, r7 
        
    b $sprintloop 

sprinthexa:
    pop {r3}
    // save the current values
    mov r6, r0
    mov r7, r1

    // convert the integer number to hexstring
    mov r0, r3
    bl hex32
    
    // count number of characters used
    mov r5, r0
    bl strlen 
    
    // copy the result to r9
    mov r9, r0
    
    // copy the hexstring into our sprintf-target string
    mov r0, r6
    mov r1, r5
    bl strcpy
    
    // add number of characters to sprint-target-string potr
    mov r0, r6
    add r0, r9
    mov r1, r7 
    b $sprintloop
    
sprintstring:
    pop {r3}
    // save the current values
    mov r6, r0
    mov r7, r1

    // count the number of characters in the string
    mov r0, r3
    bl strlen
    
    // copy the result to r9
    mov r9, r0
    
    // copy the string into our sprintf-target string
    mov r0, r6
    mov r1, r5
    bl strcpy
    
    // add number of characters to sprint-target-string potr
    mov r0, r6
    add r0, r9
    mov r1, r7 
    b $sprintloop

sprintcharacter:
    pop {r3}
    mov r6, #0x000000ff
    and r6, r3, r6
    
    strb r6, [r0]
    add r0, #1
    b $sprintloop


    
.section .data   
sprintf_buffer:
    .zero 512
    
printf_returnptr:
    .int 0
printf_registers:
    .int 0 
    .int 0
