   .data
    .globl a
a:
    .long 5
   .bss
   .text
    .globl fib
fib:
    pushq %rbp
    mov %rsp, %rbp
    sub $16, %rsp
    sub $16, %rsp
    sub $16, %rbp
    mov %rcx, -8(%rbp)
    mov a, %rax
    pushq %rax
    pop %rax
    add $16, %rsp
    add $16, %rbp
    mov %rbp, %rsp
    pop %rbp
    ret
    .globl main
main:
    pushq %rbp
    mov %rsp, %rbp
    sub $16, %rsp
    sub $16, %rsp
    sub $16, %rbp
    mov $9, %rax
    pushq %rax
    pop %rax
    mov %rax, a
