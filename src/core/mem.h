#ifndef MEM_H
#define MEM_H

typedef struct {
    double total_gb;
    double used_gb;
    double free_gb;
    double available_gb;
    double swap_total_gb;
    double swap_used_gb;
    double swap_free_gb;
} MemInfo;

// Function to read memory info
void read_mem_info(MemInfo *info);

#endif // MEM_H
