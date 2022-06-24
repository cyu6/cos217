//----------------------------------------------------------------------
// bigintaddopt.s
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
        // Return the larger of lLength1 and lLength2.
        // static long BigInt_larger(long lLength1, long lLength2)
        //--------------------------------------------------------------

        // Must be a multiple of 16
        .equ    LARGER_STACK_BYTECOUNT, 32

        // Local variable registers:
        LLARGER         .req x21

        // Parameter registers:
        LLENGTH2        .req x20
        LLENGTH1        .req x19

BigInt_larger:

        // Prolog
        sub     sp, sp, LARGER_STACK_BYTECOUNT
        str     x30, [sp]
        str     x19, [sp, 8]
        str     x20, [sp, 16]
        str     x21, [sp, 24]

        // Store parameters in registers
        mov     LLENGTH1, x0
        mov     LLENGTH2, x1

        // long lLarger

        // if (lLength1 <= lLength2) goto elselength
        cmp     LLENGTH1, LLENGTH2
        ble     elselength

        // lLarger = lLength1
        mov     LLARGER, LLENGTH1
        
        // goto endiflength
        b       endiflength

elselength:

        // lLarger = lLength2
        mov     LLARGER, LLENGTH2

endiflength:

        // Epilog and return lLarger
        mov     x0, LLARGER
        ldr     x30, [sp]
        ldr     x19, [sp, 8]
        ldr     x20, [sp, 16]
        ldr     x21, [sp, 24]
        add     sp, sp, LARGER_STACK_BYTECOUNT
        ret

        .size   BigInt_larger, (. - BigInt_larger)

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
        LSUMLENGTH      .req x25
        LINDEX          .req x24
        ULSUM           .req x23
        ULCARRY         .req x22
        
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
        str     x25, [sp, 56]

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
        bl      BigInt_larger
        mov     LSUMLENGTH, x0

        // Clear oSum's array if necessary
        // if (oSum->lLength <= lSumLength) goto endifclear
        ldr     x0, [OSUM]
        cmp     x0, LSUMLENGTH
        bls     endifclear

        // memset(oSum->aulDigits, 0, MAX_DIGITS*sizeof(unsigned long))
        mov     x0, OSUM
        add     x0, x0, DIGITS_OFFSET
        mov     x1, 0
        mov     x2, MAX_DIGITS
        mov     x3, UNSIGNED_LONG
        mul     x2, x2, x3
        bl      memset

endifclear:

        // Perform the addition
        // ulCarry = 0
        mov     ULCARRY, 0

        // lIndex = 0
        mov     LINDEX, 0

addLoop:

        // if (lIndex >= lSumLength) goto addLoopEnd
        cmp     LINDEX, LSUMLENGTH
        bhs     addLoopEnd

        // ulSum = ulCarry
        mov     ULSUM, ULCARRY

        // ulCarry = 0
        mov     ULCARRY, 0

        // ulSum += oAddend1->aulDigits[lIndex]
        mov     x0, OADDEND1
        add     x0, x0, DIGITS_OFFSET
        mov     x1, LINDEX
        ldr     x0, [x0, x1, lsl 3]
        add     ULSUM, ULSUM, x0

        // if (ulSum >= oAddend1->aulDigits[lIndex]) goto endifof1
        mov     x0, OADDEND1
        add     x0, x0, DIGITS_OFFSET
        mov     x1, LINDEX
        ldr     x0, [x0, x1, lsl 3]
        cmp     ULSUM, x0
        bhs     endifof1

        // ulCarry = 1
        mov     ULCARRY, 1

endifof1:

        // ulSum += oAddend2->aulDigits[lIndex]
        mov     x0, OADDEND2
        add     x0, x0, DIGITS_OFFSET
        mov     x1, LINDEX
        ldr     x0, [x0, x1, lsl 3]
        add     ULSUM, ULSUM, x0

        // if (ulSum >= oAddend2->aulDigits[lIndex]) goto endifof2
        mov     x0, OADDEND2
        add     x0, x0, DIGITS_OFFSET
        mov     x1, LINDEX
        ldr     x0, [x0, x1, lsl 3]
        cmp     ULSUM, x0
        bhs     endifof2
        
        // ulCarry = 1
        mov     ULCARRY, 1

endifof2:

        // oSum->aulDigits[lIndex] = ulSum
        mov     x0, OSUM
        add     x0, x0, DIGITS_OFFSET
        mov     x1, LINDEX
        str     ULSUM, [x0, x1, lsl 3]

        // lIndex++
        add     LINDEX, LINDEX, 1

        // goto addLoop
        b       addLoop

addLoopEnd:

        // Check for a carry out of the last "column" of the addition
        // if (ulCarry != 1) goto endifcarry
        cmp     ULCARRY, 1
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
        ldr     x25, [sp, 56]
        add     sp, sp, ADD_STACK_BYTECOUNT
        ret

endifdigits:

        // oSum->aulDigits[lSumLength] = 1
        mov     x0, 1
        mov     x1, OSUM
        add     x1, x1, DIGITS_OFFSET
        mov     x2, LSUMLENGTH
        str     x0, [x1, x2, lsl 3]

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
        ldr     x25, [sp, 56]
        add     sp, sp, ADD_STACK_BYTECOUNT
        ret

        .size   BigInt_add, (. - BigInt_add)
