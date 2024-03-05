#pragma once

#include <string_view>
#include <memory>
#include <optional>
#include <functional>

#include <api.hpp>

#include <windows/base_app_window.hpp>
#include <utils/vector2.hpp>
#include <utils/macros/debug.hpp>

namespace engine::factories::app_window {
	using AppWindowFactoryMethod = std::function<windows::BaseAppWindowSPtr()>;

	void ENGINE_API register_type(std::string_view name, AppWindowFactoryMethod factory_method);
	windows::BaseAppWindowSPtr ENGINE_API create(std::optional<std::string_view> preferred_type);

	inline windows::BaseAppWindowSPtr ENGINE_API create() {
		return create({});
	}

	inline windows::BaseAppWindowSPtr ENGINE_API create(std::string_view title, const engine::utils::IVector2& size) {
		auto w = create({});

		if (!w->init()) {
			DEBUG_MSG("window factory failed; window init failed!");
			return nullptr;
		}

		w->set_title(title);
		w->set_size(size);

		return w;
	}
}

