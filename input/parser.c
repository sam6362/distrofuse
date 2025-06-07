#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>

#include "parser.h"
#include "detector.h"
#include "optimizer.h"
#include "ui.h"

char validcommands[] = "install, remove";

int execute_package_command(const char* manager, const char* action, const char* package) {
    char command[512];
    char temp_file[] = "/tmp/distfuse_output_XXXXXX";
    int temp_fd = mkstemp(temp_file);
    
    if (temp_fd == -1) {
        ui_show_error("Failed to create temporary file");
        return -1;
    }
    close(temp_fd);
    
    if (strcmp(action, "install") == 0) {
        if (strcmp(manager, "apt") == 0) {
            snprintf(command, sizeof(command), 
                    "DEBIAN_FRONTEND=noninteractive apt-get install -y %s > %s 2>&1", 
                    package, temp_file);
        } else if (strcmp(manager, "dnf") == 0) {
            snprintf(command, sizeof(command), 
                    "dnf install -y %s > %s 2>&1", 
                    package, temp_file);
        } else if (strcmp(manager, "pacman") == 0) {
            snprintf(command, sizeof(command), 
                    "pacman -S --noconfirm %s > %s 2>&1", 
                    package, temp_file);
        } else if (strcmp(manager, "flatpak") == 0) {
            snprintf(command, sizeof(command), 
                    "flatpak install -y flathub %s > %s 2>&1", 
                    package, temp_file);
        } else {
            snprintf(command, sizeof(command), 
                    "%s install -y %s > %s 2>&1", 
                    manager, package, temp_file);
        }
    } else if (strcmp(action, "remove") == 0) {
        if (strcmp(manager, "apt") == 0) {
            snprintf(command, sizeof(command), 
                    "DEBIAN_FRONTEND=noninteractive apt-get remove -y %s > %s 2>&1", 
                    package, temp_file);
        } else if (strcmp(manager, "dnf") == 0) {
            snprintf(command, sizeof(command), 
                    "dnf remove -y %s > %s 2>&1", 
                    package, temp_file);
        } else if (strcmp(manager, "pacman") == 0) {
            snprintf(command, sizeof(command), 
                    "pacman -R --noconfirm %s > %s 2>&1", 
                    package, temp_file);
        } else if (strcmp(manager, "flatpak") == 0) {
            snprintf(command, sizeof(command), 
                    "flatpak uninstall -y %s > %s 2>&1", 
                    package, temp_file);
        } else {
            snprintf(command, sizeof(command), 
                    "%s remove -y %s > %s 2>&1", 
                    manager, package, temp_file);
        }
    } 
    
    int result = system(command);
    
    if (result != 0) {
        FILE* output_file = fopen(temp_file, "r");
        if (output_file) {
            char line[256];
            int found_error = 0;
            
            while (fgets(line, sizeof(line), output_file) && !found_error) {
                if (strstr(line, "Permission denied") || strstr(line, "permission denied")) {
                    ui_show_error("Permission denied - please run with sudo");
                    found_error = 1;
                } else if (strstr(line, "Unable to locate package") || strstr(line, "not found")) {
                    ui_show_error("Package not found in repositories");
                    found_error = 1;
                } else if (strstr(line, "dpkg: error") || strstr(line, "lock")) {
                    ui_show_error("Package manager is busy - please wait and try again");
                    found_error = 1;
                }
            }
            
            if (!found_error) {
                ui_show_error("Package operation failed - check your internet connection");
            }
            
            fclose(output_file);
        }
    } else {
        ui_show_success("Package operation completed successfully!");
    }
    
    unlink(temp_file);
    return result;
}

int parse_arguments(int argc, char *argv[]) {
    if (argc < 2) {
        printf("❌ Not enough parameters entered.\n");
        printf("💡 Usage: %s <command> [args...]\n", argv[0]);
        return 1;
    }

    char *subcommand_arg = argv[1];
    char *packageName_arg = (argc > 2) ? argv[2] : NULL;

    if (subcommand_check(subcommand_arg, packageName_arg) != 0) {
        printf("❌ Unidentified subcommand or incomplete argument: %s\n", subcommand_arg);
        printf("💡 Valid subcommands: %s\n", validcommands);
        return 1;
    }

    return 0;
}

int subcommand_check(char *subcommand, char *packageName) {
    if (strcmp(subcommand, "install") == 0) {
        if (packageName == NULL) {
            printf("❌ Package name required for 'install' command.\n");
            return 1;
        }
        
        ui_set_package_status(packageName, "Detecting...");
        draw_ui();
        
        PackageManager managers[MAX_MANAGERS];
        int manager_count;
        detect_available_managers(managers, &manager_count);
        
        if (manager_count == 0) {
            ui_show_error("No available package managers found!");
            return 1;
        }
        
        ManagerPerformance best;
        if (!find_best_manager(packageName, managers, manager_count, &best)) {
            ui_show_error("Package not found in any available manager");
            return 1;
        }
        
        ui_set_package_status(packageName, best.manager_name);
        draw_ui();
        
        printf("🚀 Installing '%s' using %s (latency: %.2fms)\n", 
               packageName, best.manager_name, best.latency_ms);
        
        ui_update_download_progress();
        
        ui_update_install_progress();
        
        int result = execute_package_command(best.manager_name, "install", packageName);
        
        ui_finish();
        
        return (result == 0) ? 0 : 1;
        
    } else if (strcmp(subcommand, "remove") == 0) {
        if (packageName == NULL) {
            printf("❌ Package name required for 'remove' command.\n");
            return 1;
        }
        
        ui_set_package_status(packageName, "Detecting...");
        draw_ui();
        
        PackageManager managers[MAX_MANAGERS];
        int manager_count;
        detect_available_managers(managers, &manager_count);
        
        if (manager_count == 0) {
            ui_show_error("No available package managers found!");
            return 1;
        }
        
        ManagerPerformance best;
        if (!find_best_manager(packageName, managers, manager_count, &best)) {
            ui_show_error("Package not found in any available manager");
            return 1;
        }
        
        ui_set_package_status(packageName, best.manager_name);
        draw_ui();
        
        printf("🗑️  Removing '%s' using %s\n", packageName, best.manager_name);
        
        ui_update_remove_progress();
        
        int result = execute_package_command(best.manager_name, "remove", packageName);
        
        ui_finish();
        
        return (result == 0) ? 0 : 1;
        
    } else {
        return 1;
    }
}