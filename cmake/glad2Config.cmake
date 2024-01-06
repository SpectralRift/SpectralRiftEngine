set(GLAD_SOURCES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/vendor/glad2")
add_subdirectory("${GLAD_SOURCES_DIR}/cmake" glad2)

set(GLAD_APIS)

if(ENGINE_FEATURE_EGL)
    message("EGL GLAD")
    list(APPEND GLAD_APIS API gles2=3.0 API egl=1.5)
else()
    list(APPEND GLAD_APIS API gl:compatibility=3.0)
endif()

glad_add_library(glad2 STATIC REPRODUCIBLE LOADER ${GLAD_APIS})