#include "storage.h"
#include <stdio.h>
#include <string.h>
#include <sys/statvfs.h>
#include <mntent.h>

void read_storage_info(StorageInfo *info_array, int *count) {
    *count = 0;
    FILE *fp = setmntent("/etc/mtab", "r");
    if (!fp) return;

    struct mntent *ent;
    while ((ent = getmntent(fp)) != NULL && *count < 10) {
        // Filter pseudo-filesystems
        if (strncmp(ent->mnt_fsname, "/dev/", 5) == 0) {
            struct statvfs stat;
            if (statvfs(ent->mnt_dir, &stat) == 0) {
                double block_size = stat.f_frsize;
                double total = (stat.f_blocks * block_size) / (1024.0 * 1024.0 * 1024.0);
                double free = (stat.f_bfree * block_size) / (1024.0 * 1024.0 * 1024.0);
                double used = total - free;

                if (total > 0) {
                    strncpy(info_array[*count].mount_point, ent->mnt_dir, 255);
                    info_array[*count].total_gb = total;
                    info_array[*count].free_gb = free;
                    info_array[*count].used_gb = used;
                    info_array[*count].percentage_used = (used / total) * 100.0;
                    (*count)++;
                }
            }
        }
    }
    endmntent(fp);
}
