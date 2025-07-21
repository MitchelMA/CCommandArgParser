#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include <command_tree.h>
#include <command.h>
#include <option.h>

command_tree_s command_builder()
{
    // The basic setup of initializing the root
    command_tree_s tree = {0};
    command_tree_init(&tree, // Reference to the tree
                      1      // Amount of subcommands
                      );

    // Setup of the only option
    option_s boolean_option = {0};
    option_init(&boolean_option,  // Reference to the option
                false,            // Is this option required?
                OPTION_TYPE_BOOL, // Type of the option, in this case: BOOLEAN
                NULL              // The default value of the option, NULL would be "false".
                );
    option_set_name(&boolean_option, // Reference to which option's name is going to be set
                    "--bool-opt",    // Value of the main name
                    0                // Count for aliases variadic parameters
                                     // Variadic parameters for aliases, in this case none
                    );

    // Setup of the only command in the tree
    command_s main_command = {0};
    command_init(&main_command, // Reference to the command
                 1              // The amount of options that are part of this command
                 );
    command_set_name(&main_command, // Reference to which command's name is going to be set
                     "--main",      // Value of the main name
                     0              // Count for aliases variadic parameters
                                    // Variadic parameters for aliases, in this case none
                     );

    // Later we will show how to add aliases to commands/options

    // Adding the option to the main command
    // Make sure that the option is added to the command, before the command is added to the tree-root.
    // Otherwise the option will not be part of the command in the tree-root, and you'll leak memory.
    command_add_option(&main_command   // Reference to which command the option is going to be added.
                       &boolean_option // Reference to the option that's going to be added
                       );

    // After adding the option to the command, the command can be added to the tree
    command_tree_add_command(&tree         // Reference to which root the command will be added
                             &main_command // Reference to which command will is going to be added to the root.
                             );

    return tree;

}

int main(int argc, const char* argv[])
{
    command_tree_s command_tree = command_builder();

    // Parsing the base of the passed arguments to the application
    if (!command_tree_parse_base(&command_tree, argc, argv))
    {
        // When exiting the application, make sure to clean the tree; always!
        command_tree_clean(&command_tree);
        // In a later example we will show what could be displayed to the user on parsing failure
        return EXIT_FAILURE;
    }

    // After parsing, we can get the called command
    command_s* used_command = command_tree_get_called_command(&command_tree);
    assert(used_command != NULL) // used_command cannot be NULL here. 

    // Now that we have a reference to the called command, we can parse it
    command_parse(used_command);

    // Here comes your code to handle the parsed command
    // Later we will show how to extract parameters and check called options


    // Don't forget to clean up the tree when exiting the application
    command_tree_clean(&command_tree);
    return EXIT_SUCCESS;
}
