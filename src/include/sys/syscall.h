#ifndef SYS_SYSCALL_H
#define SYS_SYSCALL_H

#define INT_SYSCALL   "int $0x80"
#define NB_SYSCALLS   2
#define SYSCALL_TEST  1

void test(const char* s);

#endif
