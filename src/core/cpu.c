#include "cpu.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

void read_static_cpu_info(CpuInfo *info) {
    FILE *fp = fopen("/proc/cpuinfo", "r");
    if (!fp) return;

    memset(info, 0, sizeof(CpuInfo));
    char line[256];
    int log_cores = 0;

    // Use a simple array to count unique physical cores if needed, 
    // but typically `cpu cores` in cpuinfo gives cores per sibling.
    // For simplicity, we'll extract it carefully.
    
    while (fgets(line, sizeof(line), fp)) {
        if (strncmp(line, "processor", 9) == 0) {
            log_cores++;
        } else if (strncmp(line, "vendor_id", 9) == 0 && info->vendor_id[0] == '\0') {
            sscanf(line, "vendor_id\t: %[^\n]", info->vendor_id);
        } else if (strncmp(line, "model name", 10) == 0 && info->model_name[0] == '\0') {
            sscanf(line, "model name\t: %[^\n]", info->model_name);
        } else if (strncmp(line, "cpu cores", 9) == 0 && info->physical_cores == 0) {
            sscanf(line, "cpu cores\t: %d", &info->physical_cores);
        } else if (strncmp(line, "cache size", 10) == 0 && info->cache_size_kb == 0) {
            int cache = 0;
            sscanf(line, "cache size\t: %d KB", &cache);
            info->cache_size_kb = cache;
        }
    }
    
    info->logical_cores = log_cores;
    if (info->physical_cores == 0) {
        info->physical_cores = log_cores; // fallback
    }

    fclose(fp);
}

void read_dynamic_cpu_info(CpuInfo *info) {
    // Read from /sys/devices/system/cpu/cpu*/cpufreq/scaling_cur_freq
    for (int i = 0; i < info->logical_cores && i < 128; i++) {
        char path[256];
        sprintf(path, "/sys/devices/system/cpu/cpu%d/cpufreq/scaling_cur_freq", i);
        FILE *fp = fopen(path, "r");
        if (fp) {
            long freq_khz;
            if (fscanf(fp, "%ld", &freq_khz) == 1) {
                info->current_freq_mhz[i] = freq_khz / 1000.0;
            }
            fclose(fp);
        } else {
            info->current_freq_mhz[i] = 0.0;
        }
    }
}
