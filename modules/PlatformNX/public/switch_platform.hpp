#pragma once

#include <factories/app_window_factory.hpp>

#include <cstdint>
#include <string_view>

namespace engine::platform {
    void ENGINE_API init_switch();
    void ENGINE_API terminate_switch();

    uintptr_t ENGINE_API load_library_switch(std::string_view lib_name);
    uintptr_t ENGINE_API get_library_func_switch(uintptr_t lib_handle, std::string_view func);
}
