cmake_minimum_required(VERSION 3.10)
project(tetrispp)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE RelWithDebInfo)
endif()

file(GLOB_RECURSE SOURCE_FILES src/*.cpp)

add_executable(tetrispp ${SOURCE_FILES})

find_package(Curses REQUIRED)

target_include_directories(tetrispp PRIVATE ${CURSES_INCLUDE_DIR})
target_link_libraries(tetrispp ${CURSES_LIBRARIES})

file(GLOB_RECURSE ALL_FILES src/*.cpp src/*.h)

add_custom_target(
    format
    COMMAND clang-format -i ${ALL_FILES}
)
