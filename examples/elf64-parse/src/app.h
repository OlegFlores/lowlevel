
#define MAGIC_ELF_LENGTH 4

#define ERROR_WRONG_ARGUMENTS 10
#define ERROR_FILE_NOT_ELF 20

#define CLI_MESSAGE_USAGE_HINT "Run: `elf64-parse _elf64_file_path_`\n"
#define CLI_MESSAGE_FORMAT_NOT_RECOGNIZED "elf64-parse: %s: file format not recognized\n"
#define CLI_MESSAGE_FORMAT_ELF32 "elf64-x86-32"
#define CLI_MESSAGE_FORMAT_ELF64 "elf64-x86-64"

const char MAGIC_ELF[MAGIC_ELF_LENGTH] = { 0x7f, 'E', 'L', 'F' };

typedef struct elf64_info_struct {
    char* file_name;
    char* magic_bytes; // space delimeted 16 first bytes
    char* class; // ELF64
    char* endiannes; // 2's complement, little endian
    char* version; // 1 (current)
    char* os_abi; // UNIX - System V
    char* abi_version; // 0
    char* arch;
    char* flags_hex;
    char* link_info;
    char* start_addr_hex;
} elf64_info;

int run_app(int argc, char **argv);

