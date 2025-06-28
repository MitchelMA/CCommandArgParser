#include "option.h"

#include <stdlib.h>
#include <string.h>

// LOCAL FUNCTION DEFINITIONS //

static bool init_option_default__bool_(option_s* option, void* default_value);
static bool init_option_default__int_(option_s* option, void* default_value);
static bool init_option_default__float_(option_s* option, void* default_value);
static bool init_option_default__string_(option_s* option, void* default_value);

static void clean_option_default__string_(option_s* option);

// END LOCAL FUNCTION DEFINITIONS //

bool option_init(option_s* option, bool is_required, option_type_e option_type, void* default_value)
{
    if (option == NULL || option_type >= MAX_OPTION_TYPE_COUNT)
        return false;

    bool init_failure = false;
    switch(option_type)
    {
    case OPTION_TYPE_BOOL:
        init_failure = init_option_default__bool_(option, default_value);
    case OPTION_TYPE_INT:
        init_failure = init_option_default__int_(option, default_value);
    break;
    case OPTION_TYPE_FLOAT:
        init_failure = init_option_default__float_(option, default_value);
    break;
    case OPTION_TYPE_STRING:
        init_failure = init_option_default__string_(option, default_value);
    break;
    default:
    break;
    }

    if (init_failure)
        return false;

    option->type = option_type;
    option->is_required = is_required;
    return true;
}

void option_clean(option_s* option)
{
    if (option == NULL)
        return;

    switch (option->type)
    {
    case OPTION_TYPE_STRING:
        clean_option_default__string_(option);
    break;
    default:
    break;
    }
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

void clean_option_default__string_(option_s* option)
{
    if (option->default_value.string_value == NULL)
        return;

    free(option->default_value.string_value);
    option->default_value.string_value = NULL;
}

// END LOCAL FUNCTION IMPLEMENTATIONS //
