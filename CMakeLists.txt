cmake_minimum_required(VERSION 3.10)
project(tetrispp)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(tetrispp
    src/Game.cpp
    src/Input.cpp
    src/Render.cpp
    src/main.cpp
)


file(GLOB_RECURSE SOURCE_FILES  src/*.h src/*.cpp )

# format with clang-format
add_custom_target(
    format
    COMMAND /usr/bin/clang-format -i ${SOURCE_FILES}
    COMMENT "Running clang-format..."
    VERBOSE 1
)


# find ncurses
find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

# linking ncurses
target_link_libraries(tetrispp ${CURSES_LIBRARIES})
