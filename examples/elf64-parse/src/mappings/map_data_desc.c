
char* map_data_desc(unsigned char data_id) {
    switch(data_id) {
        case 1:
            return "2's complement, little endian";
        case 2:
            return "2's complement, big endian";
    }
    return "N/A";
}
