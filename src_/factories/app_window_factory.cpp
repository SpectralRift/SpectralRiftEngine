#include <map>
#include <string>

#include <factories/app_window_factory.hpp>
#include <utils/macros/debug.hpp>

namespace engine::factories::app_window {
	// make sure this is accessible only in this part.
	static std::map<std::string, AppWindowFactoryMethod> factory_map;

	void register_type(std::string_view name, AppWindowFactoryMethod factory_method) {
		factory_map.emplace(name, factory_method);
		DEBUG_MSG("registered AppWindow type '%s'\n", name.data());
	}

	windows::BaseAppWindowSPtr create(std::optional<std::string_view> preferred_type) {
		if (preferred_type.has_value()) {
			if (factory_map.contains(preferred_type.value().data())) {
				DEBUG_MSG("create preferred window type '%s'\n", preferred_type.value().data());
				return factory_map.at(preferred_type.value().data())();
			}
			else {
				DEBUG_MSG("no window of such type, falling back to type '%s'\n", factory_map.begin()->first.c_str());
                return factory_map.at(factory_map.begin()->first)();
			}
		}
		else {
			DEBUG_MSG("creating window of automatic type '%s'!\n", factory_map.begin()->first.c_str());
			return factory_map.at(factory_map.begin()->first)();
		}
	}
}