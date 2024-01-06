#pragma once

#include <string_view>
#include <memory>

#include <windows/base_app_window.hpp>

namespace engine::graphics {
	struct IGraphicsContext {
		virtual bool init(windows::BaseAppWindowSPtr window) = 0;

		virtual void use() = 0;
		virtual void discard() = 0;

		virtual void present() = 0;

		virtual void destroy() = 0;

		virtual uintptr_t get_proc_func(std::string_view name) = 0;
	};

	using IGraphicsContextSPtr = std::shared_ptr<IGraphicsContext>;
}