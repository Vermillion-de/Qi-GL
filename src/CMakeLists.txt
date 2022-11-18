cmake_minimum_required(VERSION 3.0.0)

## add depdences
add_subdirectory("./deps/")

## add library OpenGL and GLU
cmake_policy(SET CMP0072 OLD)
find_package(OpenGL REQUIRED)

## add library GLM
include_directories(${GLM_INCLUDE_DIRS})
link_directories(${GLM_LIBRARY_DIRS})
add_definitions(${GLM_DEFINITIONS})

## add my algorithms
add_subdirectory("./algorithms/")

## make QiObj lib
add_library(QiObj STATIC "object.h" "object.cpp" "texture.h" "shader.h")
target_link_libraries(QiObj GLAD_LIB)
target_link_libraries(QiObj OpenGL::GL OpenGL::GLU)
target_link_libraries(QiObj STB_LIB)

## make view things
add_library(QiView_LIB STATIC "view.h" "view.cpp") 
target_link_libraries(QiView_LIB GLAD_LIB)
target_link_libraries(QiView_LIB QiObj)
target_link_libraries(QiView_LIB OpenGL::GL OpenGL::GLU)
target_link_libraries(QiView_LIB glfw)
target_link_libraries(QiView_LIB ImGUI_GLFW_LIB)
target_link_libraries(QiView_LIB ALG_LIB)
target_include_directories(QiView_LIB PUBLIC "./")
