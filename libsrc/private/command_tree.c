#include "command_tree.h"

#include "command.h"
#include "notation.h"
#include "arguments.h"

#include <stdlib.h>
#include <string.h>

bool command_tree_init(command_tree_s* tree, size_t command_capacity)
{
    if (tree == NULL || command_capacity == 0)
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
    if (tree == NULL || tree->command_capacity == 0)
        return;

    for (size_t i = 0; i < tree->command_count; ++i)
        command_clean(&tree->commands[i]);
    
    free(tree->commands);
    tree->commands = NULL;

    arguments_clean(&tree->parsed_arguments);
}

bool command_tree_add_command(command_tree_s* tree, command_s* command)
{
    if (tree == NULL)
        return false;

    if (tree->command_count >= tree->command_capacity)
        return false;

    memcpy(&tree->commands[tree->command_count], command, sizeof(command_s));
    tree->command_count++;
    return true;
}

bool command_tree_parse_base(command_tree_s* tree, int argc, const char** argv)
{
    if (tree == NULL)
        return false;

    if (argc <= 1)
        return false;

    arguments_init(&tree->parsed_arguments, *argv, argc-1, argv+1);

    // skip the calling path that's normally at argv[0]
    argc--;
    argv++;

    const char* searching_flag_name = *argv;
    if (!is_flag(searching_flag_name))
        return false;

    bool found_target = false;

    for (size_t i = 0; i < tree->command_count; ++i)
    {
        notation_s* target_notation = &tree->commands[i].notation;
        bool is_target_set = notation_has_value(target_notation, searching_flag_name);

        if (!is_target_set)
            continue;

        found_target = true;
        tree->commands[i].is_set = true;
        arguments_init(&tree->commands[i].parsed_arguments, searching_flag_name, argc-1, argv+1);

        break;
    }

    return found_target;
}

command_s* command_tree_get_called_command(command_tree_s* tree)
{
    if (tree == NULL || tree->commands == NULL || tree->command_count == 0)
        return NULL;

    for (size_t i = 0; i < tree->command_count; ++i)
        if (tree->commands[i].is_set)
            return &tree->commands[i];

    return NULL;
}
