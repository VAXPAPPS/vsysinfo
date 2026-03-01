#ifndef GPU_H
#define GPU_H

typedef struct {
    char name[256];
    long vram_mb;
} GpuInfo;

void read_gpu_info(GpuInfo *info_array, int *count, int max_count);

#endif // GPU_H
