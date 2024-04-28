#pragma once

#include <api.hpp>

namespace engine::utils {
    template <typename T>
    struct CSingleton {
        static ENGINE_API T& get() {
            static T instance;
            return &instance;
        }
    };
}