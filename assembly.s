    .globl main
main:
    pushq %rbp
    mov %rsp, %rbp
    sub $8, %rsp
    mov $0, %rax
    mov %rax, -8(%rbp)
    mov $0, %rax
    pushq %rax
    mov $0, %rax
    pushq %rax
    pop %rcx
    pop %rax
    cmp %rax, %rcx
    mov $0, %rax
    sete %al
    pushq %rax
    pop %rax
    cmp $0, %rax
    je _else_1
    mov $6, %rax
    pushq %rax
    pop %rax
    mov %rax, -8(%rbp)
    je _if_end_1
_else_1:
    mov $7, %rax
    pushq %rax
    pop %rax
    mov %rax, -8(%rbp)
_if_end_1:
    mov -8(%rbp), %rax
    pushq %rax
    pop %rax
    mov %rbp, %rsp
    pop %rbp
    ret
