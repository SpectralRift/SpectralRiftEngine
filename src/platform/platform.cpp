#include <platform/platform.hpp>

namespace engine::platform {
    void init() {
        // Platform-specific conditional initialization begins here.
#if defined(ENGINE_TARGET_WIN32)
        init_win32();
#elif defined(ENGINE_TARGET_LINUX) || defined(ENGINE_TARGET_ANDROID)
        init_unix();
#elif defined(ENGINE_TARGET_SWITCH)
        init_switch();
#endif
        // Platform-specific conditional initialization ends here.
    }

    void terminate() {
#if defined(ENGINE_TARGET_SWITCH)
        terminate_switch();
#endif
    }

    uintptr_t load_library(std::string_view lib_name) {
        // Platform-specific library loading routines begins here.
#if defined(ENGINE_TARGET_WIN32)
        return load_library_win32(lib_name);
#elif defined(ENGINE_TARGET_LINUX) || defined(ENGINE_TARGET_ANDROID)
        return load_library_unix(lib_name);
#elif defined(ENGINE_TARGET_SWITCH)
        return load_library_switch(lib_name);
#endif
        // Platform-specific library loading routines ends here.
    }

    uintptr_t get_library_func(uintptr_t lib_handle, std::string_view func) {
        // Platform-specific library address fetching routines begins here.
#if defined(ENGINE_TARGET_WIN32)
        return get_library_func_win32(lib_handle, func);
#elif defined(ENGINE_TARGET_LINUX) || defined(ENGINE_TARGET_ANDROID)
        return get_library_func_unix(lib_handle, func);
#elif defined(ENGINE_TARGET_SWITCH)
        return get_library_func_switch(lib_handle, func);
#endif
        // Platform-specific library address fetching routines ends here.
    }
}