cmake_minimum_required(VERSION 3.25)
project(CommandParser
    VERSION 0.0.1
    DESCRIPTION "Simpele crossplatform command parser in C"
    LANGUAGES C)

if (PROJECT_IS_TOP_LEVEL)
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
endif()

add_subdirectory(./libsrc)

target_compile_options(CommandParser
    PRIVATE 
    -std=gnu23)

if (PROJECT_IS_TOP_LEVEL)
    target_compile_options(CommandParser
        PRIVATE
        -Wall -Wextra -Wpedantic -pedantic -Wundef
        $<$<STREQUAL:$<UPPER_CASE:${CMAKE_BUILD_TYPE}>,DEBUG>:-O -g -ggdb>
        $<$<STREQUAL:$<UPPER_CASE:${CMAKE_BUILD_TYPE}>,RELEASE>:-Werror -Ofast>)
endif()
