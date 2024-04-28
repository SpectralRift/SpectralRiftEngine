#include "switch_controller.hpp"
#include <utils/macros/debug.hpp>

namespace engine::input {
    void SwitchPlayerController::init() {
        DEBUG_MSG("INIT pad_id_type %i\n", pad_id_type)

        // if this represents Player 1, we configure it so that it allows Handheld mode as well
        if (pad_id_type == HidNpadIdType_No1) {
            padInitializeDefault(&state);
        } else {
            padInitialize(&state, pad_id_type);
        }

        reconfigure();
    }

    void SwitchPlayerController::reconfigure() {
        padUpdate(&state);

        pad_dev_type = hidGetNpadDeviceType(pad_id_type);
        pad_style = (HidNpadStyleTag) hidGetNpadStyleSet(pad_id_type);

        connected = padIsConnected(&state);
        handheld = padIsHandheld(&state);

        DEBUG_MSG("pad_dev_type %X pad_style %X\n", pad_dev_type, pad_style)

        if (handheld) {
            int ret = hidInitializeVibrationDevices(vib_handles, 2, HidNpadIdType_Handheld,
                                                    HidNpadStyleTag_NpadHandheld);
            DEBUG_MSG("handheld hidInitializeVibrationDevices ret %i\n", ret)
        } else {
            // special cases when we need only one handle
            if (pad_dev_type & (HidDeviceTypeBits_JoyLeft | HidDeviceTypeBits_JoyRight)) {
                num_vib_handles = 1;
            }

            int ret = hidInitializeVibrationDevices(vib_handles, num_vib_handles, pad_id_type, pad_style);
            DEBUG_MSG("hidInitializeVibrationDevices ret %i\n", ret)
        }

        // a single joy-con has a singular axis, so we handle this situation as well
        if (pad_style & HidNpadStyleSet_NpadFullCtrl | handheld) {
            reserve_axis(4); // reserve 2 X/Y axis
        } else {
            reserve_axis(2); // reserve a singular X/Y axis
        }
    }

    void SwitchPlayerController::destroy() {
        connected = false;
    }

    bool SwitchPlayerController::is_connected() const {
        return connected;
    }

    bool SwitchPlayerController::is_handheld() const {
        return handheld;
    }

    double SwitchPlayerController::map_stick_axis(double pos) {
        return pos / JOYSTICK_MAX;
    }

    utils::enums::input::PadButton SwitchPlayerController::map_pad_buttons(HidNpadButton keys_down) {
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

    void SwitchPlayerController::poll() {
        padUpdate(&state);

        connected = padIsConnected(&state);
        handheld = padIsHandheld(&state);

        // if the pad isn't connected, we return out.
        if (!is_connected()) {
            return;
        }

        auto curr_pad_type = hidGetNpadDeviceType(pad_id_type);
        auto curr_pad_style = (HidNpadStyleTag) hidGetNpadStyleSet(pad_id_type);

        // if the pad style becomes mismatched, we show the config applet!
        if (curr_pad_type != pad_dev_type || curr_pad_style != pad_style) {
            connected = false;
            DEBUG_MSG("Switch Controller type/style mismatch! isHandheld = %s\n", padIsHandheld(&state) ? "yes" : "no");
            return;
        }

        if(axis_count() > 2) {
            for(int i = 0; i < 2; i++) {
                auto stick = padGetStickPos(&state, i);

                int base_axis = i * 2;

                set_axis_value(base_axis, map_stick_axis(stick.x));
                set_axis_value(base_axis + 1, map_stick_axis(stick.y));
            }
        } else {
            // special case: make sure the first axis maps to the joystick axis
            auto stick = padGetStickPos(&state, pad_dev_type & HidDeviceTypeBits_JoyRight ? 1 : 0);

            set_axis_value(0, map_stick_axis(stick.x));
            set_axis_value(1, map_stick_axis(stick.y));
        }

        buttons = map_pad_buttons((HidNpadButton) padGetButtons(&state));
    }

    void SwitchPlayerController::rumble(uint16_t left_intensity, uint16_t right_intensity) {
        HidVibrationValue vib_values[2]{};

        vib_values[0].amp_low = left_intensity == 0 ? 0.0f : 320.0f;
        vib_values[0].freq_low = left_intensity == 0 ? 160.0f : (float) left_intensity / 204.f;

        vib_values[0].amp_high = left_intensity == 0 ? 0.0f : 320.0f;
        vib_values[0].freq_high = left_intensity == 0 ? 160.0f : (float) left_intensity / 204.f;

        vib_values[1].amp_low = right_intensity == 0 ? 0.0f : 320.0f;
        vib_values[1].freq_low = right_intensity == 0 ? 160.0f : (float) right_intensity / 204.f;

        vib_values[1].amp_high = right_intensity == 0 ? 0.0f : 320.0f;
        vib_values[1].freq_high = right_intensity == 0 ? 160.0f : (float) right_intensity / 204.f;

        // if we're in that special case, and we're setting the right joycon's rumble
        if(num_vib_handles == 1) {
            if (pad_dev_type & HidDeviceTypeBits_JoyRight) {
                vib_values[0] = vib_values[1];
            }
        }

        hidSendVibrationValues(vib_handles, vib_values, num_vib_handles);
    }

    std::string SwitchPlayerController::name() {
        auto player_string = "P" + std::to_string(pad_id_type + 1);

        if (pad_style & HidNpadStyleTag_NpadJoyDual) {
            return "NX JoyCon (Dual) " + player_string;
        } else if (pad_style & HidNpadStyleTag_NpadHandheld) {
            if (pad_dev_type & HidDeviceTypeBits_HandheldLeft) {
                return "NX Handheld Left JoyCon " + player_string;
            } else if (pad_dev_type & HidDeviceTypeBits_HandheldRight) {
                return "NX Handheld Right JoyCon " + player_string;
            }
        }

        if (pad_dev_type & HidDeviceTypeBits_JoyLeft) {
            return "NX Left JoyCon " + player_string;
        } else if (pad_dev_type & HidDeviceTypeBits_JoyRight) {
            return "NX Right JoyCon " + player_string;
        }

        // if type is not set, it's probably the console in handheld mode
        if (handheld) {
            return "NX Handheld " + player_string;
        }

        return "NX Controller " + player_string;
    }
}