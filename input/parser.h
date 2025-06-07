// This section: reading program parametters
#ifndef PARSER_H
#define PARSER_H

int parse_arguments(int argc, char *argv[]); 
int subcommand_check(char *subcommand, char *packageName);

#endif