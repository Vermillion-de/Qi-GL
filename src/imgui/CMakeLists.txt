cmake_minimum_required(VERSION 3.0.0)
add_library(ImGUI_GLFW_LIB STATIC 
                    "imconfig.h"
                    "imgui.h" "imgui.cpp" 
                    "imgui_draw.cpp"
                    "imgui_internal.h"
                    "imgui_tables.cpp"
                    "imgui_widgets.cpp"
                    "imstb_rectpack.h"
                    "imstb_textedit.h"
                    "imstb_truetype.h"
                    "imgui_impl_glfw.h" "imgui_impl_glfw.cpp" 
                    "imgui_impl_opengl2.h" "imgui_impl_opengl2.cpp")
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
set(ImGUI_GLFW_HEADER "${CMAKE_CURRENT_SOURCE_DIR}/imgui_glfw.h")
