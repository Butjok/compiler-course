.globl main
main:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$4, %esp
	pushl	$0
	call	time
	addl	$4, %esp
	pushl	%eax
	call	srand
	addl	$4, %esp
	pushl	$0
	popl	-4(%ebp)
.FOR_1:
	pushl	-4(%ebp)
	pushl	$100
	popl	%ebx
	popl	%eax
	cmpl	%ebx, %eax
	setlb	%cl
	pushl	%ecx
	popl	%eax
	decb	%al
	jne	.FOR_END_1
	pushl	$.LC0
	call	frand
	pushl	%eax
	flds	(%esp)
	subl	$4, %esp
	fstpl	(%esp)
	pushl	$.LC1
	call	printf
	addl	$12, %esp
	incl	-4(%ebp)
	jmp	.FOR_1
.FOR_END_1:
	pushl	$0
	popl	%eax
	jmp	.RETURN_main
.RETURN_main:
	addl	$4, %esp
	leave
	ret

.TRUE:
	.string "true"
.FALSE:
	.string "false"

.LC0:
	.string "\n"
.LC1:
	.string "%f%s"
