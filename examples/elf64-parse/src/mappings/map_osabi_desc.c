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