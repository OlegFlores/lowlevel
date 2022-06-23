# Debug commands reminder
## What we can say about file

### Essential ELF information
```shell
➜  objdump -f ./main 

./main:     file format elf64-x86-64
architecture: i386:x86-64, flags 0x00000150:
HAS_SYMS, DYNAMIC, D_PAGED
start address 0x00000000000010a0
```

The most important part here is the start address. To extract the start address separately, use
```shell
➜  objdump -f ./main | grep "start address" | awk '{print $3}'
0x00000000000010a0
```