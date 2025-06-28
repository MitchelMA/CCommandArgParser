#ifndef COMMAND_PARSER__COMMAND_TREE_H__
#define COMMAND_PARSER__COMMAND_TREE_H__

#include "command_types.h"

bool command_tree_init(command_tree_s* tree, size_t command_capacity);
void command_tree_clean(command_tree_s* tree);

bool command_tree_add_command(command_tree_s* tree, command_s* command);

#endif // !COMMAND_PARSER__COMMAND_TREE_H__

