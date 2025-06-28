#include "arguments.h"

#include <stdlib.h>

bool arguments_init(arguments_s* arguments, const char* self, int argc, const char* const* argv_arguments)
{
    if (arguments == NULL || self == NULL)
        return false;

    arguments->self = self;
    arguments->argv_count = (size_t)argc;
    arguments->argv_arguments = argv_arguments;
    arguments->parameter_count = 0;
    arguments->parameters = NULL;
    return true;
}

bool arguments_prepare_parameters(arguments_s* arguments)
{
    if (arguments == NULL)
        return false;

    if (arguments->argv_count == 0)
    {
        arguments->parameter_count = 0;
        arguments->parameters = NULL;
    }

    arguments->parameters = malloc(sizeof(char*) * arguments->argv_count);
    arguments->parameter_count = 0;
    return arguments->parameters != NULL;
}

void arguments_clean(arguments_s* arguments)
{
    if (arguments == NULL)
        return;

    arguments->self = NULL;

    if (arguments->parameters != NULL)
        free(arguments->parameters);

    arguments->parameter_count = 0;
    arguments->parameters = NULL;
    arguments->argv_count = 0;
    arguments->argv_arguments = NULL;
}
