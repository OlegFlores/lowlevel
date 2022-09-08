#include <stdio.h> 
#include <elf.h> 
#include <string.h>
#include <inttypes.h>
#include "app.h"
#include "mappings/mappings.h"

/**
 * 
  ELF Header:
  Magic:   7f 45 4c 46 02 01 01 00 00 00 00 00 00 00 00 00
  Class:                             ELF64
  Data:                              2's complement, little endian
  Version:                           1 (current)
  OS/ABI:                            UNIX - System V
  ABI Version:                       0
  Type:                              DYN (Position-Independent Executable file)
  Machine:                           Advanced Micro Devices X86-64
  Version:                           0x1
  Entry point address:               0x1960

  Start of program headers:          64 (bytes into file)
  Start of section headers:          25008 (bytes into file)
  Flags:                             0x0
  Size of this header:               64 (bytes)
  Size of program headers:           56 (bytes)
  Number of program headers:         13
  Size of section headers:           64 (bytes)
  Number of section headers:         30
  Section header string table index: 29

 */

void print_elf_info(elf64_info* info) {
    printf("ELF Header:\n");
    printf("Magic: %s\n", info -> magic_bytes);
    printf("Class:\t\t\t\t%s\n", info -> class);
    printf("Data:\t\t\t\t%s\n", info -> endiannes);
    printf("Version:\t\t\t%s\n", info -> version);
    printf("OS/ABI:\t\t\t\t%s\n", info -> os_abi);
    printf("ABI Version:\t\t\t%s\n", info -> abi_version);
    printf("Padding:\t\t\t%s\n", info -> padding_bytes);
    printf("Type:\t\t\t\t%s\n", info -> object_file_type);
    printf("Machine:\t\t\t%s\n", info -> machine);
    printf("Version:\t\t\t%s\n", info -> elf_version);
    printf("Entry point address:\t\t%s\n", info -> entry_point_address);
}

int args_have_errors(int argc, char **argv) {
    if(argc < 2) {
        return 1;
    } else {
        return 0;
    }
}

int read_elf_header(FILE* fd, Elf64_Ehdr* elf_header) {
    const char MAGIC_ELF_BYTES[] = {ELFMAG0, ELFMAG1, ELFMAG2, ELFMAG3};
    size_t read_bytes = fread(elf_header, sizeof(char), sizeof(Elf64_Ehdr), fd); 
    printf("Bytes read from file: %ld\n", read_bytes);
    printf("elf_header size is: %ld\n", sizeof(Elf64_Ehdr));
    if (strncmp(MAGIC_ELF_BYTES, elf_header->e_ident, MAGIC_ELF_LENGTH) != 0) {
        return 1;
    }
    return 0;    
}

int run_app(int argc, char **argv) {
    char *file_name;
    Elf64_Ehdr elf_header;
    elf64_info file_info; 
    FILE* fd;
    char flags[19];
    char magic_bytes[48];
    char padding_bytes[21];
    char abi_version[4];
    char elf_version[4];
    char entry_point_address[20];
    
    if(args_have_errors(argc, argv) != 0) {
        printf(CLI_MESSAGE_USAGE_HINT);
        return ERROR_WRONG_ARGUMENTS;
    }
    
    file_name = argv[1];
    fd = fopen(file_name, "rb"); 

    if(fd == NULL) {
        printf(CLI_MESSAGE_COULD_NOT_OPEN_FILE);
        return ERROR_FILE_CANNOT_BE_OPENED;
    }
    
    if(read_elf_header(fd, &elf_header) != 0) {
        printf(CLI_MESSAGE_FORMAT_NOT_RECOGNIZED, file_name);
        return ERROR_FILE_NOT_ELF;
    }

    snprintf(abi_version, 4, "%d", elf_header.e_ident[EI_ABIVERSION]);
    snprintf(flags, 19, "0x%016x",  elf_header.e_flags);
    snprintf(elf_version, 4, "0x%1x", elf_header.e_version);
    snprintf(entry_point_address, 19, "%#lx", elf_header.e_entry);

    file_info.file_name = file_name;
    file_info.magic_bytes = map_raw_bytes_desc(elf_header.e_ident, 16, magic_bytes);;
    file_info.class = map_class_desc(elf_header.e_ident[EI_CLASS]);
    file_info.endiannes = map_data_desc(elf_header.e_ident[EI_DATA]);
    file_info.version = map_version_desc(elf_header.e_ident[EI_VERSION]);
    file_info.os_abi = map_osabi_desc(elf_header.e_ident[EI_OSABI]);
    file_info.abi_version = abi_version;
    file_info.padding_bytes = map_raw_bytes_desc(&elf_header.e_ident[EI_PAD], 7, padding_bytes);
    file_info.object_file_type = map_object_file_type_desc(elf_header.e_type);
    file_info.flags_hex = flags;
    file_info.machine = map_machine_desc(elf_header.e_machine);
    file_info.elf_version = elf_version;
    file_info.entry_point_address = entry_point_address;

    print_elf_info(&file_info);
    
    fclose(fd);
    return 0;
}
