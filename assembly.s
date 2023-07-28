    .globl main
main:
    pushq %rbp
    mov %rsp, %rbp
    sub $16, %rsp
    sub $16, %rsp
    sub $16, %rbp
    mov $2, %rax
    pushq %rax
    pop %rax
    mov %rax, -8(%rbp)
    mov $3, %rax
    pushq %rax
    pop %rax
    mov %rax, -16(%rbp)
    mov $1, %rax
    pushq %rax
    pop %rax
    cmp $0, %rax
    je _else_1
    sub $16, %rsp
    sub $16, %rbp
    mov 8(%rbp), %rax
    mov %rax, -8(%rbp)
    mov 0(%rbp), %rax
    mov %rax, -16(%rbp)
    mov $1, %rax
    pushq %rax
    pop %rax
    pushq %rax
    mov %rax, -8(%rbp)
    pop %rax
    mov %rax, -8(%rbp)
    mov -8(%rbp), %rax
    mov %rax, 8(%rbp)
    mov -16(%rbp), %rax
    mov %rax, 0(%rbp)
    add $16, %rsp
    add $16, %rbp
_else_1:
    mov -8(%rbp), %rax
    pushq %rax
    pop %rax
    add $16, %rsp
    add $16, %rbp
    mov %rbp, %rsp
    pop %rbp
    ret
