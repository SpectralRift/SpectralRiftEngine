#if ANDROID

#include <windows/android_jni_app_window.hpp>
#include <utils/macros/debug.hpp>

#include <glad/egl.h>

namespace engine::windows {
    bool AndroidJNIAppWindow::init() {
        // create graphical context
        if (gfx_ctx == nullptr) {
            gfx_ctx = std::make_shared<engine::graphics::EGLGraphicsContext>(window);

            if (!gfx_ctx->init()) {
                DEBUG_MSG("failed to create a graphical context!\n");
                return false;
            }
        }

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

    void* AndroidJNIAppWindow::get_device_ctx() {
        return NULL;
    }

    void* AndroidJNIAppWindow::get_window_handle() {
        return (void*) window;
    }

    bool AndroidJNIAppWindow::handle_input_event(int action, int source, const utils::IVector2 &pos) {
        switch (action) {
            case AMOTION_EVENT_ACTION_UP:
                ev_mouse_key_up.invoke(pos, utils::enums::input::MouseButton::MOUSE_LEFT, true);
                break;
            case AMOTION_EVENT_ACTION_DOWN:
                ev_mouse_key_down.invoke(pos, utils::enums::input::MouseButton::MOUSE_LEFT, true);
                break;
            case AMOTION_EVENT_ACTION_MOVE:
            case AMOTION_EVENT_ACTION_HOVER_MOVE:
            case AMOTION_EVENT_ACTION_HOVER_EXIT:
            case AMOTION_EVENT_ACTION_HOVER_ENTER:
                if(source != AINPUT_SOURCE_JOYSTICK) {
                    ev_mouse_move.invoke(pos, true);
                } else {
                    DEBUG_MSG("joystick input! %ix%i", pos.x, pos.y)
                }
                break;
            default:
                return false;
        }

        return true;
    }
}
#endif