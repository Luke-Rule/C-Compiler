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
_do_while_condition_1:
    sub $8, %rsp
    sub $8, %rbp
    mov 0(%rbp), %rax
    mov %rax, -8(%rbp)
