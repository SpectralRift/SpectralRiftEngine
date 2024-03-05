cmake_minimum_required(VERSION 3.13.0)
project(SpectralRiftEngine VERSION 0.7.6)

find_package(PkgConfig REQUIRED)

# define standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# configure CMake dirs in order to detect vendor dependencies
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")
list(APPEND CMAKE_PREFIX_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")

# detect cross-compilation
if (CMAKE_TOOLCHAIN_FILE)
    message("Detected toolchain file, so the engine is currently cross-compiled.")
    list(APPEND CMAKE_FIND_ROOT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")
endif ()

# include code
file(GLOB_RECURSE ENGINE_SOURCE_FILES "src/*.cpp")
file(GLOB_RECURSE ENGINE_HEADER_FILES "src/*.hpp")

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/src")

# configuration
option(ENGINE_BACKEND_OPENGL "Adds OpenGL backend support to the engine. It uses GLAD internally to load the OpenGL API." ON)
if (ENGINE_BACKEND_OPENGL)
    option(ENGINE_BACKEND_USE_OPENGL_ES "Uses OpenGL ES instead of OpenGL." OFF)
    option(ENGINE_FEATURE_EGL "Add support for EGL in order to fetch a graphics context for your window." OFF)
    option(ENGINE_FEATURE_EGL_GL_CORE "Make EGL initialise a OpenGL Core graphics context." OFF)
endif ()

# define the library
add_library(SpectralRiftEngine STATIC ${ENGINE_SOURCE_FILES} ${ENGINE_HEADER_FILES})

# define sources
target_sources(SpectralRiftEngine
        PUBLIC
        ${ENGINE_HEADER_FILES}
        PRIVATE
        ${ENGINE_SOURCE_FILES}
)

# add include dirs to be accessible by everyone
target_include_directories(SpectralRiftEngine PUBLIC src)

set(ENGINE_LIBS)
set(ENGINE_COMPILE_FLAGS ENGINE_BUILD_LIBRARY)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    message("Engine compiled with debug flags!")
    list(APPEND ENGINE_COMPILE_FLAGS ENGINE_FLAVOR_DEBUG)
endif ()

if (WIN32)
    list(APPEND ENGINE_COMPILE_FLAGS ENGINE_TARGET_WIN32)
    list(APPEND ENGINE_LIBS opengl32)
endif ()

if (LINUX)
    list(APPEND ENGINE_COMPILE_FLAGS ENGINE_TARGET_LINUX)
    list(APPEND ENGINE_LIBS X11 GL ${CMAKE_DL_LIBS})

    set(ENGINE_BACKEND_OPENGL ON)
    set(ENGINE_FEATURE_EGL ON)
endif ()

if (NINTENDO_SWITCH)
    message("Compiling for Nintendo Switch using devkitPRO; EGL/GLES enforced")

    set(ENGINE_BACKEND_OPENGL ON)
    set(ENGINE_BACKEND_USE_OPENGL_ES ON)
    set(ENGINE_FEATURE_EGL ON)
    set(ENGINE_FEATURE_EGL_GL_CORE ON)
endif ()

if (CMAKE_SYSTEM_NAME MATCHES "Android")
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
endif ()

if (ENGINE_BACKEND_OPENGL)
    list(APPEND ENGINE_COMPILE_FLAGS ENGINE_BACKEND_OPENGL)

    find_package(glad2 CONFIG REQUIRED)
    find_package(glm CONFIG REQUIRED)

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

    list(APPEND ENGINE_LIBS glad2 glm)
endif ()

# put the drivers at the end; for some reason if they aren't in the last plugins, it gives many linker errors
if (NINTENDO_SWITCH)
    pkg_search_module(DRM_NOUVEAU REQUIRED libdrm_nouveau)

    list(APPEND ENGINE_LIBS drm_nouveau nx glapi)
    list(APPEND ENGINE_COMPILE_FLAGS ENGINE_TARGET_SWITCH)
endif ()

find_package(imgui CONFIG REQUIRED)
list(APPEND ENGINE_LIBS imgui)

list(REMOVE_DUPLICATES ENGINE_LIBS)

target_link_libraries(SpectralRiftEngine PUBLIC ${ENGINE_LIBS})
target_compile_definitions(SpectralRiftEngine PUBLIC ${ENGINE_COMPILE_FLAGS})