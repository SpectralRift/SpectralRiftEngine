#pragma once

namespace engine::utils {
    /// <summary>
    /// Base Singleton templated class which exposes a static method that gets a global instance of the object.
    /// </summary>
    template <class T>
    struct ENGINE_API Singleton {
        static T &instance() {
            static T _itx;
            return _itx;
        }
    };
}