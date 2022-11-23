cmake_minimum_required(VERSION 3.23)
project(morze C)

set(CMAKE_C_STANDARD 99)

add_executable(morze main.c paramCheck.c paramCheck.h treeHandling.c treeHandling.h translateText.c translateText.h)
