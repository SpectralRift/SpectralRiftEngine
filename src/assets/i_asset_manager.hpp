#pragma once
#include <string_view>
#include <vector>
#include <memory>

namespace engine::assets {
	struct IAssetManager {
		virtual void open(std::string_view file) = 0;
		virtual bool file_exists(std::string_view file) = 0;
		virtual std::vector<uint8_t> read_all(std::string_view file) = 0;
	};

	using IAssetManagerSPtr = std::shared_ptr<IAssetManager>;
}