#include <platform/win32/win32_platform.hpp>
#include <utils/macros/debug.hpp>

#include <Windows.h>

namespace engine::platform::win32::graphics {
	uintptr_t get_gl_func(const char* name) {
		uintptr_t func = (uintptr_t)wglGetProcAddress(name);

		if (!func) {
			auto gl_lib = LoadLibraryA("opengl32.dll");
			ASSERT_MSG(gl_lib, "Could not load OpenGL DLL!");

			func = (uintptr_t)GetProcAddress(gl_lib, name);
		}

		return (uintptr_t)func;
	}
}