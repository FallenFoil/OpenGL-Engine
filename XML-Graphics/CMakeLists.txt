cmake_minimum_required(VERSION 3.13)
project(XML-Graphics)

set(CMAKE_CXX_STANDARD 14)

include_directories(tinyxml2-master)

add_executable(XML-Graphics
        tinyxml2-master/tinyxml2.cpp
        tinyxml2-master/tinyxml2.h
        main.cpp Model.cpp Model.h Scene.cpp Scene.h)
