add_library(CCommandArgParser)

file(GLOB_RECURSE COMMAND_PARSER_SOURCES ./private/*.c)


target_sources(CCommandArgParser
    PRIVATE
    ${COMMAND_PARSER_SOURCES})

target_include_directories(CCommandArgParser
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/include)
