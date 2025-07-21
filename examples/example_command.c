#include <command.h>

int main(/* ... */)
{
    // When setting up the command, you go through multiple steps
    // The first is initializing the command itself
    command_s main_command = {0}; // usually it's a good idea to zero-init them on the stack.
    command_init(&main_command, // The first parameter is a pointer to the command
                 1              // The second is the amount of options that will be part of this command
                 );
    // When we're done initializing the base of the command, we give it a name:
    command_set_name(&main_command,    // Again, the first param is a pointer to the command
                     "--main-command", // The second param is the main-name you want to give to the command
                     1,                // The third parameter is the amount of aliases for this command
                     "-mc"             // After which follows the variadic params for the aliases, who're expected to be strings
                     );

    // Optionally, you can give a description to the command. This can be used for when the user calls a "--help" command:
    command_set_description(&main_command,                  // Command pointer
                            "My very beautiful description" // The description-string
                            );

    // Now finally, the last thing to be done before the command can be added to the tree
    // We're going to add an option:
    // For this example, it's a boolean option.
    command_add_option(&main_command,    // Command pointer
                       &boolean_option // pointer to the option
                       );
    // This add function returns true or false, depending on if there's enough space available for the option to be added.

    // ---------------------------- //
    // Parsing the command_tree ... //
    // ---------------------------- //

    // After you're done with parsing the tree, you have gotten the called command
    command_s* used_command = command_tree_get_called_command(&command_tree);

    // Now that we have the called command, we should parse the command to continue
    if (!command_parse(used_command))
    {
        // Here you can print some error to the user when parsing of the command failed
        // Don't forget to clean up the command-tree on exit.
    }

    // When the command was parsed successfully, we can identify it through the following function
    // In this example, we expect to have a command with the main-name as "--main-command"
    if (command_is_of_flag(used_command, "--main-command"))
    {
        // Here we can check for extract the parameters passed on to this command:
        int parameter_count = 0;
        const char** passed_parameters = command_get_parameters(used_command, &parameter_count);

        // Retrieving the value from an option is done as follows:
        // Here we will read the value from an option expected to hold a boolean value
        // with the name "--boolean-option"
        bool bool_option_value = command_read_bool_option(used_command, "--boolean-option");
        // The option read-value functions will return the default value when the option wasn't `set`
        
        // To check if the option is present (or `set`) can be done as follows:
        bool is_bool_option_present = command_is_option_present(used_command, "--boolean-option");
    }

    // Possible check for other commands

    // Cleaning of the command-tree and returning succes or failure.
}

