	.file	"file.c"
	.text
	.def	__main;	.scl	2;	.type	32;	.endef
	.globl	main
	.def	main;	.scl	2;	.type	32;	.endef
	.seh_proc	main
main:
	pushq	%rbp
	.seh_pushreg	%rbp
	movq	%rsp, %rbp
	.seh_setframe	%rbp, 0
	subq	$64, %rsp
	.seh_stackalloc	64
	.seh_endprologue
	call	__main
	movl	$6, 48(%rsp)
	movl	$5, 40(%rsp)
	movl	$4, 32(%rsp)
	movl	$3, %r9d
	movl	$2, %r8d
	movl	$1, %edx
	movl	$72, %ecx
	call	a
	movl	$0, %eax
	addq	$64, %rsp
	popq	%rbp
	ret
	.seh_endproc
	.ident	"GCC: (x86_64-posix-seh-rev0, Built by MinGW-W64 project) 8.1.0"
	.def	a;	.scl	2;	.type	32;	.endef
