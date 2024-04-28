project(EngineCore VERSION 0.0.1)

add_library(EngineCore STATIC private/engine.cpp private/modules/module_manager.cpp private/platform/platform.cpp)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/public")
target_include_directories(EngineCore PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/public")

target_sources(EngineCore
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/public"
        PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/private"
)
target_compile_definitions(EngineCore PUBLIC ENGINE_BUILD_LIBRARY)