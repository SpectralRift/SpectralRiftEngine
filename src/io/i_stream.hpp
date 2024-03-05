#pragma once
#include <vector>
#include <memory>

namespace engine::io {
	struct IStream {
		virtual bool open() = 0;
		virtual void close() = 0;

		virtual bool is_open() = 0;

		virtual std::vector<uint8_t> read(int count) = 0;
        virtual int write(const std::vector<uint8_t>& data) = 0;

        virtual void seek(int position) = 0;
        virtual int get_position() = 0;

        virtual int size() = 0;
	};

	using IStreamSPtr = std::shared_ptr<IStream>;
}