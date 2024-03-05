add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/vendor/glad2/cmake" glad2)

if (ENGINE_BACKEND_USE_OPENGL_ES)
    glad_add_library(glad2 STATIC REPRODUCIBLE LOADER API gles2=3.0)
else ()
    glad_add_library(glad2 STATIC REPRODUCIBLE LOADER API gl:compatibility=3.0)
endif ()