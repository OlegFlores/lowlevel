#include "unistd_x86_64.h"
#include "syscall.h"
#include "types.h"

intptr write(int fd, void const* data, uintptr nbytes) {
	return (intptr) 
    syscall5(
        (void*)__NR_write,
        (void*)(intptr)fd,
        (void*)data,
        (void*)nbytes,
        0,
        0
    );
}

int calc(int a, int b) {
    return a + b;
}