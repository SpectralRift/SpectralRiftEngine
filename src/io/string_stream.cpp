#include <io/string_stream.hpp>

namespace engine::io {
    std::string StringStream::read_until(uint8_t where) {
        std::string data;
        char ch;
        while (stream->is_open() && (ch = stream->read(1)[0]) != where) {
            data.push_back(ch);
        }
        return data;
    }

    std::string StringStream::read_line() {
        std::string line;
        char ch;
        while (stream->is_open() && (ch = stream->read(1)[0]) != '\n') {
            line.push_back(ch);
        }
        return line;
    }

    std::string StringStream::read_all() {
        std::string data;

        while (stream->is_open()) {
            auto chunk = stream->read(1);
            if (chunk.empty()) {
                break;
            }

            // only \n, no \r\n
            if (chunk[0] == '\r') {
                continue;
            }

            data += static_cast<char>(chunk[0]);
        }

        return data;
    }

    void StringStream::write_line(std::string_view text) {
        std::string line(text);
        line.push_back('\n');
        stream->write(std::vector<uint8_t>(line.begin(), line.end()));
    }

    void StringStream::write(std::string_view text) {
        stream->write(std::vector<uint8_t>(text.begin(), text.end()));
    }
}