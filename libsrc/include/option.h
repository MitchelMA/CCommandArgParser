#ifndef COMMAND_PARSER__OPTION_H__
#define COMMAND_PARSER__OPTION_H__

/** \file option.h
 * This is the header file containg the functions to use in combination with the `option_s` structure.
 */

#include "command_types.h"

/**
 * @brief The init function for the `option_s` structure.
 *
 * When using this function, take particular note to **default_value**
 * as it expects different values depending on the **option_type** argument:
 * - `OPTION_TYPE_BOOL`: Expects an object of type `bool*`.
 * - `OPTION_TYPE_INT`: Expects an object of type `int*`.
 * - `OPTION_TYPE_FLOAT`: Expects an object of type `float*`.
 * - `OPTION_TYPE_STRING`: Expects an object of type `char*`.
 * - `OPTION_TYPE_MULTI_STRING`: Expects an object of type `char**`. Keep in mind that the string-array is NULL-terminated: `{"My", "String", "Array", NULL};`
 *
 * When initializing as `STRING`/`MULTI_STRING` the option will have ownership of **default_value** as
 * the argument-value gets copied over to the heap.
 *
 * @param option The option that gets initialized by this function.
 * @param is_required Indicates if the option should be a required option or not, this can later be used for warning the user that they're missing an option in their argument.
 * @param option_type Indicates what type of option this is: see `option_type_e` for possible types.
 * @param default_value The default value that this option will fall back to when it wasn't part of the user's argument.
 *
 * @return `False` to indicate initialization failure. `True` on success.
 *
 * @see option_type_e for possible types.
 */
bool option_init(option_s* option, bool is_required, option_type_e option_type, void* default_value);

/**
 * @brief Through this function, you can assign the main name and aliases to the option.
 *
 * @param option The option to which names will be assigned.
 * @param name The main name of the option.
 * @param alias_n The amount of aliases in the next variadic parameter.
 * @param ... The aliases that will be assigned to the option. This is a viariadic argument and is dependent on **alias_n** as counter. Expected as individual strings.
 *
 * @return `False` on failure when trying to set the name. `True` on success.
 */
bool option_set_name(option_s* option, const char* name, size_t alias_n, ...);

/**
 * @brief Sets the description of the passed option.
 *
 * @param option The option whose description will be set.
 * @param description The description that will be part of the option.
 *
 * @return
 * `False` on failure. This happens when the option hasn't been named yet.  
 * `True` on success.
 */
bool option_set_description(option_s* option, const char* description);

/**
 * @brief The cleaner function for the `option_s` structure.
 *
 * This function gets called when calling `command_tree_clean()`. So you will rarely need this function.
 *
 * @param option The option that gets cleaned.
 */
void option_clean(option_s* option);

/**
 * @brief Parses the option starting at the first value in its parsed_arguments::argv_arguments member.
 *
 * This function is called by `command_parse()` and I discourage you from calling it yourself.  
 * Its parsing behaviour is dependent on its type:
 * - `OPTION_TYPE_BOOL`: Doesn't consume any arguments, should return `0`.
 * - `OPTION_TYPE_INT`: Consumes the very first argument and returns `1`. If no argument passed, returns `-1`.
 * - `OPTION_TYPE_FLOAT`: Same as `OPTION_TYPE_INT`.
 * - `OPTION_TYPE_STRING`: Same as `OPTION_TYPE_INT`.
 * - `OPTION_TYPE_MULTI_STRING`: Greedy consumes all following arguments until the end or when the next flag is encountered. Returns the amount of arguments consumed, or `-1` when none.
 *
 * @param option The option that gets parsed
 *
 * @return The amount of arguments consumed, or `-1` on failure.
 */
int option_parse(option_s* option);

const char* option_get_name(const option_s* option);
const char* option_get_passed_name(const option_s* option);
const char* option_get_description(const option_s* option);

bool option_read_value(const option_s* option, void** value);
bool option_read_bool(const option_s* option);
int option_read_int(const option_s* option);
float option_read_float(const option_s* option);
const char* option_read_string(const option_s* option);
const char** option_read_multi_string(const option_s* option, size_t* count);

#endif // !COMMAND_PARSER__OPTION_H__

