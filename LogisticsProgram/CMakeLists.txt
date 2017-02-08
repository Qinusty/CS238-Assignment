cmake_minimum_required(VERSION 3.4.3)
project(LogisticsProgram)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lboost_system -std=c++11 -lpthread -Wall")
set(SOURCE_FILES main.cpp YardManager.cpp YardManager.h siding.cpp siding.h Utils.cpp Utils.h vehicle.cpp vehicle.h client.cpp client.h)
add_executable(SLP ${SOURCE_FILES})
