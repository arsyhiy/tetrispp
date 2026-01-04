# #
# # cmake_minimum_required(VERSION 3.16)
# # project(my_game LANGUAGES CXX)
# #
# # set(CMAKE_CXX_STANDARD 20)
# # set(CMAKE_CXX_STANDARD_REQUIRED ON)
# #
# # # Поиск всех .cpp файлов в директории src и подкаталогах
# # file(GLOB_RECURSE SOURCE_FILES src/*.cpp)
# #
# # # Создание исполнимого файла
# # add_executable(my_game ${SOURCE_FILES})
# #
# # # Форматирование с помощью clang-format
# # add_custom_target(
# #     format
# #     COMMAND /usr/bin/clang-format -i ${SOURCE_FILES}
# #     COMMENT "Running clang-format..."
# #     VERBOSE 1
# # )
# #
# # # Теперь можно использовать make format

# find_package(Curses REQUIRED)
# include_directories(${CURSES_INCLUDE_DIR})

# target_link_libraries(my_game ${CURSES_LIBRARIES})

# cmake_minimum_required(VERSION 3.16)
# project(my_game LANGUAGES CXX)

# set(CMAKE_CXX_STANDARD 20)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)

# add_executable(my_game
# #    src/Game.cpp
# #    src/Render.cpp
# #    src/Input.cpp
#     src/main.cpp
# #    src/game_state.cpp
# )

# # Поиск всех .cpp и .h файлов в директории src и подкаталогах
# file(GLOB_RECURSE SOURCE_FILES src/*.cpp src/*.h)

# add_custom_target(
#      format
#      COMMAND /usr/bin/clang-format -i ${SOURCE_FILES}
#      COMMENT "Running clang-format..."
#      VERBOSE 1
#  )

# # Теперь можно использовать make format


cmake_minimum_required(VERSION 3.10)
project(tetrispp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Собираем все исходники
add_executable(tetrispp
    src/main.cpp
)

# Ищем ncurses
find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

# Линкуем ncurses
target_link_libraries(tetrispp ${CURSES_LIBRARIES})

