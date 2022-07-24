# lowlevel
A set of articles to understand how the computer works

## Articles

- [How to disassemble and patch stripped ELF binary.md](./How%20to%20disassemble%20and%20patch%20stripped%20ELF%20binary.md)
- [Debug commands reminder.md](./Debug%20commands%20reminder.md)

## Examples

### No libc
Source code: [nolibc1](./examples/nolibc1)

How to write a simple C program without libc.
Interesting files to check:
- a simple [_start.S](examples/nolibc1/_start.S) file in GAS assembly with an entry point that replaces libc's one
- [Makefile](examples/nolibc1/Makefile) to build object files and link them to create a resulting executable. Also contains description of all
gcc and ld arguments (taken from `man`).
- its own [README.md](examples/nolibc1/README.md) with examples of linux command to analyse executable file content

Used many articles, among them:
- http://www.muppetlabs.com/~breadbox/software/tiny/teensy.html
- https://lwn.net/Articles/631631/
- https://www.embecosm.com/appnotes/ean9/html/ch05s02.html
- http://dbp-consulting.com/tutorials/debugging/linuxProgramStartup.html



### Write unit tests for nolibc1 program

Objective:
- update nolibc1 project: add makefile target to run unit tests
- try to mock syscall (write), there should be no output from the program, 
and the call to write should be asserted in unit test
- try to find a way to unit test assembler code

### Make a program to parse ELF files

Reqs:
- The first step: mimic `objdump -f /usr/bin/true` output, e.g.
```shell
$ objdump -f /usr/bin/true

/usr/bin/true:     file format elf64-x86-64
architecture: i386:x86-64, flags 0x00000150:
HAS_SYMS, DYNAMIC, D_PAGED
start address 0x0000000000002610 
```
- The second step: mimic `objdump -x /usr/bin/true` output, e.g.
```shell
objdump -x /usr/bin/true

/usr/bin/true:     file format elf64-x86-64
/usr/bin/true
architecture: i386:x86-64, flags 0x00000150:
HAS_SYMS, DYNAMIC, D_PAGED
start address 0x0000000000002610

Program Header:
    PHDR off    0x0000000000000040 vaddr 0x0000000000000040 paddr 0x0000000000000040 align 2**3
         filesz 0x00000000000002d8 memsz 0x00000000000002d8 flags r--
  INTERP off    0x0000000000000318 vaddr 0x0000000000000318 paddr 0x0000000000000318 align 2**0
         filesz 0x000000000000001c memsz 0x000000000000001c flags r--
    LOAD off    0x0000000000000000 vaddr 0x0000000000000000 paddr 0x0000000000000000 align 2**12
         filesz 0x0000000000001358 memsz 0x0000000000001358 flags r--
    LOAD off    0x0000000000002000 vaddr 0x0000000000002000 paddr 0x0000000000002000 align 2**12
         filesz 0x0000000000003571 memsz 0x0000000000003571 flags r-x
    LOAD off    0x0000000000006000 vaddr 0x0000000000006000 paddr 0x0000000000006000 align 2**12
         filesz 0x0000000000001c68 memsz 0x0000000000001c68 flags r--
    LOAD off    0x0000000000008c30 vaddr 0x0000000000009c30 paddr 0x0000000000009c30 align 2**12
         filesz 0x0000000000000450 memsz 0x00000000000005e8 flags rw-
 DYNAMIC off    0x0000000000008c98 vaddr 0x0000000000009c98 paddr 0x0000000000009c98 align 2**3
         filesz 0x00000000000001f0 memsz 0x00000000000001f0 flags rw-
    NOTE off    0x0000000000000338 vaddr 0x0000000000000338 paddr 0x0000000000000338 align 2**3
         filesz 0x0000000000000020 memsz 0x0000000000000020 flags r--
    NOTE off    0x0000000000000358 vaddr 0x0000000000000358 paddr 0x0000000000000358 align 2**2
         filesz 0x0000000000000044 memsz 0x0000000000000044 flags r--
0x6474e553 off    0x0000000000000338 vaddr 0x0000000000000338 paddr 0x0000000000000338 align 2**3
         filesz 0x0000000000000020 memsz 0x0000000000000020 flags r--
EH_FRAME off    0x0000000000006e4c vaddr 0x0000000000006e4c paddr 0x0000000000006e4c align 2**2
         filesz 0x0000000000000284 memsz 0x0000000000000284 flags r--
   STACK off    0x0000000000000000 vaddr 0x0000000000000000 paddr 0x0000000000000000 align 2**4
         filesz 0x0000000000000000 memsz 0x0000000000000000 flags rw-
   RELRO off    0x0000000000008c30 vaddr 0x0000000000009c30 paddr 0x0000000000009c30 align 2**0
         filesz 0x00000000000003d0 memsz 0x00000000000003d0 flags r--

Dynamic Section:
  NEEDED               libc.so.6
  INIT                 0x0000000000002000
  FINI                 0x0000000000005564
  INIT_ARRAY           0x0000000000009c30
  INIT_ARRAYSZ         0x0000000000000008
  FINI_ARRAY           0x0000000000009c38
  FINI_ARRAYSZ         0x0000000000000008
  GNU_HASH             0x00000000000003a0
  STRTAB               0x0000000000000990
  SYMTAB               0x0000000000000408
  STRSZ                0x00000000000002ef
  SYMENT               0x0000000000000018
  DEBUG                0x0000000000000000
  PLTGOT               0x0000000000009e88
  PLTRELSZ             0x00000000000003a8
  PLTREL               0x0000000000000007
  JMPREL               0x0000000000000fb0
  RELA                 0x0000000000000d58
  RELASZ               0x0000000000000258
  RELAENT              0x0000000000000018
  FLAGS                0x0000000000000008
  FLAGS_1              0x0000000008000001
  VERNEED              0x0000000000000cf8
  VERNEEDNUM           0x0000000000000001
  VERSYM               0x0000000000000c80
  RELACOUNT            0x0000000000000010

Version References:
  required from libc.so.6:
    0x0d696913 0x00 06 GLIBC_2.3
    0x09691974 0x00 05 GLIBC_2.3.4
    0x06969194 0x00 04 GLIBC_2.14
    0x0d696914 0x00 03 GLIBC_2.4
    0x09691a75 0x00 02 GLIBC_2.2.5

Sections:
Idx Name          Size      VMA               LMA               File off  Algn
  0 .interp       0000001c  0000000000000318  0000000000000318  00000318  2**0
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
  1 .note.gnu.property 00000020  0000000000000338  0000000000000338  00000338  2**3
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
  2 .note.gnu.build-id 00000024  0000000000000358  0000000000000358  00000358  2**2
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
  3 .note.ABI-tag 00000020  000000000000037c  000000000000037c  0000037c  2**2
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
  4 .gnu.hash     00000068  00000000000003a0  00000000000003a0  000003a0  2**3
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
  5 .dynsym       00000588  0000000000000408  0000000000000408  00000408  2**3
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
  6 .dynstr       000002ef  0000000000000990  0000000000000990  00000990  2**0
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
  7 .gnu.version  00000076  0000000000000c80  0000000000000c80  00000c80  2**1
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
  8 .gnu.version_r 00000060  0000000000000cf8  0000000000000cf8  00000cf8  2**3
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
  9 .rela.dyn     00000258  0000000000000d58  0000000000000d58  00000d58  2**3
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
 10 .rela.plt     000003a8  0000000000000fb0  0000000000000fb0  00000fb0  2**3
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
 11 .init         0000001b  0000000000002000  0000000000002000  00002000  2**2
                  CONTENTS, ALLOC, LOAD, READONLY, CODE
 12 .plt          00000280  0000000000002020  0000000000002020  00002020  2**4
                  CONTENTS, ALLOC, LOAD, READONLY, CODE
 13 .plt.got      00000010  00000000000022a0  00000000000022a0  000022a0  2**4
                  CONTENTS, ALLOC, LOAD, READONLY, CODE
 14 .plt.sec      00000270  00000000000022b0  00000000000022b0  000022b0  2**4
                  CONTENTS, ALLOC, LOAD, READONLY, CODE
 15 .text         00003042  0000000000002520  0000000000002520  00002520  2**4
                  CONTENTS, ALLOC, LOAD, READONLY, CODE
 16 .fini         0000000d  0000000000005564  0000000000005564  00005564  2**2
                  CONTENTS, ALLOC, LOAD, READONLY, CODE
 17 .rodata       00000e4c  0000000000006000  0000000000006000  00006000  2**5
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
 18 .eh_frame_hdr 00000284  0000000000006e4c  0000000000006e4c  00006e4c  2**2
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
 19 .eh_frame     00000b98  00000000000070d0  00000000000070d0  000070d0  2**3
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
 20 .init_array   00000008  0000000000009c30  0000000000009c30  00008c30  2**3
                  CONTENTS, ALLOC, LOAD, DATA
 21 .fini_array   00000008  0000000000009c38  0000000000009c38  00008c38  2**3
                  CONTENTS, ALLOC, LOAD, DATA
 22 .data.rel.ro  00000058  0000000000009c40  0000000000009c40  00008c40  2**5
                  CONTENTS, ALLOC, LOAD, DATA
 23 .dynamic      000001f0  0000000000009c98  0000000000009c98  00008c98  2**3
                  CONTENTS, ALLOC, LOAD, DATA
 24 .got          00000178  0000000000009e88  0000000000009e88  00008e88  2**3
                  CONTENTS, ALLOC, LOAD, DATA
 25 .data         00000080  000000000000a000  000000000000a000  00009000  2**5
                  CONTENTS, ALLOC, LOAD, DATA
 26 .bss          00000198  000000000000a080  000000000000a080  00009080  2**5
                  ALLOC
 27 .gnu_debuglink 00000034  0000000000000000  0000000000000000  00009080  2**2
                  CONTENTS, READONLY
SYMBOL TABLE:
no symbols
 
```

Can be useful:
- https://en.linuxteaching.com/article/understanding_the_elf_file_format



### Make not debuggable program

- program has an entry point
- program can run without errors
- gdb shows an error when you open the program
- you can apply  different techniques to achieve the goal: random fuzzing or updating program headers with knowledge.

### Update existing program to run arbitrary code

- take any binary ELF file (like `cat` or `ls`, etc) and find a way to embedd binary code 
into it to write info to a file /tmp/.random-output