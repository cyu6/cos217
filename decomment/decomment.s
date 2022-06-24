	.cpu generic+fp+simd
	.file	"decomment.c"
	.text
	.align	2
	.global	handleNormalState
	.type	handleNormalState, %function
handleNormalState:
.LFB0:
	.cfi_startproc
	stp	x29, x30, [sp, -48]!
	.cfi_def_cfa_offset 48
	.cfi_offset 29, -48
	.cfi_offset 30, -40
	add	x29, sp, 0
	.cfi_def_cfa_register 29
	str	w0, [x29,28]
	ldr	w0, [x29,28]
	cmp	w0, 39
	beq	.L3
	cmp	w0, 47
	beq	.L4
	cmp	w0, 34
	beq	.L5
	b	.L8
.L4:
	mov	w0, 1
	str	w0, [x29,44]
	b	.L6
.L3:
	ldr	w0, [x29,28]
	bl	putchar
	mov	w0, 4
	str	w0, [x29,44]
	b	.L6
.L5:
	ldr	w0, [x29,28]
	bl	putchar
	mov	w0, 6
	str	w0, [x29,44]
	b	.L6
.L8:
	ldr	w0, [x29,28]
	bl	putchar
	str	wzr, [x29,44]
	nop
.L6:
	ldr	w0, [x29,44]
	ldp	x29, x30, [sp], 48
	.cfi_restore 30
	.cfi_restore 29
	.cfi_def_cfa 31, 0
	ret
	.cfi_endproc
.LFE0:
	.size	handleNormalState, .-handleNormalState
	.align	2
	.global	handleMaybeCommentState
	.type	handleMaybeCommentState, %function
handleMaybeCommentState:
.LFB1:
	.cfi_startproc
	stp	x29, x30, [sp, -48]!
	.cfi_def_cfa_offset 48
	.cfi_offset 29, -48
	.cfi_offset 30, -40
	add	x29, sp, 0
	.cfi_def_cfa_register 29
	str	w0, [x29,28]
	ldr	w0, [x29,28]
	sub	w0, w0, #34
	cmp	w0, 13
	bhi	.L10
	adrp	x1, .L12
	add	x1, x1, :lo12:.L12
	ldr	w2, [x1,w0,uxtw #2]
	adr	x3, .Lrtx12
	add	x2, x3, w2, sxtw #2
	br	x2
.Lrtx12:
	.section	.rodata
	.align	0
	.align	2
.L12:
	.word	(.L11 - .Lrtx12) / 4
	.word	(.L10 - .Lrtx12) / 4
	.word	(.L10 - .Lrtx12) / 4
	.word	(.L10 - .Lrtx12) / 4
	.word	(.L10 - .Lrtx12) / 4
	.word	(.L13 - .Lrtx12) / 4
	.word	(.L10 - .Lrtx12) / 4
	.word	(.L10 - .Lrtx12) / 4
	.word	(.L14 - .Lrtx12) / 4
	.word	(.L10 - .Lrtx12) / 4
	.word	(.L10 - .Lrtx12) / 4
	.word	(.L10 - .Lrtx12) / 4
	.word	(.L10 - .Lrtx12) / 4
	.word	(.L15 - .Lrtx12) / 4
	.text
.L14:
	mov	w0, 32
	bl	putchar
	mov	w0, 2
	str	w0, [x29,44]
	b	.L16
.L15:
	ldr	w0, [x29,28]
	bl	putchar
	mov	w0, 1
	str	w0, [x29,44]
	b	.L16
.L13:
	mov	w0, 47
	bl	putchar
	ldr	w0, [x29,28]
	bl	putchar
	mov	w0, 4
	str	w0, [x29,44]
	b	.L16
.L11:
	mov	w0, 47
	bl	putchar
	ldr	w0, [x29,28]
	bl	putchar
	mov	w0, 6
	str	w0, [x29,44]
	b	.L16
.L10:
	mov	w0, 47
	bl	putchar
	ldr	w0, [x29,28]
	bl	putchar
	str	wzr, [x29,44]
	nop
.L16:
	ldr	w0, [x29,44]
	ldp	x29, x30, [sp], 48
	.cfi_restore 30
	.cfi_restore 29
	.cfi_def_cfa 31, 0
	ret
	.cfi_endproc
.LFE1:
	.size	handleMaybeCommentState, .-handleMaybeCommentState
	.align	2
	.global	handleInCommentState
	.type	handleInCommentState, %function
handleInCommentState:
.LFB2:
	.cfi_startproc
	stp	x29, x30, [sp, -48]!
	.cfi_def_cfa_offset 48
	.cfi_offset 29, -48
	.cfi_offset 30, -40
	add	x29, sp, 0
	.cfi_def_cfa_register 29
	str	w0, [x29,28]
	ldr	w0, [x29,28]
	cmp	w0, 10
	beq	.L20
	cmp	w0, 42
	bne	.L24
	mov	w0, 3
	str	w0, [x29,44]
	b	.L22
.L20:
	ldr	w0, [x29,28]
	bl	putchar
	mov	w0, 2
	str	w0, [x29,44]
	b	.L22
.L24:
	mov	w0, 2
	str	w0, [x29,44]
	nop
.L22:
	ldr	w0, [x29,44]
	ldp	x29, x30, [sp], 48
	.cfi_restore 30
	.cfi_restore 29
	.cfi_def_cfa 31, 0
	ret
	.cfi_endproc
.LFE2:
	.size	handleInCommentState, .-handleInCommentState
	.align	2
	.global	handleMaybeNormalState
	.type	handleMaybeNormalState, %function
handleMaybeNormalState:
.LFB3:
	.cfi_startproc
	stp	x29, x30, [sp, -48]!
	.cfi_def_cfa_offset 48
	.cfi_offset 29, -48
	.cfi_offset 30, -40
	add	x29, sp, 0
	.cfi_def_cfa_register 29
	str	w0, [x29,28]
	ldr	w0, [x29,28]
	cmp	w0, 42
	beq	.L27
	cmp	w0, 47
	beq	.L28
	cmp	w0, 10
	beq	.L29
	b	.L32
.L28:
	str	wzr, [x29,44]
	b	.L30
.L27:
	mov	w0, 3
	str	w0, [x29,44]
	b	.L30
.L29:
	ldr	w0, [x29,28]
	bl	putchar
	mov	w0, 2
	str	w0, [x29,44]
	b	.L30
.L32:
	mov	w0, 2
	str	w0, [x29,44]
	nop
.L30:
	ldr	w0, [x29,44]
	ldp	x29, x30, [sp], 48
	.cfi_restore 30
	.cfi_restore 29
	.cfi_def_cfa 31, 0
	ret
	.cfi_endproc
.LFE3:
	.size	handleMaybeNormalState, .-handleMaybeNormalState
	.align	2
	.global	handleInCharState
	.type	handleInCharState, %function
handleInCharState:
.LFB4:
	.cfi_startproc
	stp	x29, x30, [sp, -48]!
	.cfi_def_cfa_offset 48
	.cfi_offset 29, -48
	.cfi_offset 30, -40
	add	x29, sp, 0
	.cfi_def_cfa_register 29
	str	w0, [x29,28]
	ldr	w0, [x29,28]
	bl	putchar
	ldr	w0, [x29,28]
	cmp	w0, 39
	beq	.L35
	cmp	w0, 92
	beq	.L36
	b	.L39
.L35:
	str	wzr, [x29,44]
	b	.L37
.L36:
	mov	w0, 5
	str	w0, [x29,44]
	b	.L37
.L39:
	mov	w0, 4
	str	w0, [x29,44]
	nop
.L37:
	ldr	w0, [x29,44]
	ldp	x29, x30, [sp], 48
	.cfi_restore 30
	.cfi_restore 29
	.cfi_def_cfa 31, 0
	ret
	.cfi_endproc
.LFE4:
	.size	handleInCharState, .-handleInCharState
	.align	2
	.global	handleCharEscapeState
	.type	handleCharEscapeState, %function
handleCharEscapeState:
.LFB5:
	.cfi_startproc
	stp	x29, x30, [sp, -32]!
	.cfi_def_cfa_offset 32
	.cfi_offset 29, -32
	.cfi_offset 30, -24
	add	x29, sp, 0
	.cfi_def_cfa_register 29
	str	w0, [x29,28]
	ldr	w0, [x29,28]
	bl	putchar
	mov	w0, 4
	ldp	x29, x30, [sp], 32
	.cfi_restore 30
	.cfi_restore 29
	.cfi_def_cfa 31, 0
	ret
	.cfi_endproc
.LFE5:
	.size	handleCharEscapeState, .-handleCharEscapeState
	.align	2
	.global	handleInStringState
	.type	handleInStringState, %function
handleInStringState:
.LFB6:
	.cfi_startproc
	stp	x29, x30, [sp, -48]!
	.cfi_def_cfa_offset 48
	.cfi_offset 29, -48
	.cfi_offset 30, -40
	add	x29, sp, 0
	.cfi_def_cfa_register 29
	str	w0, [x29,28]
	ldr	w0, [x29,28]
	bl	putchar
	ldr	w0, [x29,28]
	cmp	w0, 34
	beq	.L44
	cmp	w0, 92
	beq	.L45
	b	.L48
.L44:
	str	wzr, [x29,44]
	b	.L46
.L45:
	mov	w0, 7
	str	w0, [x29,44]
	b	.L46
.L48:
	mov	w0, 6
	str	w0, [x29,44]
	nop
.L46:
	ldr	w0, [x29,44]
	ldp	x29, x30, [sp], 48
	.cfi_restore 30
	.cfi_restore 29
	.cfi_def_cfa 31, 0
	ret
	.cfi_endproc
.LFE6:
	.size	handleInStringState, .-handleInStringState
	.align	2
	.global	handleStringEscapeState
	.type	handleStringEscapeState, %function
handleStringEscapeState:
.LFB7:
	.cfi_startproc
	stp	x29, x30, [sp, -32]!
	.cfi_def_cfa_offset 32
	.cfi_offset 29, -32
	.cfi_offset 30, -24
	add	x29, sp, 0
	.cfi_def_cfa_register 29
	str	w0, [x29,28]
	ldr	w0, [x29,28]
	bl	putchar
	mov	w0, 6
	ldp	x29, x30, [sp], 32
	.cfi_restore 30
	.cfi_restore 29
	.cfi_def_cfa 31, 0
	ret
	.cfi_endproc
.LFE7:
	.size	handleStringEscapeState, .-handleStringEscapeState
	.section	.rodata
	.align	3
.LC0:
	.string	"Error: line %d: unterminated comment\n"
	.text
	.align	2
	.global	main
	.type	main, %function
main:
.LFB8:
	.cfi_startproc
	stp	x29, x30, [sp, -48]!
	.cfi_def_cfa_offset 48
	.cfi_offset 29, -48
	.cfi_offset 30, -40
	add	x29, sp, 0
	.cfi_def_cfa_register 29
	mov	w0, 1
	str	w0, [x29,44]
	str	wzr, [x29,40]
	str	wzr, [x29,36]
	b	.L52
.L66:
	ldr	w0, [x29,32]
	cmp	w0, 10
	bne	.L53
	ldr	w0, [x29,44]
	add	w0, w0, 1
	str	w0, [x29,44]
.L53:
	ldr	w0, [x29,36]
	cmp	w0, 7
	bhi	.L52
	adrp	x1, .L55
	add	x1, x1, :lo12:.L55
	ldr	w2, [x1,w0,uxtw #2]
	adr	x3, .Lrtx55
	add	x2, x3, w2, sxtw #2
	br	x2
.Lrtx55:
	.section	.rodata
	.align	0
	.align	2
.L55:
	.word	(.L54 - .Lrtx55) / 4
	.word	(.L56 - .Lrtx55) / 4
	.word	(.L57 - .Lrtx55) / 4
	.word	(.L58 - .Lrtx55) / 4
	.word	(.L59 - .Lrtx55) / 4
	.word	(.L60 - .Lrtx55) / 4
	.word	(.L61 - .Lrtx55) / 4
	.word	(.L62 - .Lrtx55) / 4
	.text
.L54:
	ldr	w0, [x29,32]
	bl	handleNormalState
	str	w0, [x29,36]
	b	.L52
.L56:
	ldr	w0, [x29,32]
	bl	handleMaybeCommentState
	str	w0, [x29,36]
	b	.L52
.L57:
	ldr	w0, [x29,32]
	cmp	w0, 10
	bne	.L63
	ldr	w0, [x29,40]
	add	w0, w0, 1
	str	w0, [x29,40]
.L63:
	ldr	w0, [x29,32]
	bl	handleInCommentState
	str	w0, [x29,36]
	b	.L52
.L58:
	ldr	w0, [x29,32]
	bl	handleMaybeNormalState
	str	w0, [x29,36]
	ldr	w0, [x29,32]
	cmp	w0, 10
	bne	.L64
	ldr	w0, [x29,40]
	add	w0, w0, 1
	str	w0, [x29,40]
.L64:
	ldr	w0, [x29,36]
	cmp	w0, wzr
	bne	.L65
	str	wzr, [x29,40]
	b	.L52
.L65:
	b	.L52
.L59:
	ldr	w0, [x29,32]
	bl	handleInCharState
	str	w0, [x29,36]
	b	.L52
.L60:
	ldr	w0, [x29,32]
	bl	handleCharEscapeState
	str	w0, [x29,36]
	b	.L52
.L61:
	ldr	w0, [x29,32]
	bl	handleInStringState
	str	w0, [x29,36]
	b	.L52
.L62:
	ldr	w0, [x29,32]
	bl	handleStringEscapeState
	str	w0, [x29,36]
	nop
.L52:
	bl	getchar
	str	w0, [x29,32]
	ldr	w0, [x29,32]
	cmn	w0, #1
	bne	.L66
	ldr	w0, [x29,36]
	cmp	w0, 1
	bne	.L67
	mov	w0, 47
	bl	putchar
.L67:
	ldr	w0, [x29,36]
	cmp	w0, 2
	beq	.L68
	ldr	w0, [x29,36]
	cmp	w0, 3
	bne	.L69
.L68:
	ldr	w1, [x29,44]
	ldr	w0, [x29,40]
	sub	w0, w1, w0
	str	w0, [x29,28]
	adrp	x0, stderr
	add	x0, x0, :lo12:stderr
	ldr	x0, [x0]
	adrp	x1, .LC0
	add	x1, x1, :lo12:.LC0
	ldr	w2, [x29,28]
	bl	fprintf
	mov	w0, 1
	b	.L70
.L69:
	mov	w0, 0
.L70:
	ldp	x29, x30, [sp], 48
	.cfi_restore 30
	.cfi_restore 29
	.cfi_def_cfa 31, 0
	ret
	.cfi_endproc
.LFE8:
	.size	main, .-main
	.ident	"GCC: (GNU) 4.8.5 20150623 (Red Hat 4.8.5-28)"
	.section	.note.GNU-stack,"",%progbits
