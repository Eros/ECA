cmake_minimum_required(VERSION 3.9)
project(ECA C)

set(CMAKE_C_STANDARD 99)

add_library(ECA library.c library.h ECA.c)