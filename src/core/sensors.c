#include "sensors.h"
#include <stdio.h>
#include <string.h>
#include <dirent.h>

void read_sensors_info(SensorInfo *info_array, int *count, int max_count) {
    *count = 0;
    
    // Simple sysfs hwmon parsing
    DIR *dir = opendir("/sys/class/hwmon");
    if (!dir) return;

    struct dirent *ent;
    while ((ent = readdir(dir)) != NULL && *count < max_count) {
        if (ent->d_name[0] == '.') continue;

        char path[512];
        char label[128] = "Unknown";
        
        // Loop over potential temp inputs (temp1_input, temp2_input, etc.)
        for (int i = 1; i <= 5 && *count < max_count; i++) {
            sprintf(path, "/sys/class/hwmon/%s/temp%d_input", ent->d_name, i);
            FILE *fp = fopen(path, "r");
            if (fp) {
                long temp_milli;
                if (fscanf(fp, "%ld", &temp_milli) == 1) {
                    // Try to read label
                    char label_path[512];
                    sprintf(label_path, "/sys/class/hwmon/%s/temp%d_label", ent->d_name, i);
                    FILE *lfp = fopen(label_path, "r");
                    if (lfp) {
                        if (fgets(label, sizeof(label), lfp)) {
                            label[strcspn(label, "\n")] = 0;
                        }
                        fclose(lfp);
                    } else {
                        // Try name of hwmon
                        sprintf(label_path, "/sys/class/hwmon/%s/name", ent->d_name);
                        lfp = fopen(label_path, "r");
                        if (lfp) {
                            if (fgets(label, sizeof(label), lfp)) {
                                label[strcspn(label, "\n")] = 0;
                                char suffix[16];
                                sprintf(suffix, " %d", i);
                                strcat(label, suffix);
                            }
                            fclose(lfp);
                        }
                    }

                    strncpy(info_array[*count].label, label, 127);
                    info_array[*count].temp_celsius = temp_milli / 1000.0;
                    (*count)++;
                }
                fclose(fp);
            }
        }
    }
    closedir(dir);
}
