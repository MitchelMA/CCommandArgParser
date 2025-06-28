#include "notation.h"

#include <stdlib.h>
#include <string.h>

bool notation_init(notation_s* notation, const char* main_name, size_t alias_n, va_list aliases)
{
    if (notation == NULL || main_name == NULL)
        return false;

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
        notation->aliases[i] = strdup(va_arg(aliases, char*));

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
        free(notation->aliases[i]);
        notation->aliases[i] = NULL;
    }
    free(notation->aliases);

    notation->alias_count = 0;
}
