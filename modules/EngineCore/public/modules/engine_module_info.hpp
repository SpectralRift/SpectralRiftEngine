#pragma once

#include <modules/i_engine_module.hpp>

#include <string>
#include <memory>

namespace engine {
    namespace modules {
        struct EngineModuleInfo {
            std::string name;
            std::string filename;
            void *moduleHandle;
            IEngineModuleSPtr module;
            bool isLoaded;
            bool isStatic;

            EngineModuleInfo() : moduleHandle(nullptr), isLoaded(false), isStatic(false) {}
            ~EngineModuleInfo() = default;
        };

        using EngineModuleInfoSPtr = std::shared_ptr<EngineModuleInfo>;
    }
}
