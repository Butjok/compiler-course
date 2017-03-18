.globl main
main:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$32, %esp
	pushl	$1
	popl	-4(%ebp)
	pushl	$2
	popl	-8(%ebp)
	pushl	-4(%ebp)
	pushl	-8(%ebp)
	popl	%ebx
	popl	%eax
	cmpl	%ebx, %eax
	seteb	%cl
	pushl	%ecx
	popl	-12(%ebp)
	pushl	-4(%ebp)
	pushl	-8(%ebp)
	popl	%ebx
	popl	%eax
	cmpl	%ebx, %eax
	setneb	%cl
	pushl	%ecx
	popl	-16(%ebp)
	pushl	-4(%ebp)
	pushl	-8(%ebp)
	popl	%ebx
	popl	%eax
	cmpl	%ebx, %eax
	setlb	%cl
	pushl	%ecx
	popl	-20(%ebp)
	pushl	-4(%ebp)
	pushl	-8(%ebp)
	popl	%ebx
	popl	%eax
	cmpl	%ebx, %eax
	setleb	%cl
	pushl	%ecx
	popl	-24(%ebp)
	pushl	-4(%ebp)
	pushl	-8(%ebp)
	popl	%ebx
	popl	%eax
	cmpl	%ebx, %eax
	setgb	%cl
	pushl	%ecx
	popl	-28(%ebp)
	pushl	-4(%ebp)
	pushl	-8(%ebp)
	popl	%ebx
	popl	%eax
	cmpl	%ebx, %eax
	setgeb	%cl
	pushl	%ecx
	popl	-32(%ebp)
	pushl	$.LC0
	pushl	-4(%ebp)
	pushl	$.LC1
	pushl	$.LC2
	call	printf
	addl	$16, %esp
	pushl	$.LC3
	pushl	-8(%ebp)
	pushl	$.LC4
	pushl	$.LC5
	call	printf
	addl	$16, %esp
	pushl	$.LC6
	pushl	$.LC7
	call	printf
	addl	$8, %esp
	pushl	-12(%ebp)
	popl	%eax
	decb	%al
	jne	.IF_FALSE_1
	pushl	$.LC8
	pushl	$.LC9
	call	printf
	addl	$8, %esp
	jmp	.IF_END_1
.IF_FALSE_1:
.IF_END_1:
	pushl	-16(%ebp)
	popl	%eax
	decb	%al
	jne	.IF_FALSE_2
	pushl	$.LC10
	pushl	$.LC11
	call	printf
	addl	$8, %esp
	jmp	.IF_END_2
.IF_FALSE_2:
.IF_END_2:
	pushl	-20(%ebp)
	popl	%eax
	decb	%al
	jne	.IF_FALSE_3
	pushl	$.LC12
	pushl	$.LC13
	call	printf
	addl	$8, %esp
	jmp	.IF_END_3
.IF_FALSE_3:
.IF_END_3:
	pushl	-24(%ebp)
	popl	%eax
	decb	%al
	jne	.IF_FALSE_4
	pushl	$.LC14
	pushl	$.LC15
	call	printf
	addl	$8, %esp
	jmp	.IF_END_4
.IF_FALSE_4:
.IF_END_4:
	pushl	-28(%ebp)
	popl	%eax
	decb	%al
	jne	.IF_FALSE_5
	pushl	$.LC16
	pushl	$.LC17
	call	printf
	addl	$8, %esp
	jmp	.IF_END_5
.IF_FALSE_5:
.IF_END_5:
	pushl	-32(%ebp)
	popl	%eax
	decb	%al
	jne	.IF_FALSE_6
	pushl	$.LC18
	pushl	$.LC19
	call	printf
	addl	$8, %esp
	jmp	.IF_END_6
.IF_FALSE_6:
.IF_END_6:
	pushl	$0
	popl	%eax
	jmp	.RETURN_main
.RETURN_main:
	addl	$32, %esp
	leave
	ret

.TRUE:
	.string "true"
.FALSE:
	.string "false"

.LC0:
	.string "\n"
.LC1:
	.string "a = "
.LC2:
	.string "%s%d%s"
.LC3:
	.string "\n"
.LC4:
	.string "b = "
.LC5:
	.string "%s%d%s"
.LC6:
	.string "\n"
.LC7:
	.string "%s"
.LC8:
	.string "Equal\n"
.LC9:
	.string "%s"
.LC10:
	.string "Not equal\n"
.LC11:
	.string "%s"
.LC12:
	.string "Less\n"
.LC13:
	.string "%s"
.LC14:
	.string "Less or equal\n"
.LC15:
	.string "%s"
.LC16:
	.string "Greater\n"
.LC17:
	.string "%s"
.LC18:
	.string "Greater or equal\n"
.LC19:
	.string "%s"
