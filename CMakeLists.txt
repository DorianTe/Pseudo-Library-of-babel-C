cmake_minimum_required(VERSION 3.3)
project(babel)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror -std=c99 -g")

set(SOURCE_FILES
    createText.c
    createText.h
    library.c
    library.h
    main.c)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})