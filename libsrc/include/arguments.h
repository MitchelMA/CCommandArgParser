#ifndef COMMAND_PARSER__ARGUMENTS_H__
#define COMMAND_PARSER__ARGUMENTS_H__

/** \file arguments.h
 * This is the header file containg the functions to use in combination with the `arguments_s` structure.
 */

#include "command_types.h"

/**
 * @brief `arguments_s` structure initializer function.
 *
 * The initializer function for the `arguments_s` function.
 * @param arguments A pointer to the arguments structure that's going to be initialized.
 * @param self The `self` value of of the `arguments_s` structure.
 * @param argc The counter of `argv_arguments` array.
 * @param argv_arguments The argv_arguments passed on from `main()` that are relevant to this arguments_s and onwards.
 *
 * @return _false_ when either **arguments** or **self** is `NULL`, otherwise _true_ is returned.
 */
bool arguments_init(arguments_s* arguments, const char* self, int argc, const char* const* argv_arguments);

/**
 * @brief the `arguments_s::parameters` member initializer function.
 *
 * The parameter initializer function.
 * This function prepares the `parameter` member of `arguments` by heap allocation.
 * This allocation is always the same length as `argv_arguments`.
 *
 * @return _false_ when **arguments** is `NULL`, otherwise _true_.
 */
bool arguments_prepare_parameters(arguments_s* arguments);

/**
 * @brief The `arguments_s`'s clean function.
 */
void arguments_clean(arguments_s* arguments);

#endif // !COMMAND_PARSER__ARGUMENTS_H__

