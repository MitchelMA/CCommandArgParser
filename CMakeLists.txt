cmake_minimum_required(VERSION 3.25)
project(CCommandArgParser
    VERSION 0.1.0
    DESCRIPTION "Simpele crossplatform command argument parser in C"
    LANGUAGES C)

if (PROJECT_IS_TOP_LEVEL)
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
endif()

add_subdirectory(./libsrc)

if (CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(CCommandArgParser
            PRIVATE
            /std:c17) # <- fuck msvc for this one (not supporting c23 yet in 2025)

    if (PROJECT_IS_TOP_LEVEL)
        target_compile_options(CCommandArgParser
                PRIVATE
                /W3
                /wd4996 # <- suppresses warning C4996: 'strdup' is a deprecated POSIX name
                $<$<STREQUAL:$<UPPER_CASE:${CMAKE_BUILD_TYPE}>,DEBUG>:/Od>
                $<$<STREQUAL:$<UPPER_CASE:${CMAKE_BUILD_TYPE}>,RELEASE>:/WX /Oi /Ox>)
    endif()
else()
    target_compile_options(CCommandArgParser
            PRIVATE
            -std=gnu23)

    if (PROJECT_IS_TOP_LEVEL)
        target_compile_options(CCommandArgParser
                PRIVATE
                -Wall -Wextra -Wpedantic -pedantic -Wundef
                $<$<STREQUAL:$<UPPER_CASE:${CMAKE_BUILD_TYPE}>,DEBUG>:-O -g -ggdb --save-temps>
                $<$<STREQUAL:$<UPPER_CASE:${CMAKE_BUILD_TYPE}>,RELEASE>:-Werror -O3>)
    endif()
endif()
