//----------------------------------------------------------------------
// mywc.s
// Author: Catherine Yu
//----------------------------------------------------------------------

        .section .rodata

printfFormatStr:
        .string "%7ld %7ld %7ld\n"

//----------------------------------------------------------------------

        .section .data

lLineCount:
        .quad   0

lWordCount:
        .quad   0

lCharCount:
        .quad   0
        
iInWord:
        .int    FALSE

//----------------------------------------------------------------------

        .section .bss

iChar:
        .skip   4

//----------------------------------------------------------------------

        .section .text

        //--------------------------------------------------------------
        // Write to stdout counts of how many lines, words, and
        // characters are in stdin.  A word is a sequence of non-white
        // space characters. Whitespace is defined by the isspace()
        // function. Return 0.
        // int main(void)
        //--------------------------------------------------------------
        
        // Must be a multiple of 16
        .equ    MAIN_STACK_BYTECOUNT, 16
        
        // Define the end of stdin, or EOF = -1
        .equ    EOF, -1

        // Define booleans
        .equ    FALSE, 0
        .equ    TRUE, 1

        .global main

main:

        // Prolog
        sub     sp, sp, MAIN_STACK_BYTECOUNT
        str     x30, [sp]

countLoop:

        // if (iChar = getchar() == EOF) goto countLoopEnd
        bl      getchar
        adr     x1, iChar
        str     w0, [x1]
        cmp     w0, EOF
        beq     countLoopEnd

        // lCharCount++
        adr     x0, lCharCount
        ldr     x1, [x0]
        add     x1, x1, 1
        str     x1, [x0]

        // if (!isspace(iChar)) go to else1
        adr     x0, iChar
        ldr     w0, [x0]
        bl      isspace
        cmp     w0, 0
        beq     else1

        // if (!iInWord) goto endif1
        adr     x0, iInWord
        ldr     w1, [x0]
        cmp     x1, 0
        beq     endif1

        // lWordCount++
        adr     x0, lWordCount
        ldr     x1, [x0]
        add     x1, x1, 1
        str     x1, [x0]

        // iInWord = FALSE
        mov     x0, FALSE
        adr     x1, iInWord
        str     w0, [x1]

        // goto endif1
        b       endif1

else1:

        // if(iInWord) goto endif1
        adr     x0, iInWord
        ldr     w1, [x0]
        cmp     x1, 0
        bne     endif1

        // iInWord = TRUE
        mov     x0, TRUE
        adr     x1, iInWord
        str     w0, [x1]

endif1:

        // if (iChar != '\n') goto endif2
        adr     x0, iChar
        ldr     w1, [x0]
        cmp     w1, '\n'
        bne     endif2

        // lLineCount++
        adr     x0, lLineCount
        ldr     x1, [x0]
        add     x1, x1, 1
        str     x1, [x0]
        
endif2: 

        // goto countLoop
        b       countLoop

countLoopEnd:

        // if(!iInWord) goto endif3
        adr     x0, iInWord
        ldr     w1, [x0]
        cmp     x1, 0
        beq     endif3

        // lWordCount++
        adr     x0, lWordCount
        ldr     x1, [x0]
        add     x1, x1, 1
        str     x1, [x0]

endif3:

        // printf("7ld %7ld %7ld\n", lLineCount, lWordCount, lCharCount)
        adr     x0, printfFormatStr
        adr     x1, lLineCount
        ldr     x1, [x1]
        adr     x2, lWordCount
        ldr     x2, [x2]
        adr     x3, lCharCount
        ldr     x3, [x3]
        bl      printf
        
        // Epilog and return 0
        mov     w0, 0
        ldr     x30, [sp]
        add     sp, sp, MAIN_STACK_BYTECOUNT
        ret

        .size   main, (. - main)
        
