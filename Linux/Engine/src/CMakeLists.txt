cmake_minimum_required(VERSION 2.8)

# Project Name
PROJECT(Engine)

set(TOOLKITS_FOLDER ${CMAKE_SOURCE_DIR}/../toolkits)

include_directories(${TOOLKITS_FOLDER}/tinyxml2)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_executable(${PROJECT_NAME}  main.cpp Model.cpp Model.h Scene.cpp Scene.h group.cpp group.h Transformation.cpp Transformation.h ${TOOLKITS_FOLDER}/tinyxml2/tinyxml2.cpp ${TOOLKITS_FOLDER}/tinyxml2/tinyxml2.h)

find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS})
link_directories(${OpenGL_LIBRARY_DIRS})
add_definitions(${OpenGL_DEFINITIONS})

if(NOT OPENGL_FOUND)
    message(ERROR " OPENGL not found!")
endif(NOT OPENGL_FOUND)

set( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} Wno-deprecated")
find_package(GLUT REQUIRED)
include_directories(${GLUT_INCLUDE_DIR})
link_directories(${GLUT_LIBRARY_DIRS})
add_definitions(${GLUT_DEFINITIONS})

find_package(GLEW REQUIRED)
if (GLEW_FOUND)
    include_directories(${GLEW_INCLUDE_DIRS})
    link_libraries(${GLEW_LIBRARIES})
endif()

find_package(DevIL REQUIRED)
link_libraries(${IL_LIBRARIES} )
include_directories(${IL_INCLUDE_DIR})

link_libraries(${TOOLKITS_FOLDER}/AntTweakBar/lib/)
include_directories(${TOOLKITS_FOLDER}/AntTweakBar/include/)


target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${GLEW_LIBRARIES} ${IL_LIBRARIES} ${TOOLKITS_FOLDER}/AntTweakBar/lib/libAntTweakBar.so)
if(NOT GLUT_FOUND)
    message(ERROR ": GLUT not found!")
endif(NOT GLUT_FOUND)
