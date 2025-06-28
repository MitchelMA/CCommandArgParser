#include "command.h"

#include <stdlib.h>
#include <string.h>

bool command_tree_init(command_tree_s* tree, size_t command_capacity)
{
    if (tree == NULL)
        return false;

    tree->commands = malloc(sizeof(command_s) * command_capacity);
    if (tree->commands == NULL)
        return false;

    tree->command_capacity = command_capacity;
    tree->command_count = 0;
    return true;
}

void command_tree_clean(command_tree_s* tree)
{
    if (tree == NULL)
        return;

    if (tree->command_capacity == 0)
        return;

    // TODO: Loop over all commands in the tree and call the future clean function
  
    
    free(tree->commands);
    tree->commands = NULL;
}

bool command_tree_add_command(command_tree_s* tree, command_s* command)
{
    if (tree == NULL)
        return false;

    if (tree->command_count >= tree->command_capacity)
        return false;

    memcpy(&tree->commands[tree->command_count], command, sizeof(command_s));
    return true;
}
