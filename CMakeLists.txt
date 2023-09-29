cmake_minimum_required(VERSION 3.27.4)

project(SDL_PONG VERSION 1.0 LANGUAGES C)

find_package(SDL2 REQUIRED)

INCLUDE_DIRECTORIES(${SDL2_INCLUDE_DIRS})

file(GLOB all_files
    "${PROJECT_SOURCE_DIR}/*.c"
    "${PROJECT_SOURCE_DIR}/*.h"
)
add_executable(${PROJECT_NAME} main.c ${all_files})
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES} SDL2_image)



