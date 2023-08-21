    .data
    .bss
    .text
    .globl main
    main:
    pushq %rbp
    mov %rsp, %rbp
    sub $8, %rsp
    sub $8, %rsp
    sub $8, %rbp
    mov $10, %rax
    pushq %rax
    pop %rcx
    call sum
    add $8, %rsp
    add $8, %rbp
    mov %rbp, %rsp
    pop %rbp
    ret
    .globl sum
    sum:
    pushq %rbp
    mov %rsp, %rbp
    sub $8, %rsp
    sub $8, %rsp
    sub $8, %rbp
    mov %rcx, -8(%rbp)
    mov -8(%rbp), %rax
    pushq %rax
    mov $0, %rax
    pushq %rax
    pop %r10
    pop %rax
    cmp %r10, %rax
    mov $0, %rax
    setg %al
    cmp $0, %rax
    je _else_1
    sub $8, %rsp
    sub $8, %rbp
    mov 0(%rbp), %rax
    mov %rax, -8(%rbp)
    mov -8(%rbp), %rax
    pushq %rax
    mov -8(%rbp), %rax
    pushq %rax
    mov $1, %rax
    pushq %rax
    pop %r10
    pop %rax
    sub %r10, %rax
    pushq %rax
    pop %rcx
    call sum
    pop %r10
    add %r10, %rax
    add $8, %rsp
    add $8, %rbp
    add $8, %rsp
    add $8, %rbp
    mov %rbp, %rsp
    pop %rbp
    ret
    jmp _if_end_1
_else_1:
    sub $8, %rsp
    sub $8, %rbp
    mov 0(%rbp), %rax
    mov %rax, -8(%rbp)
    mov $0, %rax
    add $8, %rsp
    add $8, %rbp
    add $8, %rsp
    add $8, %rbp
    mov %rbp, %rsp
    pop %rbp
    ret
_if_end_1:
    add $8, %rsp
    add $8, %rbp
