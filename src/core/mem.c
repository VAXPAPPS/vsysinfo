#include "mem.h"
#include <stdio.h>
#include <string.h>

void read_mem_info(MemInfo *info) {
    FILE *fp = fopen("/proc/meminfo", "r");
    if (!fp) return;

    memset(info, 0, sizeof(MemInfo));
    char line[256];
    
    long mem_total = 0, mem_free = 0, mem_available = 0, buffers = 0, cached = 0;
    long swap_total = 0, swap_free = 0;

    while (fgets(line, sizeof(line), fp)) {
        if (sscanf(line, "MemTotal: %ld kB", &mem_total) == 1) continue;
        if (sscanf(line, "MemFree: %ld kB", &mem_free) == 1) continue;
        if (sscanf(line, "MemAvailable: %ld kB", &mem_available) == 1) continue;
        if (sscanf(line, "Buffers: %ld kB", &buffers) == 1) continue;
        if (sscanf(line, "Cached: %ld kB", &cached) == 1) continue;
        if (sscanf(line, "SwapTotal: %ld kB", &swap_total) == 1) continue;
        if (sscanf(line, "SwapFree: %ld kB", &swap_free) == 1) continue;
    }

    fclose(fp);

    // Convert kB to GB (1 GB = 1024 * 1024 kB)
    double kb_to_gb = 1024.0 * 1024.0;
    
    info->total_gb = mem_total / kb_to_gb;
    info->free_gb = mem_free / kb_to_gb;
    
    if (mem_available > 0) {
        info->available_gb = mem_available / kb_to_gb;
        info->used_gb = info->total_gb - info->available_gb;
    } else {
        // Fallback for older kernels
        long used = mem_total - mem_free - buffers - cached;
        info->used_gb = used / kb_to_gb;
        info->available_gb = (mem_free + buffers + cached) / kb_to_gb;
    }

    info->swap_total_gb = swap_total / kb_to_gb;
    info->swap_free_gb = swap_free / kb_to_gb;
    info->swap_used_gb = (swap_total - swap_free) / kb_to_gb;
}
