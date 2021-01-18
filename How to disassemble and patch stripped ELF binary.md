# How to disassemble and patch stripped ELF binary

Let's take for example a `/bin/true` binary from `Ubuntu 20.04` distro.

First, ensure it is stripped.

## Let me see you stripped...

```
$ file /bin/true
/bin/true: ELF 64-bit LSB shared object, x86-64, version 1 (SYSV), dynamically linked, interpreter /lib64/ld-linux-x86-64.so.2, BuildID[sha1]=be53663829ed7ccdc4a913aa637ff91d280738f5, for GNU/Linux 3.2.0, stripped

```
See the stripped keyword at the end of the output.

Another way of getting aware of the absence of symbols in the binary is actually trying to disassemble it like a regular binary with debug info included.

```
gdb /bin/true
...
Reading symbols from /bin/true...
(No debugging symbols found in /bin/true)
(gdb) disassemble main
No symbol table is loaded.  Use the "file" command.

```

gdb says it cannot find any debug info built-in into the file. It also hints that we should use the "file" command. OK.


## Finding the main

```
(gdb) info file
Symbols from "/usr/bin/true".
Local exec file:
  `/usr/bin/true', file type elf64-x86-64.
  Entry point: 0x2610
  0x0000000000000318 - 0x0000000000000334 is .interp
  0x0000000000000338 - 0x0000000000000358 is .note.gnu.property
  0x0000000000000358 - 0x000000000000037c is .note.gnu.build-id
  0x000000000000037c - 0x000000000000039c is .note.ABI-tag
  0x00000000000003a0 - 0x0000000000000408 is .gnu.hash
  0x0000000000000408 - 0x0000000000000990 is .dynsym
  0x0000000000000990 - 0x0000000000000c7f is .dynstr
  0x0000000000000c80 - 0x0000000000000cf6 is .gnu.version
  0x0000000000000cf8 - 0x0000000000000d58 is .gnu.version_r
  0x0000000000000d58 - 0x0000000000000fb0 is .rela.dyn
  0x0000000000000fb0 - 0x0000000000001358 is .rela.plt
  0x0000000000002000 - 0x000000000000201b is .init
  0x0000000000002020 - 0x00000000000022a0 is .plt
  0x00000000000022a0 - 0x00000000000022b0 is .plt.got
  0x00000000000022b0 - 0x0000000000002520 is .plt.sec
  0x0000000000002520 - 0x0000000000005562 is .text
  0x0000000000005564 - 0x0000000000005571 is .fini
  0x0000000000006000 - 0x0000000000006e4c is .rodata
  0x0000000000006e4c - 0x00000000000070d0 is .eh_frame_hdr
  0x00000000000070d0 - 0x0000000000007c68 is .eh_frame
  0x0000000000009c30 - 0x0000000000009c38 is .init_array
  0x0000000000009c38 - 0x0000000000009c40 is .fini_array
  0x0000000000009c40 - 0x0000000000009c98 is .data.rel.ro
  0x0000000000009c98 - 0x0000000000009e88 is .dynamic
  0x0000000000009e88 - 0x000000000000a000 is .got
  0x000000000000a000 - 0x000000000000a080 is .data
  0x000000000000a080 - 0x000000000000a218 is .bss

```

Two lines are important here:
- `Entry point: 0x2610` entry point of the executable file (a wrapper for the main)
- `0x0000000000002520 - 0x0000000000005562 is .text` (the range of addresses where the instructions reside)

We see that entry point address is within the range of the .text section. Ok. Let's see the actual instructions there:

```
(gdb) x/20i 0x2610
   0x2610:  endbr64 
   0x2614:  xor    ebp,ebp
   0x2616:  mov    r9,rdx
   0x2619:  pop    rsi
   0x261a:  mov    rdx,rsp
   0x261d:  and    rsp,0xfffffffffffffff0
   0x2621:  push   rax
   0x2622:  push   rsp
   0x2623:  lea    r8,[rip+0x2f16]        # 0x5540
   0x262a:  lea    rcx,[rip+0x2e9f]        # 0x54d0
   0x2631:  lea    rdi,[rip+0xffffffffffffff18]        # 0x2550    <---- this is the address of the main
   0x2638:  call   QWORD PTR [rip+0x79a2]        # 0x9fe0
   0x263e:  hlt    
   0x263f:  nop
   0x2640:  lea    rdi,[rip+0x7a39]        # 0xa080 <program_invocation_short_name>
   0x2647:  lea    rax,[rip+0x7a32]        # 0xa080 <program_invocation_short_name>
   0x264e:  cmp    rax,rdi
   0x2651:  je     0x2668
   0x2653:  mov    rax,QWORD PTR [rip+0x797e]        # 0x9fd8
   0x265a:  test   rax,rax

```

`x/20i 0x2610` is "Show me 20 first instructions starting from the address 0x2610, the entry point of our app".

Moreover I found a more convenient way with objdump utility:

```
$ objdump -M intel -d -F -j .text /bin/true | grep -A20 2610
    2610: f3 0f 1e fa           endbr64 
    2614: 31 ed                 xor    ebp,ebp
    2616: 49 89 d1              mov    r9,rdx
    2619: 5e                    pop    rsi
    261a: 48 89 e2              mov    rdx,rsp
    261d: 48 83 e4 f0           and    rsp,0xfffffffffffffff0
    2621: 50                    push   rax
    2622: 54                    push   rsp
    2623: 4c 8d 05 16 2f 00 00  lea    r8,[rip+0x2f16]        # 5540 <__ctype_b_loc@plt+0x3030> (File Offset: 0x5540)
    262a: 48 8d 0d 9f 2e 00 00  lea    rcx,[rip+0x2e9f]        # 54d0 <__ctype_b_loc@plt+0x2fc0> (File Offset: 0x54d0)
    2631: 48 8d 3d 18 ff ff ff  lea    rdi,[rip+0xffffffffffffff18]        # 2550 <__ctype_b_loc@plt+0x40> (File Offset: 0x2550) <---- this is the address of the main
    2638: ff 15 a2 79 00 00     call   QWORD PTR [rip+0x79a2]        # 9fe0 <__libc_start_main@GLIBC_2.2.5> (File Offset: 0x9fe0)
    263e: f4                    hlt    
    263f: 90                    nop
    2640: 48 8d 3d 39 7a 00 00  lea    rdi,[rip+0x7a39]        # a080 <__progname@@GLIBC_2.2.5> (File Offset: 0xa080)
    2647: 48 8d 05 32 7a 00 00  lea    rax,[rip+0x7a32]        # a080 <__progname@@GLIBC_2.2.5> (File Offset: 0xa080)
    264e: 48 39 f8              cmp    rax,rdi
    2651: 74 15                 je     2668 <__ctype_b_loc@plt+0x158> (File Offset: 0x2668)
    2653: 48 8b 05 7e 79 00 00  mov    rax,QWORD PTR [rip+0x797e]        # 9fd8 <_ITM_deregisterTMCloneTable> (File Offset: 0x9fd8)
    265a: 48 85 c0              test   rax,rax
    265d: 74 09                 je     2668 <__ctype_b_loc@plt+0x158> (File Offset: 0x2668)


```

These are two identical listing from which we can deduce that the instruction on the line `2638` calls \__libc_start_main function that as the first parameter should receive a pointer to the actual main function of the app.

objdump unlike gdb calculates and shows an actual address that is loaded into `rdi` register on the line 2631: `0x2550`. But it is not difficult to calculate it manually. We see the instruction `lea    rdi,[rip+0xffffffffffffff18]` that means add to the contents of the rip register the number `0xffffffffffffff18` and put the content of the memory at the resulting address into rdi register. Let's review it step by step:

1. `rip` by definition contains an address of the next instruction. In our case it is `0x2638`
2. 0xffffffffffffff18 is negative and represented in complementary code. In order to convert it to the normal view decrement it by one and do binary invert. Putting the things simpler, do the following: 0x18 - 1 = 0x17, 0xFF(255) - 0x17(23) = E8(232). -E8 is the offset of the data relative to the`rip`.
3. main's address is 0x2638 - 0xE8 = 0x2550

Why have we decided that the address loaded into `rdi` register is the actual main's address? To answer this, I refer to the code of [sysdeps/x86_64/start.S](https://sourceware.org/git/?p=glibc.git;a=blob_plain;f=sysdeps/x86_64/start.S;hb=HEAD) of glibc library. This file contains the source code we have disassembled a few steps earlier. It's single purpose is to prepare arguments and call \__libc_start_main function. The first param is our desired address of the main function.

## Patch me if you can

By the moment we have the address of the main `0x2550`. The task is to patch the return value of the main function so that /bin/true returns falsy value to the caller.

Quick check of the original return value:
```
$ /bin/true
$ echo $?
0
```

OK. It returns 0, which is interpreted by a shell as a good result code (literally zero errors). I will try to return 1 instead.

```
  objdump -M intel -d -F -j .text /bin/true | grep -A20 "2550:"

    2550: f3 0f 1e fa           endbr64 
    2554: 83 ff 02              cmp    edi,0x2
    2557: 74 03                 je     255c <__ctype_b_loc@plt+0x4c> (File Offset: 0x255c)
    2559: 31 c0                 xor    eax,eax
    255b: c3                    ret    
    255c: 55                    push   rbp
    255d: 53                    push   rbx
    255e: 48 89 f3              mov    rbx,rsi
    2561: 51                    push   rcx
    2562: 48 8b 3e              mov    rdi,QWORD PTR [rsi]
    2565: e8 c6 05 00 00        call   2b30 <__ctype_b_loc@plt+0x620> (File Offset: 0x2b30)
    256a: 48 8d 35 9c 3a 00 00  lea    rsi,[rip+0x3a9c]        # 600d <_IO_stdin_used@@Base+0xd> (File Offset: 0x600d)
    2571: bf 06 00 00 00        mov    edi,0x6
    2576: e8 f5 fe ff ff        call   2470 <setlocale@plt> (File Offset: 0x2470)
    257b: 48 8d 35 14 3b 00 00  lea    rsi,[rip+0x3b14]        # 6096 <_IO_stdin_used@@Base+0x96> (File Offset: 0x6096)
    2582: 48 8d 3d ff 3a 00 00  lea    rdi,[rip+0x3aff]        # 6088 <_IO_stdin_used@@Base+0x88> (File Offset: 0x6088)
    2589: e8 a2 fd ff ff        call   2330 <bindtextdomain@plt> (File Offset: 0x2330)
    258e: 48 8d 3d f3 3a 00 00  lea    rdi,[rip+0x3af3]        # 6088 <_IO_stdin_used@@Base+0x88> (File Offset: 0x6088)
    2595: e8 76 fd ff ff        call   2310 <textdomain@plt> (File Offset: 0x2310)
    259a: 48 8d 3d df 04 00 00  lea    rdi,[rip+0x4df]        # 2a80 <__ctype_b_loc@plt+0x570> (File Offset: 0x2a80)
    25a1: e8 aa 2f 00 00        call   5550 <__ctype_b_loc@plt+0x3040> (File Offset: 0x5550)
```

These are the first lines of the main function, it's head. Line `255b` is `ret` which means exit from the function. This is our target. We can see an if-else starting from `cmp    edi,0x2`. One branch leads to return with the code 0 (`xor eax,eax`). This is the branch that is executed when no command line arguments passed to the /bin/true. Another branch is more complex and calls different libc functions along the way. For simplicity we concentrate on the first one.

Our task is that `eax` register contain a value other than 0 just before the ret instruction. `xor    eax,eax` is the instruction which produces the return code. We need to substitute it with another directive of the same size. The binary representation of the `xor    eax,eax` is `31 c0`(see previous column) which are two bytes. Thanks to the resource https://defuse.ca/online-x86-assembler.htm#disassembly you can convert any assembler instruction into binary and vice/versa very simply. Just put some asm expressions, select x64 architecture and press Assemble.

Input:
```
mov %eax, 1
mov %al, 1 
inc al
```

Output from defuse.ca:
```
0:  b8 01 00 00 00          mov    eax,0x1
5:  b0 01                   mov    al,0x1
7:  fe c0                   inc    al
```

The latter two instrcutions have 2 bytes size, but the `mov al,0x1` will idempotentially(constantly) do what we want, set the lower byte of eax register to 1.

Now, let's rephase out task to narrow it down.

There should be rewritten 2 bytes in file /bin/true from `31 c0`(`xor eax,eax`) to `b0 01`(`mov al,0x1`). OK.

### Hardcore mode ON

Let's use a brilliant utility `xxd` to convert our binary file into a text file where strings of hex numbers correspond to original bytes.
```
$ xxd /bin/true > ~/bin_true.hex
$ head bin_true.hex 
00000000: 7f45 4c46 0201 0100 0000 0000 0000 0000  .ELF............
00000010: 0300 3e00 0100 0000 1026 0000 0000 0000  ..>......&......
00000020: 4000 0000 0000 0000 d891 0000 0000 0000  @...............
00000030: 0000 0000 4000 3800 0d00 4000 1e00 1d00  ....@.8...@.....
00000040: 0600 0000 0400 0000 4000 0000 0000 0000  ........@.......
00000050: 4000 0000 0000 0000 4000 0000 0000 0000  @.......@.......
00000060: d802 0000 0000 0000 d802 0000 0000 0000  ................
00000070: 0800 0000 0000 0000 0300 0000 0400 0000  ................
00000080: 1803 0000 0000 0000 1803 0000 0000 0000  ................
00000090: 1803 0000 0000 0000 1c00 0000 0000 0000  ................
```
Good. Now use any editor to replace two bytes at the offset. In order to do this:
1. Go to the offset 2550. Simply search for "2550:"
2. Having found this line, find there `31 c0` char sequence. It could be splitted into two groups.
```
...
00002550: f30f 1efa 83ff 0274 0331 c0c3 5553 4889  .......t.1..USH.
...
```

3. Change `31 c0` to `b0 01`

```
...
00002550: f30f 1efa 83ff 0274 03b0 01c3 5553 4889  .......t.1..USH.
...
```
4. Save the file and convert it back to binary:
```
$ xxd -r ~/bin_true.hex > ~/true_patched
$ chmod +x ~/true_patched
$ ./true_patched
$ echo $?
1
```

The task is done.

## Summary

In order to analyse and patch any stripped ELF binary you have to perform the following steps:
1. Ensure the file is actually stripped
2. Find the entrypoint
3. Find the main function address
4. Analyse main(or other) function code and decide what to patch and how
5. Remember not to add or remove bytes inside the file, bytes should be replaced inline. 2 bytes instruction by 2 bytes instruction, 3,4,n-bytes instruction(s) by instruction(s) occupying the same space.
6. Do the trick using `xxd` utility or any other hexeditor by your choice.
7. Have the profit.

## Links and appreciations

- Youtube channel [LiveOverflow](https://www.youtube.com/channel/UClcE-kVhqyiHCcjYwcpfj9w) and especially the video [Finding main() in Stripped Binary - bin 0x2C](https://www.youtube.com/watch?v=N1US3c6CpSw)
- Very handy resource to assemble/disassemble the code: https://defuse.ca/online-x86-assembler.htm#disassembly
- PDF: [Intel® 64 and IA-32 Architectures Software Developer’s Manual](file:///home/flores/pets/lowlevel/64-ia-32-architectures-software-developer-instruction-set-reference-manual-325383.pdf) where you can find description of any instruction (xor or mov for example)
- [Source code of glibc](git://sourceware.org/git/glibc.git)






