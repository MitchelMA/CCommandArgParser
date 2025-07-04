#ifndef COMMAND_PARSER__COMMAND_TYPES_H__
#define COMMAND_PARSER__COMMAND_TYPES_H__

#include "extra/shared_value.h"

#include <stddef.h>
#include <stdbool.h>

typedef enum option_type_
{
    OPTION_TYPE_BOOL,
    OPTION_TYPE_INT,
    OPTION_TYPE_FLOAT,
    OPTION_TYPE_STRING,
    MAX_OPTION_TYPE_COUNT
} option_type_e;

typedef struct arguments_
{
    size_t parameter_count;
    size_t argv_count;

    char const* self;
    const char** parameters;
    const char* const* argv_arguments;
} arguments_s;

typedef struct notation_
{
    size_t alias_count;
    char* main_name;
    char** aliases;
    char* description;
} notation_s;

typedef struct option_
{
    shared_value_s shared_notation; // underlying: notation_s
    arguments_s parsed_arguments;

    option_type_e type;
    bool is_required;
    union {
        bool  bool_value;
        int   int_value;
        float float_value;
        const char* string_value;
    } default_value;
    void* set_value;
} option_s;

typedef struct command_
{
    notation_s notation;
    arguments_s parsed_arguments;

    bool is_set;
    size_t option_capacity;
    size_t option_count;
    option_s* options;
} command_s;

typedef struct command_tree_
{
    size_t command_capacity;
    size_t command_count;
    arguments_s parsed_arguments;

    command_s* commands;
    char* description;
} command_tree_s;

#endif // !COMMAND_PARSER__COMMAND_TYPES_H__

