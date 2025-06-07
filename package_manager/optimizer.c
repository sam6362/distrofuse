#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include "optimizer.h"

static double measure_latency(const char *command) {
    struct timeval start, end;
    char full_command[256];
    snprintf(full_command, sizeof(full_command), "%s --version", command);
    
    gettimeofday(&start, NULL);
    int ret = system(full_command);
    (void)ret;
    gettimeofday(&end, NULL);
    
    return (end.tv_sec - start.tv_sec) * 1000.0 + 
           (end.tv_usec - start.tv_usec) / 1000.0;
}

static int check_package_available(const char *manager, const char *package) {
    char command[256];
    snprintf(command, sizeof(command), "%s search %s > /dev/null 2>&1", manager, package);
    return system(command) == 0;
}

int find_best_manager(const char *package_name, PackageManager managers[], int count, ManagerPerformance *result) {
    ManagerPerformance best = {0};
    best.latency_ms = 999999.9;
    int found = 0;

    for (int i = 0; i < count; i++) {
        if (!managers[i].available) continue;
        
        ManagerPerformance perf;
        strncpy(perf.manager_name, managers[i].name, MANAGER_NAME_LEN - 1);
        perf.manager_name[MANAGER_NAME_LEN - 1] = '\0';
        
        perf.package_available = check_package_available(managers[i].command, package_name);
        if (!perf.package_available) continue;
        
        perf.latency_ms = measure_latency(managers[i].command);
        
        perf.download_speed_mbps = 100.0 - perf.latency_ms;
        
        if (perf.latency_ms < best.latency_ms) {
            best = perf;
            found = 1;
        }
    }
    
    if (found) {
        *result = best;
        return 1;
    }
    return 0;
}