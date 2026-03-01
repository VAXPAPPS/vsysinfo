#ifndef CPU_H
#define CPU_H

typedef struct {
    char model_name[256];
    char vendor_id[128];
    int physical_cores;
    int logical_cores;
    double current_freq_mhz[128]; // Max 128 cores for simplicity
    double cache_size_kb; // L3 or max cache
} CpuInfo;

// Function to read static CPU info (model, vendor, cores)
void read_static_cpu_info(CpuInfo *info);

// Function to read dynamic CPU info (frequencies)
void read_dynamic_cpu_info(CpuInfo *info);

#endif // CPU_H
