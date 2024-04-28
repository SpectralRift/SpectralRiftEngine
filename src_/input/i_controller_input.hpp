#pragma once

#include <string>
#include <cstdint>

#include <utils/vector2.hpp>

namespace engine::input {
    struct IControllerInput {
        virtual void init() = 0;
        virtual void destroy() = 0;

        virtual void rumble(uint16_t left_intensity, uint16_t right_intensity) = 0;
        virtual utils::FVector2 get_axis(uint8_t axis_id) = 0;

        virtual void poll() = 0;

        virtual int get_id() const = 0;
        virtual std::string get_name() = 0;
    };
}