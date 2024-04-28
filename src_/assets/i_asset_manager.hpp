#pragma once

#include <string_view>
#include <vector>
#include <memory>

#include <io/i_stream.hpp>

namespace engine::assets {
	struct IAssetManager {
		virtual io::IStreamSPtr get_asset(std::string_view file) = 0;
		virtual bool file_exists(std::string_view file) = 0;
	};

	using IAssetManagerSPtr = std::shared_ptr<IAssetManager>;
}