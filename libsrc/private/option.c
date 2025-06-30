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

static const char* parse_read_first_val_(option_s* option, bool flag_can_follow, const char* default_present, int* consumed);
static int parse_option__bool_(option_s* option);
static int parse_option__int_(option_s* option);
static int parse_option__float_(option_s* option);
static int parse_option__string_(option_s* option);

// END LOCAL FUNCTION DEFINITIONS //

bool option_init(option_s* option, bool is_required, option_type_e option_type, void* default_value)
{
    if (option == NULL || option_type >= MAX_OPTION_TYPE_COUNT)
        return false;

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
    default:
    break;
    }

    if (!init_success)
        return false;

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
    init_success = notation_init(&option->notation, name, alias_n, alias_args);
    va_end(alias_args);

    return init_success;
}

void option_clean(option_s* option)
{
    if (option == NULL)
        return;

    notation_clean(&option->notation);
    arguments_clean(&option->parsed_arguments);
    free(option->set_value);
}

int option_parse(option_s* option)
{
    if (option == NULL)
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

    default:
    break;
    }

    return arguments_consumed;
}

const char* option_get_name(const option_s* option)
{
    return option->notation.main_name;
}

const char* option_get_passed_name(const option_s* option)
{
    return option->parsed_arguments.self;
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

const char* parse_read_first_val_(option_s* option, bool flag_can_follow, const char* default_present, int* consumed)
{
    const char* value = NULL;
    bool consumes = !flag_can_follow || (option->parsed_arguments.argv_count >= 1 &&
                    !notation_is_valid_flag(*option->parsed_arguments.argv_arguments));

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

    int consumed_count = 0;
    const char* value = parse_read_first_val_(option, true, "true", &consumed_count);

    int falsy = strcmp(value, "false");
    int truethy = strcmp(value, "true");

    if (falsy != 0 && truethy != 0)
        return -1;

    option->set_value = malloc(sizeof(bool));
    if (option->set_value == NULL)
        return -1;

    *(bool*)option->set_value = (bool) (truethy == 0);
    return consumed_count;
}


int parse_option__int_(option_s* option)
{
    if (option == NULL)
        return 0;

    int consumed_count = 0;
    const char* text_value = parse_read_first_val_(option, false, "0", &consumed_count);

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

    float float_value = atof(text_value);

    option->set_value = malloc(sizeof(float));
    if (option->set_value == NULL)
        return -1;

    *(float*)option->set_value = float_value;
    return consumed_count;
}


static int parse_option__string_(option_s* option)
{
    if (option == NULL)
        return 0;

    int consumed_count = 0;
    const char* text_value = parse_read_first_val_(option, false, "", &consumed_count);

    option->set_value = malloc(sizeof(char*));
    if (option->set_value == NULL)
        return -1;

    *(const char**)option->set_value = text_value;
    return consumed_count;
}

// END LOCAL FUNCTION IMPLEMENTATIONS //
