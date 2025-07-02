#ifndef COMMAND_PARSER__COMMAND_TREE_H__
#define COMMAND_PARSER__COMMAND_TREE_H__

#include "command_types.h"

bool command_tree_init(command_tree_s* tree, size_t command_capacity);
bool command_tree_set_description(command_tree_s* tree, const char* description);
void command_tree_clean(command_tree_s* tree);

bool command_tree_add_command(command_tree_s* tree, command_s* command);
const command_s* command_tree_get_command(const command_tree_s* tree, const char* command_flag);
bool command_tree_has_command(const command_tree_s* tree, const char* command_flag);

bool command_tree_parse_base(command_tree_s* tree, int argc, const char** argv);
command_s* command_tree_get_called_command(command_tree_s* tree);

#endif // !COMMAND_PARSER__COMMAND_TREE_H__

