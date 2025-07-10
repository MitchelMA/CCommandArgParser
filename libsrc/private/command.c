#include "command.h"

#include "option.h"
#include "notation.h"
#include "arguments.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>

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
    va_start(alias_args, alias_n);
    init_success = notation_init(&command->notation, name, alias_n, alias_args);
    va_end(alias_args);

    return init_success;
}

bool command_set_description(command_s* command, const char* description)
{
    if (command == NULL)
        return false;

    return notation_set_description(&command->notation, description);
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
    arguments_clean(&command->parsed_arguments);
}

bool command_add_option(command_s* command, option_s* option)
{
    if (command == NULL)
        return false;

    if (command->option_count >= command->option_capacity)
        return false;

    memcpy(&command->options[command->option_count], option, sizeof(option_s));
    if (!shared_value_copy_into(&command->options[command->option_count].shared_notation, &option->shared_notation))
    {
        // Reset back to 0 value on failure
        memset(&command->options[command->option_count], 0, sizeof(option_s));
        return false;
    }
    command->option_count++;
    return true;
}

bool command_parse(command_s* command)
{
    if (command == NULL)
        return false;

    if (command->parsed_arguments.argv_count == 0)
        return true;

    if (!arguments_prepare_parameters(&command->parsed_arguments))
        return false;


    if (command->option_count == 0)
    {
        for (size_t i = 0; i < command->parsed_arguments.argv_count; ++i)
            command->parsed_arguments.parameters[i] = command->parsed_arguments.argv_arguments[i];

        command->parsed_arguments.parameter_count = command->parsed_arguments.argv_count;
        return true;
    }

    for (int64_t i = 0; i < (int64_t)command->parsed_arguments.argv_count; ++i)
    {
        bool arg_is_flag = notation_is_valid_flag(command->parsed_arguments.argv_arguments[i]);
        // when argument is not a flag
        if (!arg_is_flag)
        {
            command->parsed_arguments.parameters[command->parsed_arguments.parameter_count] =
                command->parsed_arguments.argv_arguments[i];

            command->parsed_arguments.parameter_count++;
            continue;
        }

        // when the argument is a flag, but not used by any options within this command
        option_s* found_option = command_find_option(command, command->parsed_arguments.argv_arguments[i]);
        if (found_option == NULL)
        {
            fprintf(stderr, "Found unkown option `%s`\n", command->parsed_arguments.argv_arguments[i]);
            command->parsed_arguments.parameters[command->parsed_arguments.parameter_count] =
                command->parsed_arguments.argv_arguments[i];

            command->parsed_arguments.parameter_count++;
            continue;
        }

        arguments_init(&found_option->parsed_arguments,
                       command->parsed_arguments.argv_arguments[i],
                       (int)command->parsed_arguments.argv_count - ((int)i + 1),
                       command->parsed_arguments.argv_arguments + (i + 1));

        int consumed = option_parse(found_option);
        if (consumed < 0)
        {
            fprintf(stderr, "Failed to parse flag `%s`: value invalid\n",
                    command->parsed_arguments.argv_arguments[i]);

            continue;
        }
        i += consumed;
    }

    return true;
}

bool command_is_option_present(const command_s* command, const char* option_flag)
{
    if (command == NULL)
        return false;

    for (size_t i = 0; i < command->option_count; ++i)
        if (notation_has_value(shared_value_read_const(&command->options[i].shared_notation), option_flag))
            return command->options[i].set_value != NULL;

    return false;
}

bool command_has_missing_required_options(const command_s* command)
{
    if (command == NULL)
        return false;

    for (size_t i = 0; i < command->option_count; ++i)
        if (command->options[i].is_required && command->options[i].set_value == NULL)
            return true;

    return false;
}


option_s* command_get_missing_required_options(const command_s* command, int* missing_count)
{
    if (command == NULL || command->option_count == 0)
    {
        *missing_count = 0;
        return NULL;
    }

    option_s* ret_arr = malloc(sizeof(option_s) * command->option_count);
    int missing_required_count = 0;
    
    for (size_t i = 0; i < command->option_count; ++i)
    {
        if (!command->options[i].is_required || command->options[i].set_value != NULL)
            continue;

        ret_arr[missing_required_count] = command->options[i];
        missing_required_count++;
    }

    *missing_count = missing_required_count;
    return ret_arr;
}

option_s* command_find_option(const command_s* command, const char* option_flag)
{
    if (command == NULL || command->option_count == 0)
        return NULL;

    for (size_t i = 0; i < command->option_count; ++i)
        if (notation_has_value(shared_value_read_const(&command->options[i].shared_notation), option_flag))
            return &command->options[i];

    return NULL;
}

bool command_is_of_flag(const command_s* command, const char* command_name)
{
    return notation_has_value(&command->notation, command_name);
}

const char* command_get_name(const command_s* command)
{
    return command->notation.main_name;
}

const char* command_get_passed_name(const command_s* command)
{
    return command->parsed_arguments.self;
}

const char* command_get_description(const command_s* command)
{
    if (command == NULL)
        return NULL;

    return command->notation.description;
}

const char** command_get_parameters(const command_s* command, int* parameter_count)
{
    *parameter_count = (int)command->parsed_arguments.parameter_count;
    return command->parsed_arguments.parameters;
}

const void* command_read_option(const command_s* command, const char* option_flag)
{
    const option_s* found_option = command_find_option(command, option_flag);
    if (found_option == NULL)
        return NULL;

    char* pointer_holder = NULL;
    if (!option_read_value(found_option, (void**)&pointer_holder))
        return NULL;

    return (void*)pointer_holder;
}

bool command_read_bool_option(const command_s* command, const char* option_flag)
{
    const option_s* found_option = command_find_option(command, option_flag);
    return option_read_bool(found_option);
}

int command_read_int_option(const command_s* command, const char* option_flag)
{
    const option_s* found_option = command_find_option(command, option_flag);
    return option_read_int(found_option);
}

float command_read_float_option(const command_s* command, const char* option_flag)
{
    const option_s* found_option = command_find_option(command, option_flag);
    return option_read_float(found_option);
}

const char* command_read_string_option(const command_s* command, const char* option_flag)
{
    const option_s* found_option = command_find_option(command, option_flag);
    return option_read_string(found_option);
}

const char** command_read_multi_string_option(const command_s* command, const char* option_flag, size_t* string_count)
{
    const option_s* found_option = command_find_option(command, option_flag);
    return option_read_multi_string(found_option, string_count);
}

