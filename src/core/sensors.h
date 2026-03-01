#ifndef SENSORS_H
#define SENSORS_H

typedef struct {
    char label[128];
    double temp_celsius;
} SensorInfo;

void read_sensors_info(SensorInfo *info_array, int *count, int max_count);

#endif // SENSORS_H
