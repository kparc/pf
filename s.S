#include<asm/unistd_64.h>

#define F(n) .globl n;n:
#define S(n) F(n)mov %rcx,%r10;mov $__NR_##n,%rax;syscall;ret;

F(_start)lea 8(%rsp),%rdi;jmp main;

S(write)
S(exit)

//:~
