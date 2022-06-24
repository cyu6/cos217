	.cpu generic+fp+simd
	.file	"stra.c"
	.section	.rodata
	.align	3
.LC0:
	.string	"pcSrc != ((void *)0)"
	.align	3
.LC1:
	.string	"stra.c"
	.text
	.align	2
	.global	Str_getLength
	.type	Str_getLength, %function
Str_getLength:
.LFB0:
	.cfi_startproc
	stp	x29, x30, [sp, -48]!
	.cfi_def_cfa_offset 48
	.cfi_offset 29, -48
	.cfi_offset 30, -40
	add	x29, sp, 0
	.cfi_def_cfa_register 29
	str	x0, [x29,24]
	str	xzr, [x29,40]
	ldr	x0, [x29,24]
	cmp	x0, xzr
	bne	.L2
	adrp	x0, .LC0
	add	x0, x0, :lo12:.LC0
	adrp	x1, .LC1
	add	x1, x1, :lo12:.LC1
	adrp	x2, __PRETTY_FUNCTION__.2446
	add	x3, x2, :lo12:__PRETTY_FUNCTION__.2446
	mov	w2, 17
	bl	__assert_fail
.L2:
	b	.L3
.L4:
	ldr	x0, [x29,40]
	add	x0, x0, 1
	str	x0, [x29,40]
.L3:
	ldr	x1, [x29,24]
	ldr	x0, [x29,40]
	add	x0, x1, x0
	ldrb	w0, [x0]
	cmp	w0, wzr
	bne	.L4
	ldr	x0, [x29,40]
	ldp	x29, x30, [sp], 48
	.cfi_restore 30
	.cfi_restore 29
	.cfi_def_cfa 31, 0
	ret
	.cfi_endproc
.LFE0:
	.size	Str_getLength, .-Str_getLength
	.section	.rodata
	.align	3
.LC2:
	.string	"pcDest != ((void *)0)"
	.text
	.align	2
	.global	Str_copy
	.type	Str_copy, %function
Str_copy:
.LFB1:
	.cfi_startproc
	stp	x29, x30, [sp, -48]!
	.cfi_def_cfa_offset 48
	.cfi_offset 29, -48
	.cfi_offset 30, -40
	add	x29, sp, 0
	.cfi_def_cfa_register 29
	str	x0, [x29,24]
	str	x1, [x29,16]
	ldr	x0, [x29,16]
	bl	Str_getLength
	str	x0, [x29,32]
	ldr	x0, [x29,24]
	cmp	x0, xzr
	bne	.L7
	adrp	x0, .LC2
	add	x0, x0, :lo12:.LC2
	adrp	x1, .LC1
	add	x1, x1, :lo12:.LC1
	adrp	x2, __PRETTY_FUNCTION__.2456
	add	x3, x2, :lo12:__PRETTY_FUNCTION__.2456
	mov	w2, 29
	bl	__assert_fail
.L7:
	ldr	x0, [x29,16]
	cmp	x0, xzr
	bne	.L8
	adrp	x0, .LC0
	add	x0, x0, :lo12:.LC0
	adrp	x1, .LC1
	add	x1, x1, :lo12:.LC1
	adrp	x2, __PRETTY_FUNCTION__.2456
	add	x3, x2, :lo12:__PRETTY_FUNCTION__.2456
	mov	w2, 30
	bl	__assert_fail
.L8:
	str	xzr, [x29,40]
	b	.L9
.L10:
	ldr	x1, [x29,24]
	ldr	x0, [x29,40]
	add	x0, x1, x0
	ldr	x2, [x29,16]
	ldr	x1, [x29,40]
	add	x1, x2, x1
	ldrb	w1, [x1]
	strb	w1, [x0]
	ldr	x0, [x29,40]
	add	x0, x0, 1
	str	x0, [x29,40]
.L9:
	ldr	x1, [x29,40]
	ldr	x0, [x29,32]
	cmp	x1, x0
	bcc	.L10
	ldr	x1, [x29,24]
	ldr	x0, [x29,40]
	add	x0, x1, x0
	strb	wzr, [x0]
	ldr	x0, [x29,24]
	ldp	x29, x30, [sp], 48
	.cfi_restore 30
	.cfi_restore 29
	.cfi_def_cfa 31, 0
	ret
	.cfi_endproc
.LFE1:
	.size	Str_copy, .-Str_copy
	.align	2
	.global	Str_concat
	.type	Str_concat, %function
Str_concat:
.LFB2:
	.cfi_startproc
	stp	x29, x30, [sp, -64]!
	.cfi_def_cfa_offset 64
	.cfi_offset 29, -64
	.cfi_offset 30, -56
	add	x29, sp, 0
	.cfi_def_cfa_register 29
	str	x0, [x29,24]
	str	x1, [x29,16]
	ldr	x0, [x29,16]
	bl	Str_getLength
	str	x0, [x29,48]
	ldr	x0, [x29,24]
	bl	Str_getLength
	str	x0, [x29,40]
	ldr	x0, [x29,24]
	cmp	x0, xzr
	bne	.L13
	adrp	x0, .LC2
	add	x0, x0, :lo12:.LC2
	adrp	x1, .LC1
	add	x1, x1, :lo12:.LC1
	adrp	x2, __PRETTY_FUNCTION__.2467
	add	x3, x2, :lo12:__PRETTY_FUNCTION__.2467
	mov	w2, 45
	bl	__assert_fail
.L13:
	ldr	x0, [x29,16]
	cmp	x0, xzr
	bne	.L14
	adrp	x0, .LC0
	add	x0, x0, :lo12:.LC0
	adrp	x1, .LC1
	add	x1, x1, :lo12:.LC1
	adrp	x2, __PRETTY_FUNCTION__.2467
	add	x3, x2, :lo12:__PRETTY_FUNCTION__.2467
	mov	w2, 46
	bl	__assert_fail
.L14:
	str	xzr, [x29,56]
	b	.L15
.L16:
	ldr	x1, [x29,40]
	ldr	x0, [x29,56]
	add	x0, x1, x0
	ldr	x1, [x29,24]
	add	x0, x1, x0
	ldr	x2, [x29,16]
	ldr	x1, [x29,56]
	add	x1, x2, x1
	ldrb	w1, [x1]
	strb	w1, [x0]
	ldr	x0, [x29,56]
	add	x0, x0, 1
	str	x0, [x29,56]
.L15:
	ldr	x1, [x29,56]
	ldr	x0, [x29,48]
	cmp	x1, x0
	bls	.L16
	ldr	x0, [x29,24]
	ldp	x29, x30, [sp], 64
	.cfi_restore 30
	.cfi_restore 29
	.cfi_def_cfa 31, 0
	ret
	.cfi_endproc
.LFE2:
	.size	Str_concat, .-Str_concat
	.section	.rodata
	.align	3
.LC3:
	.string	"pcStr1 != ((void *)0)"
	.align	3
.LC4:
	.string	"pcStr2 != ((void *)0)"
	.text
	.align	2
	.global	Str_compare
	.type	Str_compare, %function
Str_compare:
.LFB3:
	.cfi_startproc
	stp	x29, x30, [sp, -64]!
	.cfi_def_cfa_offset 64
	.cfi_offset 29, -64
	.cfi_offset 30, -56
	add	x29, sp, 0
	.cfi_def_cfa_register 29
	str	x19, [sp,16]
	.cfi_offset 19, -48
	str	x0, [x29,40]
	str	x1, [x29,32]
	ldr	x0, [x29,40]
	cmp	x0, xzr
	bne	.L19
	adrp	x0, .LC3
	add	x0, x0, :lo12:.LC3
	adrp	x1, .LC1
	add	x1, x1, :lo12:.LC1
	adrp	x2, __PRETTY_FUNCTION__.2476
	add	x3, x2, :lo12:__PRETTY_FUNCTION__.2476
	mov	w2, 59
	bl	__assert_fail
.L19:
	ldr	x0, [x29,32]
	cmp	x0, xzr
	bne	.L20
	adrp	x0, .LC4
	add	x0, x0, :lo12:.LC4
	adrp	x1, .LC1
	add	x1, x1, :lo12:.LC1
	adrp	x2, __PRETTY_FUNCTION__.2476
	add	x3, x2, :lo12:__PRETTY_FUNCTION__.2476
	mov	w2, 60
	bl	__assert_fail
.L20:
	str	xzr, [x29,56]
	b	.L21
.L25:
	ldr	x1, [x29,40]
	ldr	x0, [x29,56]
	add	x0, x1, x0
	ldrb	w1, [x0]
	ldr	x2, [x29,32]
	ldr	x0, [x29,56]
	add	x0, x2, x0
	ldrb	w0, [x0]
	cmp	w1, w0
	bcs	.L22
	mov	w0, -1
	b	.L23
.L22:
	ldr	x1, [x29,32]
	ldr	x0, [x29,56]
	add	x0, x1, x0
	ldrb	w1, [x0]
	ldr	x2, [x29,40]
	ldr	x0, [x29,56]
	add	x0, x2, x0
	ldrb	w0, [x0]
	cmp	w1, w0
	bcs	.L24
	mov	w0, 1
	b	.L23
.L24:
	ldr	x0, [x29,56]
	add	x0, x0, 1
	str	x0, [x29,56]
.L21:
	ldr	x0, [x29,40]
	bl	Str_getLength
	mov	x1, x0
	ldr	x0, [x29,56]
	cmp	x1, x0
	bhi	.L25
	ldr	x0, [x29,40]
	bl	Str_getLength
	mov	x19, x0
	ldr	x0, [x29,32]
	bl	Str_getLength
	cmp	x19, x0
	bcs	.L26
	mov	w0, -1
	b	.L23
.L26:
	mov	w0, 0
.L23:
	ldr	x19, [sp,16]
	ldp	x29, x30, [sp], 64
	.cfi_restore 30
	.cfi_restore 29
	.cfi_restore 19
	.cfi_def_cfa 31, 0
	ret
	.cfi_endproc
.LFE3:
	.size	Str_compare, .-Str_compare
	.section	.rodata
	.align	3
.LC5:
	.string	"pcHaystack != ((void *)0)"
	.align	3
.LC6:
	.string	"pcNeedle != ((void *)0)"
	.text
	.align	2
	.global	Str_search
	.type	Str_search, %function
Str_search:
.LFB4:
	.cfi_startproc
	stp	x29, x30, [sp, -64]!
	.cfi_def_cfa_offset 64
	.cfi_offset 29, -64
	.cfi_offset 30, -56
	add	x29, sp, 0
	.cfi_def_cfa_register 29
	str	x19, [sp,16]
	.cfi_offset 19, -48
	str	x0, [x29,40]
	str	x1, [x29,32]
	str	xzr, [x29,56]
	str	xzr, [x29,48]
	ldr	x0, [x29,40]
	cmp	x0, xzr
	bne	.L28
	adrp	x0, .LC5
	add	x0, x0, :lo12:.LC5
	adrp	x1, .LC1
	add	x1, x1, :lo12:.LC1
	adrp	x2, __PRETTY_FUNCTION__.2487
	add	x3, x2, :lo12:__PRETTY_FUNCTION__.2487
	mov	w2, 87
	bl	__assert_fail
.L28:
	ldr	x0, [x29,32]
	cmp	x0, xzr
	bne	.L29
	adrp	x0, .LC6
	add	x0, x0, :lo12:.LC6
	adrp	x1, .LC1
	add	x1, x1, :lo12:.LC1
	adrp	x2, __PRETTY_FUNCTION__.2487
	add	x3, x2, :lo12:__PRETTY_FUNCTION__.2487
	mov	w2, 88
	bl	__assert_fail
.L29:
	ldr	x0, [x29,32]
	bl	Str_getLength
	cmp	x0, xzr
	bne	.L30
	ldr	x0, [x29,40]
	b	.L31
.L30:
	ldr	x0, [x29,40]
	bl	Str_getLength
	mov	x19, x0
	ldr	x0, [x29,32]
	bl	Str_getLength
	cmp	x19, x0
	bcs	.L32
	mov	x0, 0
	b	.L31
.L32:
	b	.L33
.L39:
	b	.L34
.L37:
	ldr	x1, [x29,56]
	ldr	x0, [x29,48]
	add	x0, x1, x0
	ldr	x1, [x29,40]
	add	x0, x1, x0
	ldrb	w1, [x0]
	ldr	x2, [x29,32]
	ldr	x0, [x29,48]
	add	x0, x2, x0
	ldrb	w0, [x0]
	cmp	w1, w0
	bne	.L35
	ldr	x0, [x29,48]
	add	x0, x0, 1
	str	x0, [x29,48]
	b	.L34
.L35:
	b	.L36
.L34:
	ldr	x1, [x29,32]
	ldr	x0, [x29,48]
	add	x0, x1, x0
	ldrb	w0, [x0]
	cmp	w0, wzr
	bne	.L37
.L36:
	ldr	x1, [x29,32]
	ldr	x0, [x29,48]
	add	x0, x1, x0
	ldrb	w0, [x0]
	cmp	w0, wzr
	bne	.L38
	ldr	x1, [x29,40]
	ldr	x0, [x29,56]
	add	x0, x1, x0
	b	.L31
.L38:
	str	xzr, [x29,48]
	ldr	x0, [x29,56]
	add	x0, x0, 1
	str	x0, [x29,56]
.L33:
	ldr	x1, [x29,40]
	ldr	x0, [x29,56]
	add	x0, x1, x0
	ldrb	w0, [x0]
	cmp	w0, wzr
	bne	.L39
	mov	x0, 0
.L31:
	ldr	x19, [sp,16]
	ldp	x29, x30, [sp], 64
	.cfi_restore 30
	.cfi_restore 29
	.cfi_restore 19
	.cfi_def_cfa 31, 0
	ret
	.cfi_endproc
.LFE4:
	.size	Str_search, .-Str_search
	.section	.rodata
	.align	3
	.type	__PRETTY_FUNCTION__.2446, %object
	.size	__PRETTY_FUNCTION__.2446, 14
__PRETTY_FUNCTION__.2446:
	.string	"Str_getLength"
	.align	3
	.type	__PRETTY_FUNCTION__.2456, %object
	.size	__PRETTY_FUNCTION__.2456, 9
__PRETTY_FUNCTION__.2456:
	.string	"Str_copy"
	.align	3
	.type	__PRETTY_FUNCTION__.2467, %object
	.size	__PRETTY_FUNCTION__.2467, 11
__PRETTY_FUNCTION__.2467:
	.string	"Str_concat"
	.align	3
	.type	__PRETTY_FUNCTION__.2476, %object
	.size	__PRETTY_FUNCTION__.2476, 12
__PRETTY_FUNCTION__.2476:
	.string	"Str_compare"
	.align	3
	.type	__PRETTY_FUNCTION__.2487, %object
	.size	__PRETTY_FUNCTION__.2487, 11
__PRETTY_FUNCTION__.2487:
	.string	"Str_search"
	.ident	"GCC: (GNU) 4.8.5 20150623 (Red Hat 4.8.5-28)"
	.section	.note.GNU-stack,"",%progbits
