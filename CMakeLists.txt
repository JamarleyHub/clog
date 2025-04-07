cmake_minimum_required(VERSION 3.30)
project(clog VERSION 1.1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 17)

add_library(clog SHARED src/clog.c)

add_subdirectory(src)