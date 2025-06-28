#ifndef COMMAND_PARSER__ARGUMENTS_H__
#define COMMAND_PARSER__ARGUMENTS_H__

#include "command_types.h"

bool arguments_init(arguments_s* arguments, const char* self, int argc, const char* const* argv_arguments);
bool arguments_prepare_parameters(arguments_s* arguments);
void arguments_clean(arguments_s* arguments);

#endif // !COMMAND_PARSER__ARGUMENTS_H__

