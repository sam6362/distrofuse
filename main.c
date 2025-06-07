#include <stdio.h>
#include <string.h>

#include "parser.h"

int main(int argc, char *argv[]) {
    if (argc == 2 && strcmp(argv[1], "--version") == 0) {
        printf("%s\n", DISTFUSE_APP_VERSION);
        return 0;
    }
    
    return parse_arguments(argc, argv);
}