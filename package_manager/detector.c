#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "detector.h"

void detect_available_managers(PackageManager managers[], int *count) {
    const char *manager_names[] = {"snap", "flatpak", "dnf", "apt", "pacman", "zypper", "apk", "yum", "rpm"};
    const char *manager_paths[] = {"/usr/bin/snap", "/usr/bin/flatpak", "/usr/bin/dnf", "/usr/bin/apt", "/usr/bin/pacman", 
                                  "/usr/bin/zypper", "/usr/bin/apk", "/usr/bin/yum", "/usr/bin/rpm"};

    size_t num_managers = sizeof(manager_paths)/sizeof(manager_paths[0]);
    
    *count = 0;
    for (size_t i = 0; i < num_managers; i++) {
        if (access(manager_paths[i], F_OK) == 0) {
            strncpy(managers[*count].name, manager_names[i], MANAGER_NAME_LEN - 1);
            managers[*count].name[MANAGER_NAME_LEN - 1] = '\0';
            
            strncpy(managers[*count].command, manager_names[i], MANAGER_NAME_LEN - 1);
            managers[*count].command[MANAGER_NAME_LEN - 1] = '\0';
            
            managers[*count].available = 1;
            (*count)++;
            if (*count >= MAX_MANAGERS) break;
        }
    }
}