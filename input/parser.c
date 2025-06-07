#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "parser.h"
#include "detector.h"
#include "optimizer.h"

char validcommands[] = "install, remove";

int parse_arguments(int argc, char *argv[]) {
    if (argc < 2) {
        printf("[ERR] Not enough parameters entered.\n");
        printf("[LOG] Usage: %s <command> [args...]\n", argv[0]);
        return 1;
    }

    char *command = argv[1];

    if (strcmp(command, "distfuse") == 0) {
        if (argc < 3) {
            printf("[ERR] Subcommand required for the 'distfuse' command.\n");
            printf("[LOG] Usage: %s distfuse <subcommand> [args...]\n", argv[0]);
            return 1;
        }
        
        char *subcommand_arg = argv[2];
        char *packageName_arg = (argc > 3) ? argv[3] : NULL;

        if (subcommand_check(subcommand_arg, packageName_arg) != 0) {
            printf("[ERR] Unidentified 'distfuse' subcommand or incomplete argument: %s\n", subcommand_arg);
            printf("[LOG] Valid subcommands: %s\n", validcommands);
            return 1;
        }
    } else {
        printf("[ERR] Unidentified command: %s\n", command);
        printf("[LOG] Valid commands: distfuse\n");
        return 1;
    }

    return 0;
}

int subcommand_check(char *subcommand, char *packageName) {
    if (strcmp(subcommand, "install") == 0) {
        if (packageName == NULL) {
            printf("[ERR] Package name required for 'install' command.\n");
            return 1;
        }
        
        PackageManager managers[MAX_MANAGERS];
        int manager_count;
        detect_available_managers(managers, &manager_count);
        
        if (manager_count == 0) {
            printf("[ERR] No available package managers found!\n");
            return 1;
        }
        
        ManagerPerformance best;
        if (!find_best_manager(packageName, managers, manager_count, &best)) {
            printf("[ERR] Package '%s' not found in any available manager\n", packageName);
            return 1;
        }
        
        printf("[LOG] Installing '%s' using %s (latency: %.2f ms)\n", 
               packageName, best.manager_name, best.latency_ms);
        
        char command[256];
        snprintf(command, sizeof(command), "%s install -y %s", best.manager_name, packageName);
        printf("[EXEC] %s\n", command);
        int ret = system(command);
        if (ret != 0) {
            printf("[WARN] Command execution returned non-zero exit code: %d\n", ret);
        }
        
        return 0;
    } else if (strcmp(subcommand, "remove") == 0) {
        if (packageName == NULL) {
            printf("[ERR] Package name required for 'remove' command.\n");
            return 1;
        }
        
        PackageManager managers[MAX_MANAGERS];
        int manager_count;
        detect_available_managers(managers, &manager_count);
        
        if (manager_count == 0) {
            printf("[ERR] No available package managers found!\n");
            return 1;
        }
        
        ManagerPerformance best;
        if (!find_best_manager(packageName, managers, manager_count, &best)) {
            printf("[ERR] Package '%s' not found in any available manager\n", packageName);
            return 1;
        }
        
        printf("[LOG] Removing '%s' using %s\n", packageName, best.manager_name);
        
        char command[256];
        snprintf(command, sizeof(command), "%s remove -y %s", best.manager_name, packageName);
        printf("[EXEC] %s\n", command);
        int ret = system(command);
        if (ret != 0) {
            printf("[WARN] Command execution returned non-zero exit code: %d\n", ret);
        }
        
        return 0;
    } else {
        return 1; // Unidentified subcommand
    }
}