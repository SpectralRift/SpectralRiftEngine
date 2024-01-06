#pragma once

#include <api.hpp>

#include <graphics/interfaces/i_render_backend.hpp>
#include <graphics/interfaces/i_graphics_context.hpp>
#include <assets/i_asset_manager.hpp>

namespace engine {
    bool ENGINE_API init();
	void ENGINE_API terminate();

	void ENGINE_API use_backend(graphics::IRenderBackendSPtr backend);
	graphics::IRenderBackendSPtr ENGINE_API get_backend();

	void ENGINE_API use_graphics_context(graphics::IGraphicsContextSPtr ctx);
	graphics::IGraphicsContextSPtr ENGINE_API get_graphics_context();

	void ENGINE_API use_asset_manager(assets::IAssetManagerSPtr manager);
	assets::IAssetManagerSPtr ENGINE_API get_asset_manager();

    // ToDo: implement device manager to control/fetch data from a device
}
