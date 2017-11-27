	.file	"main.c"
	.section	.rodata
.LC0:
	.string	"/msgmsg2"
	.data
	.align 4
	.type	mqueue_name, @object
	.size	mqueue_name, 4
mqueue_name:
	.long	.LC0
	.section	.rodata
.LC1:
	.string	"/tmp/map_conf"
	.data
	.align 4
	.type	conf_mapping_file, @object
	.size	conf_mapping_file, 4
conf_mapping_file:
	.long	.LC1
	.local	msg_id
	.comm	msg_id,4,4
	.comm	m,24,4
	.section	.rodata
.LC2:
	.string	"%s received invalid input\n"
.LC3:
	.string	"successfull connection"
.LC4:
	.string	"%s\n"
	.align 4
.LC5:
	.string	"sending data failed with errno %d\n"
.LC6:
	.string	"BYE"
.LC7:
	.string	"thread %d or %d died\n"
	.text
	.globl	handle_client_thread_function
	.type	handle_client_thread_function, @function
handle_client_thread_function:
.LFB2:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	pushl	%edi
	pushl	%esi
	pushl	%ebx
	subl	$204, %esp
	.cfi_offset 7, -12
	.cfi_offset 6, -16
	.cfi_offset 3, -20
	movl	8(%ebp), %eax
	movl	%eax, -188(%ebp)
	movl	%gs:20, %eax
	movl	%eax, -28(%ebp)
	xorl	%eax, %eax
	movl	$0, -180(%ebp)
	movl	$224, (%esp)
	call	syscall
	movl	%eax, -176(%ebp)
	call	pthread_self
	movl	%eax, -172(%ebp)
	movl	-188(%ebp), %eax
	movl	%eax, -168(%ebp)
	cmpl	$0, -168(%ebp)
	jne	.L2
	movl	stderr, %eax
	movl	$__func__.4627, 8(%esp)
	movl	$.LC2, 4(%esp)
	movl	%eax, (%esp)
	call	fprintf
	movl	$0, %eax
	jmp	.L12
.L2:
	movl	$0, -184(%ebp)
	jmp	.L4
.L11:
	movl	$.LC3, (%esp)
	call	puts
	leal	-138(%ebp), %edx
	movl	$50, %ebx
	movl	$0, %eax
	movl	%edx, %ecx
	andl	$2, %ecx
	testl	%ecx, %ecx
	je	.L5
	movw	%ax, (%edx)
	addl	$2, %edx
	subl	$2, %ebx
.L5:
	movl	%ebx, %ecx
	shrl	$2, %ecx
	movl	%edx, %edi
	rep stosl
	movl	%edi, %edx
	movl	%ebx, %ecx
	andl	$2, %ecx
	testl	%ecx, %ecx
	je	.L6
	movw	%ax, (%edx)
	addl	$2, %edx
.L6:
	andl	$1, %ebx
	movl	%ebx, %ecx
	testl	%ecx, %ecx
	je	.L7
	movb	%al, (%edx)
	addl	$1, %edx
.L7:
	movl	$0, 12(%esp)
	movl	$50, 8(%esp)
	leal	-138(%ebp), %eax
	movl	%eax, 4(%esp)
	movl	-180(%ebp), %eax
	movl	%eax, (%esp)
	call	recv
	movl	%eax, -164(%ebp)
	movl	$m, (%esp)
	call	pthread_mutex_lock
	leal	-88(%ebp), %ebx
	movl	$0, %eax
	movl	$15, %edx
	movl	%ebx, %edi
	movl	%edx, %ecx
	rep stosl
	movl	msg_id, %eax
	leal	1(%eax), %edx
	movl	%edx, msg_id
	movl	%eax, -88(%ebp)
	call	getppid
	movl	%eax, -84(%ebp)
	movl	$50, 8(%esp)
	leal	-138(%ebp), %eax
	movl	%eax, 4(%esp)
	leal	-88(%ebp), %eax
	addl	$8, %eax
	movl	%eax, (%esp)
	call	strncpy
	movl	-168(%ebp), %eax
	movl	4(%eax), %eax
	movl	$1, 12(%esp)
	movl	$60, 8(%esp)
	leal	-88(%ebp), %edx
	movl	%edx, 4(%esp)
	movl	%eax, (%esp)
	call	mq_send
	movl	%eax, -160(%ebp)
	cmpl	$-1, -160(%ebp)
	jne	.L8
	call	__errno_location
	movl	(%eax), %eax
	movl	%eax, (%esp)
	call	strerror
	movl	stderr, %edx
	movl	%eax, 8(%esp)
	movl	$.LC4, 4(%esp)
	movl	%edx, (%esp)
	call	fprintf
	call	__errno_location
	movl	(%eax), %eax
	movl	%eax, 4(%esp)
	movl	$.LC5, (%esp)
	call	printf
.L8:
	movl	$m, (%esp)
	call	pthread_mutex_unlock
	movl	-180(%ebp), %eax
	movl	%eax, (%esp)
	call	close
	movl	$3, 8(%esp)
	movl	$.LC6, 4(%esp)
	leal	-88(%ebp), %eax
	addl	$8, %eax
	movl	%eax, (%esp)
	call	strncmp
	testl	%eax, %eax
	je	.L10
.L4:
	movl	-168(%ebp), %eax
	movl	(%eax), %eax
	leal	-184(%ebp), %edx
	movl	%edx, 8(%esp)
	leal	-154(%ebp), %edx
	movl	%edx, 4(%esp)
	movl	%eax, (%esp)
	call	accept
	movl	%eax, -180(%ebp)
	cmpl	$0, -180(%ebp)
	jg	.L11
.L10:
	movl	-172(%ebp), %eax
	movl	%eax, 8(%esp)
	movl	-176(%ebp), %eax
	movl	%eax, 4(%esp)
	movl	$.LC7, (%esp)
	call	printf
	movl	$0, %eax
.L12:
	movl	-28(%ebp), %esi
	xorl	%gs:20, %esi
	je	.L13
	call	__stack_chk_fail
.L13:
	addl	$204, %esp
	popl	%ebx
	.cfi_restore 3
	popl	%esi
	.cfi_restore 6
	popl	%edi
	.cfi_restore 7
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE2:
	.size	handle_client_thread_function, .-handle_client_thread_function
	.section	.rodata
.LC8:
	.string	" inet_pton failed \n "
	.align 4
.LC9:
	.string	"unable to set up socket with protcol %d\n"
	.align 4
.LC10:
	.string	"bind failed: %s on ip: %s and port %d\n"
.LC11:
	.string	"%s %d\n"
.LC12:
	.string	"somehting"
	.text
	.globl	receive_http_requests
	.type	receive_http_requests, @function
receive_http_requests:
.LFB3:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	pushl	%ebx
	subl	$116, %esp
	.cfi_offset 3, -12
	movl	12(%ebp), %eax
	movl	%eax, -92(%ebp)
	movl	20(%ebp), %eax
	movl	%eax, -96(%ebp)
	movl	%gs:20, %eax
	movl	%eax, -12(%ebp)
	xorl	%eax, %eax
	movl	$16, 8(%esp)
	movl	$0, 4(%esp)
	leal	-28(%ebp), %eax
	movl	%eax, (%esp)
	call	memset
	movw	$2, -28(%ebp)
	cmpl	$0, -96(%ebp)
	je	.L15
	movl	$4, 8(%esp)
	movl	$0, 4(%esp)
	leal	-72(%ebp), %eax
	movl	%eax, (%esp)
	call	memset
	movzwl	-28(%ebp), %eax
	movzwl	%ax, %eax
	leal	-72(%ebp), %edx
	movl	%edx, 8(%esp)
	movl	-96(%ebp), %edx
	movl	%edx, 4(%esp)
	movl	%eax, (%esp)
	call	inet_pton
	cmpl	$1, %eax
	je	.L17
	movl	stderr, %eax
	movl	%eax, 12(%esp)
	movl	$20, 8(%esp)
	movl	$1, 4(%esp)
	movl	$.LC8, (%esp)
	call	fwrite
	jmp	.L17
.L15:
	movl	$2130706433, -72(%ebp)
	movl	-72(%ebp), %eax
	movl	%eax, (%esp)
	call	inet_ntoa
	movl	%eax, -96(%ebp)
.L17:
	movl	-72(%ebp), %eax
	movl	%eax, -24(%ebp)
	movl	16(%ebp), %eax
	sall	$2, %eax
	movl	%eax, (%esp)
	call	malloc
	movl	%eax, -48(%ebp)
	movl	$0, -64(%ebp)
	jmp	.L18
.L24:
	movl	-64(%ebp), %eax
	leal	0(,%eax,4), %edx
	movl	-48(%ebp), %eax
	leal	(%edx,%eax), %ebx
	movl	$0, 8(%esp)
	movl	$1, 4(%esp)
	movl	$2, (%esp)
	call	socket
	movl	%eax, (%ebx)
	movl	-64(%ebp), %eax
	leal	0(,%eax,4), %edx
	movl	-48(%ebp), %eax
	addl	%edx, %eax
	movl	(%eax), %eax
	cmpl	$-1, %eax
	jne	.L19
	movl	stderr, %eax
	movl	8(%ebp), %edx
	movl	%edx, 8(%esp)
	movl	$.LC9, 4(%esp)
	movl	%eax, (%esp)
	call	fprintf
	nop
	movl	-12(%ebp), %eax
	xorl	%gs:20, %eax
	je	.L33
	jmp	.L34
.L19:
	movl	-64(%ebp), %eax
	leal	(%eax,%eax), %edx
	movl	-92(%ebp), %eax
	addl	%edx, %eax
	movzwl	(%eax), %eax
	movzwl	%ax, %eax
	movl	%eax, (%esp)
	call	htons
	movw	%ax, -26(%ebp)
	movl	-64(%ebp), %eax
	leal	0(,%eax,4), %edx
	movl	-48(%ebp), %eax
	addl	%edx, %eax
	movl	(%eax), %eax
	movl	$16, 8(%esp)
	leal	-28(%ebp), %edx
	movl	%edx, 4(%esp)
	movl	%eax, (%esp)
	call	bind
	cmpl	$-1, %eax
	jne	.L21
	movzwl	-26(%ebp), %eax
	movzwl	%ax, %eax
	movl	%eax, (%esp)
	call	ntohs
	movzwl	%ax, %ebx
	call	__errno_location
	movl	(%eax), %eax
	movl	%eax, (%esp)
	call	strerror
	movl	stderr, %edx
	movl	%ebx, 16(%esp)
	movl	-96(%ebp), %ecx
	movl	%ecx, 12(%esp)
	movl	%eax, 8(%esp)
	movl	$.LC10, 4(%esp)
	movl	%edx, (%esp)
	call	fprintf
.L21:
	movl	-64(%ebp), %eax
	leal	0(,%eax,4), %edx
	movl	-48(%ebp), %eax
	addl	%edx, %eax
	movl	(%eax), %eax
	movl	$100, 4(%esp)
	movl	%eax, (%esp)
	call	listen
	cmpl	$-1, %eax
	jne	.L22
	call	__errno_location
	movl	(%eax), %eax
	cmpl	$98, %eax
	jne	.L23
	call	__errno_location
	movl	(%eax), %ebx
	call	__errno_location
	movl	(%eax), %eax
	movl	%eax, (%esp)
	call	strerror
	movl	stderr, %edx
	movl	%ebx, 12(%esp)
	movl	%eax, 8(%esp)
	movl	$.LC11, 4(%esp)
	movl	%edx, (%esp)
	call	fprintf
	jmp	.L22
.L23:
	call	__errno_location
	movl	(%eax), %eax
	cmpl	$95, %eax
	jne	.L22
	movl	$.LC12, (%esp)
	call	printf
.L22:
	addl	$1, -64(%ebp)
.L18:
	movl	-64(%ebp), %eax
	cmpl	16(%ebp), %eax
	jl	.L24
	movl	16(%ebp), %eax
	movl	%eax, -44(%ebp)
	movw	$3, -76(%ebp)
	movw	$56, -74(%ebp)
	movl	-44(%ebp), %eax
	movl	$8, 4(%esp)
	movl	%eax, (%esp)
	call	calloc
	movl	%eax, -40(%ebp)
	movl	$0, -60(%ebp)
	jmp	.L25
.L26:
	movl	-60(%ebp), %eax
	leal	0(,%eax,8), %edx
	movl	-40(%ebp), %eax
	addl	%eax, %edx
	movzwl	-76(%ebp), %eax
	movw	%ax, 4(%edx)
	movl	-60(%ebp), %eax
	leal	0(,%eax,8), %edx
	movl	-40(%ebp), %eax
	addl	%edx, %eax
	movw	$0, 6(%eax)
	movl	-60(%ebp), %eax
	leal	0(,%eax,8), %edx
	movl	-40(%ebp), %eax
	addl	%eax, %edx
	movl	-60(%ebp), %eax
	leal	0(,%eax,4), %ecx
	movl	-48(%ebp), %eax
	addl	%ecx, %eax
	movl	(%eax), %eax
	movl	%eax, (%edx)
	addl	$1, -60(%ebp)
.L25:
	movl	-60(%ebp), %eax
	cmpl	-44(%ebp), %eax
	jl	.L26
	movl	-48(%ebp), %eax
	movl	%eax, (%esp)
	call	free
	movl	$0, -56(%ebp)
.L31:
	movl	-44(%ebp), %eax
	movl	$-1, 8(%esp)
	movl	%eax, 4(%esp)
	movl	-40(%ebp), %eax
	movl	%eax, (%esp)
	call	poll
	movl	%eax, -56(%ebp)
	cmpl	$0, -56(%ebp)
	je	.L27
	movl	$0, -52(%ebp)
	jmp	.L28
.L30:
	movl	-52(%ebp), %eax
	leal	0(,%eax,8), %edx
	movl	-40(%ebp), %eax
	addl	%edx, %eax
	movzwl	6(%eax), %eax
	andw	-76(%ebp), %ax
	testw	%ax, %ax
	je	.L29
	movl	-52(%ebp), %eax
	leal	0(,%eax,8), %edx
	movl	-40(%ebp), %eax
	addl	%edx, %eax
	movl	(%eax), %eax
	movl	%eax, -36(%ebp)
	movl	24(%ebp), %eax
	movl	%eax, -32(%ebp)
	leal	-36(%ebp), %eax
	movl	%eax, 12(%esp)
	movl	$handle_client_thread_function, 8(%esp)
	movl	$0, 4(%esp)
	leal	-68(%ebp), %eax
	movl	%eax, (%esp)
	call	pthread_create
	movl	-52(%ebp), %eax
	leal	0(,%eax,8), %edx
	movl	-40(%ebp), %eax
	addl	%edx, %eax
	movw	$0, 6(%eax)
	subl	$1, -56(%ebp)
.L29:
	addl	$1, -52(%ebp)
.L28:
	movl	-52(%ebp), %eax
	cmpl	-44(%ebp), %eax
	jge	.L27
	cmpl	$0, -56(%ebp)
	jg	.L30
.L27:
	jmp	.L31
.L34:
	call	__stack_chk_fail
.L33:
	addl	$116, %esp
	popl	%ebx
	.cfi_restore 3
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE3:
	.size	receive_http_requests, .-receive_http_requests
	.section	.rodata
.LC13:
	.string	"__func__ failed: %d and %s\n"
.LC14:
	.string	"pid %d stdout,error %d\n"
.LC15:
	.string	"out by signal %s\n"
.LC16:
	.string	"stopped by signal %s\n"
	.text
	.globl	handle_dead_children
	.type	handle_dead_children, @function
handle_dead_children:
.LFB4:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	pushl	%ebx
	subl	$52, %esp
	.cfi_offset 3, -12
.L41:
	leal	-32(%ebp), %eax
	movl	%eax, (%esp)
	call	wait
	movl	%eax, -12(%ebp)
	cmpl	$-1, -12(%ebp)
	jne	.L36
	call	__errno_location
	movl	(%eax), %eax
	cmpl	$10, %eax
	jne	.L37
	movl	$0, %eax
	jmp	.L42
.L37:
	call	__errno_location
	movl	(%eax), %eax
	movl	%eax, (%esp)
	call	strerror
	movl	%eax, %ebx
	call	__errno_location
	movl	(%eax), %eax
	movl	%ebx, 8(%esp)
	movl	%eax, 4(%esp)
	movl	$.LC13, (%esp)
	call	printf
.L36:
	movl	-32(%ebp), %eax
	movl	%eax, 8(%esp)
	movl	-12(%ebp), %eax
	movl	%eax, 4(%esp)
	movl	$.LC14, (%esp)
	call	printf
	movl	-32(%ebp), %eax
	movl	%eax, -28(%ebp)
	movl	-28(%ebp), %eax
	andl	$127, %eax
	addl	$1, %eax
	sarb	%al
	testb	%al, %al
	jle	.L39
	movl	-32(%ebp), %eax
	movl	%eax, -24(%ebp)
	movl	-24(%ebp), %eax
	andl	$127, %eax
	movl	%eax, (%esp)
	call	strsignal
	movl	%eax, 4(%esp)
	movl	$.LC15, (%esp)
	call	printf
.L39:
	movl	-32(%ebp), %eax
	movl	%eax, -20(%ebp)
	movl	-20(%ebp), %eax
	movzbl	%al, %eax
	cmpl	$127, %eax
	jne	.L40
	movl	-32(%ebp), %eax
	movl	%eax, -16(%ebp)
	movl	-16(%ebp), %eax
	andl	$65280, %eax
	sarl	$8, %eax
	movl	%eax, (%esp)
	call	strsignal
	movl	%eax, 4(%esp)
	movl	$.LC16, (%esp)
	call	printf
	jmp	.L41
.L40:
	jmp	.L41
.L42:
	addl	$52, %esp
	popl	%ebx
	.cfi_restore 3
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE4:
	.size	handle_dead_children, .-handle_dead_children
	.section	.rodata
.LC17:
	.string	"PATH"
.LC18:
	.string	":"
.LC19:
	.string	":/home/aviva/clinuxtot/:."
.LC20:
	.string	"%s%s"
	.text
	.globl	printPath
	.type	printPath, @function
printPath:
.LFB5:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$40, %esp
	movl	$.LC17, (%esp)
	call	getenv
	movl	%eax, -16(%ebp)
	movl	$.LC18, 4(%esp)
	movl	-16(%ebp), %eax
	movl	%eax, (%esp)
	call	strtok
	movl	%eax, -24(%ebp)
	jmp	.L44
.L45:
	movl	$.LC18, 4(%esp)
	movl	$0, (%esp)
	call	strtok
	movl	%eax, -24(%ebp)
.L44:
	cmpl	$0, -24(%ebp)
	jne	.L45
	movl	environ, %eax
	movl	%eax, -20(%ebp)
	jmp	.L46
.L49:
	movl	-20(%ebp), %eax
	movl	(%eax), %eax
	movl	$4, 8(%esp)
	movl	$.LC17, 4(%esp)
	movl	%eax, (%esp)
	call	strncmp
	testl	%eax, %eax
	jne	.L47
	movl	-20(%ebp), %eax
	movl	(%eax), %eax
	movl	%eax, (%esp)
	call	strlen
	addl	$26, %eax
	movl	%eax, (%esp)
	call	malloc
	movl	%eax, -12(%ebp)
	movl	-20(%ebp), %eax
	movl	(%eax), %eax
	movl	$.LC19, 12(%esp)
	movl	%eax, 8(%esp)
	movl	$.LC20, 4(%esp)
	movl	-12(%ebp), %eax
	movl	%eax, (%esp)
	call	sprintf
	movl	-20(%ebp), %eax
	movl	-12(%ebp), %edx
	movl	%edx, (%eax)
.L47:
	addl	$4, -20(%ebp)
.L46:
	cmpl	$0, -20(%ebp)
	je	.L43
	movl	-20(%ebp), %eax
	movl	(%eax), %eax
	testl	%eax, %eax
	jne	.L49
.L43:
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE5:
	.size	printPath, .-printPath
	.section	.rodata
.LC21:
	.string	"client"
	.align 4
.LC22:
	.string	"client failed to instantiate: %d,%s\n"
	.text
	.globl	proc_child_process
	.type	proc_child_process, @function
proc_child_process:
.LFB6:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	pushl	%ebx
	subl	$20, %esp
	.cfi_offset 3, -12
	call	printPath
	movl	8(%ebp), %eax
	movl	%eax, (%esp)
	call	mq_close
	movl	$0, 8(%esp)
	movl	$.LC21, 4(%esp)
	movl	$.LC21, (%esp)
	call	execlp
	call	__errno_location
	movl	(%eax), %eax
	movl	%eax, (%esp)
	call	strerror
	movl	%eax, %ebx
	call	__errno_location
	movl	(%eax), %edx
	movl	stderr, %eax
	movl	%ebx, 12(%esp)
	movl	%edx, 8(%esp)
	movl	$.LC22, 4(%esp)
	movl	%eax, (%esp)
	call	fprintf
	movl	$1, (%esp)
	call	exit
	.cfi_endproc
.LFE6:
	.size	proc_child_process, .-proc_child_process
	.globl	setup_reader_units
	.type	setup_reader_units, @function
setup_reader_units:
.LFB7:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$40, %esp
	movl	8(%ebp), %eax
	sall	$2, %eax
	movl	%eax, (%esp)
	call	malloc
	movl	%eax, %edx
	movl	16(%ebp), %eax
	movl	%edx, (%eax)
	movl	$0, -12(%ebp)
	jmp	.L52
.L56:
	call	fork
	movl	%eax, -16(%ebp)
	cmpl	$0, -16(%ebp)
	jne	.L53
	movl	12(%ebp), %eax
	movl	%eax, (%esp)
	call	proc_child_process
	jmp	.L52
.L53:
	cmpl	$-1, -16(%ebp)
	jne	.L54
	call	__errno_location
	movl	(%eax), %eax
	movl	%eax, (%esp)
	call	strerror
	movl	stderr, %edx
	movl	%eax, 8(%esp)
	movl	$.LC4, 4(%esp)
	movl	%edx, (%esp)
	call	fprintf
	movl	$-1, (%esp)
	call	exit
.L54:
	movl	16(%ebp), %eax
	movl	(%eax), %ecx
	movl	-12(%ebp), %eax
	leal	1(%eax), %edx
	movl	%edx, -12(%ebp)
	sall	$2, %eax
	leal	(%ecx,%eax), %edx
	movl	-16(%ebp), %eax
	movl	%eax, (%edx)
.L52:
	cmpl	$0, -16(%ebp)
	je	.L51
	movl	-12(%ebp), %eax
	cmpl	8(%ebp), %eax
	jl	.L56
.L51:
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE7:
	.size	setup_reader_units, .-setup_reader_units
	.section	.rodata
.LC23:
	.string	"handle children thread is up!"
	.text
	.globl	setup_child_sig_thread
	.type	setup_child_sig_thread, @function
setup_child_sig_thread:
.LFB8:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$24, %esp
	movl	$0, 12(%esp)
	movl	$handle_dead_children, 8(%esp)
	movl	$0, 4(%esp)
	movl	8(%ebp), %eax
	movl	%eax, (%esp)
	call	pthread_create
	testl	%eax, %eax
	jne	.L57
	movl	$.LC23, (%esp)
	call	puts
.L57:
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE8:
	.size	setup_child_sig_thread, .-setup_child_sig_thread
	.section	.rodata
.LC24:
	.string	"%s FAILED! %s %d\n"
.LC25:
	.string	"%s %ld %ld\n"
	.text
	.globl	setup_mq
	.type	setup_mq, @function
setup_mq:
.LFB9:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	pushl	%ebx
	subl	$36, %esp
	.cfi_offset 3, -12
	movl	8(%ebp), %eax
	movl	$60, 8(%eax)
	movl	8(%ebp), %eax
	movl	$10, 4(%eax)
	movl	mqueue_name, %eax
	movl	8(%ebp), %edx
	movl	%edx, 12(%esp)
	movl	$384, 8(%esp)
	movl	$66, 4(%esp)
	movl	%eax, (%esp)
	call	mq_open
	movl	12(%ebp), %edx
	movl	%eax, (%edx)
	movl	12(%ebp), %eax
	movl	(%eax), %eax
	cmpl	$-1, %eax
	jne	.L59
	call	__errno_location
	movl	(%eax), %ebx
	call	__errno_location
	movl	(%eax), %eax
	movl	%eax, (%esp)
	call	strerror
	movl	stderr, %edx
	movl	%ebx, 16(%esp)
	movl	%eax, 12(%esp)
	movl	$__func__.4721, 8(%esp)
	movl	$.LC24, 4(%esp)
	movl	%edx, (%esp)
	call	fprintf
	movl	8(%ebp), %eax
	movl	4(%eax), %ecx
	movl	8(%ebp), %eax
	movl	8(%eax), %edx
	movl	stderr, %eax
	movl	%ecx, 16(%esp)
	movl	%edx, 12(%esp)
	movl	$__func__.4721, 8(%esp)
	movl	$.LC25, 4(%esp)
	movl	%eax, (%esp)
	call	fprintf
	movl	$-1, (%esp)
	call	exit
.L59:
	addl	$36, %esp
	popl	%ebx
	.cfi_restore 3
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE9:
	.size	setup_mq, .-setup_mq
	.section	.rodata
.LC26:
	.string	"127.0.0.1"
	.text
	.globl	execute_server
	.type	execute_server, @function
execute_server:
.LFB10:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$56, %esp
	movl	%gs:20, %eax
	movl	%eax, -12(%ebp)
	xorl	%eax, %eax
	movl	$3, -16(%ebp)
	leal	-16(%ebp), %eax
	movl	%eax, 4(%esp)
	movl	$m, (%esp)
	call	pthread_mutex_init
	movw	$8081, -22(%ebp)
	movw	$8080, -20(%ebp)
	movw	$8079, -18(%ebp)
	movl	8(%ebp), %eax
	movl	%eax, 16(%esp)
	movl	$.LC26, 12(%esp)
	movl	$3, 8(%esp)
	leal	-22(%ebp), %eax
	movl	%eax, 4(%esp)
	movl	$2, (%esp)
	call	receive_http_requests
	movl	-12(%ebp), %eax
	xorl	%gs:20, %eax
	je	.L62
	call	__stack_chk_fail
.L62:
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE10:
	.size	execute_server, .-execute_server
	.section	.rodata
.LC27:
	.string	"%d\n"
	.align 4
.LC28:
	.string	"thread joining failed with errno %d and %s\n"
.LC29:
	.string	"client with pid %d is kaputt\n"
	.text
	.globl	on_program_exit
	.type	on_program_exit, @function
on_program_exit:
.LFB11:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	pushl	%ebx
	subl	$36, %esp
	.cfi_offset 3, -12
	movl	$0, -16(%ebp)
	movl	8(%ebp), %eax
	movl	%eax, (%esp)
	call	pthread_cancel
	movl	%eax, -16(%ebp)
	cmpl	$0, -16(%ebp)
	je	.L64
	movl	-16(%ebp), %eax
	movl	%eax, 4(%esp)
	movl	$.LC27, (%esp)
	call	printf
	call	__errno_location
	movl	(%eax), %eax
	movl	%eax, (%esp)
	call	strerror
	movl	%eax, %ebx
	call	__errno_location
	movl	(%eax), %edx
	movl	stderr, %eax
	movl	%ebx, 12(%esp)
	movl	%edx, 8(%esp)
	movl	$.LC28, 4(%esp)
	movl	%eax, (%esp)
	call	fprintf
.L64:
	movl	12(%ebp), %eax
	movl	(%eax), %eax
	movl	%eax, -12(%ebp)
	movl	$0, -20(%ebp)
	jmp	.L65
.L66:
	movl	-20(%ebp), %eax
	leal	0(,%eax,4), %edx
	movl	-12(%ebp), %eax
	addl	%edx, %eax
	movl	(%eax), %eax
	movl	$15, 4(%esp)
	movl	%eax, (%esp)
	call	kill
	movl	-20(%ebp), %eax
	leal	0(,%eax,4), %edx
	movl	-12(%ebp), %eax
	addl	%edx, %eax
	movl	(%eax), %eax
	movl	%eax, 4(%esp)
	movl	$.LC29, (%esp)
	call	printf
	addl	$1, -20(%ebp)
.L65:
	movl	-20(%ebp), %eax
	cmpl	16(%ebp), %eax
	jl	.L66
	movl	12(%ebp), %eax
	movl	(%eax), %eax
	movl	%eax, (%esp)
	call	free
	addl	$36, %esp
	popl	%ebx
	.cfi_restore 3
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE11:
	.size	on_program_exit, .-on_program_exit
	.section	.rodata
.LC30:
	.string	"server_configure.json"
	.text
	.globl	main
	.type	main, @function
main:
.LFB12:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	andl	$-16, %esp
	subl	$64, %esp
	movl	$0, 28(%esp)
	movl	$.LC30, 4(%esp)
	leal	28(%esp), %eax
	movl	%eax, (%esp)
	call	build_config
	leal	20(%esp), %eax
	movl	%eax, 4(%esp)
	leal	32(%esp), %eax
	movl	%eax, (%esp)
	call	setup_mq
	movl	20(%esp), %eax
	leal	24(%esp), %edx
	movl	%edx, 8(%esp)
	movl	%eax, 4(%esp)
	movl	$5, (%esp)
	call	setup_reader_units
	leal	16(%esp), %eax
	movl	%eax, (%esp)
	call	setup_child_sig_thread
	movl	20(%esp), %eax
	movl	%eax, (%esp)
	call	execute_server
	movl	16(%esp), %eax
	movl	$2, 8(%esp)
	leal	24(%esp), %edx
	movl	%edx, 4(%esp)
	movl	%eax, (%esp)
	call	on_program_exit
	movl	$0, %eax
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE12:
	.size	main, .-main
	.section	.rodata
	.type	__func__.4627, @object
	.size	__func__.4627, 30
__func__.4627:
	.string	"handle_client_thread_function"
	.type	__func__.4721, @object
	.size	__func__.4721, 9
__func__.4721:
	.string	"setup_mq"
	.ident	"GCC: (Ubuntu 4.8.4-2ubuntu1~14.04.1) 4.8.4"
	.section	.note.GNU-stack,"",@progbits
