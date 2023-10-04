#pragma once

#include <windows/base_app_window.hpp>
#include <string_view>

namespace engine::graphics {
	struct IGraphicsContext {
		virtual bool init(windows::BaseAppWindowSPtr window) = 0;

		virtual void use() = 0;
		virtual void discard() = 0;

		virtual void present() = 0;

		virtual void destroy() = 0;
	};
}