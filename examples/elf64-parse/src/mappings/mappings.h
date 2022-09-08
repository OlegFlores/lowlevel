#include <elf.h>

char* map_class_desc(unsigned char class_id);
char* map_data_desc(unsigned char data_id);
char* map_machine_desc(Elf64_Half machine_value);
char* map_object_file_type_desc(Elf64_Half object_file_type_id);
char* map_osabi_desc(unsigned char osabi_id);
char* map_raw_bytes_desc(unsigned char* magic_bytes_arr, size_t n_bytes, char* out_str);
char* map_version_desc(unsigned char version_id);