cmake_minimum_required(VERSION 3.13)
project(Generator)

set(CMAKE_CXX_STANDARD 14)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)


add_executable(Generator main.cpp Ponto.cpp Ponto.h)