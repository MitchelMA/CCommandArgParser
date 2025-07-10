#ifndef COMMAND_PARSER__COMMAND_H__
#define COMMAND_PARSER__COMMAND_H__

#include "command_types.h"

bool command_init(command_s* command, size_t option_capacity);
bool command_set_name(command_s* command, const char* name, size_t alias_n, ...);
bool command_set_description(command_s* command, const char* description);
void command_clean(command_s* command);

bool command_add_option(command_s* command, option_s* option);

bool command_parse(command_s* command);
bool command_is_option_present(const command_s* command, const char* option_flag);
bool command_has_missing_required_options(const command_s* command);
option_s* command_get_missing_required_options(const command_s* command, int* missing_count);
option_s* command_find_option(const command_s* command, const char* option_flag);

bool command_is_of_flag(const command_s* command, const char* command_name);
const char* command_get_name(const command_s* command);
const char* command_get_description(const command_s* command);
const char* command_get_passed_name(const command_s* command);
const char** command_get_parameters(const command_s* command, int* parameter_count);

const void* command_read_option(const command_s* command, const char* option_flag);
bool command_read_bool_option(const command_s* command, const char* option_flag);
int command_read_int_option(const command_s* command, const char* option_flag);
float command_read_float_option(const command_s* command, const char* option_flag);
const char* command_read_string_option(const command_s* command, const char* option_flag);
const char** command_read_multi_string_option(const command_s* command, const char* option_flag, size_t* string_count);

#endif // !COMMAND_PARSER__COMMAND_H__

