#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "ui.h"

CommandResult* execute_command_silent(const char* command) {
    CommandResult* result = malloc(sizeof(CommandResult));
    if (!result) return NULL;
    
    result->stdout_output = NULL;
    result->stderr_output = NULL;
    result->stdout_size = 0;
    result->stderr_size = 0;
    result->exit_code = -1;
    
    // Create temporary files for output capture
    char stdout_file[] = "/tmp/distfuse_stdout_XXXXXX";
    char stderr_file[] = "/tmp/distfuse_stderr_XXXXXX";
    
    int stdout_fd = mkstemp(stdout_file);
    int stderr_fd = mkstemp(stderr_file);
    
    if (stdout_fd == -1 || stderr_fd == -1) {
        free(result);
        return NULL;
    }
    
    // Build command with redirections
    char full_command[1024];
    snprintf(full_command, sizeof(full_command), 
             "%s > %s 2> %s", command, stdout_file, stderr_file);
    
    // Execute command
    result->exit_code = system(full_command);
    
    // Read stdout
    close(stdout_fd);
    FILE* stdout_fp = fopen(stdout_file, "r");
    if (stdout_fp) {
        fseek(stdout_fp, 0, SEEK_END);
        result->stdout_size = ftell(stdout_fp);
        fseek(stdout_fp, 0, SEEK_SET);
        
        if (result->stdout_size > 0) {
            result->stdout_output = malloc(result->stdout_size + 1);
            if (result->stdout_output) {
                fread(result->stdout_output, 1, result->stdout_size, stdout_fp);
                result->stdout_output[result->stdout_size] = '\0';
            }
        }
        fclose(stdout_fp);
    }
    
    // Read stderr
    close(stderr_fd);
    FILE* stderr_fp = fopen(stderr_file, "r");
    if (stderr_fp) {
        fseek(stderr_fp, 0, SEEK_END);
        result->stderr_size = ftell(stderr_fp);
        fseek(stderr_fp, 0, SEEK_SET);
        
        if (result->stderr_size > 0) {
            result->stderr_output = malloc(result->stderr_size + 1);
            if (result->stderr_output) {
                fread(result->stderr_output, 1, result->stderr_size, stderr_fp);
                result->stderr_output[result->stderr_size] = '\0';
            }
        }
        fclose(stderr_fp);
    }
    
    // Clean up temp files
    unlink(stdout_file);
    unlink(stderr_file);
    
    return result;
}

int execute_with_custom_log(const char* command, const char* operation, const char* package) {
    printf("🔧 Executing: %s %s\n", operation, package);
    
    CommandResult* result = execute_command_silent(command);
    if (!result) {
        ui_show_error("Failed to execute command");
        return -1;
    }
    
    // Check if command was successful
    if (result->exit_code == 0) {
        printf("✅ %s completed successfully for package: %s\n", operation, package);
    } else {
        printf("❌ %s failed for package: %s (exit code: %d)\n", 
               operation, package, result->exit_code);
        
        // Show relevant error info without spam
        if (result->stderr_output && result->stderr_size > 0) {
            // Filter out common noise
            if (strstr(result->stderr_output, "Permission denied")) {
                ui_show_error("Permission denied - try running with sudo");
            } else if (strstr(result->stderr_output, "not found")) {
                ui_show_error("Package not found in repositories");
            } else if (strstr(result->stderr_output, "lock")) {
                ui_show_error("Package manager is locked - another process may be running");
            } else {
                // Show first line of error only
                char* first_line = strtok(result->stderr_output, "\n");
                if (first_line) {
                    ui_show_error(first_line);
                }
            }
        }
    }
    
    int exit_code = result->exit_code;
    free_command_result(result);
    return exit_code;
}

void free_command_result(CommandResult* result) {
    if (!result) return;
    
    if (result->stdout_output) free(result->stdout_output);
    if (result->stderr_output) free(result->stderr_output);
    free(result);
}

int command_exists(const char* command) {
    char check_cmd[256];
    snprintf(check_cmd, sizeof(check_cmd), "which %s > /dev/null 2>&1", command);
    return system(check_cmd) == 0;
}