#include <stdio.h>

char* map_raw_bytes_desc(unsigned char* magic_bytes_arr, size_t n_bytes, char* out_str) {
    for(int i = 0; i < n_bytes; i++) {
        snprintf(&out_str[i*3], 3, "%02x", magic_bytes_arr[i]);
        if(i < n_bytes - 1) {
            snprintf(&out_str[i*3 + 2], 2, " ");
        }
    }
    return out_str;
}
