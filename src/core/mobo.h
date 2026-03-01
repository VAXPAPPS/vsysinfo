#ifndef MOBO_H
#define MOBO_H

typedef struct {
    char vendor[128];
    char name[128];
    char version[128];
    char bios_vendor[128];
    char bios_version[128];
    char bios_date[128];
} MoboInfo;

void read_mobo_info(MoboInfo *info);

#endif // MOBO_H
