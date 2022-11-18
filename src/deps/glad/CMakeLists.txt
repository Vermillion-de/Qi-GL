add_library(GLAD_LIB STATIC
            "./src/glad.c"
            "./include/glad.h"
            "./include/khrplatform.h"
        )
target_include_directories(GLAD_LIB PUBLIC "./include")
