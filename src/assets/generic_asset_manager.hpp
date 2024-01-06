#pragma once
#include <assets/i_asset_manager.hpp>

#include <api.hpp>

namespace engine::assets {
	struct ENGINE_API GenericAssetManager : public IAssetManager {
		void open(std::string_view file);
		bool file_exists(std::string_view file);
		std::vector<uint8_t> read_all(std::string_view file);
	};
}