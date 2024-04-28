#pragma once

#include <string>
#include <memory>

#include <utils/vector3.hpp>
#include <utils/color.hpp>

#include <core/i_renderable.hpp>

namespace engine::core {
    struct BaseComponent : public IRenderable {
        // f this i may get rid of it but im too dumb atm
        virtual const std::string get_name() = 0;

        virtual void create() {}
        virtual void destroy() {}

        utils::FVector3 position;
        utils::FVector3 rotation;
    };

    using BaseComponentSPtr = std::shared_ptr<BaseComponent>;
}