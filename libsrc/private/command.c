#include "command.h"

#include <stdlib.h>
#include <string.h>

bool command_init(command_s* command, size_t option_capacity)
{
    if (command == NULL || option_capacity == 0)
        return false;

    command->options = malloc(sizeof(option_s) * option_capacity);
    if (command->options == NULL)
        return false;

    command->option_capacity = option_capacity;
    command->option_count = 0;
    return true;
}

void command_clean(command_s* command)
{
    if (command == NULL || command->option_capacity == 0)
        return;

    // TODO: Loop over all the options and call the future clean function
  
    free(command->options);
    command->options = NULL;
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
