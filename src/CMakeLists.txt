cmake_minimum_required(VERSION 3.0.0)

add_library(QiView_LIB "view.h" "view.cpp" "object.h" "object.cpp") 

## add library OpenGL and GLU
cmake_policy(SET CMP0072 OLD)
find_package(OpenGL REQUIRED)
message(STATUS "OpenGL lib at ${OPENGL_LIBRARY}")
target_link_libraries(QiView_LIB OpenGL::GL OpenGL::GLU)

## add library GLFW3
find_package(glfw3 REQUIRED)
message(STATUS "Found GLFW3 lib at ${GLFW_LIBRARY}")
target_link_libraries(QiView_LIB glfw)

## add library GLAD
add_subdirectory("./glad")
message(STATUS "Using GLAD in ./glad/")
target_link_libraries(QiView_LIB GLAD_LIB)

## add imgui
add_subdirectory("./imgui")
target_link_libraries(QiView_LIB ImGUI_GLFW_LIB)

## add stb_image.h

## add shaders

set(QiView_INCLUDE "view.h" "obj.h")
