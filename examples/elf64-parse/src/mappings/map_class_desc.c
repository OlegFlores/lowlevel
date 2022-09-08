char* map_class_desc(unsigned char class_id) {
    switch(class_id) {
        case 1:
            return "ELF32";
        case 2:
            return "ELF64";
    }
    return "N/A";
}