#include <stdio.h> 
#include <elf.h> 
#include <string.h>
#include <glib.h>
#include "app.h"

const char MAGIC_ELF[MAGIC_ELF_LENGTH] = { 0x7f, 'E', 'L', 'F' };

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
}

int args_have_errors(int argc, char **argv) {
    if(argc < 2) {
        return 1;
    } else {
        return 0;
    }
}

int read_elf_header(FILE* fd, Elf64_Ehdr* elf_header) {
    size_t read_bytes = fread(elf_header, sizeof(char), sizeof(Elf64_Ehdr), fd); 
    printf("Bytes read from file: %ld\n", read_bytes);
    printf("elf_header size is: %ld\n", sizeof(Elf64_Ehdr));
    if (strncmp(MAGIC_ELF, elf_header->e_ident, MAGIC_ELF_LENGTH) != 0) {
        return 1;
    }
    return 0;    
}

char* map_osabi_desc(unsigned char osabi_id) {
    switch(osabi_id) {
        case 0x00: return "UNIX - System V";
        case 0x01: return "HP-UX";
        case 0x02: return "NetBSD";
        case 0x03: return "Linux";
        case 0x04: return "GNU Hurd";
        case 0x06: return "Solaris";
        case 0x07: return "AIX (Monterey)";
        case 0x08: return "IRIX";
        case 0x09: return "FreeBSD";
        case 0x0A: return "Tru64";
        case 0x0B: return "Novell Modesto";
        case 0x0C: return "OpenBSD";
        case 0x0D: return "OpenVMS";
        case 0x0E: return "NonStop Kernel";
        case 0x0F: return "AROS";
        case 0x10: return "FenixOS";
        case 0x11: return "Nuxi CloudABI";
        case 0x12: return "Stratus Technologies OpenVOS";
    }
    return "N/A";
}

char* map_object_file_type_desc(Elf64_Half object_file_type_id) {
    switch(object_file_type_id) {
        case ET_NONE: return "NONE (Unknown)";
        case ET_REL: return "REL (Relocatable file)";
        case ET_EXEC: return "EXEC (Executable file)";
        case ET_DYN: return "DYN (Position-Independent Executable file)";
        case ET_CORE: return "Core (Core file)";
        default: return "Reserved inclusive range";
    }
    return "N/A";
}

char* map_class_desc(unsigned char class_id) {
    switch(class_id) {
        case 1:
            return "ELF32";
        case 2:
            return "ELF64";
    }
    return "N/A";
}

char* map_data_desc(unsigned char data_id) {
    switch(data_id) {
        case 1:
            return "2's complement, little endian";
        case 2:
            return "2's complement, big endian";
    }
    return "N/A";
}

char* map_version_desc(unsigned char version_id) {
    switch(version_id) {
        case 1:
            return "1 (current)";
    }
    return "N/A";
}

char* map_raw_bytes_desc(unsigned char* magic_bytes_arr, size_t n_bytes, char* out_str) {
    for(int i = 0; i < n_bytes; i++) {
        snprintf(&out_str[i*3], 3, "%02x", magic_bytes_arr[i]);
        if(i < n_bytes - 1) {
            snprintf(&out_str[i*3 + 2], 2, " ");
        }
    }
    return out_str;
}


void print_key_value( gpointer key, gpointer value, gpointer user_data ) {
    int real_key = GPOINTER_TO_INT( key );
    char* real_value = (char*)value;

    printf( "%d => %s\n", real_key, real_value );
    return;
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
    snprintf( flags, 19, "0x%016x",  elf_header.e_flags);

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

    print_elf_info(&file_info);
    
    fclose(fd);
    return 0;
}


