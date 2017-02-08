cmake_minimum_required(VERSION 3.4.3)
project(SBox)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror -std=gnu99")

set(SOURCE_FILES main.c Inglenook.c Inglenook.h Utils.h Utils.c)
add_executable(SBox ${SOURCE_FILES})