    .globl main
main:
    pushq %rbp
    mov %rsp, %rbp
    sub $0, %rsp
    mov $0, %rax
    pushq %rax
    pop %rax
    pushq %rax
    cmp $1, %rax
    je _skip_or_1
    mov $0, %rax
    pushq %rax
    pop %rcx
    pop %rax
    pushq %rcx
_skip_or_1:
    pop %rax
    mov %rbp, %rsp
    pop %rbp
    ret
