#ifndef COMMAND_PARSER__NOTATION_H__
#define COMMAND_PARSER__NOTATION_H__

#include "command_types.h"

#include <stdarg.h>

bool notation_init(notation_s* notation, const char* main_name, size_t alias_n, va_list aliases);
void notation_clean(notation_s* notation);

bool notation_has_value(const notation_s* notation, const char* value);

bool notation_is_valid_flag(const char* value);

#endif // !COMMAND_PARSER__NOTATION_H__

