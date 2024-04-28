#pragma once

#include <string>

#include <utils/vector2.hpp>
#include <input/base_controller_input.hpp>
#include <utils/enums/input.hpp>

extern "C" {
#include <switch/runtime/pad.h>
}

namespace engine::input {
    struct SwitchPlayerController : public BaseControllerInput {
        explicit SwitchPlayerController(HidNpadIdType _id) : pad_id_type(_id) {}

        void init();
        void destroy();

        void rumble(uint16_t left_intensity, uint16_t right_intensity);

        utils::FVector2 get_axis(uint8_t axis_id) {
            switch(axis_id) {
                default:
                case 0:
                    return analog_left;
                case 1:
                    return analog_right;
            }
        }

        utils::enums::input::PadButton get_buttons() {
            return buttons;
        }

        void poll();

        bool is_connected() const;
        bool is_handheld() const;

        void reconfigure();

        std::string name();
    protected:
        static double map_stick_axis(double pos);
        static utils::enums::input::PadButton map_pad_buttons(HidNpadButton keys_down);

        PadState state{};
        u32 pad_dev_type{0};

        bool connected{false};
        bool handheld{false};

        HidNpadIdType pad_id_type{};
        HidNpadStyleTag pad_style{};

        uint8_t num_vib_handles{2};
        HidVibrationDeviceHandle vib_handles[2]{};

        utils::FVector2 analog_left{};
        utils::FVector2 analog_right{};

        utils::enums::input::PadButton buttons{};
    };
}