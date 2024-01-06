#ifdef ENGINE_TARGET_LINUX
#include <windows/x11_app_window.hpp>
#include <utils/macros/debug.hpp>

#include <GL/glx.h>

namespace engine::windows
{
    bool X11AppWindow::init()
    {
        GLint glx_attr[] = {GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None};
        XSetWindowAttributes win_attr{0};

        // init threads
        if (XInitThreads() == NULL)
        {
            DEBUG_MSG("XInitThreads failed!\n");
            return false;
        }
        else
        {
        }

        // open default display
        display = XOpenDisplay(0);
        if (display == NULL)
        {
            DEBUG_MSG("Cannot open default display!\n");
            return false;
        }

        auto root = DefaultRootWindow(display);
        auto glx_vi = glXChooseVisual(display, 0, glx_attr);

        if (glx_vi == NULL)
        {
            printf("no appropriate glx_visual found\n");
            return false;
        }

        // create a color map
        auto color_map = XCreateColormap(display, root, glx_vi->visual, 0);

        // set win attributes
        win_attr.colormap = color_map;
        win_attr.event_mask = ExposureMask | KeyPressMask;

        window = XCreateWindow(display, root, 0, 0, 1, 1, 0, glx_vi->depth, InputOutput, glx_vi->visual, CWColormap | CWEventMask, &win_attr);

        wm_delete_message = XInternAtom(display, "WM_DELETE_WINDOW", 0);
        XSetWMProtocols(display, window, &wm_delete_message, 1);

        quit_requested = false;

        return true;
    }

    void X11AppWindow::destroy()
    {
        XDestroyWindow(display, window);
        XCloseDisplay(display);
    }

    void X11AppWindow::set_title(std::string_view title)
    {
        int ret = XStoreName(display, window, title.data());
    }

    void X11AppWindow::set_position(utils::IVector2 position)
    {
        XMoveWindow(display, window, position.x, position.y);
    }

    void X11AppWindow::set_size(utils::IVector2 size)
    {
        XResizeWindow(display, window, size.x, size.y);
    }

    utils::IVector2 X11AppWindow::get_size()
    {
        XWindowAttributes xwa;
        XGetWindowAttributes(display, window, &xwa);

        return {xwa.width, xwa.height};
    }

    utils::IVector2 X11AppWindow::get_position()
    {
        XWindowAttributes xwa;
        XGetWindowAttributes(display, window, &xwa);

        return {xwa.x, xwa.y};
    }

    void X11AppWindow::process_events()
    {
        XEvent ev;
        long event_filters = KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask;

        // allow the reporting of the mentioned events
        XSelectInput(display, window, event_filters);

        // lock the access to the display so that we could check for events
        XLockDisplay(display);

        // check if we have any window events
        if (XCheckTypedEvent(display, ClientMessage, &ev))
        {
            if (ev.xclient.data.l[0] == wm_delete_message)
            {
                quit_requested = true;
            }
        }
        else if (XCheckWindowEvent(display, window, event_filters, &ev))
        {
            // DEBUG_MSG("X11 Event: %i\n", ev.type);
        }

        // unlock access to this display
        XUnlockDisplay(display);
    }

    void X11AppWindow::update()
    {
        // no updating in xlib
    }

    void X11AppWindow::show()
    {
        XMapWindow(display, window);
    }

    void X11AppWindow::hide()
    {
        XUnmapWindow(display, window);
    }

    uintptr_t X11AppWindow::get_device_ctx() {}
    uintptr_t X11AppWindow::get_window_handle() {}
}
#endif