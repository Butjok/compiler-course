.globl main
main:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$36, %esp
	pushl	$0x42340000
	popl	-4(%ebp)
	pushl	$0x42480000
	popl	-8(%ebp)
	pushl	-4(%ebp)
	call	pi
	pushl	%eax
	flds	(%esp)
	flds	4(%esp)
	fmulp
	addl	$4, %esp
	fstps	(%esp)
	pushl	$0x43340000
	flds	(%esp)
	flds	4(%esp)
	fdivp
	addl	$4, %esp
	fstps	(%esp)
	popl	-12(%ebp)
	pushl	$0x00000000
	popl	-16(%ebp)
	pushl	$0x00000000
	popl	-20(%ebp)
	pushl	-12(%ebp)
	call	cos
	addl	$4, %esp
	pushl	%eax
	pushl	-8(%ebp)
	flds	(%esp)
	flds	4(%esp)
	fmulp
	addl	$4, %esp
	fstps	(%esp)
	popl	-24(%ebp)
	pushl	-12(%ebp)
	call	sin
	addl	$4, %esp
	pushl	%eax
	pushl	-8(%ebp)
	flds	(%esp)
	flds	4(%esp)
	fmulp
	addl	$4, %esp
	fstps	(%esp)
	popl	-28(%ebp)
	pushl	$0x41200000
	popl	-32(%ebp)
	pushl	$1
	popl	-36(%ebp)
	pushl	$.LC0
	pushl	$.LC1
	call	printf
	addl	$8, %esp
	pushl	$1
	call	sleep
	addl	$4, %esp
.WHILE_1:
	pushl	-36(%ebp)
	popl	%eax
	decb	%al
	jne	.WHILE_END_1
	pushl	$.LC2
	pushl	-20(%ebp)
	flds	(%esp)
	subl	$4, %esp
	fstpl	(%esp)
	pushl	$.LC3
	pushl	-16(%ebp)
	flds	(%esp)
	subl	$4, %esp
	fstpl	(%esp)
	pushl	$.LC4
	call	printf
	addl	$20, %esp
	pushl	$500000
	call	usleep
	addl	$4, %esp
	pushl	-16(%ebp)
	pushl	-24(%ebp)
	flds	(%esp)
	flds	4(%esp)
	faddp
	addl	$4, %esp
	fstps	(%esp)
	popl	-16(%ebp)
	pushl	-20(%ebp)
	pushl	-28(%ebp)
	flds	(%esp)
	flds	4(%esp)
	faddp
	addl	$4, %esp
	fstps	(%esp)
	popl	-20(%ebp)
	pushl	-28(%ebp)
	pushl	-32(%ebp)
	flds	(%esp)
	flds	4(%esp)
	fsubp
	addl	$4, %esp
	fstps	(%esp)
	popl	-28(%ebp)
	pushl	-20(%ebp)
	pushl	$0
	fildl	(%esp)
	fstps	(%esp)
	flds	(%esp)
	flds	4(%esp)
	fcompp
	fstsw	%ax
	sahf
	setbeb	%cl
	addl	$8, %esp
	pushl	%ecx
	popl	%eax
	decb	%al
	jne	.IF_FALSE_2
	pushl	$0
	popl	-36(%ebp)
	jmp	.IF_END_2
.IF_FALSE_2:
.IF_END_2:
	jmp	.WHILE_1
.WHILE_END_1:
	pushl	$.LC5
	pushl	$.LC6
	call	printf
	addl	$8, %esp
	pushl	$0
	popl	%eax
	jmp	.RETURN_main
.RETURN_main:
	addl	$36, %esp
	leave
	ret

.TRUE:
	.string "true"
.FALSE:
	.string "false"

.LC0:
	.string "FIRE!\n"
.LC1:
	.string "%s"
.LC2:
	.string "...\n"
.LC3:
	.string ", "
.LC4:
	.string "%f%s%f%s"
.LC5:
	.string "BOOM!\n"
.LC6:
	.string "%s"
