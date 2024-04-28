message("Compiling for ANDROID, so enforcing EGL!")

list(APPEND ENGINE_COMPILE_FLAGS ENGINE_TARGET_ANDROID)
list(APPEND ENGINE_LIBS android log ${CMAKE_DL_LIBS})

set(ENGINE_BACKEND_OPENGL ON)
set(ENGINE_BACKEND_USE_OPENGL_ES ON)
set(ENGINE_FEATURE_EGL_GL_CORE OFF)
set(ENGINE_FEATURE_EGL ON)

option(ENGINE_JNI "Adds support for the Java VM Native Interface (JNI) in the engine, which can be used in order to interface with Java-based classes." ON)
if (ENGINE_JNI)
    list(APPEND ENGINE_COMPILE_FLAGS ENGINE_FEATURE_JNI)
endif ()