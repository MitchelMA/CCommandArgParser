#include "command.h"

#include "option.h"
#include "notation.h"

#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

bool command_init(command_s* command, size_t option_capacity)
{
    if (command == NULL || option_capacity == 0)
        return false;

    command->options = malloc(sizeof(option_s) * option_capacity);
    if (command->options == NULL)
        return false;

    command->is_on = false;
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
