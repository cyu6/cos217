//----------------------------------------------------------------------
// bigintadd.s
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

        // Local variable stack offsets:
        .equ    LLARGER, 8

        // Parameter stack offsets:
        .equ    LLENGTH2, 16
        .equ    LLENGTH1, 24

BigInt_larger:

        // Prolog
        sub     sp, sp, LARGER_STACK_BYTECOUNT
        str     x30, [sp]
        str     x0, [sp, LLENGTH1]
        str     x1, [sp, LLENGTH2]

        // long lLarger

        // if (lLength1 <= lLength2) goto elselength
        ldr     x0, [sp, LLENGTH1]
        ldr     x1, [sp, LLENGTH2]
        cmp     x0, x1
        ble     elselength

        // lLarger = lLength1
        ldr     x0, [sp, LLENGTH1]
        str     x0, [sp, LLARGER]

        // goto endiflength
        b       endiflength

elselength:

        // lLarger = lLength2
        ldr     x0, [sp, LLENGTH2]
        str     x0, [sp, LLARGER]

endiflength:

        // Epilog and return lLarger
        ldr     x0, [sp, LLARGER]
        ldr     x30, [sp]
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

        // Local variables stack offsets:
        .equ    LSUMLENGTH, 8
        .equ    LINDEX, 16
        .equ    ULSUM, 24
        .equ    ULCARRY, 32
        
        // Parameter stack offsets:
        .equ    OSUM, 40
        .equ    OADDEND2, 48
        .equ    OADDEND1, 56

        // Structure field offsets:
        .equ    DIGITS_OFFSET, 8

        .global BigInt_add

BigInt_add:

        // Prolog
        sub     sp, sp, ADD_STACK_BYTECOUNT
        str     x30, [sp]
        str     x0, [sp, OADDEND1]
        str     x1, [sp, OADDEND2]
        str     x2, [sp, OSUM]

        // unsigned long ulCarry
        // unsigned long ulSum
        // long lIndex
        // long lSumLength

        // Determine the larger length
        // lSumLength = BigInt_larger(oAddend1->lLength,
        //                            oAddend2->lLength)
        ldr     x0, [sp, OADDEND1]
        ldr     x0, [x0]
        ldr     x1, [sp, OADDEND2]
        ldr     x1, [x1]
        bl      BigInt_larger
        str     x0, [sp, LSUMLENGTH]

        // Clear oSum's array if necessary
        // if (oSum->lLength <= lSumLength) goto endifclear
        ldr     x0, [sp, OSUM]
        ldr     x0, [x0]
        ldr     x1, [sp, LSUMLENGTH]
        cmp     x0, x1
        bls     endifclear

        // memset(oSum->aulDigits, 0, MAX_DIGITS*sizeof(unsigned long))
        ldr     x0, [sp, OSUM]
        add     x0, x0, DIGITS_OFFSET
        mov     x1, 0
        mov     x2, MAX_DIGITS
        mov     x3, UNSIGNED_LONG
        mul     x2, x2, x3
        bl      memset

endifclear:

        // Perform the addition
        // ulCarry = 0
        mov     x0, 0
        str     x0, [sp, ULCARRY]

        // lIndex = 0
        mov     x0, 0
        str     x0, [sp, LINDEX]

addLoop:

        // if (lIndex >= lSumLength) goto addLoopEnd
        ldr     x0, [sp, LINDEX]
        ldr     x1, [sp, LSUMLENGTH]
        cmp     x0, x1
        bhs     addLoopEnd

        // ulSum = ulCarry
        ldr     x0, [sp, ULCARRY]
        str     x0, [sp, ULSUM]

        // ulCarry = 0
        mov     x0, 0
        str     x0, [sp, ULCARRY]

        // ulSum += oAddend1->aulDigits[lIndex]
        ldr     x0, [sp, ULSUM]
        ldr     x1, [sp, OADDEND1]
        add     x1, x1, DIGITS_OFFSET
        ldr     x2, [sp, LINDEX]
        ldr     x1, [x1, x2, lsl 3]
        add     x0, x0, x1
        str     x0, [sp, ULSUM]

        // if (ulSum >= oAddend1->aulDigits[lIndex]) goto endifof1
        ldr     x0, [sp, ULSUM]
        ldr     x1, [sp, OADDEND1]
        add     x1, x1, DIGITS_OFFSET
        ldr     x2, [sp, LINDEX]
        ldr     x1, [x1, x2, lsl 3]
        cmp     x0, x1
        bhs     endifof1

        // ulCarry = 1
        mov     x0, 1
        str     x0, [sp, ULCARRY]

endifof1:

        // ulSum += oAddend2->aulDigits[lIndex]
        ldr     x0, [sp, ULSUM]
        ldr     x1, [sp, OADDEND2]
        add     x1, x1, DIGITS_OFFSET
        ldr     x2, [sp, LINDEX]
        ldr     x1, [x1, x2, lsl 3]
        add     x0, x0, x1
        str     x0, [sp, ULSUM]

        // if (ulSum >= oAddend2->aulDigits[lIndex]) goto endifof2
        ldr     x0, [sp, ULSUM]
        ldr     x1, [sp, OADDEND2]
        add     x1, x1, DIGITS_OFFSET
        ldr     x2, [sp, LINDEX]
        ldr     x1, [x1, x2, lsl 3]
        cmp     x0, x1
        bhs     endifof2
        
        // ulCarry = 1
        mov     x0, 1
        str     x0, [sp, ULCARRY]

endifof2:

        // oSum->aulDigits[lIndex] = ulSum
        ldr     x0, [sp, ULSUM]
        ldr     x1, [sp, OSUM]
        add     x1, x1, DIGITS_OFFSET
        ldr     x2, [sp, LINDEX]
        str     x0, [x1, x2, lsl 3]

        // lIndex++
        ldr     x0, [sp, LINDEX]
        mov     x1, 1
        add     x0, x0, x1
        str     x0, [sp, LINDEX]

        // goto addLoop
        b       addLoop

addLoopEnd:

        // Check for a carry out of the last "column" of the addition
        // if (ulCarry != 1) goto endifcarry
        ldr     x0, [sp, ULCARRY]
        cmp     x0, 1
        bne     endifcarry

        // if (lSumLength != MAX_DIGITS) goto endifdigits
        ldr     x0, [sp, LSUMLENGTH]
        cmp     x0, MAX_DIGITS
        bne     endifdigits

        // return FALSE
        mov     w0, FALSE
        ldr     x30, [sp]
        add     sp, sp, ADD_STACK_BYTECOUNT
        ret

endifdigits:

        // oSum->aulDigits[lSumLength] = 1
        mov     x0, 1
        ldr     x1, [sp, OSUM]
        add     x1, x1, DIGITS_OFFSET
        ldr     x2, [sp, LSUMLENGTH]
        str     x0, [x1, x2, lsl 3]

        // lSumLength++
        ldr     x0, [sp, LSUMLENGTH]
        mov     x1, 1
        add     x0, x0, x1
        str     x0, [sp, LSUMLENGTH]

endifcarry:

        // Set the length of the sum
        // oSum->lLength = lSumLength
        ldr     x0, [sp, LSUMLENGTH]
        ldr     x1, [sp, OSUM]
        str     x0, [x1]

        // Epilog and return TRUE
        mov     w0, TRUE
        ldr     x30, [sp]
        add     sp, sp, ADD_STACK_BYTECOUNT
        ret

        .size   BigInt_add, (. - BigInt_add)
