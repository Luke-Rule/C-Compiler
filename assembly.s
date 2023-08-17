    .globl main
main:
    pushq %rbp
    mov %rsp, %rbp
    sub $8, %rsp
    sub $8, %rsp
    sub $8, %rbp
    mov $9000, %rax
    pushq %rax
    pop %rax
    neg %rax
    pushq %rax
    pop %rcx
    call abs
    add $0, %rsp
    pushq %rax
    pop %rax
    add $8, %rsp
    add $8, %rbp
    mov %rbp, %rsp
    pop %rbp
    ret
