.globl main
main:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$24, %esp
	pushl	$0
	call	time
	addl	$4, %esp
	pushl	%eax
	call	srand
	addl	$4, %esp
	pushl	$0
	popl	-4(%ebp)
	pushl	$0
	popl	-8(%ebp)
	pushl	$0
	popl	-12(%ebp)
.FOR_1:
	pushl	-12(%ebp)
	pushl	$10000000
	popl	%ebx
	popl	%eax
	cmpl	%ebx, %eax
	setlb	%cl
	pushl	%ecx
	popl	%eax
	decb	%al
	jne	.FOR_END_1
	call	frand
	pushl	%eax
	popl	-16(%ebp)
	call	frand
	pushl	%eax
	popl	-20(%ebp)
	pushl	-16(%ebp)
	pushl	-16(%ebp)
	flds	(%esp)
	flds	4(%esp)
	fmulp
	addl	$4, %esp
	fstps	(%esp)
	pushl	-20(%ebp)
	pushl	-20(%ebp)
	flds	(%esp)
	flds	4(%esp)
	fmulp
	addl	$4, %esp
	fstps	(%esp)
	flds	(%esp)
	flds	4(%esp)
	faddp
	addl	$4, %esp
	fstps	(%esp)
	pushl	$1
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
	incl	-8(%ebp)
	jmp	.IF_END_2
.IF_FALSE_2:
.IF_END_2:
	incl	-4(%ebp)
	incl	-12(%ebp)
	jmp	.FOR_1
.FOR_END_1:
	pushl	$0x40800000
	pushl	-8(%ebp)
	fildl	(%esp)
	fstps	(%esp)
	flds	(%esp)
	flds	4(%esp)
	fmulp
	addl	$4, %esp
	fstps	(%esp)
	pushl	-4(%ebp)
	fildl	(%esp)
	fstps	(%esp)
	flds	(%esp)
	flds	4(%esp)
	fdivp
	addl	$4, %esp
	fstps	(%esp)
	popl	-24(%ebp)
	pushl	$.LC0
	pushl	-24(%ebp)
	flds	(%esp)
	subl	$4, %esp
	fstpl	(%esp)
	pushl	$.LC1
	pushl	$.LC2
	call	printf
	addl	$16, %esp
	pushl	$.LC3
	call	pi
	pushl	%eax
	flds	(%esp)
	subl	$4, %esp
	fstpl	(%esp)
	pushl	$.LC4
	pushl	$.LC5
	call	printf
	addl	$16, %esp
	pushl	$0
	popl	%eax
	jmp	.RETURN_main
.RETURN_main:
	addl	$24, %esp
	leave
	ret

.TRUE:
	.string "true"
.FALSE:
	.string "false"

.LC0:
	.string "\n"
.LC1:
	.string "Calculated PI = "
.LC2:
	.string "%s%f%s"
.LC3:
	.string "\n"
.LC4:
	.string "Predefined PI = "
.LC5:
	.string "%s%f%s"
