# Command Argument Parser

## Why?
This is a small project that I setup for my own use to make advanced argument parsing a lot easier on different platforms.

## Examples
### Quick-start
The following example will show a quick-start on how to set-up the process of:
  - Building a command-tree.
  - Adding a command to the command-tree.
  - Adding an option to the command.
  - Parsing the command-tree.
  - Parsing the command.


\include examples/example_quickstart.c
[examples/example_quickstart.c](./examples/example_quickstart.c)

### Command-tree
The first specific example is that of the command-tree. This structure is the root of the tree
and therefore the most important base structure.

\todo write the example for how the command-tree should be used. [./examples/example_commandtree.c]
\include ./examples/example_commandtree.c
[examples/example_commandtree.c](./examples/example_commandtree.c)

### Command
In this example, it's shown what can be done with specifically the command structure:

\include examples/example_command.c
[examples/example_command.c](./examples/example_command.c)

### Option

In this example, we're going over on how options should be used.

\todo write the example for how the option should be used. [./examples/example_option.c]
\include examples/example_option.c
[examples/example_option.c](./examples/example_option.c)
