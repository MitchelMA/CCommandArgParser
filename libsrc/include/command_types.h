#ifndef COMMAND_PARSER__COMMAND_TYPES_H__
#define COMMAND_PARSER__COMMAND_TYPES_H__

#include <stddef.h>

typedef enum option_type_
{
    OPTION_TYPE_BOOL,
    OPTION_TYPE_INT,
    OPTION_TYPE_FLOAT,
    OPTION_TYPE_STRING,
    MAX_OPTION_TYPE_COUNT
} option_type_e;

typedef struct option_
{
    option_type_e type;
    bool is_required;
    union {
        bool  bool_value;
        int   int_value;
        float float_value;
        char* string_value;
    } default_value;
} option_s;

typedef struct command_
{
    size_t option_capacity;
    size_t option_count;
    option_s* options;
} command_s;

typedef struct command_tree_
{
    size_t command_capacity;
    size_t command_count;
    command_s* commands;
} command_tree_s;

#endif // !COMMAND_PARSER__COMMAND_TYPES_H__

