cmake_minimum_required(VERSION 3.24)
project(project C)

set(CMAKE_C_STANDARD 11)

find_package(CURL REQUIRED)

add_executable(project main.c)

target_link_libraries(project ${CURL_LIBRARIES})