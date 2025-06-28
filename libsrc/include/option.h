#ifndef COMMAND_PARSER__OPTION_H__
#define COMMAND_PARSER__OPTION_H__

#include "command_types.h"

bool option_init(option_s* option, bool is_required, option_type_e option_type, void* default_value);
void option_clean(option_s* option);

#endif // !COMMAND_PARSER__OPTION_H__

