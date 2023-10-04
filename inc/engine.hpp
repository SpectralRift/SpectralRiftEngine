#pragma once

#include <api.hpp>
#include <graphics/i_render_backend.hpp>

namespace engine {
	void ENGINE_API init();
	void ENGINE_API terminate();

	void ENGINE_API use_backend(graphics::IRenderBackendSPtr backend);
	graphics::IRenderBackendSPtr ENGINE_API get_backend();
}
