cmake_minimum_required(VERSION 3.20)
project(imgui)

# define standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(IMGUI_SOURCES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui")

add_library(imgui STATIC
        ${IMGUI_SOURCES_DIR}/imgui.cpp
        ${IMGUI_SOURCES_DIR}/imgui.h
        ${IMGUI_SOURCES_DIR}/imstb_rectpack.h
        ${IMGUI_SOURCES_DIR}/imstb_textedit.h
        ${IMGUI_SOURCES_DIR}/imstb_truetype.h
        ${IMGUI_SOURCES_DIR}/imgui_demo.cpp
        ${IMGUI_SOURCES_DIR}/imgui_draw.cpp
        ${IMGUI_SOURCES_DIR}/imgui_internal.h
        ${IMGUI_SOURCES_DIR}/imgui_tables.cpp
        ${IMGUI_SOURCES_DIR}/imgui_widgets.cpp
        )

target_include_directories(imgui PUBLIC
        $<BUILD_INTERFACE:${IMGUI_SOURCES_DIR}>
        $<INSTALL_INTERFACE:include>)

set(BACKENDS)

#if (ENGINE_BACKEND_OPENGL)
#    list(APPEND BACKENDS opengl2)
#endif ()

foreach (BACKEND ${BACKENDS})
    set(IMPL_NAME imgui_impl_${BACKEND})
    add_library(${IMPL_NAME} STATIC ${IMGUI_SOURCES_DIR}/backends/${IMPL_NAME}.cpp ${IMGUI_SOURCES_DIR}/backends/${IMPL_NAME}.h)

    target_link_libraries(${IMPL_NAME} PUBLIC imgui)
    target_include_directories(${IMPL_NAME} PUBLIC
            $<BUILD_INTERFACE:${IMGUI_SOURCES_DIR}/backends>
            $<INSTALL_INTERFACE:include>)
endforeach ()