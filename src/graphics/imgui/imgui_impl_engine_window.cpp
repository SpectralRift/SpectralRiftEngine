#include <graphics/imgui/imgui_impl_engine_window.hpp>
#include <utils/macros/debug.hpp>

#include <algorithm>

namespace engine::graphics::imgui {
    struct ImGui_ImplEngineWindow_Data {
        windows::BaseAppWindowSPtr window;
    };

    static ImGui_ImplEngineWindow_Data *ImGui_ImplEngineWindow_GetBackendData() {
        return ImGui::GetCurrentContext()
               ? (ImGui_ImplEngineWindow_Data *) ImGui::GetIO().BackendPlatformUserData
               : nullptr;
    }

    static uint8_t ImGui_ImplEngineWindow_MouseButtonToImGuiMouseButton(utils::enums::input::MouseButton button) {
        switch (button) {
            case utils::enums::input::MOUSE_LEFT:
                return ImGuiMouseButton_Left;
            case utils::enums::input::MOUSE_MIDDLE:
                return ImGuiMouseButton_Middle;
            case utils::enums::input::MOUSE_RIGHT:
                return ImGuiMouseButton_Right;
            default:
                return 0xFF;
        }
    }

    static ImGuiKey ImGui_ImplEngineWindow_KeyTypeToImGuiKey(utils::enums::input::KeyType key) {
        switch (key) {
            case utils::enums::input::KEY_0:
            case utils::enums::input::KEY_1:
            case utils::enums::input::KEY_2:
            case utils::enums::input::KEY_3:
            case utils::enums::input::KEY_4:
            case utils::enums::input::KEY_5:
            case utils::enums::input::KEY_6:
            case utils::enums::input::KEY_7:
            case utils::enums::input::KEY_8:
            case utils::enums::input::KEY_9:
                return (ImGuiKey) (ImGuiKey_0 + (key - utils::enums::input::KEY_0));
            case utils::enums::input::KEY_A:
            case utils::enums::input::KEY_B:
            case utils::enums::input::KEY_C:
            case utils::enums::input::KEY_D:
            case utils::enums::input::KEY_E:
            case utils::enums::input::KEY_F:
            case utils::enums::input::KEY_G:
            case utils::enums::input::KEY_H:
            case utils::enums::input::KEY_I:
            case utils::enums::input::KEY_J:
            case utils::enums::input::KEY_K:
            case utils::enums::input::KEY_L:
            case utils::enums::input::KEY_M:
            case utils::enums::input::KEY_N:
            case utils::enums::input::KEY_O:
            case utils::enums::input::KEY_P:
            case utils::enums::input::KEY_Q:
            case utils::enums::input::KEY_R:
            case utils::enums::input::KEY_S:
            case utils::enums::input::KEY_T:
            case utils::enums::input::KEY_U:
            case utils::enums::input::KEY_V:
            case utils::enums::input::KEY_W:
            case utils::enums::input::KEY_X:
            case utils::enums::input::KEY_Y:
            case utils::enums::input::KEY_Z:
                return (ImGuiKey) (ImGuiKey_A + (key - utils::enums::input::KEY_A));
            case utils::enums::input::KEY_SPACE:
                return ImGuiKey_Space;
            default:
                return (ImGuiKey) -1;
        }
    }

    bool ImGui_ImplEngineWindow_Init(const windows::BaseAppWindowSPtr &app_window) {
        ImGuiIO &io = ImGui::GetIO();
        IM_ASSERT(io.BackendPlatformUserData == nullptr && "Already initialized a platform backend!");

        io.IniFilename = nullptr;
        io.LogFilename = nullptr;

        auto *bd = IM_NEW(ImGui_ImplEngineWindow_Data)();
        io.BackendPlatformUserData = (void *) bd;
        io.BackendPlatformName = "imgui_impl_engine_window";
        io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;

#ifdef ENGINE_TARGET_SWITCH
        io.ConfigFlags |= ImGuiConfigFlags_IsTouchScreen;
        io.MouseDrawCursor = true;
#endif

        bd->window = app_window;

        bd->window->add_mouse_move_listener([](const utils::IVector2 &pos, bool isTouch) {
            ImGuiIO &io = ImGui::GetIO();

            io.AddMouseSourceEvent(isTouch ? ImGuiMouseSource_TouchScreen : ImGuiMouseSource_Mouse);
            io.AddMousePosEvent((float) pos.x, (float) pos.y);
        });

        bd->window->add_mouse_key_down_listener(
                [](const utils::IVector2 &pos, utils::enums::input::MouseButton button, bool isTouch) {
                    ImGuiIO &io = ImGui::GetIO();

                    auto btn = ImGui_ImplEngineWindow_MouseButtonToImGuiMouseButton(button);

                    if (btn != 0xFF) {
                        io.AddMouseSourceEvent(isTouch ? ImGuiMouseSource_TouchScreen : ImGuiMouseSource_Mouse);
                        io.AddMousePosEvent((float) pos.x, (float) pos.y);
                        io.AddMouseButtonEvent(btn, true);
                    }
                });

        bd->window->add_mouse_key_up_listener([](const utils::IVector2 &pos, utils::enums::input::MouseButton button, bool isTouch) {
            ImGuiIO &io = ImGui::GetIO();

            auto btn = ImGui_ImplEngineWindow_MouseButtonToImGuiMouseButton(button);

            if (btn != 0xFF) {
                io.AddMouseSourceEvent(isTouch ? ImGuiMouseSource_TouchScreen : ImGuiMouseSource_Mouse);
                io.AddMousePosEvent((float) pos.x, (float) pos.y);
                io.AddMouseButtonEvent(btn, false);
            }
        });

        bd->window->add_pad_key_state_update_listener(
                [](utils::enums::input::PadButton button, utils::enums::input::PadButton) {
                    ImGuiIO &io = ImGui::GetIO();
                    io.BackendFlags |= ImGuiBackendFlags_HasGamepad;

                    io.AddKeyEvent(ImGuiKey_GamepadStart, (button & utils::enums::input::PadButton::PAD_BUTTON_SWITCH_PLUS) != 0);
                    io.AddKeyEvent(ImGuiKey_GamepadBack, (button & utils::enums::input::PadButton::PAD_BUTTON_SWITCH_MINUS) != 0);
                    io.AddKeyEvent(ImGuiKey_GamepadFaceLeft, (button & utils::enums::input::PadButton::PAD_BUTTON_X) != 0);
                    io.AddKeyEvent(ImGuiKey_GamepadFaceRight, (button & utils::enums::input::PadButton::PAD_BUTTON_B) != 0);
                    io.AddKeyEvent(ImGuiKey_GamepadFaceUp, (button & utils::enums::input::PadButton::PAD_BUTTON_Y) != 0);
                    io.AddKeyEvent(ImGuiKey_GamepadFaceDown, (button & utils::enums::input::PadButton::PAD_BUTTON_A) != 0);
                    io.AddKeyEvent(ImGuiKey_GamepadDpadLeft, (button & utils::enums::input::PadButton::PAD_BUTTON_DPAD_LEFT) != 0);
                    io.AddKeyEvent(ImGuiKey_GamepadDpadRight, (button & utils::enums::input::PadButton::PAD_BUTTON_DPAD_RIGHT) != 0);
                    io.AddKeyEvent(ImGuiKey_GamepadDpadUp, (button & utils::enums::input::PadButton::PAD_BUTTON_DPAD_UP) != 0);
                    io.AddKeyEvent(ImGuiKey_GamepadDpadDown, (button & utils::enums::input::PadButton::PAD_BUTTON_DPAD_DOWN) != 0);
                    io.AddKeyEvent(ImGuiKey_GamepadL1, (button & utils::enums::input::PadButton::PAD_BUTTON_L) != 0);
                    io.AddKeyEvent(ImGuiKey_GamepadR1, (button & utils::enums::input::PadButton::PAD_BUTTON_R) != 0);
                    io.AddKeyEvent(ImGuiKey_GamepadL2, (button & utils::enums::input::PadButton::PAD_BUTTON_STICK_L) != 0);
                    io.AddKeyEvent(ImGuiKey_GamepadR2, (button & utils::enums::input::PadButton::PAD_BUTTON_STICK_R) != 0);
                    io.AddKeyEvent(ImGuiKey_GamepadL3, (button & utils::enums::input::PadButton::PAD_BUTTON_L2) != 0);
                    io.AddKeyEvent(ImGuiKey_GamepadR3, (button & utils::enums::input::PadButton::PAD_BUTTON_R2) != 0);
                });

        bd->window->add_key_down_listener([](utils::enums::input::KeyType key) {
            ImGuiIO &io = ImGui::GetIO();
            auto k = ImGui_ImplEngineWindow_KeyTypeToImGuiKey(key);

            io.BackendFlags &= ~ImGuiBackendFlags_HasGamepad;

            io.AddKeyEvent(ImGuiMod_Ctrl, false);
            io.AddKeyEvent(ImGuiMod_Shift, false);
            io.AddKeyEvent(ImGuiMod_Alt, false);
            io.AddKeyEvent(ImGuiMod_Super, false);

            if (k != (ImGuiKey) -1) {
                io.AddKeyEvent(k, true);
                io.SetKeyEventNativeData(k, key, -1);
            }
        });

        bd->window->add_key_up_listener([](utils::enums::input::KeyType key) {
            ImGuiIO &io = ImGui::GetIO();
            auto k = ImGui_ImplEngineWindow_KeyTypeToImGuiKey(key);

            io.BackendFlags &= ~ImGuiBackendFlags_HasGamepad;

            io.AddKeyEvent(ImGuiMod_Ctrl, false);
            io.AddKeyEvent(ImGuiMod_Shift, false);
            io.AddKeyEvent(ImGuiMod_Alt, false);
            io.AddKeyEvent(ImGuiMod_Super, false);

            if (k != (ImGuiKey) -1) {
                io.AddKeyEvent(k, false);
                io.SetKeyEventNativeData(k, key, -1);

                if(k >= ImGuiKey_0 && k <= ImGuiKey_Z) {
                    io.AddInputCharacter((char) key);
                }
            }
        });

        return true;
    }

    void ImGui_ImplEngineWindow_Shutdown() {
        auto bd = ImGui_ImplEngineWindow_GetBackendData();
        IM_ASSERT(bd != nullptr && "No platform backend to shutdown, or already shutdown?");
        ImGuiIO &io = ImGui::GetIO();

        io.BackendPlatformUserData = nullptr;
        io.BackendPlatformName = nullptr;
        io.BackendFlags &= ~ImGuiBackendFlags_HasMouseCursors;
        IM_DELETE(bd);
    }

    void ImGui_ImplEngineWindow_NewFrame(double delta) {
        ImGuiIO &io = ImGui::GetIO();
        auto *bd = ImGui_ImplEngineWindow_GetBackendData();

        IM_ASSERT(bd != nullptr && "Did you call ImGui_ImplEngineWindow_Init()?");
        IM_ASSERT(bd->window != nullptr && "Window is not initialised correctly!");

        auto size = bd->window->get_size();

        io.DisplaySize = ImVec2((float) size.x, (float) size.y);
        io.DeltaTime = delta;

//        auto axis_l = bd->window->input_manager()->get_axis_value(0);
//        auto axis_r = bd->window->input_manager()->get_axis_value(1);
//
//        io.AddKeyAnalogEvent(ImGuiKey_GamepadLStickLeft, axis_l.x <= -.25f, axis_l.x < 0.f ? axis_l.x * -.1f : 0.f);
//        io.AddKeyAnalogEvent(ImGuiKey_GamepadLStickRight, axis_l.x >= .25f, axis_l.x < 0.f ? 0.f : axis_l.x);
//        io.AddKeyAnalogEvent(ImGuiKey_GamepadLStickUp, axis_l.y >= .25f, axis_l.y < 0.f ? 0.f : axis_l.y);
//        io.AddKeyAnalogEvent(ImGuiKey_GamepadLStickDown, axis_l.y <= -.25f, axis_l.y < 0.f ? axis_l.y * -.1f : 0.f);
//
//        io.AddKeyAnalogEvent(ImGuiKey_GamepadRStickLeft, axis_r.x <= -.25f, axis_r.x < 0.f ? axis_r.x * -.1f : 0.f);
//        io.AddKeyAnalogEvent(ImGuiKey_GamepadRStickRight, axis_r.x >= .25f, axis_r.x < 0.f ? 0.f : axis_r.x);
//        io.AddKeyAnalogEvent(ImGuiKey_GamepadRStickUp, axis_r.y >= .25f, axis_r.y < 0.f ? 0.f : axis_r.y);
//        io.AddKeyAnalogEvent(ImGuiKey_GamepadRStickDown, axis_r.y <= -.25f, axis_r.y < 0.f ? axis_r.y * -.1f : 0.f);
    }
}