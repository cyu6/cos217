//----------------------------------------------------------------------
// bigintaddoptopt.s
// Author: Catherine Yu
//----------------------------------------------------------------------

        // Booleans
        .equ    TRUE, 1
        .equ    FALSE, 0

        .equ    MAX_DIGITS, 32768
        .equ    UNSIGNED_LONG, 8
        
//----------------------------------------------------------------------

        .section .rodata

//----------------------------------------------------------------------

        .section .data

//----------------------------------------------------------------------
 
        .section .bss

//----------------------------------------------------------------------

        .section .text

        //--------------------------------------------------------------
        // Assign the sum of oAddend1 and oAddend2 to oSum. oSum should
        // be distinct from oAddend1 and oAddend2. Return 0 (FALSE) if
        // an overflow occurred, and 1 (TRUE) otherwise.
        // int BigInt_add(BigInt_T oAddend1, BigInt_T oAddend2, BigInt_T
        //                oSum)
        //--------------------------------------------------------------
        
        // Must be a multiple of 16
        .equ    ADD_STACK_BYTECOUNT, 64

        // Local variable registers:
        LSUMLENGTH      .req x24
        LINDEX          .req x23
        ULSUM           .req x22
        
        // Parameter registers:
        OSUM            .req x21
        OADDEND2        .req x20
        OADDEND1        .req x19

        // Structure field offsets:
        .equ    DIGITS_OFFSET, 8

        .global BigInt_add

BigInt_add:

        // Prolog
        sub     sp, sp, ADD_STACK_BYTECOUNT
        str     x30, [sp]
        str     x19, [sp, 8]
        str     x20, [sp, 16]
        str     x21, [sp, 24]
        str     x22, [sp, 32]
        str     x23, [sp, 40]
        str     x24, [sp, 48]

        // Store parameters in registers
        mov     OADDEND1, x0
        mov     OADDEND2, x1
        mov     OSUM, x2

        // unsigned long ulCarry
        // unsigned long ulSum
        // long lIndex
        // long lSumLength

        // Determine the larger length
        // lSumLength = BigInt_larger(oAddend1->lLength,
        //                            oAddend2->lLength)
        ldr     x0, [OADDEND1]
        ldr     x1, [OADDEND2]

        // if (oAddend1->lLength <= oAddend2->lLength) goto elselength
        cmp     x0, x1
        ble     elselength

        // lSumLength = oAddend1->lLength
        mov     LSUMLENGTH, x0

        // goto endiflength
        b       endiflength
        
elselength:

        // lSumLength = oAddend2->lLength
        mov     LSUMLENGTH, x1

endiflength:
        
        // Clear oSum's array if necessary
        // if (oSum->lLength <= lSumLength) goto endifclear
        ldr     x0, [OSUM]
        cmp     x0, LSUMLENGTH
        bls     endifclear

        // memset(oSum->aulDigits, 0, MAX_DIGITS*sizeof(unsigned long))
        add     x0, OSUM, DIGITS_OFFSET
        mov     x1, xzr
        mov     x2, MAX_DIGITS
        mov     x3, UNSIGNED_LONG
        mul     x2, x2, x3
        bl      memset

endifclear:

        // Perform the addition
        // lIndex = 0
        mov     LINDEX, xzr

        // if (lIndex >= lSumLength) goto addLoopEnd
        // if no branch, C = 0
        cmp     LINDEX, LSUMLENGTH
        bge     addLoopEnd

        // ulSum = 0
        mov     ULSUM, xzr
        
addLoop:

        // calculate lsl 3
        lsl     x2, LINDEX, 3
        
        // oAddend1->aulDigits[lIndex]
        add     x0, OADDEND1, DIGITS_OFFSET
        ldr     x0, [x0, x2]

        // oAddend2->aulDigits[lIndex]
        add     x1, OADDEND2, DIGITS_OFFSET
        ldr     x1, [x1, x2]

        // ulSum += oAddend1->aulDigits[lIndex]
        adcs    ULSUM, ULSUM, x0
        
        // branch if C = 1
        bhs     yescarry

        // ulSum += oAddend2->aulDigits[lIndex] (changes C)
        adcs    ULSUM, ULSUM, x1
        b       endoverflow

yescarry:

        // ulSum += oAddend2->aulDigits[lIndex] (normal)
        add     ULSUM, ULSUM, x1
        
endoverflow:
        
        // oSum->aulDigits[lIndex] = ulSum
        add     x0, OSUM, DIGITS_OFFSET
        str     ULSUM, [x0, x2]

        // lIndex++
        add     LINDEX, LINDEX, 1

        // ulSum = 0 or 1 (depending on C)
        adcs    ULSUM, xzr, xzr
        
        // if (lIndex < lSumLength) goto addLoop
        // if return to beginning of loop, C = 0
        cmp     LINDEX, LSUMLENGTH
        blt     addLoop

addLoopEnd:

        // Check for a carry out of the last "column" of the addition
        // if (ulSum == 1) goto endifcarry
        cmp     ULSUM, 1
        bne     endifcarry

        // if (lSumLength != MAX_DIGITS) goto endifdigits
        cmp     LSUMLENGTH, MAX_DIGITS
        bne     endifdigits

        // return FALSE
        mov     w0, FALSE
        ldr     x30, [sp]
        ldr     x19, [sp, 8]
        ldr     x20, [sp, 16]
        ldr     x21, [sp, 24]
        ldr     x22, [sp, 32]
        ldr     x23, [sp, 40]
        ldr     x24, [sp, 48]
        add     sp, sp, ADD_STACK_BYTECOUNT
        ret

endifdigits:

        // oSum->aulDigits[lSumLength] = 1
        mov     x0, 1
        add     x1, OSUM, DIGITS_OFFSET
        str     x0, [x1, LSUMLENGTH, lsl 3]

        // lSumLength++
        add     LSUMLENGTH, LSUMLENGTH, 1

endifcarry:

        // Set the length of the sum
        // oSum->lLength = lSumLength
        str     LSUMLENGTH, [OSUM]

        // Epilog and return TRUE
        mov     w0, TRUE
        ldr     x30, [sp]
        ldr     x19, [sp, 8]
        ldr     x20, [sp, 16]
        ldr     x21, [sp, 24]
        ldr     x22, [sp, 32]
        ldr     x23, [sp, 40]
        ldr     x24, [sp, 48]
        add     sp, sp, ADD_STACK_BYTECOUNT
        ret

        .size   BigInt_add, (. - BigInt_add)
