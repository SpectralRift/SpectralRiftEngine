#include <stdio.h>
#include <vector>
#include <string>
#include <string_view>

#include <io/i_stream.hpp>

namespace engine::io {
    class CFileStream : public IStream {
    public:
        CFileStream(std::string_view _filename) : filename(_filename) {
            open();
        }

        bool open() {
#if defined(ENGINE_TARGET_SWITCH)
            file = fopen(filename.data(), "rb");
#else
            file = fopen(filename.data(), "rb+");
#endif
            if (file != NULL) {
                // get file size
                fseek(file, 0, SEEK_END);
                f_size = ftell(file);
                fseek(file, 0, SEEK_SET);

                return true;
            }
            return false;
        }

        void close() {
            if (file != NULL) {
                fclose(file);
                file = NULL;
            }
        }

        bool is_open() {
            return file != NULL;
        }

        std::vector<uint8_t> read(int count) {
            std::vector<uint8_t> data(count);
            fread(data.data(), sizeof(uint8_t), count, file);

            if (feof(file)) {
                data.clear();
            } else {
                data.resize(ftell(file));
            }

            return data;
        }

        int write(const std::vector<uint8_t> &data) {
#if defined(ENGINE_TARGET_SWITCH)
            return 0;
#else
            fwrite(data.data(), sizeof(uint8_t), data.size(), file);
            return static_cast<int>(ftell(file));
#endif
        }

        void seek(int position) {
            fseek(file, position, SEEK_SET);
        }

        int get_position() {
            return static_cast<int>(ftell(file));
        }

        int size() {
            return f_size;
        }

    private:
        FILE* file{NULL};
        std::string filename;
        int f_size{-1};
    };
} // namespace engine::io