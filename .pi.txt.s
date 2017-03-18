.globl main
main:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$12, %esp
	pushl	$0
	fildl	(%esp)
	fstps	(%esp)
	popl	-4(%ebp)
	pushl	$1
	popl	-8(%ebp)
	pushl	$1
	popl	-12(%ebp)
.FOR_1:
	pushl	-8(%ebp)
	pushl	$1000000
	popl	%ebx
	popl	%eax
	cmpl	%ebx, %eax
	setlb	%cl
	pushl	%ecx
	popl	%eax
	decb	%al
	jne	.FOR_END_1
	pushl	-4(%ebp)
	pushl	-12(%ebp)
	fildl	(%esp)
	fstps	(%esp)
	pushl	$0x3f800000
	pushl	-8(%ebp)
	fildl	(%esp)
	fstps	(%esp)
	flds	(%esp)
	flds	4(%esp)
	fdivp
	addl	$4, %esp
	fstps	(%esp)
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
	popl	-4(%ebp)
	pushl	-8(%ebp)
	pushl	$2
	popl	%ebx
	popl	%eax
	addl	%ebx, %eax
	pushl	%eax
	popl	-8(%ebp)
	pushl	-12(%ebp)
	popl	%eax
	negl	%eax
	pushl	%eax
	popl	-12(%ebp)
	jmp	.FOR_1
.FOR_END_1:
	pushl	$4
	fildl	(%esp)
	fstps	(%esp)
	pushl	-4(%ebp)
	flds	(%esp)
	flds	4(%esp)
	fmulp
	addl	$4, %esp
	fstps	(%esp)
	popl	-4(%ebp)
	pushl	$.LC0
	pushl	-4(%ebp)
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
	addl	$12, %esp
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
