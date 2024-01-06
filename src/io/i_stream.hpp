#pragma once
#include <vector>
#include <memory>

namespace engine::io {
	struct IStream {
		virtual bool open() = 0;
		virtual void close() = 0;

		virtual bool is_open() = 0;
		virtual std::vector<uint8_t> read(int count) = 0;
	};

	using IStreamSPtr = std::shared_ptr<IStream>;
}