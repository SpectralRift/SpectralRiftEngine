#include <engine.hpp>

#include <iostream>

#include <utils/macros/debug.hpp>

#include <windows/base_app_window.hpp>
#include <factories/app_window_factory.hpp>

#include <platform/platform.hpp>

void engine::init() {
	DEBUG_MSG("engine begin!\n");

	engine::platform::init();
}

void engine::terminate() {
	DEBUG_MSG("engine termination!\n");
}