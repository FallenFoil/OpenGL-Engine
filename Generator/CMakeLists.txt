cmake_minimum_required(VERSION 3.13)
project(Generator)

set(CMAKE_CXX_STANDARD 14)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

#set ( PROJECT_LINK_LIBS libPonto.so )
#link_directories( ../shared)
#include_directories(../shared)


add_executable(Generator main.cpp)
target_link_libraries(Generator ${PROJECT_LINK_LIBS} )