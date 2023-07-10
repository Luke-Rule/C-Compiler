    .globl main
main:
    mov $2, %rax
    pushq %rax
    mov $5, %rax
    pushq %rax
    pop %rax
    pop %rcx
    imul %rcx, %rax
    pushq %rax
    mov $4, %rax
    pushq %rax
    pop %rax
    neg %rax
    pushq %rax
    pop %rax
    pop %rcx
    add %rcx, %rax
    pushq %rax
    mov $6, %rax
    pushq %rax
    pop %rax
    pop %rcx
    imul %rcx, %rax
    pushq %rax
    pop %rax
    ret
