/**
	The code of _start function is 
	based on instructions from 
	System V Application Binary Interface
	AMD64 Architecture Processor Supplement
	(With LP64 and ILP32 Programming Models)
	Version 1.0
	December 22, 2018
	https://raw.githubusercontent.com/wiki/hjl-tools/x86-psABI/x86-64-psABI-1.0.pdf

	For the latest officially available check https://refspecs.linuxbase.org/
**/
.intel_syntax noprefix
.text
.globl _start, syscall5
.set SYS_exit, 60

_start:
	/* rbp should be set to null */
	xor rbp, rbp
	/* rdi = argc - the number of passed command line arguments */
	pop rdi
	/* the pop instruction already added 8 to rsp */
	mov rsi, rsp /* rest of the stack is an array argv of command line arguments */

	/* zero the las 4 bits of rsp, aligning it to 16 bytes */
	and rsp, 0xfffffffffffffff0
	call main
	mov rdi, rax /* syscall param 1 = rax (ret value of main) */
	mov rax, SYS_exit
	syscall

	ret


