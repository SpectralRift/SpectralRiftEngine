#if defined(ENGINE_TARGET_WIN32)
#include <platform/win32/win32_platform.hpp>
#include <Windows.h>

namespace engine::platform {
    uintptr_t load_library_win32(std::string_view lib_name) {
        HMODULE lib_handle = LoadLibraryA(lib_name.data());
        return reinterpret_cast<uintptr_t>(lib_handle);
    }

    uintptr_t get_library_func_win32(uintptr_t lib_handle, std::string_view func) {
        FARPROC func_ptr = GetProcAddress(reinterpret_cast<HMODULE>(lib_handle), func.data());
        return reinterpret_cast<uintptr_t>(func_ptr);
    }
}
#endif