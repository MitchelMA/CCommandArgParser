#ifndef COMMAND_PARSER__NOTATION_H__
#define COMMAND_PARSER__NOTATION_H__

/** \file notation.h
 * This is the header file containg the functions to use in combination with the `notation_s` structure.
 */

#include "command_types.h"

#include <stdarg.h>

/**
 * @brief `notation_s` structure initializer function.
 *
 * @param notation A pointer to the notation structure that's going to be initialized.
 * @param main_name The main name associated with this notation structure.
 * @param alias_n The length of the va-list **aliases**. Value is allowed to be _0_.
 * @param aliases A va-list of `const char*` items with a length of **alias_n**.
 *
 * @return
 * _false_ when **notation** or **main_name** is NULL.  
 * _false_ when **main_name** is invalid as described by `notation_is_valid_flag()`.  
 * _false_ on any allocation failure, be it a call to `malloc()` or `strdup()`.  
 * _true_ when none of the aforementioned apply.
 */
bool notation_init(notation_s* notation, const char* main_name, size_t alias_n, va_list aliases);

/**
 * @brief Sets the value of the description on **notation**.
 *
 * @param notation The notation whose description member will be set.
 * @param description The description whose value it's going to be set with.
 *
 * @return
 * _false_ when either **notation** or **description is NULL.  
 * _false_ when strdup fails.  
 * _true_ when aforementioned do not apply.
 */
bool notation_set_description(notation_s* notation, const char* description);

/**
 * @brief the cleanup function of the `notation_s` structure.
 *
 * @param notation A pointer to the structure that's going to be cleaned up.
 */
void notation_clean(notation_s* notation);

/**
 * @brief checks if the given value **value** can be found anywhere withing the passed **notation**.
 *
 * @param notation The notation that's going to get checked.
 * @param value The value that's searched for in either the `main_name` or any of the aliases of **notation**.
 *
 * @return
 * _true_ when **value** was found within **notation**, _false_ otherwise.
 */
bool notation_has_value(const notation_s* notation, const char* value);

/**
 * @brief This function determines if a given name **value** is a valid flag value.
 *
 * @param value The string to be tested.
 *
 * @return
 * _false_ when **value** does not begin with `"-"`.  
 * _true_ when **value** does begin with `"-"`
 */
bool notation_is_valid_flag(const char* value);

#endif // !COMMAND_PARSER__NOTATION_H__

