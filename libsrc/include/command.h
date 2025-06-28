#ifndef COMMAND_PARSER__COMMAND_H__
#define COMMAND_PARSER__COMMAND_H__

#include "command_types.h"

bool command_init(command_s* command, size_t option_capacity);
void command_clean(command_s* command);

bool command_add_option(command_s* command, option_s* option);

#endif // !COMMAND_PARSER__COMMAND_H__

