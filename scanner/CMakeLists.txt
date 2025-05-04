cmake_minimum_required(VERSION 3.30)
project(scanner C)

set(CMAKE_C_STANDARD 17)

add_executable(scanner main.c
        tokens.h
        scanner.c
        cPreprocesado.c
        ejemplo.c
)
