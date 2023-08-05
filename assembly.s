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
_for_condition_110:
    mov -16(%rbp), %rax
    pushq %rax
    mov $2, %rax
    pushq %rax
    pop %rcx
    pop %rax
    cmp %rcx, %rax
    mov $0, %rax
    setl %al
    pushq %rax
    pop %rax
    cmp $0, %rax
    je _for_end_110
    sub $16, %rsp
    sub $16, %rbp
    mov 8(%rbp), %rax
    mov %rax, -8(%rbp)
    mov 0(%rbp), %rax
    mov %rax, -16(%rbp)
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
    mov %rax, 8(%rbp)
    mov %rax, 24(%rbp)
    add $16, %rsp
    add $16, %rbp
_for_skip_110:
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
    jmp _for_condition_110
_for_end_110:
    add $16, %rsp
    add $16, %rbp
    sub $16, %rsp
    sub $16, %rbp
    mov 8(%rbp), %rax
    mov %rax, -8(%rbp)
    mov $0, %rax
    pushq %rax
    pop %rax
    mov %rax, -16(%rbp)
_for_condition_120:
    mov -16(%rbp), %rax
    pushq %rax
    mov $2, %rax
    pushq %rax
    pop %rcx
    pop %rax
    cmp %rcx, %rax
    mov $0, %rax
    setl %al
    pushq %rax
    pop %rax
    cmp $0, %rax
    je _for_end_120
    sub $16, %rsp
    sub $16, %rbp
    mov 8(%rbp), %rax
    mov %rax, -8(%rbp)
    mov 0(%rbp), %rax
    mov %rax, -16(%rbp)
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
    mov %rax, 8(%rbp)
    mov %rax, 24(%rbp)
    add $16, %rsp
    add $16, %rbp
_for_skip_120:
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
    jmp _for_condition_120
_for_end_120:
    add $16, %rsp
    add $16, %rbp
    sub $16, %rsp
    sub $16, %rbp
    mov 8(%rbp), %rax
    mov %rax, -8(%rbp)
    mov $0, %rax
    pushq %rax
    pop %rax
    mov %rax, -16(%rbp)
_for_condition_130:
    mov -16(%rbp), %rax
    pushq %rax
    mov $2, %rax
    pushq %rax
    pop %rcx
    pop %rax
    cmp %rcx, %rax
    mov $0, %rax
    setl %al
    pushq %rax
    pop %rax
    cmp $0, %rax
    je _for_end_130
    sub $16, %rsp
    sub $16, %rbp
    mov 8(%rbp), %rax
    mov %rax, -8(%rbp)
    mov 0(%rbp), %rax
    mov %rax, -16(%rbp)
    sub $16, %rsp
    sub $16, %rbp
    mov 8(%rbp), %rax
    mov %rax, -8(%rbp)
    mov 0(%rbp), %rax
    mov %rax, -16(%rbp)
    mov $0, %rax
    pushq %rax
    pop %rax
    mov %rax, -16(%rbp)
_for_condition_13010:
    mov -16(%rbp), %rax
    pushq %rax
    mov $6, %rax
    pushq %rax
    pop %rcx
    pop %rax
    cmp %rcx, %rax
    mov $0, %rax
    setl %al
    pushq %rax
    pop %rax
    cmp $0, %rax
    je _for_end_13010
    sub $16, %rsp
    sub $16, %rbp
    mov 8(%rbp), %rax
    mov %rax, -8(%rbp)
    mov 0(%rbp), %rax
    mov %rax, -16(%rbp)
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
    mov %rax, 8(%rbp)
    mov %rax, 24(%rbp)
    mov %rax, 40(%rbp)
    mov %rax, 56(%rbp)
    add $16, %rsp
    add $16, %rbp
_for_skip_13010:
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
    jmp _for_condition_13010
_for_end_13010:
    add $16, %rsp
    add $16, %rbp
    add $16, %rsp
    add $16, %rbp
_for_skip_130:
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
    jmp _for_condition_130
_for_end_130:
    add $16, %rsp
    add $16, %rbp
    sub $16, %rsp
    sub $16, %rbp
    mov 8(%rbp), %rax
    mov %rax, -8(%rbp)
    mov $0, %rax
    pushq %rax
    pop %rax
    mov %rax, -16(%rbp)
_for_condition_140:
    mov -16(%rbp), %rax
    pushq %rax
    mov $6, %rax
    pushq %rax
    pop %rcx
    pop %rax
    cmp %rcx, %rax
    mov $0, %rax
    setl %al
    pushq %rax
    pop %rax
    cmp $0, %rax
    je _for_end_140
    sub $16, %rsp
    sub $16, %rbp
    mov 8(%rbp), %rax
    mov %rax, -8(%rbp)
    mov 0(%rbp), %rax
    mov %rax, -16(%rbp)
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
    mov %rax, 8(%rbp)
    mov %rax, 24(%rbp)
    add $16, %rsp
    add $16, %rbp
_for_skip_140:
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
    jmp _for_condition_140
_for_end_140:
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
