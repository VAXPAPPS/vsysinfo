#ifndef STORAGE_H
#define STORAGE_H

typedef struct {
    char mount_point[256];
    double total_gb;
    double used_gb;
    double free_gb;
    double percentage_used;
} StorageInfo;

// MAX 10 mount points for simplicity
void read_storage_info(StorageInfo *info_array, int *count);

#endif // STORAGE_H
