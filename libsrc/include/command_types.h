#ifndef COMMAND_PARSER__COMMAND_TYPES_H__
#define COMMAND_PARSER__COMMAND_TYPES_H__

/** \file command_types.h
 * This header file contains all the defenitions of the immediate values
 * used within the command-tree structure important to the user:
 *  - Enum: `option_type_e`; the different types of options
 *  - Struct: `arguments_s`; the structure containg argument information for commands/options.
 *  - Struct: `notation_s`; the structure containing the info on how a command/option should be addressed.
 *  - Struct: `option_s`; the option can be an extra flag containing data registered to a command
 *  - Struct: `command_s`; the command is the first called flag in argv and should indicate the main logic of what the caller wants to do, it is registered to a `command_tree_s`.
 *  - Struct: `command_tree_s`: the root of the tree-like structure, this is where commands are registered to.
 */

#include "extra/shared_value.h"

#include <stddef.h>
#include <stdbool.h>

/**
 * This enum is used by `option_s` to denote what type of option it is.
 */
typedef enum option_type_
{
    OPTION_TYPE_BOOL,         /**< This is the boolean-type flag. Example usage: `--my-bool-flag` */
    OPTION_TYPE_INT,          /**< This is the int-type flag. Example usage: `--my-int-flag 2` */
    OPTION_TYPE_FLOAT,        /**< This is the float-type-flag. Example usage: `--my-float-flag 2.3` */
    OPTION_TYPE_STRING,       /**< This is the string-type flag. Example usage: `--my-string-flag word`*/
    OPTION_TYPE_MULTI_STRING, /**< This is the mult-string-type flag. This flag is special in the sence that it greedy-reads all the following values until the end or when another flag is found. Example usage: `--my-m-string-flag all these disconnected words are read` */
    MAX_OPTION_TYPE_COUNT
} option_type_e;


/**
 * This structure holds all the litteral passed values from argv+argc.
 * Additionally it will also hold `self`. `self` can mean different things in different situations:
 *  - `command_tree_s`: The very first value in argv, so the name of the application.
 *  - `command_s` + `option_s`: The value used to call this command/option.
 *
 * `parameters` hold the relevant values passed to a command or option. It is also heap-allocated and free'd when cleaning the `command_tree_s` itself.
 * `parameters` is also the only heap-allocated member in this structure.
 *
 * For functionality and usage of this structure, look into the `arguments.h` header-file.
 */
typedef struct arguments_
{
    size_t parameter_count;
    size_t argv_count;

    char const* self;
    const char** parameters;
    const char* const* argv_arguments;
} arguments_s;

/**
 * This structure stores the info on how a command/option should be addressed.
 * It also stores the description of the command or option.
 *
 * All the strings are heap-allocted and free'd again when cleaning up the command-tree.
 *
 * For functionality and usage of this structure, look into the `notation.h` header-file.
 */
typedef struct notation_
{
    size_t alias_count;
    char* main_name;
    char** aliases;
    char* description;
} notation_s;

/**
 * This is the option structure. This structure is used to specify extra data in a command or expect typed info.
 * Say for example you have a command, and that command has different (optional) extra parameters.
 * This is what an option could be used for.
 *
 * An option can be of the following types - which are also denoted at `option_type_e`:
 *  - BOOLEAN
 *  - INTEGER
 *  - FLOATING-POINT NUMBER
 *  - STRING
 *  - MULTI-STRING
 *
 * `set_value` is also always heap-allocated to hold the typed data:
 *  - `OPTION_TYPE_BOOL`: the size of a boolean.
 *  - `OPTION_TYPE_INT`: the size of an integer.
 *  - `OPTION_TYPE_FLOAT`: the size of a float.
 *  - `OPTION_TYPE_STRING`: the size of a pointer to a string, so not the string itself.
 *  - `OPTION_TYPE_MULTI_STRING`: the length of the string-array passed to the option, again also not the strings themselves.
 *
 * `set_value` is free'd when the command-tree is cleaned.
 *
 * For functionality and usage of this structure, look into the `option.h` header-file.
 */
typedef struct option_
{
    shared_value_s shared_notation; /**< the underlying type is `notation_s`. This needs to be a shared value since it is allocated before the option is registered to a command, which could otherwise lead to a double-free when used in multiple commands. */
    arguments_s parsed_arguments;

    bool is_required;

    option_type_e type;
    union {
        bool  bool_value;
        int   int_value;
        float float_value;
        char*  string_value;
        char** multi_string_value;
    } default_value;
    void* set_value; /**< the member holding the passed information, NULL when the flag wasn't called. */
} option_s;

/**
 * This is the command structure. This structure is used to indicate the expected logic of the application by the caller/callee.
 * It holds an array of options denoted by: `option_capacity`, `option_count`, and the `options` array.
 * The parameters that were used when calling the command are stored in `parsed_arguments`.
 *
 * For functionality and usage of this structure, look into the `command.h` header-file.
 */
typedef struct command_
{
    notation_s notation;
    arguments_s parsed_arguments;

    bool is_set;
    size_t option_capacity;
    size_t option_count;
    option_s* options;
} command_s;

/**
 * This is the command_tree structure. This structure is the root of the tree, and holds all its direct descendands: commands.
 *
 * Since you can't call a tree-root itself, there's no reason for this structure to hold a `notation_s`.
 * For this reason I chose for the root to hold its own description, this description should be the description of the application itself.
 *
 * For functionality and usage of this structure, look into the `command_tree.h` header-file.
 */
typedef struct command_tree_
{
    size_t command_capacity;
    size_t command_count;
    arguments_s parsed_arguments;

    command_s* commands;
    char* description;
} command_tree_s;

#endif // !COMMAND_PARSER__COMMAND_TYPES_H__

