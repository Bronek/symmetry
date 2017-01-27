cmake_minimum_required(VERSION 3.6)
project(symmetry)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native -Wall -pedantic -Wextra")

set(SOURCE_FILES main.cpp)

find_package(GTest REQUIRED)

include_directories(${LIB_INCLUDE})

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} ${GTEST_BOTH_LIBRARIES})
