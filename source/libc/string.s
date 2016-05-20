/**
 * \file implements string.h for a C-style library
 * \author Stefan Naumann, me@stefannaumann.de
 * \date 30.01.2016
 * \hardware Raspberry Pi (ARM11)
 **/
.globl memcpy
.globl memset
.globl memcmp
.globl strncpy
.globl strcpy
.globl strlen 
.globl strcat
.globl strcmp

/** 
 * \brief copies the memory block from a to b until size Bytes or the first Nullbyte
 * \param r0 target
 * \param r1 source
 * \param r2 size
 * \return target-address
 * \note copies also the nullbyte
 **/
strncpy:
    size .req r2
    source .req r1
    target .req r0
    
    mov r4, target
    $strncpybyte:
        teq size, #0
        beq $strncpyout
        
        ldrb r3, [source]
        strb r3, [target]
        
        // nullbyte read -> return
        teq r3, #0
        beq $strncpyout
        
        add source, #1
        add target, #1
        sub size, #1
        
        b $strncpybyte
    
    $strncpyout:
    mov r0, r4
    mov pc, lr
    
    .unreq size
    .unreq source
    .unreq target

/**
 * \brief copies a string A to a string B until the first Nullbyte is read
 * \param r0 target address 
 * \param r1 source address
 * \return target address
 * \note also copies the nullbyte at the end
 **/
strcpy:
    source .req r1
    target .req r0
    
    mov r4, target
    
    $strcpybyte:        
        ldrb r3, [source]
        strb r3, [target]
        
        teq r3, #0
        beq $strcpyout
        
        add source, #1
        add target, #1
        
        b $strcpybyte
    
    $strcpyout:
    mov r0, r4
    mov pc, lr
    
    .unreq source
    .unreq target

/**
 * \brief return the length of a string
 * \param r0 the string
 * \return the length of the string
 **/
strlen:
    source .req r0
    counter .req r1
    
    mov r1, #0
    $countloop:
        ldrb r2, [source]
        teq r2, #0
        beq $strlenout
        
        add counter, #1
        add source, #1
        b $countloop
    
$strlenout:
    mov r0, counter
    mov pc, lr
    
    .unreq source
    .unreq counter
    
/**
 * \brief concats two strings
 * \param r0 destination (must be big enough to hold dest + source!)
 * \param r1 source
 * \return destination
 * \note destination-array must be big enough to hold both dest and source!
 **/
strcat:
    push {lr}
    push {r0}
    push {r1}
    bl strend
    pop {r1}
    bl strcpy
    pop {r0}
    pop {pc}

/**
 * \brief finds the end of a string (nullbyte) and returns the address
 * \param r0 address to a string
 * \return address of the first nullbyte
 * \note uses r0 and r1; will overwrite the parameter!
 **/
strend:
    source .req r0
    $findnull:
        ldrb r1, [r0]
        teq r1, #0
        moveq pc, lr
        
        add r0, #1
        b $findnull;
    .unreq source
    
/** 
 * \brief compares two string until one or both of them reach nullbyte
 * \param r0 string 1
 * \param r1 string 2
 * \return 0 if r0==r1; 1 if r0>r1; -1 if r0<r1
 **/
strcmp:    
    first .req r0
    second .req r1
    
    $strcmploop:
        ldrb r2, [first]
        ldrb r3, [second]
    
        teq r2, #0
        beq $firstnull
        
        teq r3, #0
        moveq r0, #1
        moveq pc, lr
        
        cmp r2, r3
        movhi r0, #1
        movhi pc, lr
        
        cmp r3, r2
        movhi r0, #0
        subhi r0, #1
        movhi pc, lr
        
        add first, #1
        add second, #1
        b $strcmploop;
    
$firstnull:
    teq r3, #0
    moveq r0, #0
    movne r0, #0
    subne r0, #1
    mov pc, lr
    
    .unreq first
    .unreq second

    
/** 
 * \brief copies from memory address a to b exactly n bytes
 * \param r0 target
 * \param r1 source
 * \param r2 size
 * \return target-address
 **/
memcpy:
    size .req r2
    source .req r1
    target .req r0

    teq size, #0
    moveq pc, lr
    
    mov r4, target
    
    $qwordwise:
        cmp size, #3
        bls $bytewise
        
        ldr r3, [source]
        str r3, [target]
        add source, #4
        add target, #4
        sub size, #4
        
        b $qwordwise
        
    $bytewise:
        teq size, #0
        beq $memcpyout
        
        ldrb r3, [source]
        strb r3, [target]
        add source, #1
        add target, #1
        sub size, #1
        
        b $bytewise
    
    $memcpyout:
    mov r0, r4
    mov pc, lr
    
    .unreq size
    .unreq source
    .unreq target

/**
 * \brief sets a specific value x times into a specific piece of memory
 * \param r0 target
 * \param r1 value
 * \param r2 size
 * \return target
 **/
memset:
    size .req r2
    value .req r1
    target .req r0
    
    mov r4, target
    
    $setmem:
        teq size, #0
        beq $memsetout
        
        strb value, [target]
        add target, #1
        sub size, #1
        b $setmem
    
$memsetout:
    mov r0, r4
    mov pc, lr

    .unreq size
    .unreq value
    .unreq target
    
/**
 * \brief compares two pieces of memory
 * \param r0 first memory block
 * \param r1 second memory block
 * \param r2 size
 * \return 0 if equal, -1 if r0 < r1, 1 if r0 > r1
 **/
memcmp:
    first .req r0
    second .req r1
    size .req r2
    
    $quadcmp:
        cmp size, #3
        bls $bytecmp
        
        ldr r3, [first]
        ldr r4, [second]
        cmp r3, r4
        movhi r0, #1
        bhi $memcmpout
        
        
        cmpls r4, r3
        movhi r0, #0
        subhi r0, #1
        bhi $memcmpout
        
        add first, #4
        add second, #4
        sub size, #4
        
        b $quadcmp
        
    $bytecmp:
        teq size, #0
        moveq r0, #0
        beq $memcmpout
        
        ldrb r3, [first]
        ldrb r4, [second]
        cmp r3, r4
        movhi r0, #1
        bhi $memcmpout
        
        cmpls r4, r3
        movhi r0, #0
        subhi r0, #1
        bhi $memcmpout
        
        add first, #1
        add second, #1
        sub size, #1
        
        b $bytecmp
    
$memcmpout:
    mov pc, lr
    .unreq first
    .unreq second
    .unreq size
