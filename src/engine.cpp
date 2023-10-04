#include <engine.hpp>

#include <iostream>

#include <utils/macros/debug.hpp>

#include <windows/base_app_window.hpp>
#include <factories/app_window_factory.hpp>

#include <platform/platform.hpp>
#include <utils/macros/debug.hpp>

namespace engine {
	static graphics::IRenderBackendSPtr g_backend{};

	void init() {
		DEBUG_MSG("engine begin!\n");
		engine::platform::init();
	}

	void terminate() {
		DEBUG_MSG("engine termination!\n");
	}

	void use_backend(graphics::IRenderBackendSPtr backend) {
		g_backend = backend;
	}

	graphics::IRenderBackendSPtr get_backend() {
		ASSERT_MSG(g_backend, "a backend was not set previously using engine::use_backend");
		return g_backend;
	}
}