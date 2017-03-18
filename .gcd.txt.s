.globl gcd
gcd:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	12(%ebp)
	pushl	$0
	popl	%ebx
	popl	%eax
	cmpl	%ebx, %eax
	seteb	%cl
	pushl	%ecx
	popl	%eax
	decb	%al
	jne	.IF_FALSE_1
	pushl	8(%ebp)
	popl	%eax
	jmp	.RETURN_gcd
	jmp	.IF_END_1
.IF_FALSE_1:
	pushl	8(%ebp)
	pushl	12(%ebp)
	popl	%ebx
	popl	%eax
	xorl	%edx, %edx
	idivl	%ebx
	pushl	%edx
	pushl	12(%ebp)
	call	gcd
	addl	$8, %esp
	pushl	%eax
	popl	%eax
	jmp	.RETURN_gcd
.IF_END_1:
.RETURN_gcd:
	addl	$0, %esp
	leave
	ret

.globl main
main:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$8, %esp
	pushl	$28
	popl	-4(%ebp)
	pushl	$21
	popl	-8(%ebp)
	pushl	$.LC0
	pushl	-8(%ebp)
	pushl	-4(%ebp)
	call	gcd
	addl	$8, %esp
	pushl	%eax
	pushl	$.LC1
	pushl	-8(%ebp)
	pushl	$.LC2
	pushl	-4(%ebp)
	pushl	$.LC3
	pushl	$.LC4
	call	printf
	addl	$32, %esp
	pushl	$0
	popl	%eax
	jmp	.RETURN_main
.RETURN_main:
	addl	$8, %esp
	leave
	ret

.TRUE:
	.string "true"
.FALSE:
	.string "false"

.LC0:
	.string "\n"
.LC1:
	.string ") = "
.LC2:
	.string ", "
.LC3:
	.string "gcd("
.LC4:
	.string "%s%d%s%d%s%d%s"
