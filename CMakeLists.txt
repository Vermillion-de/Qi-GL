cmake_minimum_required(VERSION 3.0.0)
project(Qi-GL)

add_executable(main "main.cpp")
add_subdirectory("./src")
include_directories("./src/")

target_link_libraries(main QiView_LIB)