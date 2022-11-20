add_library(GL_EGINE STATIC "glegine.cpp" "./include/glegine.h" "shader.h" "texture.h")
target_include_directories(GL_EGINE PUBLIC "./include/") 