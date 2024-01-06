#include <assets/generic_asset_manager.hpp>

namespace engine::assets {
    void GenericAssetManager::open(std::string_view file) {
        return;
    }

    bool GenericAssetManager::file_exists(std::string_view file) {
        return false;
    }

    std::vector<uint8_t> GenericAssetManager::read_all(std::string_view file) {
        return {};
    }
}