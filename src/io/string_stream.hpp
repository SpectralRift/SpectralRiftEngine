#pragma once

#include <string>

#include <io/i_stream.hpp>

namespace engine::io {
    struct StringStream {
        StringStream(IStreamSPtr strStream) : stream(strStream) {}

        std::string read_until(uint8_t where);
        std::string read_line();
        std::string read_all();

        void write_line(std::string_view text);
        void write(std::string_view text);
    private:
        IStreamSPtr stream;
    };
}