option(ENGINE_BACKEND_OPENGL "Adds OpenGL backend support to the engine. It uses GLAD internally to load the OpenGL API." ON)

if (ENGINE_BACKEND_OPENGL)
    include(engine/backends/OpenGL)
endif ()