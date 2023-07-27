    .globl main
main:
    pushq %rbp
    mov %rsp, %rbp
    sub $8, %rsp
    sub $8, %rsp
    sub $8, %rbp
    mov $3, %rax
    pushq %rax
    pop %rax
    mov %rax, -8(%rbp)
    mov $0, %rax
    pushq %rax
    pop %rax
    cmp $0, %rax
    je _else_1
    sub $8, %rsp
    sub $8, %rbp
    mov 0(%rbp), %rax
    mov %rax, -8(%rbp)
    mov $5, %rax
    pushq %rax
    pop %rax
    mov %rax, -8(%rbp)
    mov -8(%rbp), %rax
    mov %rax, 0(%rbp)
    add $8, %rsp
    add $8, %rbp
    jmp _if_end_1
_else_1:
    sub $8, %rsp
    sub $8, %rbp
    mov 0(%rbp), %rax
    mov %rax, -8(%rbp)
    mov $0, %rax
    pushq %rax
    pop %rax
    cmp $0, %rax
    je _else_11
    sub $8, %rsp
    sub $8, %rbp
    mov 0(%rbp), %rax
    mov %rax, -8(%rbp)
    mov $4, %rax
    pushq %rax
    pop %rax
    mov %rax, -8(%rbp)
    mov -8(%rbp), %rax
    mov %rax, 0(%rbp)
    add $8, %rsp
    add $8, %rbp
    jmp _if_end_11
_else_11:
    sub $8, %rsp
    sub $8, %rbp
    mov 0(%rbp), %rax
    mov %rax, -8(%rbp)
    mov $1, %rax
    pushq %rax
    pop %rax
    cmp $0, %rax
    je _else_111
    sub $8, %rsp
    sub $8, %rbp
    mov 0(%rbp), %rax
    mov %rax, -8(%rbp)
    mov $4, %rax
    pushq %rax
    pop %rax
    mov %rax, -8(%rbp)
    mov -8(%rbp), %rax
    mov %rax, 0(%rbp)
    add $8, %rsp
    add $8, %rbp
    jmp _if_end_111
_else_111:
    sub $8, %rsp
    sub $8, %rbp
    mov 0(%rbp), %rax
    mov %rax, -8(%rbp)
    mov $1, %rax
    pushq %rax
    pop %rax
    mov %rax, -8(%rbp)
    mov -8(%rbp), %rax
    mov %rax, 0(%rbp)
    add $8, %rsp
    add $8, %rbp
_if_end_111:
    mov -8(%rbp), %rax
    mov %rax, 0(%rbp)
    add $8, %rsp
    add $8, %rbp
_if_end_11:
    mov -8(%rbp), %rax
    mov %rax, 0(%rbp)
    add $8, %rsp
    add $8, %rbp
_if_end_1:
    mov -8(%rbp), %rax
    pushq %rax
    pop %rax
    add $8, %rsp
    add $8, %rbp
    mov %rbp, %rsp
    pop %rbp
    ret
