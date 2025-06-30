#include "notation.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

bool notation_init(notation_s* notation, const char* main_name, size_t alias_n, va_list aliases)
{
    if (notation == NULL || main_name == NULL)
        return false;

    if (!notation_is_valid_flag(main_name))
    {
        fprintf(stderr, "`%s` is not a valid flag name, flags should begin with `-`\n", main_name);
        return false;
    }

    notation->main_name = strdup(main_name);
    if (notation->main_name == NULL)
        return false;

    if (alias_n == 0)
    {
        notation->aliases = NULL;
        notation->alias_count = 0;
        return true;
    }

    notation->aliases = malloc(sizeof(char*) * alias_n);
    if (notation->aliases == NULL)
    {
        free(notation->main_name);
        notation->alias_count = 0;
        return false;
    }

    notation->alias_count = alias_n;
    for (size_t i = 0; i < alias_n; ++i)
    {
        const char* val = va_arg(aliases, char*);
        if (notation_is_valid_flag(val))
        {
            notation->aliases[i] = strdup(val);
            continue;
        }

        fprintf(stderr, "`%s` is not a valid flag name, flags should begin with `-`\n", val);
        notation->aliases[i] = NULL;
    }

    return true;
}

void notation_clean(notation_s* notation)
{
    if (notation == NULL || notation->main_name == NULL)
        return;

    free(notation->main_name);
    notation->main_name = NULL;

    if (notation->aliases == NULL)
    {
        notation->alias_count = 0;
        return;
    }

    for (size_t i = 0; i < notation->alias_count; ++i)
    {
        if (notation->aliases[i] == NULL)
            continue;

        free(notation->aliases[i]);
        notation->aliases[i] = NULL;
    }
    free(notation->aliases);

    notation->alias_count = 0;
}

bool notation_has_value(const notation_s* notation, const char* value)
{
    if (notation == NULL || notation->main_name == NULL)
        return false;

    if (strcmp(value, notation->main_name) == 0)
        return true;

    if (notation->aliases == NULL || notation->alias_count == 0)
        return false;

    for (size_t i = 0; i < notation->alias_count; ++i)
    {
        if (strcmp(notation->aliases[i], value) == 0)
            return true;
    }

    return false;
}

bool notation_is_valid_flag(const char* value)
{
    if (strlen(value) <= 0)
        return false;

    return value[0] == '-';
}
