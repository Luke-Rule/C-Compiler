    .globl main
main:
    pushq %rbp
    mov %rsp, %rbp
    sub $8, %rsp
    sub $8, %rsp
    sub $8, %rbp
    mov $0, %rax
    pushq %rax
    pop %rax
    mov %rax, -8(%rbp)
_while_condition_1:
    mov -8(%rbp), %rax
    pushq %rax
    mov $5, %rax
    pushq %rax
    pop %rcx
    pop %rax
    cmp %rcx, %rax
    mov $0, %rax
    setl %al
    pushq %rax
    pop %rax
    cmp $0, %rax
    je _while_end_1
    mov -8(%rbp), %rax
    pushq %rax
    mov $2, %rax
    pushq %rax
    pop %rax
    pop %rcx
    add %rcx, %rax
    pushq %rax
