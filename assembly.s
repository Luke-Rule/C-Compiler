    .globl main
main:
    pushq %rbp
    mov %rsp, %rbp
    sub $16, %rsp
    sub $16, %rsp
    sub $16, %rbp
    mov $0, %rax
    pushq %rax
    pop %rax
    mov %rax, -8(%rbp)
    sub $16, %rsp
    sub $16, %rbp
    mov 8(%rbp), %rax
    mov %rax, -8(%rbp)
    mov $0, %rax
    pushq %rax
    pop %rax
    mov %rax, -16(%rbp)
_for_condition_10:
    mov -16(%rbp), %rax
    pushq %rax
    mov $10, %rax
    pushq %rax
    pop %rcx
    pop %rax
    cmp %rcx, %rax
    mov $0, %rax
    setl %al
    pushq %rax
    pop %rax
    cmp $0, %rax
    je _for_end_10
    sub $16, %rsp
    sub $16, %rbp
    mov 8(%rbp), %rax
    mov %rax, -8(%rbp)
    mov 0(%rbp), %rax
    mov %rax, -16(%rbp)
    mov -16(%rbp), %rax
    pushq %rax
    mov $1, %rax
    pushq %rax
    pop %rax
    pop %rcx
    add %rcx, %rax
    pushq %rax
    pop %rax
    mov %rax, -16(%rbp)
    mov %rax, 0(%rbp)
    add $16, %rsp
    add $16, %rbp
jmp _for_skip_end_10
_for_skip_10:
    add $16, %rsp
    add $16, %rbp
_for_skip_end_10:
    mov -16(%rbp), %rax
    pushq %rax
    mov $1, %rax
    pushq %rax
    pop %rax
    pop %rcx
    add %rcx, %rax
    pushq %rax
    pop %rax
    mov %rax, -16(%rbp)
    jmp _for_condition_10
    _for_break_10:
    add $16, %rsp
    add $16, %rbp
_for_end_10:
    add $16, %rsp
    add $16, %rbp
    mov -8(%rbp), %rax
    pushq %rax
    pop %rax
    add $16, %rsp
    add $16, %rbp
    mov %rbp, %rsp
    pop %rbp
    ret
