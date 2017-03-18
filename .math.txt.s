.globl main
main:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	$.LC0
	call	pi
	pushl	%eax
	flds	(%esp)
	subl	$4, %esp
	fstpl	(%esp)
	pushl	$.LC1
	call	printf
	addl	$12, %esp
	pushl	$0
	popl	%eax
	jmp	.RETURN_main
.RETURN_main:
	addl	$0, %esp
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
