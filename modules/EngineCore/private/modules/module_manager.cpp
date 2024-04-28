#include <modules/module_manager.hpp>

namespace engine {
    namespace modules {
        EngineModuleInfoSPtr ModuleManager::GetModuleInfo(std::string_view name) {
            return {nullptr};
        }

        bool ModuleManager::ModuleExists(std::string_view name) {
            return false;
        }

        bool ModuleManager::IsModuleLoaded(std::string_view name) {
            return false;
        }

        bool ModuleManager::LoadModule(std::string_view name) {
            return false;
        }

        bool ModuleManager::LoadModuleIfExists(std::string_view name) {
            return false;
        }

        bool ModuleManager::UnloadModule(std::string_view name) {
            return false;
        }

        void ModuleManager::_RegisterModule(std::string_view name, IEngineModuleSPtrDelegate delegate) {

        }
    }
}