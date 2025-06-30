add_library(CommandParser)

file(GLOB_RECURSE COMMAND_PARSER_SOURCES ./private/*.c)


target_sources(CommandParser
    PRIVATE
    ${COMMAND_PARSER_SOURCES})

target_include_directories(CommandParser
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/include)
