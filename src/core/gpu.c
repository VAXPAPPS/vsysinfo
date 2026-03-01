#include "gpu.h"
#include <stdio.h>
#include <string.h>

void read_gpu_info(GpuInfo *info_array, int *count, int max_count) {
    *count = 0;
    
    // Simplest reliable way without external libpci dependency: lspci command stream
    FILE *fp = popen("lspci | grep -i 'vga\\|3d\\|display'", "r");
    if (!fp) return;

    char line[512];
    while (fgets(line, sizeof(line), fp) && *count < max_count) {
        // Line format usually: "00:02.0 VGA compatible controller: Intel Corporation ..."
        char *colon = strchr(line, ':');
        if (colon) {
            colon = strchr(colon + 1, ':'); // second colon
            if (colon) {
                // Skip the colon and leading spaces
                colon++;
                while (*colon == ' ') colon++;
                
                // Remove trailing newline
                colon[strcspn(colon, "\n")] = 0;
                
                strncpy(info_array[*count].name, colon, 255);
                (*count)++;
            }
        }
    }
    pclose(fp);
}
