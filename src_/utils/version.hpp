#pragma once

#include <cstdint>

namespace engine::utils {
    struct Version {
        Version() : Version(0, 0, 0) {}
        Version(int major, int minor, int patch = 0) : major(major), minor(minor), patch(patch) {}

        int pack_version_simple() const {
            return (int) (major * 100 + minor * 10);
        }

        int pack_version_full() const {
            return (int) (major * 1000 + minor * 100 + patch * 10);
        }

        int major{0};
        int minor{0};
        int patch{0};
    };
}