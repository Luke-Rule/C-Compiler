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
    sub $8, %rsp
    sub $8, %rbp
    mov 0(%rbp), %rax
    mov %rax, -8(%rbp)
    mov -8(%rbp), %rax
    pushq %rax
    mov $1, %rax
    pushq %rax
    pop %rax
    pop %rcx
    add %rcx, %rax
    pushq %rax
    pop %rax
    mov %rax, -8(%rbp)
    mov %rax, 0(%rbp)
    mov $0, %rax
    pushq %rax
    pop %rax
    add $8, %rsp
    add $8, %rbp
    add $8, %rsp
    add $8, %rbp
    mov %rbp, %rsp
    pop %rbp
    ret
    jmp _while_condition_1
    _while_skip_1:
    add $8, %rsp
    add $8, %rbp
jmp _while_condition_1
    _while_break_1:
    add $8, %rsp
    add $8, %rbp
_while_end_1:
    mov -8(%rbp), %rax
    pushq %rax
    pop %rax
    add $8, %rsp
    add $8, %rbp
    mov %rbp, %rsp
    pop %rbp
    ret
