#if defined(ENGINE_TARGET_SWITCH)

#include <windows/switch_app_window.hpp>
#include <utils/macros/debug.hpp>
#include <graphics/contexts/egl_graphics_context.hpp>

namespace engine::windows {
    bool SwitchAppWindow::init() {
        window_handle = nwindowGetDefault();

        // initialize gamepad
        padConfigureInput(1, HidNpadStyleSet_NpadStandard);
        padInitializeDefault(&pad_state);

        hidInitializeTouchScreen();

        // create graphical context
        if (gfx_ctx == nullptr) {
            gfx_ctx = std::make_shared<engine::graphics::EGLGraphicsContext>(window_handle);

            if (!gfx_ctx->init()) {
                DEBUG_MSG("failed to create a graphical context!\n");
                return false;
            }
        }

        return true;
    }

    void SwitchAppWindow::set_title(std::string_view title) {
        // won't work on console
    }

    void SwitchAppWindow::set_position(utils::IVector2 position) {
        // won't work on console
    }

    void SwitchAppWindow::set_size(utils::IVector2 size) {
        // destroy graphical context
        if (gfx_ctx != nullptr) {
            gfx_ctx->destroy();
        }

        // set window size
        nwindowSetDimensions(window_handle, size.x, size.y);

        // recreate graphical context
        if (gfx_ctx != nullptr) {
            gfx_ctx->init();
        }
    }

    utils::IVector2 SwitchAppWindow::get_size() {
        utils::IVector2 res;
        nwindowGetDimensions(window_handle, (u32 *) &res.x, (u32 *) &res.y);

        return res;
    }

    utils::IVector2 SwitchAppWindow::get_position() {
        return {0, 0};
    }

    utils::IVector2 SwitchAppWindow::map_touch_relative(const utils::IVector2 &pos) {
        // map touchscreen coordinate to relative of 1280x720 (0.f - 1.f), then remap it to window size
        auto screen_size = get_size();

        return {
                (int) ((float) screen_size.x * ((float) pos.x / 1280.f)),
                (int) ((float) screen_size.y * ((float) pos.y / 720.f))
        };
    }

    utils::FVector2 SwitchAppWindow::map_stick(const utils::FVector2 &pos) {
        utils::FVector2 stick;

        if (pos.x != 0.f) {
            stick.x = pos.x / JOYSTICK_MAX;
        }

        if (pos.y != 0.f) {
            stick.y = pos.y / JOYSTICK_MAX;
        }

        return stick;
    }

    utils::enums::input::PadButton SwitchAppWindow::map_switch_pad_buttons(HidNpadButton keys_down) {
        uint16_t new_state{utils::enums::input::PadButton::PAD_BUTTON_UNKNOWN};

        if (keys_down & HidNpadButton_A) {
            new_state |= utils::enums::input::PadButton::PAD_BUTTON_A;
        }
        if (keys_down & HidNpadButton_B) {
            new_state |= utils::enums::input::PadButton::PAD_BUTTON_B;
        }
        if (keys_down & HidNpadButton_X) {
            new_state |= utils::enums::input::PadButton::PAD_BUTTON_X;
        }
        if (keys_down & HidNpadButton_Y) {
            new_state |= utils::enums::input::PadButton::PAD_BUTTON_Y;
        }
        if (keys_down & HidNpadButton_L) {
            new_state |= utils::enums::input::PadButton::PAD_BUTTON_L;
        }
        if (keys_down & HidNpadButton_R) {
            new_state |= utils::enums::input::PadButton::PAD_BUTTON_R;
        }
        if (keys_down & HidNpadButton_StickL) {
            new_state |= utils::enums::input::PadButton::PAD_BUTTON_STICK_L;
        }
        if (keys_down & HidNpadButton_StickR) {
            new_state |= utils::enums::input::PadButton::PAD_BUTTON_STICK_R;
        }
        if (keys_down & HidNpadButton_ZL) {
            new_state |= utils::enums::input::PadButton::PAD_BUTTON_L2;
        }
        if (keys_down & HidNpadButton_ZR) {
            new_state |= utils::enums::input::PadButton::PAD_BUTTON_R2;
        }
        if (keys_down & HidNpadButton_AnySL) {
            new_state |= utils::enums::input::PadButton::PAD_BUTTON_L3;
        }
        if (keys_down & HidNpadButton_AnySR) {
            new_state |= utils::enums::input::PadButton::PAD_BUTTON_R3;
        }
        if (keys_down & HidNpadButton_Up) {
            new_state |= utils::enums::input::PadButton::PAD_BUTTON_DPAD_UP;
        }
        if (keys_down & HidNpadButton_Down) {
            new_state |= utils::enums::input::PadButton::PAD_BUTTON_DPAD_DOWN;
        }
        if (keys_down & HidNpadButton_Left) {
            new_state |= utils::enums::input::PadButton::PAD_BUTTON_DPAD_LEFT;
        }
        if (keys_down & HidNpadButton_Right) {
            new_state |= utils::enums::input::PadButton::PAD_BUTTON_DPAD_RIGHT;
        }
        if (keys_down & HidNpadButton_Plus) {
            new_state |= utils::enums::input::PadButton::PAD_BUTTON_SWITCH_PLUS;
        }
        if (keys_down & HidNpadButton_Minus) {
            new_state |= utils::enums::input::PadButton::PAD_BUTTON_SWITCH_MINUS;
        }

        return (utils::enums::input::PadButton) new_state;
    }

    void SwitchAppWindow::process_events() {
        if (!appletMainLoop()) {
            quit();
        }

        // gamepad update
        padUpdate(&pad_state);
        auto keys_down = padGetButtons(&pad_state);

        if (keys_down != last_keys_down) {
            ev_pad_key_state_update.invoke(
                    SwitchAppWindow::map_switch_pad_buttons((HidNpadButton) keys_down),
                    SwitchAppWindow::map_switch_pad_buttons((HidNpadButton) last_keys_down)
            );

            last_keys_down = keys_down;
        }

        // set controller sticks
        auto l_stick = padGetStickPos(&pad_state, 0);
        auto r_stick = padGetStickPos(&pad_state, 1);

//        m_input_mgr->_set_axis_value(0, map_stick({(float) l_stick.x, (float) l_stick.y}));
//        m_input_mgr->_set_axis_value(1, map_stick({(float) r_stick.x, (float) r_stick.y}));

        // touchscreen update
        HidTouchScreenState state{0};

        // this is fun. we have to manage the touchscreen state by ourselves.
        if (hidGetTouchScreenStates(&state, 1) > 0) {
            // this bit masked value tells us what fingers are currently on the screen
            uint16_t fingers = 0;

            // we set our bit mask to have the pressed fingers
            for (int i = 0; i < state.count; i++) {
                fingers |= (1 << state.touches[i].finger_id);
            }

            // handle changes here
            if (fingers != last_fingers) {
                // signal finger up for old fingers
                for (int i = 0; i < 16; i++) {
                    uint16_t mask = (1 << i);

                    // if finger is currently up (bit unset) and last state was down(bit set), signal finger up
                    if ((last_fingers & mask) && !(fingers & mask)) {
                        // for now handle only the first finger (id 0)
//                        DEBUG_MSG("FINGER UP: %i (%i, %i)\n", i, last_pos[i].x, last_pos[i].y);
                        if (i == 0) {
                            ev_mouse_key_up.invoke(last_pos[i], utils::enums::input::MouseButton::MOUSE_LEFT, true);
                        }
                    }
                }

                for (int i = 0; i < state.count; i++) {
                    // if our finger_id was not set previously, we consider it touch down
                    if ((last_fingers & (1 << state.touches[i].finger_id)) == 0) {
                        last_pos[state.touches[i].finger_id] = map_touch_relative(
                                {(int) state.touches[i].x, (int) state.touches[i].y});

                        // for now handle only the first finger (id 0)
                        if (state.touches[i].finger_id == 0) {
                            ev_mouse_key_down.invoke(last_pos[state.touches[i].finger_id],
                                                     utils::enums::input::MouseButton::MOUSE_LEFT, true);
                        }

//                        DEBUG_MSG("FINGER DOWN: %i (%i, %i)\n", state.touches[i].finger_id, state.touches[i].x, state.touches[i].y)
                    }
                }

                last_fingers = fingers;
            } else if (fingers == last_fingers && fingers > 0) {
                // no change with fingers on screen = dragging;
                for (int i = 0; i < state.count; i++) {
                    last_pos[state.touches[i].finger_id] = map_touch_relative(
                            {(int) state.touches[i].x, (int) state.touches[i].y});

                    // for now handle only the first finger (id 0)
                    if (state.touches[i].finger_id == 0) {
                        ev_mouse_move.invoke(last_pos[state.touches[i].finger_id], true);
                    }
                }
            }
        }
    }

    void SwitchAppWindow::update() {

    }

    void SwitchAppWindow::show() {}

    void SwitchAppWindow::hide() {}

    void *SwitchAppWindow::get_device_ctx() {
        return NULL;
    }

    void *SwitchAppWindow::get_window_handle() {
        return window_handle;
    }
}
#endif