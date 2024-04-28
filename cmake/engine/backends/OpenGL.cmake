find_package(PkgConfig REQUIRED)

if (ENGINE_BACKEND_OPENGL)
    option(ENGINE_BACKEND_USE_OPENGL_ES "Uses OpenGL ES instead of OpenGL." OFF)
    option(ENGINE_FEATURE_EGL "Add support for EGL in order to fetch a graphics context for your window." OFF)
    option(ENGINE_FEATURE_EGL_GL_CORE "Make EGL initialise a OpenGL Core graphics context by default." OFF)

    list(APPEND ENGINE_COMPILE_FLAGS ENGINE_BACKEND_OPENGL)

    find_package(glad2 CONFIG REQUIRED)

    if (WIN32)
        list(APPEND ENGINE_LIBS opengl32)
    else()
        list(APPEND ENGINE_LIBS GL)
    endif ()

    if (ENGINE_FEATURE_EGL)
        pkg_search_module(EGL REQUIRED egl)

        include_directories(${EGL_INCLUDE_DIRS})

        list(APPEND ENGINE_COMPILE_FLAGS ENGINE_FEATURE_EGL ENGINE_FEATURE_EGL_SYSTEM)
        list(APPEND ENGINE_LIBS EGL)

        target_compile_definitions(glad2 PUBLIC GLAD_GLES2_USE_SYSTEM_EGL=1)
        target_link_libraries(glad2 PUBLIC EGL)
    endif ()

    if (ENGINE_BACKEND_USE_OPENGL_ES)
        pkg_search_module(GLES2 REQUIRED glesv2)

        include_directories(${GLES2_INCLUDE_DIRS})
        list(APPEND ENGINE_LIBS GLESv2)
        list(APPEND ENGINE_COMPILE_FLAGS ENGINE_BACKEND_OPENGL_ES)
    endif ()

    if (ENGINE_FEATURE_EGL_GL_CORE)
        list(APPEND ENGINE_COMPILE_FLAGS ENGINE_FEATURE_EGL_GL_CORE)
    endif ()

    list(APPEND ENGINE_LIBS glad2)
endif ()