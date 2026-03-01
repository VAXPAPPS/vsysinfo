#ifndef OS_INFO_H
#define OS_INFO_H

typedef struct {
    char os_name[256];
    char kernel_version[256];
    char os_type[128];
    char computer_model[256];
    char shell_type[128];
} OsInfo;

void read_os_info(OsInfo *info);

#endif // OS_INFO_H
