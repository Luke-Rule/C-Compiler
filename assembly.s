.data
.globl a
    a:
        .long 0
.bss
    b:
        .zero 8
.text
.globl fib
    fib:
        pushq %rbp
        mov %rsp, %rbp
        sub $24, %rsp
        sub $24, %rsp
        sub $24, %rbp
        mov %rcx, -8(%rbp)
        mov -8(%rbp), %rax
        pushq %rax
        mov $0, %rax
        pushq %rax
        pop %r10
        pop %rax
        cmp %rax, %r10
        mov $0, %rax
        sete %al
        cmp $0, %rax
        mov $0, %rax
        sete %al
        cmp $0, %rax
        mov $0, %rax
        sete %al
        pushq %rax
        cmp $0, %rax
        jne _skip_or_1
        mov -8(%rbp), %rax
        pushq %rax
        mov $1, %rax
        pushq %rax
        pop %r10
        pop %rax
        cmp %rax, %r10
        mov $0, %rax
        sete %al
        pop %r10
        cmp $0, %rax
        mov $0, %rax
        sete %al
        cmp $0, %rax
        mov $0, %rax
        sete %al
        pushq %rax
    _skip_or_1:
        pop %rax
        cmp $0, %rax
        je _else_1
        sub $24, %rsp
        sub $24, %rbp
        mov 16(%rbp), %rax
        mov %rax, -8(%rbp)
        add $24, %rsp
        add $24, %rbp
        add $24, %rsp
        add $24, %rbp
        mov %rbp, %rsp
        pop %rbp
        ret
        jmp _if_end_1
    _else_1:
        sub $24, %rsp
        sub $24, %rbp
        mov 16(%rbp), %rax
        mov %rax, -8(%rbp)
        pushq %rax
        mov $1, %rax
        pushq %rax
        pop %r10
        pop %rax
        sub %r10, %rax
        pushq %rax
        pop %rcx
        call fib
        pushq %rax
        mov -8(%rbp), %rax
        pushq %rax
        mov $2, %rax
        pushq %rax
        pop %r10
        pop %rax
        sub %r10, %rax
        pushq %rax
        pop %rcx
        call fib
        pop %r10
        add %r10, %rax
        add $24, %rsp
        add $24, %rbp
        add $24, %rsp
        add $24, %rbp
        mov %rbp, %rsp
        pop %rbp
        ret
    _if_end_1:
        add $24, %rsp
        add $24, %rbp

.globl main
    main:
        pushq %rbp
        mov %rsp, %rbp
        sub $24, %rsp
        sub $24, %rsp
        sub $24, %rbp
        mov $10, %rax
        mov %rax, -8(%rbp)
        pushq %rax
        pop %rcx
        call fib
        add $24, %rsp
        add $24, %rbp
        mov %rbp, %rsp
        pop %rbp
        ret
