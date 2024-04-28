#pragma once
#include <assets/i_asset_manager.hpp>

#include "api.hpp"

namespace engine::assets {
	struct ENGINE_API GenericAssetManager : public IAssetManager {
        io::IStreamSPtr get_asset(std::string_view file);
		bool file_exists(std::string_view file);
    private:
        static std::string get_assets_folder(std::string_view file);
	};
}