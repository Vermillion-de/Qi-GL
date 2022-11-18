cmake_minimum_required(VERSION 3.0.0)
add_library(ImGUI_GLFW_LIB STATIC 
                    "./include/imconfig.h"
                    "./include/imgui.h" "./src/imgui.cpp" 
                    "./src/imgui_draw.cpp"
                    "./include/imgui_internal.h"
                    "./src/imgui_tables.cpp"
                    "./src/imgui_widgets.cpp"
                    "./include/imstb_rectpack.h"
                    "./include/imstb_textedit.h"
                    "./include/imstb_truetype.h"
                    "./include/imgui_impl_glfw.h" "./src/imgui_impl_glfw.cpp" 
                    "./include/imgui_impl_opengl2.h" "./src/imgui_impl_opengl2.cpp"
                    "./include/imgui_impl_opengl3.h" "./src/imgui_impl_opengl3.cpp" "./include/imgui_impl_opengl3_loader.h")
target_include_directories(ImGUI_GLFW_LIB PUBLIC "./include")
# set(ImGUI_GLFW_HEADER "${CMAKE_CURRENT_SOURCE_DIR}/imgui_glfw.h")
