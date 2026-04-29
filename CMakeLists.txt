cmake_minimum_required(VERSION 3.16)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
    message(FATAL_ERROR "In-source builds are not allowed")
endif()

project(
    tetrispp
    VERSION 1.0.0
    LANGUAGES CXX
)


set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE RelWithDebInfo)
endif()


file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS
    src/*.cpp
)

add_executable(tetrispp ${SOURCE_FILES})

target_include_directories(tetrispp PRIVATE
    src
)


find_package(Curses REQUIRED)

target_include_directories(tetrispp PRIVATE
    ${CURSES_INCLUDE_DIR}
)

target_link_libraries(tetrispp PRIVATE
    ${CURSES_LIBRARIES}
)


if(MSVC)
    target_compile_options(tetrispp PRIVATE
        /W4
    )
else()
    target_compile_options(tetrispp PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        -Wshadow
        -Wconversion
    )
endif()


option(ENABLE_SANITIZERS "Enable sanitizers" ON)

if(ENABLE_SANITIZERS AND CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(tetrispp PRIVATE
        -fsanitize=address,undefined
    )

    target_link_options(tetrispp PRIVATE
        -fsanitize=address,undefined
    )
endif()


file(GLOB_RECURSE ALL_FILES CONFIGURE_DEPENDS
    src/*.cpp
    src/*.hpp
    src/*.h
)

find_program(CLANG_FORMAT clang-format)

if(CLANG_FORMAT)
    add_custom_target(format
        COMMAND ${CLANG_FORMAT} -i ${ALL_FILES}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )
endif()

install(TARGETS tetrispp
    RUNTIME DESTINATION bin
)

