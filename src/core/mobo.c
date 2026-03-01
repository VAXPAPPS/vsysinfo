#include "mobo.h"
#include <stdio.h>
#include <string.h>

static void read_sys_file(const char *path, char *out, size_t max_len) {
    FILE *fp = fopen(path, "r");
    if (fp) {
        if (fgets(out, max_len, fp)) {
            // Remove newline
            out[strcspn(out, "\n")] = 0;
        } else {
            strcpy(out, "Unknown");
        }
        fclose(fp);
    } else {
        strcpy(out, "Permission Denied / Unknown");
    }
}

void read_mobo_info(MoboInfo *info) {
    memset(info, 0, sizeof(MoboInfo));

    read_sys_file("/sys/class/dmi/id/board_vendor", info->vendor, sizeof(info->vendor));
    read_sys_file("/sys/class/dmi/id/board_name", info->name, sizeof(info->name));
    read_sys_file("/sys/class/dmi/id/board_version", info->version, sizeof(info->version));
    
    read_sys_file("/sys/class/dmi/id/bios_vendor", info->bios_vendor, sizeof(info->bios_vendor));
    read_sys_file("/sys/class/dmi/id/bios_version", info->bios_version, sizeof(info->bios_version));
    read_sys_file("/sys/class/dmi/id/bios_date", info->bios_date, sizeof(info->bios_date));
}
