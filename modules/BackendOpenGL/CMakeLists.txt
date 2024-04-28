project(BackendOpenGL VERSION 0.0.1)

add_library(BackendOpenGL STATIC private/main.cpp)
target_link_libraries(BackendOpenGL EngineCore)