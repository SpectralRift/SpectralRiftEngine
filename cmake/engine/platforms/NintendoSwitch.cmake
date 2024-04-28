find_package(PkgConfig REQUIRED)

message("Compiling for Nintendo Switch using devkitPRO; EGL/GLES enforced")

set(ENGINE_BACKEND_OPENGL ON)
set(ENGINE_BACKEND_USE_OPENGL_ES ON)
set(ENGINE_FEATURE_EGL ON)
set(ENGINE_FEATURE_EGL_GL_CORE ON)

# put the drivers at the end; for some reason if they aren't in the last plugins, it gives many linker errors
pkg_search_module(DRM_NOUVEAU REQUIRED libdrm_nouveau)

list(APPEND ENGINE_LIBS drm_nouveau nx glapi)
list(APPEND ENGINE_COMPILE_FLAGS ENGINE_TARGET_SWITCH)