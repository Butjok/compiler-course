.globl abs
abs:
	movl	4(%esp), %eax
.L0:
	negl	%eax
	js		.L0
	ret

.globl fabs
fabs:
	flds	4(%esp)
	fabs
	fstps	4(%esp)
	movl	4(%esp), %eax
	ret

.globl sqrt
sqrt:
	flds	4(%esp)
	fsqrt
	fstps	4(%esp)
	movl	4(%esp), %eax
	ret

.globl sin
sin:
	flds	4(%esp)
	fsin
	fstps	4(%esp)
	movl	4(%esp), %eax
	ret

.globl cos
cos:
	flds	4(%esp)
	fcos
	fstps	4(%esp)
	movl	4(%esp), %eax
	ret

.globl tan
tan:
	flds	4(%esp)
	fptan
	fstps	4(%esp)
	fstps	4(%esp)
	movl	4(%esp), %eax
	ret

.globl ctan
ctan:
	flds	4(%esp)
	fptan
	fdivrp
	fstps	4(%esp)
	movl	4(%esp), %eax
	ret

.globl atan
atan:
	flds	4(%esp)
	fld1
	fpatan
	fstps	4(%esp)
	movl	4(%esp), %eax
	ret

.globl pi
pi:
	fldpi
	subl	$4, %esp
	fstps	(%esp)
	movl	(%esp), %eax
	addl	$4, %esp
	ret

.globl l2e
l2e:
	fldl2e
	subl	$4, %esp
	fstps	(%esp)
	movl	(%esp), %eax
	addl	$4, %esp
	ret

.globl l2t
l2t:
	fldl2t
	subl	$4, %esp
	fstps	(%esp)
	movl	(%esp), %eax
	addl	$4, %esp
	ret

.globl ln2
ln2:
	fldln2
	subl	$4, %esp
	fstps	(%esp)
	movl	(%esp), %eax
	addl	$4, %esp
	ret

.globl lg2
lg2:
	fldlg2
	subl	$4, %esp
	fstps	(%esp)
	movl	(%esp), %eax
	addl	$4, %esp
	ret
