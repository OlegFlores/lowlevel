void* syscall5(
	void* number,
	void* arg1,
	void* arg2,
	void* arg3,
	void* arg4,
	void* arg5
);

typedef unsigned long int uintptr; /* size_t */
typedef long int intptr; /* ssize_t */

static intptr write(int fd, void const* data, uintptr nbytes)
{
	return (intptr)
		syscall5(
			(void*)1, /* SYS_write */
			(void*)(intptr)fd,
			(void*)data,
			(void*)nbytes,
			0, /* ignored */
			0 /* ignored */
		);
}

int main_func(int argc, char* argv[])
{
	write(1, "hello\n", 6);
	return 0;
}
