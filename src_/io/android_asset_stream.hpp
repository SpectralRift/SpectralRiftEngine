#pragma once

#if ANDROID
#include <android/asset_manager.h>

namespace engine::io {
    class AndroidAssetStream : public IStream {
    public:
        AndroidAssetStream(std::string_view _filename, AAssetManager *_mgr) : filename(_filename), asset_mgr(_mgr) {
            open();
        }

        bool open() {
            if(asset == NULL) {
                asset = AAssetManager_open(asset_mgr, filename.data(), 0);
                offset = 0;

                return is_open();
            }

            return true;
        }

        void close() {
            if(asset != NULL) {
                AAsset_close(asset);
            }
        }

        bool is_open() {
            return asset != NULL;
        }

        std::vector<uint8_t> read(int count) {
            std::vector<uint8_t> data(count);
            int bytes_read = AAsset_read(asset, reinterpret_cast<char *>(data.data()), count);

            offset += bytes_read;

            if (bytes_read <= 0) {
                data.clear();
            } else {
                data.resize(bytes_read);
            }

            return data;
        }

        int write(const std::vector<uint8_t> &data) {
            return 0;
        }

        void seek(int position) {
            offset = position;
            AAsset_seek64(asset, position, SEEK_SET);
        }

        int get_position() {
            return offset;
        }

        int size() {
            return (int) AAsset_getLength(asset);
        }

    private:
        AAssetManager *asset_mgr{NULL};
        AAsset* asset{NULL};
        std::string filename;
        int offset{0};
    };
}
#endif