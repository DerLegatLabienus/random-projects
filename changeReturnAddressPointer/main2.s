	.file	"main2.c"
	.section	.rodata
.LC0:
	.string	"%d\n"
	.text
	.globl	function
	.type	function, @function
function:
.LFB0:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$104, %esp
	movl	%gs:20, %eax
	movl	%eax, -12(%ebp)
	xorl	%eax, %eax
#APP
# 6 "main2.c" 1
	lea 4(%ebp),%eax
# 0 "" 2
#NO_APP
	movl	%eax, -80(%ebp)
	movl	-80(%ebp), %edx
	leal	-76(%ebp), %eax
	subl	%eax, %edx
	movl	%edx, %eax
	movl	%eax, 4(%esp)
	movl	$.LC0, (%esp)
	call	printf
	movl	-80(%ebp), %edx
	leal	-76(%ebp), %eax
	subl	%eax, %edx
	movl	-80(%ebp), %ecx
	leal	-76(%ebp), %eax
	subl	%eax, %ecx
	movl	%ecx, %eax
	movzbl	-76(%ebp,%eax), %eax
	addl	$40, %eax
	movb	%al, -76(%ebp,%edx)
	movl	-12(%ebp), %eax
	xorl	%gs:20, %eax
	je	.L2
	call	__stack_chk_fail
.L2:
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE0:
	.size	function, .-function
	.section	.rodata
.LC1:
	.string	"x = %d\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB1:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	andl	$-16, %esp
	subl	$32, %esp
	movl	$7, 28(%esp)
	call	function
	movl	28(%esp), %eax
	addl	$1, %eax
	movl	%eax, 28(%esp)
	movl	28(%esp), %eax
	movl	%eax, 4(%esp)
	movl	$.LC1, (%esp)
	call	printf
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE1:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 4.8.4-2ubuntu1~14.04.1) 4.8.4"
	.section	.note.GNU-stack,"",@progbits
