#include <elf.h>

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