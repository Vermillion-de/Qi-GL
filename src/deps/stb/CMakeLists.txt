# set(STB_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/stb_image.h")
# message(STATUS "STB IN ${STB_INCLUDE}")
add_library(STB_LIB INTERFACE)
target_include_directories(STB_LIB INTERFACE "./")