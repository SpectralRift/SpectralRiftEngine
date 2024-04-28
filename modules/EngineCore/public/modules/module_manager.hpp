#pragma once

#include <api.hpp>

#include <utils/c_singleton.hpp>

#include <modules/i_engine_module.hpp>
#include <modules/engine_module_info.hpp>

#include <string_view>
#include <memory>
#include <vector>

namespace engine {
    namespace modules {
        struct ModuleManager : public utils::CSingleton<ModuleManager> {
            ENGINE_API EngineModuleInfoSPtr GetModuleInfo(std::string_view name);
            ENGINE_API bool ModuleExists(std::string_view name);
            ENGINE_API bool IsModuleLoaded(std::string_view name);

            ENGINE_API bool LoadModule(std::string_view name);
            ENGINE_API bool LoadModuleIfExists(std::string_view name);
            ENGINE_API bool UnloadModule(std::string_view name);

            void _RegisterModule(std::string_view name, IEngineModuleSPtrDelegate delegate);
        private:
            std::vector<EngineModuleInfoSPtr> modules;
        };
    }
}