    .globl main
main:
    pushq %rbp
    mov %rsp, %rbp
    sub $32, %rsp
    mov $0, %rax
    mov %rax, -8(%rbp)
    mov $6, %rax
    pushq %rax
    pop %rax
    pushq %rax
    mov %rax, -8(%rbp)
    pop %rax
    mov $7, %rax
    pushq %rax
    pop %rax
    mov %rax, -16(%rbp)
    mov $1, %rax
    pushq %rax
    pop %rax
    mov %rax, -24(%rbp)
    mov -16(%rbp), %rax
    pushq %rax
    mov -8(%rbp), %rax
    pushq %rax
    pop %rcx
    pop %rax
    cmp %rcx, %rax
    mov $0, %rax
    setl %al
    pushq %rax
    pop %rax
    mov %rax, -32(%rbp)
    mov -32(%rbp), %rax
    pushq %rax
    mov -24(%rbp), %rax
    pushq %rax
    pop %rcx
    pop %rax
    cmp $0, %rax
    pushq %rax
    je _skip_and_1
    pop %rax
    pushq %rcx
_skip_and_1:
    pop %rax
    mov %rbp, %rsp
    pop %rbp
    ret
