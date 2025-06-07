#ifndef PACKAGE_MANAGER_DETECTOR_H
#define PACKAGE_MANAGER_DETECTOR_H

#define MAX_MANAGERS 10
#define MANAGER_NAME_LEN 20

typedef struct {
    char name[MANAGER_NAME_LEN];
    char command[MANAGER_NAME_LEN];
    int available;
} PackageManager;

void detect_available_managers(PackageManager managers[], int *count);

#endif