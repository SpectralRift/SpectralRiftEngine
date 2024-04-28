#include <assets/generic_asset_manager.hpp>
#include <io/file_stream.hpp>

#include <filesystem>

namespace engine::assets {
    std::string GenericAssetManager::get_assets_folder(std::string_view file) {
        std::filesystem::path assets_dir{"assets"};
        std::filesystem::path file_path{file};

#if defined(ENGINE_TARGET_SWITCH)
        auto path = std::string("romfs:/") + file_path.string();
#else
        auto path = (assets_dir / file_path).string();
#endif

        return path;
    }

    io::IStreamSPtr GenericAssetManager::get_asset(std::string_view file) {
        return std::make_shared<io::FileStream>(get_assets_folder(file));
    }

    bool GenericAssetManager::file_exists(std::string_view file) {
        return std::filesystem::exists(get_assets_folder(file));
    }
}