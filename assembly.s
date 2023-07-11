    .globl main
main:
    mov $0, %rax
    pushq %rax
    mov $4, %rax
    pushq %rax
    pop %rax
    neg %rax
    pushq %rax
    pop %rcx
    pop %rax
    cmp %rcx, %rax
    mov $0, %rax
    setg %al
    pushq %rax
    mov $4, %rax
    pushq %rax
    mov $3, %rax
    pushq %rax
    pop %rcx
    pop %rax
    cmp %rax, %rcx
    mov $0, %rax
    sete %al
    pushq %rax
    pop %rcx
    pop %rax
    cmp $0, %rax
    pushq %rax
    je _skip_and_1
    pop %rax
    pushq %rcx
_skip_and_1:
    mov $6, %rax
    pushq %rax
    mov $7, %rax
    pushq %rax
    pop %rcx
    pop %rax
    cmp %rcx, %rax
    mov $0, %rax
    setle %al
    pushq %rax
    mov $6, %rax
    pushq %rax
    mov $8, %rax
    pushq %rax
    pop %rcx
    pop %rax
    cmp %rax, %rcx
    mov $0, %rax
    sete %al
    pushq %rax
    mov $6, %rax
    pushq %rax
    mov $6, %rax
    pushq %rax
    pop %rcx
    pop %rax
    cmp %rax, %rcx
    mov $0, %rax
    sete %al
    pushq %rax
    mov $9, %rax
    pushq %rax
    mov $2, %rax
    pushq %rax
    pop %rax
    cmp $0, %rax
    mov $0, %rax
    sete %al
    pushq %rax
    pop %rcx
    pop %rax
    cmp %rcx, %rax
    mov $0, %rax
    setg %al
    pushq %rax
    pop %rcx
    pop %rax
    cmp $0, %rax
    pushq %rax
    je _skip_and_2
    pop %rax
    pushq %rcx
_skip_and_2:
    pop %rcx
    pop %rax
    cmp $1, %rax
    pushq %rax
    je _skip_or_1
    pop %rax
    pushq %rcx
_skip_or_1:
    pop %rcx
    pop %rax
    cmp $0, %rax
    pushq %rax
    je _skip_and_3
    pop %rax
    pushq %rcx
_skip_and_3:
    pop %rcx
    pop %rax
    cmp $1, %rax
    pushq %rax
    je _skip_or_2
    pop %rax
    pushq %rcx
_skip_or_2:
    pop %rax
    ret
