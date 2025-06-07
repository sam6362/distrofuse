#ifndef SILENT_EXEC_H
#define SILENT_EXEC_H

#include <stdio.h>

typedef struct {
    int exit_code;
    char* stdout_output;
    char* stderr_output;
    size_t stdout_size;
    size_t stderr_size;
} CommandResult;

CommandResult* execute_command_silent(const char* command);

int execute_with_custom_log(const char* command, const char* operation, const char* package);

void free_command_result(CommandResult* result);

int command_exists(const char* command);

#endif