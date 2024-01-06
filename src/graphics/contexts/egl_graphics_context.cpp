#if defined(ENGINE_FEATURE_EGL)
#include <graphics/contexts/egl_graphics_context.hpp>

#include <system_error>

#include <utils/macros/debug.hpp>
#include <engine.hpp>

#ifdef ENGINE_TARGET_ANDROID

#include <android/native_window.h>
#include <android/native_window_jni.h>
#include <platform/platform.hpp>

#endif

namespace engine::graphics {
    std::string_view EGLGraphicsContext::egl_get_error_string(int error) {
        switch (error) {
            case EGL_SUCCESS:
                return "EGL_SUCCESS";
            case EGL_NOT_INITIALIZED:
                return "EGL_NOT_INITIALIZED";
            case EGL_BAD_ACCESS:
                return "EGL_BAD_ACCESS";
            case EGL_BAD_ALLOC:
                return "EGL_BAD_ALLOC";
            case EGL_BAD_ATTRIBUTE:
                return "EGL_BAD_ATTRIBUTE";
            case EGL_BAD_CONTEXT:
                return "EGL_BAD_CONTEXT";
            case EGL_BAD_CONFIG:
                return "EGL_BAD_CONFIG";
            case EGL_BAD_CURRENT_SURFACE:
                return "EGL_BAD_CURRENT_SURFACE";
            case EGL_BAD_DISPLAY:
                return "EGL_BAD_DISPLAY";
            case EGL_BAD_SURFACE:
                return "EGL_BAD_SURFACE";
            case EGL_BAD_MATCH:
                return "EGL_BAD_MATCH";
            case EGL_BAD_PARAMETER:
                return "EGL_BAD_PARAMETER";
            case EGL_BAD_NATIVE_PIXMAP:
                return "EGL_BAD_NATIVE_PIXMAP";
            case EGL_BAD_NATIVE_WINDOW:
                return "EGL_BAD_NATIVE_WINDOW";
            case EGL_CONTEXT_LOST:
                return "EGL_CONTEXT_LOST";
            default:
                return "Unknown";
        }
    }

    uintptr_t EGLGraphicsContext::egl_loader(std::string_view name) {
        return engine::get_graphics_context()->get_proc_func(name);
    }

    bool EGLGraphicsContext::init(windows::BaseAppWindowSPtr window) {
        const EGLint attribsCtx[] = {
                EGL_CONTEXT_CLIENT_VERSION, 2,
                EGL_NONE
        };

        const EGLint attribs[] = {
                EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
#if defined(ENGINE_TARGET_ANDROID)
                EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
#endif
                EGL_BLUE_SIZE, 8,
                EGL_GREEN_SIZE, 8,
                EGL_RED_SIZE, 8,
                EGL_NONE
        };

        EGLConfig config;
        EGLint numConfigs;
        EGLint format;

        int egl_version = gladLoaderLoadEGL(EGL_DEFAULT_DISPLAY);

        if (!egl_version) {
            DEBUG_MSG("Failed to load EGL through glad!\n")
            return false;
        }

        DEBUG_MSG("Loaded EGL %d.%d.\n", GLAD_VERSION_MAJOR(egl_version),
                  GLAD_VERSION_MINOR(egl_version))

        if ((display = eglGetDisplay(EGL_DEFAULT_DISPLAY)) == EGL_NO_DISPLAY) {
            DEBUG_MSG("eglGetDisplay failed! %s\n", egl_get_error_string(eglGetError()).data())
            return false;
        }

        if (!eglInitialize(display, 0, 0)) {
            DEBUG_MSG("eglInitialize failed! %s\n", egl_get_error_string(eglGetError()).data())
            return false;
        }

        DEBUG_MSG("eglChooseConfig\n")

        if (!eglChooseConfig(display, attribs, &config, 1, &numConfigs)) {
            DEBUG_MSG("eglChooseConfig failed! %s\n", egl_get_error_string(eglGetError()).data())
            return false;
        }

        DEBUG_MSG("eglCreateWindowSurface\n")

        if (!(surface = eglCreateWindowSurface(display, config,
                                               (EGLNativeWindowType) window->get_window_handle(),
                                               0))) {
            DEBUG_MSG("eglCreateWindowSurface failed! %s\n",
                      egl_get_error_string(eglGetError()).data())
            return false;
        }

        DEBUG_MSG("eglGetConfigAttrib\n")

        if (!eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format)) {
            DEBUG_MSG("eglGetConfigAttrib failed! %s\n",
                      egl_get_error_string(eglGetError()).data())
            return false;
        }

#if defined(ENGINE_TARGET_ANDROID)
        DEBUG_MSG("ANativeWindow_setBuffersGeometry\n")

        DEBUG_MSG("our window PTR %x\n", window->get_window_handle())

        // set buffers geometry - only on Android
        ANativeWindow_setBuffersGeometry((ANativeWindow *) window->get_window_handle(), 0, 0,
                                         format);
#endif

        DEBUG_MSG("eglCreateContext\n")

        if (!(context = eglCreateContext(display, config, 0, attribsCtx))) {
            DEBUG_MSG("eglCreateContext failed! %s\n", egl_get_error_string(eglGetError()).data())
            return false;
        }

        return true;
    }

    void EGLGraphicsContext::use() {
        if (!eglMakeCurrent(display, surface, surface, context)) {
            DEBUG_MSG("eglMakeCurrent failed! %s\n", egl_get_error_string(eglGetError()).data())
        }
    }

    void EGLGraphicsContext::present() {
        if (!eglSwapBuffers(display, surface)) {
            DEBUG_MSG("eglSwapBuffers failed! %s\n", egl_get_error_string(eglGetError()).data())
        }
    }

    void EGLGraphicsContext::discard() {
        if (!eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT)) {
            DEBUG_MSG("eglMakeCurrent failed! %s\n", egl_get_error_string(eglGetError()).data())
        }
    }

    void EGLGraphicsContext::destroy() {
        discard();
        eglDestroyContext(display, context);
        eglDestroySurface(display, surface);
        eglTerminate(display);
    }

    uintptr_t EGLGraphicsContext::get_proc_func(std::string_view name) {
        return reinterpret_cast<uintptr_t>(eglGetProcAddress(name.data()));
    }
}
#endif