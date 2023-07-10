    .globl main
main:
    mov $6, %rax
    pushq %rax
    mov $3, %rax
    pushq %rax
    pop %rax
    neg %rax
    pushq %rax
    pop %rbx
    pop %rax
    mov %eax, %eax
    cdq
    idiv %ebx
    pushq %rax
    pop %rax
    ret
