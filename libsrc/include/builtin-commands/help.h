#ifndef COMMAND_PARSER__BUILTIN__HELP_H__
#define COMMAND_PARSER__BUILTIN__HELP_H__

#include "command_types.h"

#include <stdio.h>

bool command_tree_add_help(command_tree_s* command_tree);
bool handle_help_command(const command_tree_s* command_tree, const command_s* called_command);

void print_global_help(FILE* stream, const command_tree_s* command_tree);
void print_command_help(FILE* stream, const command_s* command);
void print_option_help(FILE* stream, const option_s* option);

#endif // !COMMAND_PARSER__BUILTIN__HELP_H__

