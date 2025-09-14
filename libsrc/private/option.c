#include "option.h"

#include "notation.h"
#include "arguments.h"

#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

// LOCAL FUNCTION DEFINITIONS //

static bool init_option_default__bool_(option_s* option, void* default_value);
static bool init_option_default__int_(option_s* option, void* default_value);
static bool init_option_default__float_(option_s* option, void* default_value);
static bool init_option_default__string_(option_s* option, void* default_value);
static bool init_option_default__multi_string_(option_s* option, void* default_value);

static const char* parse_read_first_val_(option_s* option, bool flag_can_follow, const char* default_present, int* consumed);
static int parse_option__bool_(option_s* option);
static int parse_option__int_(option_s* option);
static int parse_option__float_(option_s* option);
static int parse_option__string_(option_s* option);
static int parse_option__multi_string_(option_s* option);

static void clean_option__string_(option_s* option);
static void clean_option__multi_string_(option_s* option);

static void notation_generic_cleaner_(void* notation);

// END LOCAL FUNCTION DEFINITIONS //

bool option_init(option_s* option, bool is_required, option_type_e option_type, void* default_value)
{
    if (option == NULL || option_type >= MAX_OPTION_TYPE_COUNT)
        return false;

    if (!shared_value_init_unused(&option->shared_notation, sizeof(notation_s)))
    {
        shared_value_clean(&option->shared_notation);
        return false;
    }

    option->set_value = NULL;

    bool init_success = false;
    switch(option_type)
    {
    case OPTION_TYPE_BOOL:
        init_success = init_option_default__bool_(option, default_value);
    break;
    case OPTION_TYPE_INT:
        init_success = init_option_default__int_(option, default_value);
    break;
    case OPTION_TYPE_FLOAT:
        init_success = init_option_default__float_(option, default_value);
    break;
    case OPTION_TYPE_STRING:
        init_success = init_option_default__string_(option, default_value);
    break;
    case OPTION_TYPE_MULTI_STRING:
        init_success = init_option_default__multi_string_(option, default_value);
    default:
    break;
    }

    if (!init_success)
    {
        shared_value_clean(&option->shared_notation);
        return false;
    }

    option->type = option_type;
    option->is_required = is_required;
    return true;
}

bool option_set_name(option_s* option, const char* name, size_t alias_n, ...)
{
    if (option == NULL || name == NULL)
        return false;

    bool init_success = false;

    va_list alias_args;
    va_start(alias_args, alias_n);
    init_success = notation_init((notation_s*)shared_value_read(&option->shared_notation), name, alias_n, alias_args);
    va_end(alias_args);

    return init_success;
}

bool option_set_description(option_s* option, const char* description)
{
    if (option == NULL)
        return false;

    return notation_set_description((notation_s*)shared_value_read(&option->shared_notation), description);
}

void option_clean(option_s* option)
{
    if (option == NULL)
        return;

    // Using the shared-value as a memory-guard here
    if (shared_value_use_count(&option->shared_notation) < 2)
    {
        switch (option->type)
        {
        case OPTION_TYPE_STRING:
            clean_option__string_(option);
        break;
        case OPTION_TYPE_MULTI_STRING:
            clean_option__multi_string_(option);
        break;

        default:
        break;
        }
    }

    arguments_clean(&option->parsed_arguments);
    free(option->set_value);
    shared_value_clean_ex(&option->shared_notation, notation_generic_cleaner_);
}

int option_parse(option_s* option)
{
    if (option == NULL || option->set_value != NULL)
        return 0;

    int arguments_consumed = 0;

    switch(option->type)
    {
    case OPTION_TYPE_BOOL:
        arguments_consumed = parse_option__bool_(option);
    break;
    case OPTION_TYPE_INT:
        arguments_consumed = parse_option__int_(option);
    break;
    case OPTION_TYPE_FLOAT:
        arguments_consumed = parse_option__float_(option);
    break;
    case OPTION_TYPE_STRING:
        arguments_consumed = parse_option__string_(option);
    break;
    case OPTION_TYPE_MULTI_STRING:
        arguments_consumed = parse_option__multi_string_(option);
    break;

    default:
    break;
    }

    return arguments_consumed;
}

const char* option_get_name(const option_s* option)
{
    if (option == NULL)
        return NULL;

    const notation_s* notation = shared_value_read_const(&option->shared_notation);
    if (notation == NULL)
        return NULL;

    return notation->main_name;
}

const char* option_get_passed_name(const option_s* option)
{
    return option->parsed_arguments.self;
}

const char* option_get_description(const option_s* option)
{
    if (option == NULL)
        return NULL;

    const notation_s* notation = shared_value_read_const(&option->shared_notation);
    if (notation == NULL)
        return NULL;

    return notation->description;
}

bool option_read_value(const option_s* option, void** value)
{
    if (option == NULL)
        return false;

    if (value == NULL)
        return false;

    if (option->set_value == NULL)
    {
        *value = (void*)&option->default_value;
        return true;
    }

    *value = option->set_value;
    return true;
}

bool option_read_bool(const option_s* option)
{
    if (option == NULL || option->type != OPTION_TYPE_BOOL)
        return false;

    bool* boolean_value = NULL;
    option_read_value(option, (void**)&boolean_value);
    return *boolean_value;
}

int option_read_int(const option_s* option)
{
    if (option == NULL || option->type != OPTION_TYPE_INT)
        return 0;

    int* integer_value = NULL;
    option_read_value(option, (void**)&integer_value);
    return *integer_value;
}

float option_read_float(const option_s* option)
{
    if (option == NULL || option->type != OPTION_TYPE_FLOAT)
        return 0.0f;

    float* float_value = NULL;
    option_read_value(option, (void**)&float_value);
    return *float_value;
}

const char* option_read_string(const option_s* option)
{
    if (option == NULL || option->type != OPTION_TYPE_STRING)
        return "";

    const char** string_value = NULL;
    option_read_value(option, (void**)&string_value);
    return *string_value;
}

const char** option_read_multi_string(const option_s* option, size_t* count)
{
    if (option == NULL || option->type != OPTION_TYPE_MULTI_STRING)
        return NULL;

    const char** string_array = NULL;
    if (!option_read_value(option, (void**)&string_array))
        return NULL;

    // Since the multi-string is stored differently than the other values.
    // And `option_read_value` is a generic funtion, we need to discard one level of indirection.
    if (option->set_value == NULL)
        string_array = (const char**)*string_array;

    // Since the string-array is also NULL-terminated, we can calulate it's length by looping
    if (count != NULL)
        for (*count = 0; string_array[*count] != NULL; ++(*count)) {};

    return string_array;
}

// LOCAL FUNCTION IMPLEMENTATIONS //

bool init_option_default__bool_(option_s* option, void* default_value)
{
    if (default_value != NULL)
        option->default_value.bool_value = *(bool*)default_value;
    else
        option->default_value.bool_value = false;

    return true;
}

bool init_option_default__int_(option_s* option, void* default_value)
{
    if (default_value != NULL)
        option->default_value.int_value = *(int*)default_value;
    else
        option->default_value.int_value = 0;

    return true;
}

bool init_option_default__float_(option_s* option, void* default_value)
{
    if (default_value != NULL)
        option->default_value.float_value = *(float*)default_value;
    else
        option->default_value.float_value = 0.0f;

    return true;
}

bool init_option_default__string_(option_s* option, void* default_value)
{
    if (default_value != NULL)
    {
        option->default_value.string_value = strdup((char*)default_value);
        if (option->default_value.string_value == NULL)
            return false;
    }
    else
        option->default_value.string_value = NULL;

    return true;
}

bool init_option_default__multi_string_(option_s* option, void* default_value)
{
    if (default_value == NULL)
    {
        option->default_value.multi_string_value = NULL;
        return true;
    }

    // loop over the default value assuming its a char** array with the last value being a NULL value
    const char** items = default_value;
    size_t item_count = 0;
    for (item_count = 0; items[item_count] != NULL; ++item_count) {};
    if (item_count == 0)
        return true;

    // encode it such that the ending entry is always a NULL value
    // so it becomes a NULL terminated array
    option->default_value.multi_string_value = malloc(sizeof(char*) * (item_count + 1));
    if (option->default_value.multi_string_value == NULL)
        return false;

    for (size_t i = 0; i < item_count; ++i)
    {
        option->default_value.multi_string_value[i] = strdup(items[i]);
        if (option->default_value.multi_string_value[i] == NULL)
            return false;
    }

    option->default_value.multi_string_value[item_count] = NULL;
    return true;
}

const char* parse_read_first_val_(option_s* option, bool flag_can_follow, const char* default_present, int* consumed)
{
    const char* value = NULL;
    bool consumes = option->parsed_arguments.argv_count >= 1 &&
                    (!flag_can_follow || !notation_is_valid_flag(*option->parsed_arguments.argv_arguments));

    // parse --option value
    if (consumes)
    {
        option->parsed_arguments.parameters = malloc(sizeof(char*) * 1);
        option->parsed_arguments.parameter_count = 1;

        value = *option->parsed_arguments.argv_arguments;
        *option->parsed_arguments.parameters = value;
    }
    // default value when flag is present
    else
    {
        value = default_present;
        option->parsed_arguments.parameter_count = 0;
    }


    *consumed = (int)consumes;
    return value;
}

int parse_option__bool_(option_s* option)
{
    if (option == NULL)
        return 0;

    option->set_value = malloc(sizeof(bool));
    if (option->set_value == NULL)
        return -1;

    *(bool*)option->set_value = true;
    return 0;
}

int parse_option__int_(option_s* option)
{
    if (option == NULL)
        return 0;

    int consumed_count = 0;
    const char* text_value = parse_read_first_val_(option, false, "0", &consumed_count);
    if (text_value == NULL)
        return -1;

    int int_value = atoi(text_value);

    option->set_value = malloc(sizeof(int));
    if (option->set_value == NULL)
        return -1;

    *(int*)option->set_value = int_value;
    return consumed_count;
}

int parse_option__float_(option_s* option)
{
    if (option == NULL)
        return 0;

    int consumed_count = 0;
    const char* text_value = parse_read_first_val_(option, false, "0.0", &consumed_count);
    if (text_value == NULL)
        return -1;

    float float_value = (float)atof(text_value);

    option->set_value = malloc(sizeof(float));
    if (option->set_value == NULL)
        return -1;

    *(float*)option->set_value = float_value;
    return consumed_count;
}

int parse_option__string_(option_s* option)
{
    if (option == NULL)
        return 0;

    int consumed_count = 0;
    const char* text_value = parse_read_first_val_(option, false, NULL, &consumed_count);

    if (consumed_count == 0 || text_value == NULL)
        return -1;

    option->set_value = malloc(sizeof(char*));
    if (option->set_value == NULL)
        return -1;

    *(const char**)option->set_value = text_value;
    return consumed_count;
}

int parse_option__multi_string_(option_s* option)
{
    if (option == NULL)
        return 0;

    int valid_arg_count = 0;
    for (; valid_arg_count < (int)option->parsed_arguments.argv_count &&
         !notation_is_valid_flag(option->parsed_arguments.argv_arguments[valid_arg_count]);
         ++valid_arg_count) {};

    // at elast 1 value is required
    if (valid_arg_count < 1)
        return -1;

    option->set_value = malloc(sizeof(char*) * (valid_arg_count + 1));
    if (option->set_value == NULL)
        return -1;

    option->parsed_arguments.parameters = malloc(sizeof(char*) * valid_arg_count);
    if (option->parsed_arguments.parameters == NULL)
    {
        free(option->set_value);
        option->set_value = NULL;
        return -1;
    }

    option->parsed_arguments.parameter_count = valid_arg_count;

    for (int i = 0; i < valid_arg_count; ++i)
    {
        ((const char**)option->set_value)[i] = option->parsed_arguments.argv_arguments[i];
        option->parsed_arguments.parameters[i] = option->parsed_arguments.argv_arguments[i];
    }

    ((char**)option->set_value)[valid_arg_count] = NULL;
    return valid_arg_count;
}

void clean_option__string_(option_s* option)
{
    if (option == NULL || option->type != OPTION_TYPE_STRING || option->default_value.string_value == NULL)
        return;

    free(option->default_value.string_value);
}

void clean_option__multi_string_(option_s* option)
{
    if (option == NULL || option->type != OPTION_TYPE_MULTI_STRING || option->default_value.multi_string_value == NULL)
        return;

    for (size_t i = 0; option->default_value.multi_string_value[i] != NULL; ++i)
        free(option->default_value.multi_string_value[i]);

    free(option->default_value.multi_string_value);
}


void notation_generic_cleaner_(void* notation) { notation_clean(notation); }

// END LOCAL FUNCTION IMPLEMENTATIONS //
