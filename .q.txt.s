.globl main
main:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$28, %esp
	pushl	$1
	fildl	(%esp)
	fstps	(%esp)
	popl	-4(%ebp)
	pushl	$0
	fildl	(%esp)
	fstps	(%esp)
	popl	-8(%ebp)
	pushl	$4
	popl	%eax
	negl	%eax
	pushl	%eax
	fildl	(%esp)
	fstps	(%esp)
	popl	-12(%ebp)
	pushl	-8(%ebp)
	pushl	-8(%ebp)
	flds	(%esp)
	flds	4(%esp)
	fmulp
	addl	$4, %esp
	fstps	(%esp)
	pushl	$4
	fildl	(%esp)
	fstps	(%esp)
	pushl	-4(%ebp)
	flds	(%esp)
	flds	4(%esp)
	fmulp
	addl	$4, %esp
	fstps	(%esp)
	pushl	-12(%ebp)
	flds	(%esp)
	flds	4(%esp)
	fmulp
	addl	$4, %esp
	fstps	(%esp)
	flds	(%esp)
	flds	4(%esp)
	fsubp
	addl	$4, %esp
	fstps	(%esp)
	popl	-16(%ebp)
	pushl	-16(%ebp)
	pushl	$0
	fildl	(%esp)
	fstps	(%esp)
	flds	(%esp)
	flds	4(%esp)
	fcompp
	fstsw	%ax
	sahf
	setbb	%cl
	addl	$8, %esp
	pushl	%ecx
	popl	%eax
	decb	%al
	jne	.IF_FALSE_1
	pushl	$.LC0
	pushl	$.LC1
	call	printf
	addl	$8, %esp
	pushl	$0
	popl	%eax
	jmp	.RETURN_main
	jmp	.IF_END_1
.IF_FALSE_1:
.IF_END_1:
	pushl	-16(%ebp)
	pushl	$0
	fildl	(%esp)
	fstps	(%esp)
	flds	(%esp)
	flds	4(%esp)
	fcompp
	fstsw	%ax
	sahf
	seteb	%cl
	addl	$8, %esp
	pushl	%ecx
	popl	%eax
	decb	%al
	jne	.IF_FALSE_2
	pushl	-8(%ebp)
	flds	(%esp)
	fchs
	fstps	(%esp)
	pushl	$2
	fildl	(%esp)
	fstps	(%esp)
	pushl	-4(%ebp)
	flds	(%esp)
	flds	4(%esp)
	fmulp
	addl	$4, %esp
	fstps	(%esp)
	flds	(%esp)
	flds	4(%esp)
	fdivp
	addl	$4, %esp
	fstps	(%esp)
	popl	-20(%ebp)
	pushl	$.LC2
	pushl	-20(%ebp)
	flds	(%esp)
	subl	$4, %esp
	fstpl	(%esp)
	pushl	$.LC3
	pushl	$.LC4
	call	printf
	addl	$16, %esp
	pushl	$0
	popl	%eax
	jmp	.RETURN_main
	jmp	.IF_END_2
.IF_FALSE_2:
.IF_END_2:
	pushl	-8(%ebp)
	flds	(%esp)
	fchs
	fstps	(%esp)
	pushl	-16(%ebp)
	call	sqrt
	addl	$4, %esp
	pushl	%eax
	flds	(%esp)
	flds	4(%esp)
	faddp
	addl	$4, %esp
	fstps	(%esp)
	pushl	$2
	fildl	(%esp)
	fstps	(%esp)
	pushl	-4(%ebp)
	flds	(%esp)
	flds	4(%esp)
	fmulp
	addl	$4, %esp
	fstps	(%esp)
	flds	(%esp)
	flds	4(%esp)
	fdivp
	addl	$4, %esp
	fstps	(%esp)
	popl	-24(%ebp)
	pushl	-8(%ebp)
	flds	(%esp)
	fchs
	fstps	(%esp)
	pushl	-16(%ebp)
	call	sqrt
	addl	$4, %esp
	pushl	%eax
	flds	(%esp)
	flds	4(%esp)
	fsubp
	addl	$4, %esp
	fstps	(%esp)
	pushl	$2
	fildl	(%esp)
	fstps	(%esp)
	pushl	-4(%ebp)
	flds	(%esp)
	flds	4(%esp)
	fmulp
	addl	$4, %esp
	fstps	(%esp)
	flds	(%esp)
	flds	4(%esp)
	fdivp
	addl	$4, %esp
	fstps	(%esp)
	popl	-28(%ebp)
	pushl	$.LC5
	pushl	-24(%ebp)
	flds	(%esp)
	subl	$4, %esp
	fstpl	(%esp)
	pushl	$.LC6
	pushl	$.LC7
	call	printf
	addl	$16, %esp
	pushl	$.LC8
	pushl	-28(%ebp)
	flds	(%esp)
	subl	$4, %esp
	fstpl	(%esp)
	pushl	$.LC9
	pushl	$.LC10
	call	printf
	addl	$16, %esp
	pushl	$0
	popl	%eax
	jmp	.RETURN_main
.RETURN_main:
	addl	$28, %esp
	leave
	ret

.TRUE:
	.string "true"
.FALSE:
	.string "false"

.LC0:
	.string "No real solution\n"
.LC1:
	.string "%s"
.LC2:
	.string "\n"
.LC3:
	.string "X = "
.LC4:
	.string "%s%f%s"
.LC5:
	.string "\n"
.LC6:
	.string "X1 = "
.LC7:
	.string "%s%f%s"
.LC8:
	.string "\n"
.LC9:
	.string "X2 = "
.LC10:
	.string "%s%f%s"
