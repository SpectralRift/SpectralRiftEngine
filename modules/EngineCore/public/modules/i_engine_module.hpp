#pragma once

#include <memory>
#include <functional>

namespace engine {
    namespace modules {
        struct IEngineModule {
            virtual ~IEngineModule() = default;

            virtual void ModuleStartup() {};
            virtual void ModuleShutdown() {};

            virtual void OnModuleUnload() {};
            virtual void OnModuleReload() {};

            [[nodiscard]] virtual bool IsEngineModule() const {
                return true;
            }
        };

        using IEngineModuleSPtr = std::shared_ptr<IEngineModule>;
        using IEngineModuleUPtr = std::unique_ptr<IEngineModule>;
        using IEngineModuleSPtrDelegate = std::function<IEngineModuleSPtr()>;
    }
}
