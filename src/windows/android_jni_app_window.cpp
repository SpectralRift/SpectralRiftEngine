#if ANDROID

#include <windows/android_jni_app_window.hpp>
#include <utils/macros/debug.hpp>

#include <glad/egl.h>

namespace engine::windows {
    bool AndroidJNIAppWindow::init() {
        return true;
    }

    void AndroidJNIAppWindow::destroy() {
        ANativeWindow_release(window);
    }

    void AndroidJNIAppWindow::set_title(std::string_view title) {
        // wont work on mobile
    }

    void AndroidJNIAppWindow::set_position(utils::IVector2 position) {
        // wont work on mobile
    }

    void AndroidJNIAppWindow::set_size(utils::IVector2 size) {
        // wont work on mobile
    }

    utils::IVector2 AndroidJNIAppWindow::get_size() {
        return {ANativeWindow_getWidth(window), ANativeWindow_getHeight(window)};
    }

    utils::IVector2 AndroidJNIAppWindow::get_position() {
        return {0, 0};
    }

    void AndroidJNIAppWindow::process_events() {}

    void AndroidJNIAppWindow::update() {}

    void AndroidJNIAppWindow::show() {}

    void AndroidJNIAppWindow::hide() {}

    uintptr_t AndroidJNIAppWindow::get_device_ctx() {
        return PTR_NULL;
    }

    uintptr_t AndroidJNIAppWindow::get_window_handle() {
        return (uintptr_t) window;
    }
}
#endif