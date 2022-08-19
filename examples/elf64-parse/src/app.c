#include <stdio.h> 
#include <elf.h> 
#include <string.h>
#include <glib.h>
#include "app.h"

GHashTable* _machines;

// /bin/true:     file format elf64-x86-64
// architecture: i386:x86-64, flags 0x00000150:
// HAS_SYMS, DYNAMIC, D_PAGED
// start address 0x0000000000002610
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

    printf("architecture: %s, flags: %s:\n", info -> arch, info -> flags_hex);
}

int args_have_errors(int argc, char **argv) {
    if(argc < 2) {
        return 1;
    } else {
        return 0;
    }
}

char* find_machine_description(GHashTable* machines, int machine_value) {
    void* res = g_hash_table_lookup(machines, GINT_TO_POINTER(machine_value));
    if(res == NULL) {
        return "N/A";
    } else {
        return (char*)res;
    }
} 

void create_hashtable_machines(GHashTable* machines) {
    g_hash_table_insert( machines, GINT_TO_POINTER( 0x00 ), strdup("No specific instruction set") ); 
    g_hash_table_insert( machines, GINT_TO_POINTER( 0x01 ), strdup("AT&T WE 32100") ); 
    g_hash_table_insert( machines, GINT_TO_POINTER( 0x02 ), strdup("SPARC") ); 
    g_hash_table_insert( machines, GINT_TO_POINTER( 0x03 ), strdup("x86") ); 
    g_hash_table_insert( machines, GINT_TO_POINTER( 0x04 ), strdup("Motorola 68000 (M68k)") ); 
    g_hash_table_insert( machines, GINT_TO_POINTER( 0x05 ), strdup("Motorola 88000 (M88k)") ); 
    g_hash_table_insert( machines, GINT_TO_POINTER( 0x06 ), strdup("Intel MCU") ); 
    g_hash_table_insert( machines, GINT_TO_POINTER( 0x07 ), strdup("Intel 80860") ); 
    g_hash_table_insert( machines, GINT_TO_POINTER( 0x08 ), strdup("MIPS") ); 
    g_hash_table_insert( machines, GINT_TO_POINTER( 0x09 ), strdup("IBM System/370") ); 
    g_hash_table_insert( machines, GINT_TO_POINTER( 0x0A ), strdup("MIPS RS3000 Little-endian") ); 
    g_hash_table_insert( machines, GINT_TO_POINTER( 0x0B ), strdup("Reserved for future use") ); 
    g_hash_table_insert( machines, GINT_TO_POINTER( 0x0C ), strdup("Reserved for future use") ); 
    g_hash_table_insert( machines, GINT_TO_POINTER( 0x0D ), strdup("Reserved for future use") ); 
    g_hash_table_insert( machines, GINT_TO_POINTER( 0x0E ), strdup("Hewlett-Packard PA-RISC") ); 
    g_hash_table_insert( machines, GINT_TO_POINTER( 0x0F ), strdup("Reserved for future use") ); 
    g_hash_table_insert( machines, GINT_TO_POINTER( 0x13 ), strdup("Intel 80960") ); 
    g_hash_table_insert( machines, GINT_TO_POINTER( 0x14 ), strdup("PowerPC") ); 
    g_hash_table_insert( machines, GINT_TO_POINTER( 0x15 ), strdup("PowerPC (64-bit)") ); 
    g_hash_table_insert( machines, GINT_TO_POINTER( 0x16 ), strdup("S390, including S390x") ); 
    g_hash_table_insert( machines, GINT_TO_POINTER( 0x17 ), strdup("IBM SPU/SPC") ); 
    g_hash_table_insert( machines, GINT_TO_POINTER( 0x18 ), strdup("Reserved for future use") ); 
    g_hash_table_insert( machines, GINT_TO_POINTER( 0x19 ), strdup("Reserved for future use") ); 
    g_hash_table_insert( machines, GINT_TO_POINTER( 0x1A ), strdup("Reserved for future use") ); 
    g_hash_table_insert( machines, GINT_TO_POINTER( 0x1B ), strdup("Reserved for future use") ); 
    g_hash_table_insert( machines, GINT_TO_POINTER( 0x1C ), strdup("Reserved for future use") ); 
    g_hash_table_insert( machines, GINT_TO_POINTER( 0x1D ), strdup("Reserved for future use") ); 
    g_hash_table_insert( machines, GINT_TO_POINTER( 0x1E ), strdup("Reserved for future use") ); 
    g_hash_table_insert( machines, GINT_TO_POINTER( 0x1F ), strdup("Reserved for future use") ); 
    g_hash_table_insert( machines, GINT_TO_POINTER( 0x20 ), strdup("Reserved for future use") ); 
    g_hash_table_insert( machines, GINT_TO_POINTER( 0x21 ), strdup("Reserved for future use") ); 
    g_hash_table_insert( machines, GINT_TO_POINTER( 0x22 ), strdup("Reserved for future use") ); 
    g_hash_table_insert( machines, GINT_TO_POINTER( 0x23 ), strdup("Reserved for future use") ); 
    g_hash_table_insert( machines, GINT_TO_POINTER( 0x24 ), strdup("NEC V800") ); 
    g_hash_table_insert( machines, GINT_TO_POINTER( 0x25 ), strdup("Fujitsu FR20") ); 
    g_hash_table_insert( machines, GINT_TO_POINTER( 0x26 ), strdup("TRW RH-32") ); 
    g_hash_table_insert( machines, GINT_TO_POINTER( 0x27 ), strdup("Motorola RCE") ); 
    g_hash_table_insert( machines, GINT_TO_POINTER( 0x28 ), strdup("ARM (up to ARMv7/Aarch32)") ); 
    g_hash_table_insert( machines, GINT_TO_POINTER( 0x29 ), strdup("Digital Alpha") ); 
    g_hash_table_insert( machines, GINT_TO_POINTER( 0x2A ), strdup("SuperH") ); 
    g_hash_table_insert( machines, GINT_TO_POINTER( 0x2B ), strdup("SPARC Version 9") ); 
    g_hash_table_insert( machines, GINT_TO_POINTER( 0x2C ), strdup("Siemens TriCore embedded processor") ); 
    g_hash_table_insert( machines, GINT_TO_POINTER( 0x2D ), strdup("Argonaut RISC Core") ); 
    g_hash_table_insert( machines, GINT_TO_POINTER( 0x2E ), strdup("Hitachi H8/300") ); 
    g_hash_table_insert( machines, GINT_TO_POINTER( 0x2F ), strdup("Hitachi H8/300H") ); 
    g_hash_table_insert( machines, GINT_TO_POINTER( 0x30 ), strdup("Hitachi H8S") ); 
    g_hash_table_insert( machines, GINT_TO_POINTER( 0x31 ), strdup("Hitachi H8/500") ); 
    g_hash_table_insert( machines, GINT_TO_POINTER( 0x32 ), strdup("IA-64") ); 
    g_hash_table_insert( machines, GINT_TO_POINTER( 0x33 ), strdup("Stanford MIPS-X") ); 
    g_hash_table_insert( machines, GINT_TO_POINTER( 0x34 ), strdup("Motorola ColdFire") ); 
    g_hash_table_insert( machines, GINT_TO_POINTER( 0x35 ), strdup("Motorola M68HC12") ); 
    g_hash_table_insert( machines, GINT_TO_POINTER( 0x36 ), strdup("Fujitsu MMA Multimedia Accelerator") ); 
    g_hash_table_insert( machines, GINT_TO_POINTER( 0x37 ), strdup("Siemens PCP") ); 
    g_hash_table_insert( machines, GINT_TO_POINTER( 0x38 ), strdup("Sony nCPU embedded RISC processor") ); 
    g_hash_table_insert( machines, GINT_TO_POINTER( 0x39 ), strdup("Denso NDR1 microprocessor") ); 
    g_hash_table_insert( machines, GINT_TO_POINTER( 0x3A ), strdup("Motorola Star*Core processor") ); 
    g_hash_table_insert( machines, GINT_TO_POINTER( 0x3B ), strdup("Toyota ME16 processor") ); 
    g_hash_table_insert( machines, GINT_TO_POINTER( 0x3C ), strdup("STMicroelectronics ST100 processor") ); 
    g_hash_table_insert( machines, GINT_TO_POINTER( 0x3D ), strdup("Advanced Logic Corp. TinyJ embedded processor family") ); 
    g_hash_table_insert( machines, GINT_TO_POINTER( 0x3E ), strdup("AMD x86-64") ); 
    g_hash_table_insert( machines, GINT_TO_POINTER( 0x3F ), strdup("Sony DSP Processor") );
    g_hash_table_insert( machines, GINT_TO_POINTER( 0x40 ), strdup("Digital Equipment Corp. PDP-10") ); 
    g_hash_table_insert( machines, GINT_TO_POINTER( 0x41 ), strdup("Digital Equipment Corp. PDP-11") ); 
    g_hash_table_insert( machines, GINT_TO_POINTER( 0x42 ), strdup("Siemens FX66 microcontroller") ); 
    g_hash_table_insert( machines, GINT_TO_POINTER( 0x43 ), strdup("STMicroelectronics ST9+ 8/16 bit microcontroller") ); 
    g_hash_table_insert( machines, GINT_TO_POINTER( 0x44 ), strdup("STMicroelectronics ST7 8-bit microcontroller") ); 
    g_hash_table_insert( machines, GINT_TO_POINTER( 0x45 ), strdup("Motorola MC68HC16 Microcontroller") ); 
    g_hash_table_insert( machines, GINT_TO_POINTER( 0x46 ), strdup("Motorola MC68HC11 Microcontroller") ); 
    g_hash_table_insert( machines, GINT_TO_POINTER( 0x47 ), strdup("Motorola MC68HC08 Microcontroller") ); 
    g_hash_table_insert( machines, GINT_TO_POINTER( 0x48 ), strdup("Motorola MC68HC05 Microcontroller") ); 
    g_hash_table_insert( machines, GINT_TO_POINTER( 0x49 ), strdup("Silicon Graphics SVx") ); 
    g_hash_table_insert( machines, GINT_TO_POINTER( 0x4A ), strdup("STMicroelectronics ST19 8-bit microcontroller") ); 
    g_hash_table_insert( machines, GINT_TO_POINTER( 0x4B ), strdup("Digital VAX") ); 
    g_hash_table_insert( machines, GINT_TO_POINTER( 0x4C ), strdup("Axis Communications 32-bit embedded processor") ); 
    g_hash_table_insert( machines, GINT_TO_POINTER( 0x4D ), strdup("Infineon Technologies 32-bit embedded processor") ); 
    g_hash_table_insert( machines, GINT_TO_POINTER( 0x4E ), strdup("Element 14 64-bit DSP Processor") ); 
    g_hash_table_insert( machines, GINT_TO_POINTER( 0x4F ), strdup("LSI Logic 16-bit DSP Processor") ); 
    g_hash_table_insert( machines, GINT_TO_POINTER( 0x8C ), strdup("TMS320C6000 Family") ); 
    g_hash_table_insert( machines, GINT_TO_POINTER( 0xAF ), strdup("MCST Elbrus e2k") ); 
    g_hash_table_insert( machines, GINT_TO_POINTER( 0xB7 ), strdup("ARM 64-bits (ARMv8/Aarch64)") ); 
    g_hash_table_insert( machines, GINT_TO_POINTER( 0xDC ), strdup("Zilog Z80") ); 
    g_hash_table_insert( machines, GINT_TO_POINTER( 0xF3 ), strdup("RISC-V") ); 
    g_hash_table_insert( machines, GINT_TO_POINTER( 0xF7 ), strdup("Berkeley Packet Filter") );
    g_hash_table_insert( machines, GINT_TO_POINTER( 0x101 ), strdup("WDC 65C816") );
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
    
    if(args_have_errors(argc, argv) != 0) {
        printf(CLI_MESSAGE_USAGE_HINT);
        return ERROR_WRONG_ARGUMENTS;
    }
    
    file_name = argv[1];
    fd = fopen(file_name, "rb"); 
    
    if(read_elf_header(fd, &elf_header) != 0) {
        printf(CLI_MESSAGE_FORMAT_NOT_RECOGNIZED, file_name);
        return ERROR_FILE_NOT_ELF;
    }

    char flags[19];
    snprintf( flags, 19, "0x%016x",  elf_header.e_flags);
    char magic_bytes[48];
    for(int i = 0; i < 16; i++) {
        snprintf(&magic_bytes[i*3], 3, "%02x", elf_header.e_ident[i]);
        if(i < 15) {
            snprintf(&magic_bytes[i*3 + 2], 2, " ");
        }
    }
    

    _machines = g_hash_table_new_full(g_direct_hash, g_direct_equal, NULL, g_free);
    create_hashtable_machines(_machines);

    file_info.file_name = file_name;
    file_info.magic_bytes = magic_bytes;
    switch(elf_header.e_ident[EI_CLASS]) {
        case 1:
            file_info.class = "ELF32";
            break;
        case 2:
            file_info.class = "ELF64";
            break;
        default:
            file_info.class = "N/A";
    }
    switch(elf_header.e_ident[EI_DATA]) {
        case 1:
            file_info.endiannes = "2's complement, little endian";
            break;
        case 2:
            file_info.endiannes = "2's complement, big endian";
            break;
        default:
            file_info.class = "N/A";
    }
    switch(elf_header.e_ident[EI_VERSION]) {
        case 1:
            file_info.version = "1 (current)";
            break;
        default:
            file_info.version = "N/A";
    }
    switch(elf_header.e_ident[EI_OSABI]) {
        case 0x00: file_info.os_abi = "UNIX - System V"; break;
        case 0x01: file_info.os_abi = "HP-UX"; break;
        case 0x02: file_info.os_abi = "NetBSD"; break;
        case 0x03: file_info.os_abi = "Linux"; break;
        case 0x04: file_info.os_abi = "GNU Hurd"; break;
        case 0x06: file_info.os_abi = "Solaris"; break;
        case 0x07: file_info.os_abi = "AIX (Monterey)"; break;
        case 0x08: file_info.os_abi = "IRIX"; break;
        case 0x09: file_info.os_abi = "FreeBSD"; break;
        case 0x0A: file_info.os_abi = "Tru64"; break;
        case 0x0B: file_info.os_abi = "Novell Modesto"; break;
        case 0x0C: file_info.os_abi = "OpenBSD"; break;
        case 0x0D: file_info.os_abi = "OpenVMS"; break;
        case 0x0E: file_info.os_abi = "NonStop Kernel"; break;
        case 0x0F: file_info.os_abi = "AROS"; break;
        case 0x10: file_info.os_abi = "FenixOS"; break;
        case 0x11: file_info.os_abi = "Nuxi CloudABI"; break;
        case 0x12: file_info.os_abi = "Stratus Technologies OpenVOS"; break;
        default: file_info.os_abi = "N/A";
    }
    char abi_version[4];
    snprintf(abi_version, 4, "%d", elf_header.e_ident[EI_ABIVERSION]);
    file_info.abi_version = abi_version;
    
    file_info.arch = find_machine_description(_machines, elf_header.e_machine);
    file_info.flags_hex = flags;

    print_elf_info(&file_info);
    // CLEAN-UP:
    g_hash_table_destroy( _machines );
    _machines = NULL;

    fclose(fd);
    return 0; 
    // Print out the contents of the map: g_hash_table_foreach( _machines, print_key_value, NULL );
}
