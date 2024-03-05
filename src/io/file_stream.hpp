#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include <string>
#include <string_view>

#include <io/i_stream.hpp>

namespace engine::io {
    class FileStream : public IStream {
    public:
        FileStream(std::string_view _filename) : filename(_filename) {
            open();
        }

        bool open() {
            if (!fileStream.is_open()) {
#if defined(ENGINE_TARGET_SWITCH)
                fileStream.open(filename, std::ios::binary | std::ios::in);
#else
                fileStream.open(filename, std::ios::binary | std::ios::in | std::ios::out);
#endif

                // get file size
                fileStream.seekg(0, std::ios::end);
                f_size = (int) fileStream.tellg();
                fileStream.seekg(0, std::ios::beg);

                return fileStream.is_open();
            }
            return true;
        }

        void close() {
            if (fileStream.is_open()) {
                fileStream.close();
            }
        }

        bool is_open() {
            return fileStream.is_open();
        }

        std::vector<uint8_t> read(int count) {
            std::vector<uint8_t> data(count);
            fileStream.read(reinterpret_cast<char *>(data.data()), count);

            if (fileStream.gcount() == 0) {
                data.clear();
            } else {
                data.resize(fileStream.gcount());
            }

            return data;
        }

        int write(const std::vector<uint8_t> &data) {
#if defined(ENGINE_TARGET_SWITCH)
            return 0;
#else
            fileStream.write(reinterpret_cast<const char *>(data.data()), data.size());
            return static_cast<int>(fileStream.gcount());
#endif
        }

        void seek(int position) {
            fileStream.seekg(position);
        }

        int get_position() {
            return static_cast<int>(fileStream.tellg());
        }

        int size() {
            return f_size;
        }

    private:
        std::fstream fileStream;
        std::string filename;
        int f_size{0};
    };

} // namespace engine::io