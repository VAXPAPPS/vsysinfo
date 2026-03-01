#include "os_info.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/utsname.h>

void read_os_info(OsInfo *info) {
    memset(info, 0, sizeof(OsInfo));

    // OS Name from /etc/os-release
    FILE *fp = fopen("/etc/os-release", "r");
    if (fp) {
        char line[512];
        while (fgets(line, sizeof(line), fp)) {
            if (strncmp(line, "PRETTY_NAME=", 12) == 0) {
                char *start = line + 12;
                if (*start == '"') start++;
                char *end = strchr(start, '"');
                if (!end) end = strchr(start, '\n');
                if (end) *end = '\0';
                strncpy(info->os_name, start, 255);
                break;
            }
        }
        fclose(fp);
    }
    if (info->os_name[0] == '\0') {
        strcpy(info->os_name, "Unknown Linux");
    }

    // Kernel & OS Type
    struct utsname buffer;
    if (uname(&buffer) == 0) {
        strncpy(info->kernel_version, buffer.release, 255);
        strncpy(info->os_type, buffer.machine, 127);
    } else {
        strcpy(info->kernel_version, "Unknown");
        strcpy(info->os_type, "Unknown");
    }

    // Computer Model from sysfs
    FILE *mfp = fopen("/sys/class/dmi/id/sys_vendor", "r");
    FILE *pfp = fopen("/sys/class/dmi/id/product_name", "r");
    char vendor[128] = "Unknown";
    char product[128] = "Unknown";
    
    if (mfp) {
        if(fgets(vendor, sizeof(vendor), mfp)) {
            vendor[strcspn(vendor, "\n")] = 0;
        }
        fclose(mfp);
    }
    if (pfp) {
        if(fgets(product, sizeof(product), pfp)) {
            product[strcspn(product, "\n")] = 0;
        }
        fclose(pfp);
    }
    
    snprintf(info->computer_model, 255, "%s %s", vendor, product);

    // Shell execution path
    char *shell = getenv("SHELL");
    if (shell) {
        char *last_slash = strrchr(shell, '/');
        if (last_slash) {
            strncpy(info->shell_type, last_slash + 1, 127);
        } else {
            strncpy(info->shell_type, shell, 127);
        }
    } else {
        strcpy(info->shell_type, "Unknown");
    }
}
