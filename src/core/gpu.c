#include "gpu.h"
#include <stdio.h>
#include <string.h>

void read_gpu_info(GpuInfo *info_array, int *count, int max_count) {
    *count = 0;
    
    // Use lspci -v to get memory info as well
    FILE *fp = popen("lspci -v", "r");
    if (!fp) return;

    char line[512];
    int current_gpu = -1;
    long current_vram_total = 0;

    while (fgets(line, sizeof(line), fp)) {
        // New device block
        if (line[0] != '\t' && line[0] != ' ') {
            if (current_gpu >= 0 && *count < max_count) {
                info_array[*count].vram_mb = current_vram_total;
                (*count)++;
                current_gpu = -1;
                current_vram_total = 0;
            }

            // Check if VGA or 3D controller
            if (strstr(line, "VGA compatible controller") || strstr(line, "3D controller") || strstr(line, "Display controller")) {
                current_gpu = *count;
                current_vram_total = 0;
                
                char *colon = strchr(line, ':');
                if (colon) {
                    colon = strchr(colon + 1, ':'); // second colon
                    if (colon) {
                        colon++;
                        while (*colon == ' ') colon++;
                        colon[strcspn(colon, "\n")] = 0;
                        
                        // Extract just the name, strip the " (rev xx)" if present
                        char *rev = strstr(colon, " (rev");
                        if (rev) *rev = '\0';
                        
                        strncpy(info_array[current_gpu].name, colon, 255);
                    }
                }
            }
        } 
        // Inside a device block
        else if (current_gpu >= 0) {
            // Look for "Memory at ... (size=XXX[KMG])"
            // specifically prefetchable memory usually indicates VRAM on dedicated GPUs
            // we will just sum up all memory regions marked as prefetchable, or all sizes if needed
            // "Memory at c0000000 (64-bit, prefetchable) [size=256M]"
            char *size_ptr = strstr(line, "[size=");
            if (size_ptr) {
                // Determine if it's VRAM. Usually it's prefetchable memory that constitutes the main VRAM.
                // We'll add all sizes for simplicity, or just prefetchable. Let's add all prefetchable sizes.
                if (strstr(line, "prefetchable")) {
                    size_ptr += 6; // skip "[size="
                    long amount = 0;
                    char unit = 0;
                    if (sscanf(size_ptr, "%ld%c", &amount, &unit) == 2) {
                        if (unit == 'M') current_vram_total += amount;
                        else if (unit == 'G') current_vram_total += amount * 1024;
                        else if (unit == 'K') current_vram_total += amount / 1024;
                    }
                }
            }
        }
    }
    
    // Save last one
    if (current_gpu >= 0 && *count < max_count) {
        info_array[*count].vram_mb = current_vram_total;
        (*count)++;
    }

    pclose(fp);
}
