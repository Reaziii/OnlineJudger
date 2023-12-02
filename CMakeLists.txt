cmake_minimum_required(VERSION 3.26)
project(Judgerv2 C)

set(CMAKE_C_STANDARD 11)

add_executable(Judgerv2 main.c
        utils/utils.h
        utils/utils.c
        utils/enums.h
        utils/config.h
        runner.h
        runner.c)
