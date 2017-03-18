.globl frand
frand:
	pushl	%ebp
	movl	%esp, %ebp
	call	rand
	pushl	%eax
	fildl	(%esp)
	fstps	(%esp)
	call	rand_max
	pushl	%eax
	fildl	(%esp)
	fstps	(%esp)
	pushl	$0x00000000
	flds	(%esp)
	flds	4(%esp)
	faddp
	addl	$4, %esp
	fstps	(%esp)
	flds	(%esp)
	flds	4(%esp)
	fdivp
	addl	$4, %esp
	fstps	(%esp)
	popl	%eax
	jmp	.RETURN_frand
.RETURN_frand:
	addl	$0, %esp
	leave
	ret

.globl rand_max
rand_max:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	$2147483647
	popl	%eax
	jmp	.RETURN_rand_max
.RETURN_rand_max:
	addl	$0, %esp
	leave
	ret

.TRUE:
	.string "true"
.FALSE:
	.string "false"

