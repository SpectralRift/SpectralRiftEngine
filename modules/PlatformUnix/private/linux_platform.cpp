#if defined(ENGINE_TARGET_LINUX) || defined(ENGINE_TARGET_ANDROID)
#include <platform/linux/linux_platform.hpp>
#include <dlfcn.h>

namespace engine::platform {
    uintptr_t load_library_unix(std::string_view lib_name) {
        return (uintptr_t) dlopen(lib_name.data(), RTLD_LAZY);
    }

    uintptr_t get_library_func_unix(uintptr_t lib_handle, std::string_view func) {
        return (uintptr_t) dlsym((void*) lib_handle, func.data());
    }
}
#endif