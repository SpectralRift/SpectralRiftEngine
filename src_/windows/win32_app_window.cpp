#ifdef ENGINE_TARGET_WIN32

#include <system_error>

#include <windows/win32_app_window.hpp>
#include <utils/macros/debug.hpp>
#include <graphics/contexts/win32_graphics_context.hpp>

namespace engine::windows {
    // call this function in order to make sure that our window is initialised before we make any API calls to WinAPI
    void Win32AppWindow::assert_window_init() {
        if (!window_handle) {
            ASSERT_MSG(init(), "could not init window!\n");
        }
    }

    LRESULT CALLBACK Win32AppWindow::window_processor_static(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
        auto *itx = reinterpret_cast<Win32AppWindow *>(GetWindowLongA(hWnd, GWLP_USERDATA));

        if (itx)
            return itx->window_processor(hWnd, message, wParam, lParam);
        else
            return DefWindowProcA(hWnd, message, wParam, lParam);
    }

    utils::enums::input::KeyType Win32AppWindow::map_key_type(uintptr_t wParam) {
        switch(wParam) {
            case VK_SPACE:
                return utils::enums::input::KEY_SPACE;
            default:
                return (utils::enums::input::KeyType) wParam;
        }
    }

    LRESULT CALLBACK Win32AppWindow::window_processor(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
        switch (message) {
            case WM_SHOWWINDOW:
                DEBUG_MSG("window shown!\n");
                break;
            case WM_SIZE:
                ev_window_resize.invoke({LOWORD(lParam), HIWORD(lParam)});
                break;
            case WM_SYSCOMMAND:
                if (wParam == SC_CLOSE) {
                    DEBUG_MSG("received close system message\n");
                    quit();

                    return 0;
                }
                break;
            case WM_KEYDOWN:
                ev_key_down.invoke(map_key_type(wParam));
                break;
            case WM_KEYUP:
                ev_key_up.invoke(map_key_type(wParam));
                break;
            case WM_MOUSEMOVE:
                ev_mouse_move.invoke({LOWORD(lParam), HIWORD(lParam)}, false);
                break;
            case WM_LBUTTONDOWN:
                ev_mouse_key_down.invoke({LOWORD(lParam), HIWORD(lParam)},
                                         utils::enums::input::MouseButton::MOUSE_LEFT, false);
                break;
            case WM_LBUTTONUP:
                ev_mouse_key_up.invoke({LOWORD(lParam), HIWORD(lParam)},
                                       utils::enums::input::MouseButton::MOUSE_LEFT, false);
                break;
            case WM_MBUTTONDOWN:
                ev_mouse_key_down.invoke({LOWORD(lParam), HIWORD(lParam)},
                                         utils::enums::input::MouseButton::MOUSE_MIDDLE, false);
                break;
            case WM_MBUTTONUP:
                ev_mouse_key_up.invoke({LOWORD(lParam), HIWORD(lParam)},
                                       utils::enums::input::MouseButton::MOUSE_MIDDLE, false);
                break;
            case WM_RBUTTONDOWN:
                ev_mouse_key_down.invoke({LOWORD(lParam), HIWORD(lParam)},
                                         utils::enums::input::MouseButton::MOUSE_RIGHT, false);
                break;
            case WM_RBUTTONUP:
                ev_mouse_key_up.invoke({LOWORD(lParam), HIWORD(lParam)},
                                       utils::enums::input::MouseButton::MOUSE_RIGHT, false);
                break;
            default:
                break;
        }

        return DefWindowProcA(hWnd, message, wParam, lParam);
    }

    bool Win32AppWindow::engine_window_class_exists() {
        WNDCLASSA c;
        return GetClassInfoA(GetModuleHandleW(NULL), default_win_class_name, &c) > 0;
    }

    bool Win32AppWindow::init() {
        // register class if there is no window class defined
        if (!engine_window_class_exists()) {
            WNDCLASSEXA win_class;

            win_class.cbSize = sizeof(WNDCLASSEX);

            win_class.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
            win_class.lpfnWndProc = window_processor_static;
            win_class.cbClsExtra = 0;
            win_class.cbWndExtra = 0;
            win_class.hInstance = GetModuleHandleW(NULL);
            win_class.hIcon = LoadIcon(NULL, IDI_APPLICATION);
            win_class.hIconSm = win_class.hIcon;
            win_class.hCursor = LoadCursor(nullptr, IDC_ARROW);
            win_class.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
            win_class.lpszMenuName = NULL;
            win_class.lpszClassName = default_win_class_name;

            if (!RegisterClassExA(&win_class)) {
                DEBUG_MSG("RegisterClassExA failed: %s\n", std::system_category().message(GetLastError()).c_str());
                return false;
            }
        }

        if (!window_handle) {
            // create the window with no size at all; it will be set after
            window_handle = CreateWindowExA(0L, default_win_class_name, default_win_title,
                                            WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX & ~WS_THICKFRAME, 0, 0, 0, 0, 0, 0,
                                            GetModuleHandleW(NULL), 0);

            if (!window_handle) {
                DEBUG_MSG("CreateWindowA failed: %s\n", std::system_category().message(GetLastError()).c_str());
                return false;
            }

            SetWindowLongA(window_handle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

            if(gfx_ctx == nullptr) {
                gfx_ctx = std::make_shared<engine::graphics::Win32GraphicsContext>(window_handle);

                if(!gfx_ctx->init()) {
                    DEBUG_MSG("failed to create a graphical context!\n");
                    return false;
                }
            }
        }

        quit_requested = false;

        return true;
    }

    void Win32AppWindow::destroy() {
        BaseAppWindow::destroy();

        if (window_handle) {
            if (!DestroyWindow(window_handle)) {
                DEBUG_MSG("DestroyWindow failed: %s\n", std::system_category().message(GetLastError()).c_str());
            } else {
                window_handle = NULL;
            }
        }

        // make sure we don't have any windows registered on the engine default window class
        if (engine_window_class_exists() && !FindWindowA(default_win_class_name, nullptr)) {
            UnregisterClassA(default_win_class_name, GetModuleHandleW(nullptr));
        }
    }

    void Win32AppWindow::set_title(std::string_view title) {
        assert_window_init();

        if (!SetWindowTextA(window_handle, title.data())) {
            DEBUG_MSG("SetWindowTextA failed: %s\n", std::system_category().message(GetLastError()).c_str());
        }
    }

    void Win32AppWindow::set_position(utils::IVector2 position) {
        assert_window_init();

        auto current_size = get_size();

        if (!SetWindowPos(window_handle, 0, position.x, position.y, current_size.x, current_size.y, 0)) {
            DEBUG_MSG("SetWindowPos failed: %s\n", std::system_category().message(GetLastError()).c_str());
        }
    }

    void Win32AppWindow::set_size(utils::IVector2 size) {
        assert_window_init();

        auto current_pos = get_position();

        if (!SetWindowPos(window_handle, 0, current_pos.x, current_pos.y, size.x, size.y, 0)) {
            DEBUG_MSG("SetWindowPos failed: %s\n", std::system_category().message(GetLastError()).c_str());
        }
    }

    void Win32AppWindow::process_events() {
        assert_window_init();

        MSG msg;

        while (PeekMessageW(&msg, window_handle, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    void Win32AppWindow::update() {
        assert_window_init();

        if (!UpdateWindow(window_handle)) {
            DEBUG_MSG("UpdateWindow failed: %s\n", std::system_category().message(GetLastError()).c_str());
        }
    }

    void Win32AppWindow::show() {
        assert_window_init();

        ShowWindow(window_handle, SW_SHOW);
    }

    bool Win32AppWindow::is_focused() {
        assert_window_init();

        return GetFocus() == window_handle;
    }

    void Win32AppWindow::hide() {
        assert_window_init();

        ShowWindow(window_handle, SW_HIDE);
    }

    utils::IVector2 Win32AppWindow::get_size() {
        assert_window_init();

        RECT win_rect;

        if (!GetClientRect(window_handle, &win_rect)) {
            DEBUG_MSG("GetClientRect failed: %s\n", std::system_category().message(GetLastError()).c_str());
        } else {
            return {win_rect.right - win_rect.left, win_rect.bottom - win_rect.top};
        }

        return {0, 0};
    }

    utils::IVector2 Win32AppWindow::get_position() {
        assert_window_init();

        RECT win_rect;

        if (!GetWindowRect(window_handle, &win_rect)) {
            DEBUG_MSG("GetWindowRect failed: %s\n", std::system_category().message(GetLastError()).c_str());
        } else {
            return {win_rect.left, win_rect.top};
        }

        return {0, 0};
    }
}

#endif