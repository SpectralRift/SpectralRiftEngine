#pragma once

#include <string_view>
#include <memory>

namespace engine::graphics {
    struct IGraphicsContext {
        virtual bool init() = 0;

        virtual void destroy() = 0;

        virtual void use() = 0;

        virtual void discard() = 0;

        virtual void present() = 0;
    };

    using IGraphicsContextSPtr = std::shared_ptr<IGraphicsContext>;
}