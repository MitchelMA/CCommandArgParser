#include "command.h"

#include "option.h"
#include "notation.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

bool command_init(command_s* command, size_t option_capacity)
{
    if (command == NULL || option_capacity == 0)
        return false;

    command->options = malloc(sizeof(option_s) * option_capacity);
    if (command->options == NULL)
        return false;

    command->is_set = false;
    command->option_capacity = option_capacity;
    command->option_count = 0;
    return true;
}

bool command_set_name(command_s* command, const char* name, size_t alias_n, ...)
{
    if (command == NULL || name == NULL)
        return false;

    bool init_success = false;

    va_list alias_args;
    va_start(alias_args);
    init_success = notation_init(&command->notation, name, alias_n, alias_args);
    va_end(alias_args);

    return init_success;
}

void command_clean(command_s* command)
{
    if (command == NULL)
        return;

    for (size_t i = 0; i < command->option_count; ++i)
        option_clean(&command->options[i]);
  
    if (command->options != NULL)
    {
        free(command->options);
        command->options = NULL;
    }

    notation_clean(&command->notation);
}

bool command_add_option(command_s* command, option_s* option)
{
    if (command == NULL)
        return false;

    if (command->option_count >= command->option_capacity)
        return false;

    memcpy(&command->options[command->option_count], option, sizeof(option_s));
    command->option_count++;
    return true;
}

bool command_tree_parse_base(command_tree_s* tree, int argc, const char** argv)
{
    if (tree == NULL)
        return false;

    if (argc <= 1)
        return false;

    tree->parsed_arguments.self = (char*)*argv;

    // skip the calling path that's normally at argv[0]
    argc--;
    argv++;

    tree->parsed_arguments.parameter_count = 0;
    tree->parsed_arguments.argv_count = (size_t)argv;
    tree->parsed_arguments.parameters = NULL;
    tree->parsed_arguments.argv_arguments = argv;

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
        // TODO: do something to init the arguments_s

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
