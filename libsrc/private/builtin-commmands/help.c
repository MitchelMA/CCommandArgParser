#include "builtin-commands/help.h"

#include "command_tree.h"
#include "command.h"
#include "option.h"

bool command_tree_add_help(command_tree_s* command_tree)
{
    if (command_tree == NULL)
        return false;

    command_s help_command = {0};
    command_init(&help_command, 0);
    command_set_name(&help_command, "--help", 1, "-h");
    
    bool success = command_tree_add_command(command_tree, &help_command);
    if (!success)
        command_clean(&help_command);

    return success;
}

bool handle_help_command(const command_tree_s* command_tree, const command_s* called_command)
{
    if (command_tree == NULL || called_command == NULL)
        return false;

    // when the called command is not --help/-h
    if (!command_is_of_flag(called_command, "--help"))
        return false;

    // print out the global help when no extra parameters were passed
    if (called_command->parsed_arguments.parameter_count == 0)
    {
        print_global_help(stdout, command_tree);
        return true;
    }

    // when extra parameters were given
    const char* target_command_name = *called_command->parsed_arguments.parameters;
    const command_s* target_command = command_tree_get_command(command_tree, target_command_name);

    if (target_command == NULL)
    {
        fprintf(stderr, "Command `%s` could not be found\n", target_command_name);
        return true;
    }

    print_command_help(stdout, target_command);
    return true;
}

void print_global_help(FILE* stream, const command_tree_s* command_tree)
{
    if (stream == NULL || command_tree == NULL)
        return;

    fprintf(stream, "[GLOBAL DESCRIPTION OF COMMAND_TREE]\n\n");

    fprintf(stream, "Commands:\n");
    for (size_t i = 0; i < command_tree->command_count; ++i)
        fprintf(stream, "\t%-10s | %s\n",
                command_get_name(&command_tree->commands[i]),
                "[DESCRIPTION OF COMMAND]");
}

void print_command_help(FILE* stream, const command_s* command)
{
    if (stream == NULL || command == NULL)
        return;

    fprintf(stream, "%s:\n\t[DESCRIPTION OF COMMAND]\n\n", command_get_name(command));

    if (command->option_count <= 0)
        return;

    size_t required_count = 0;
    fprintf(stream, "Options:\n");
    for (size_t i = 0; i < command->option_count; ++i)
    {
        required_count += command->options[i].is_required;
        print_option_help(stream, &command->options[i]);
        fprintf(stream, "\n");
    }

    if (required_count > 0)
        fprintf(stream, "\n*: required option\n");
}

void print_option_help(FILE* stream, const option_s* option)
{
    if (stream == NULL || option == NULL)
        return;

    fprintf(stream, "\t%-1s%-10s | %-16s | %s",
            option->is_required ? "*" : "",
            option_get_name(option),
            "[TYPE INFO]",
            "[OPTION DESCRIPTION]");
}
