#ifdef ENGINE_TARGET_LINUX

#include <windows/x11_app_window.hpp>
#include <graphics/contexts/egl_graphics_context.hpp>
#include <utils/macros/debug.hpp>

#include <GL/glx.h>

namespace engine::windows {
    utils::enums::input::MouseButton X11AppWindow::map_xorg_mouse_button(int button) {
        switch (button) {
            default: break;
            case Button1:
                return utils::enums::input::MouseButton::MOUSE_LEFT;
            case Button2:
                return utils::enums::input::MouseButton::MOUSE_MIDDLE;
            case Button3:
                return utils::enums::input::MouseButton::MOUSE_RIGHT;
        }

        return utils::enums::input::MouseButton::MOUSE_NONE;
    }

    utils::enums::input::KeyType X11AppWindow::map_xorg_key_type(KeySym key) {
        if (key >= XK_0 && key <= XK_9)
            return (utils::enums::input::KeyType) (utils::enums::input::KeyType::KEY_0 + (key - XK_0));
        else if (key >= XK_A && key <= XK_Z)
            return (utils::enums::input::KeyType) (utils::enums::input::KeyType::KEY_A + (key - XK_A));
        else if (key >= XK_a && key <= XK_z)
            return (utils::enums::input::KeyType) (utils::enums::input::KeyType::KEY_A + (key - XK_a));

        return utils::enums::input::KeyType::KEY_UNKNOWN;
    }

    bool X11AppWindow::init() {
        GLint glx_attr[] = {GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None};
        XSetWindowAttributes win_attr{0};

        // open default display
        display = XOpenDisplay(0);
        if (display == nullptr) {
            DEBUG_MSG("Cannot open default display!\n");
            return false;
        }

        auto root = DefaultRootWindow(display);
        auto glx_vi = glXChooseVisual(display, 0, glx_attr);

        if (glx_vi == nullptr) {
            printf("no appropriate glx_visual found\n");
            return false;
        }

        // create a color map
        auto color_map = XCreateColormap(display, root, glx_vi->visual, 0);

        // set win attributes
        win_attr.colormap = color_map;
        win_attr.event_mask = ExposureMask | KeyPressMask;

        window = XCreateWindow(display, root, 0, 0, 1, 1, 0, glx_vi->depth, InputOutput, glx_vi->visual,
                               CWColormap | CWEventMask, &win_attr);

        wm_delete_message = XInternAtom(display, "WM_DELETE_WINDOW", 0);
        XSetWMProtocols(display, window, &wm_delete_message, 1);

        quit_requested = false;

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

    void X11AppWindow::destroy() {
        BaseAppWindow::destroy();

        XDestroyWindow(display, window);
        XCloseDisplay(display);
    }

    void X11AppWindow::set_title(std::string_view title) {
        XStoreName(display, window, title.data());
    }

    void X11AppWindow::set_position(utils::IVector2 position) {
        XMoveWindow(display, window, position.x, position.y);
    }

    void X11AppWindow::set_size(utils::IVector2 size) {
        XResizeWindow(display, window, size.x, size.y);
    }

    utils::IVector2 X11AppWindow::get_size() {
        XWindowAttributes xwa;
        XGetWindowAttributes(display, window, &xwa);

        return {xwa.width, xwa.height};
    }

    utils::IVector2 X11AppWindow::get_position() {
        XWindowAttributes xwa;
        XGetWindowAttributes(display, window, &xwa);

        return {xwa.x, xwa.y};
    }

    void X11AppWindow::process_events() {
        XEvent ev;

        // allow the reporting of the mentioned events
        XSelectInput(display, window, KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask);

        while(XPending(display)) {
            XNextEvent(display, &ev);

            switch (ev.type) {
                case MotionNotify:
                    ev_mouse_move.invoke({ev.xmotion.x, ev.xmotion.y}, false);
                    break;
                case ButtonPress:
                    ev_mouse_key_down.invoke({ev.xmotion.x, ev.xmotion.y}, map_xorg_mouse_button(ev.xbutton.button), false);
                    break;
                case ButtonRelease:
                    ev_mouse_key_up.invoke({ev.xmotion.x, ev.xmotion.y}, map_xorg_mouse_button(ev.xbutton.button), false);
                    break;
                case KeyPress:
                    ev_key_down.invoke(map_xorg_key_type(XLookupKeysym(&ev.xkey, 0)));
                    break;
                case KeyRelease:
                    ev_key_up.invoke(map_xorg_key_type(XLookupKeysym(&ev.xkey, 0)));
                    break;
                case Expose:
                    XWindowAttributes xwa;
                    XGetWindowAttributes(display, window, &xwa);
                    break;
                case ClientMessage:
                    if (ev.xclient.data.l[0] == wm_delete_message) {
                        quit_requested = true;
                    }
                    break;
                default:
                    break;
            }
        }
    }

    void X11AppWindow::update() {
        // no updating in xlib
    }

    void X11AppWindow::show() {
        XMapWindow(display, window);
    }

    void X11AppWindow::hide() {
        XUnmapWindow(display, window);
    }

    void *X11AppWindow::get_device_ctx() {
        return NULL;
    }

    void *X11AppWindow::get_window_handle() {
        return (void *) window;
    }
}
#endif