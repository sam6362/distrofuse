#ifndef PACKAGE_MANAGER_OPTIMIZER_H
#define PACKAGE_MANAGER_OPTIMIZER_H

#include "detector.h"

typedef struct {
    char manager_name[MANAGER_NAME_LEN];
    double latency_ms;
    double download_speed_mbps;
    int package_available;
} ManagerPerformance;

int find_best_manager(const char *package_name, PackageManager managers[], int count, ManagerPerformance *result);

#endif