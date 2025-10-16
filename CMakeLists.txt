cmake_minimum_required(VERSION 3.16)
project(ImBlueprint LANGUAGES CXX VERSION 0.0.1)

include(FetchContent)

# Configuration options
option(IMBLUEPRINT_BUILD_SHARED "Build ImBlueprint as a shared library" OFF)
option(IMBLUEPRINT_EXAMPLES "Include ImBlueprint examples" ON)

# Global parameters
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(IMBLUEPRINT_GENERATED_DIR ${CMAKE_BINARY_DIR}/generated)
set(IMBLUEPRINT_SINGLE_HEADER_PATH ${CMAKE_BINARY_DIR}/generated/imblueprint/imblueprint.h)

if (IMBLUEPRINT_EXAMPLES)
    find_package(glfw3 REQUIRED)
    find_package(OpenGL REQUIRED)
    find_package(SDL2 CONFIG REQUIRED)
endif ()

if (NOT DEFINED IMBLUEPRINT_IMGUI_TARGET)
    message("IMBLUEPRINT_IMGUI_TARGET not defined. Downloading and creating imgui target")
    FetchContent_Declare(
            imgui
            GIT_REPOSITORY https://github.com/ocornut/imgui.git
            GIT_TAG 44aa9a4b3a6f27d09a4eb5770d095cbd376dfc4b
    )

    FetchContent_MakeAvailable(imgui)

    if (IMBLUEPRINT_EXAMPLES)
        add_library(imgui STATIC
                ${imgui_SOURCE_DIR}/imgui.cpp
                ${imgui_SOURCE_DIR}/imgui_draw.cpp
                ${imgui_SOURCE_DIR}/imgui_tables.cpp
                ${imgui_SOURCE_DIR}/imgui_widgets.cpp
                ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
                ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
                ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl2.cpp
        )
        target_include_directories(imgui PUBLIC
                $<BUILD_INTERFACE:${imgui_SOURCE_DIR}/src>
                $<BUILD_INTERFACE:${imgui_SOURCE_DIR}/backends>
                $<INSTALL_INTERFACE:>
        )

        target_include_directories(imgui PUBLIC ${SDL2_INCLUDE_DIRS})
        target_link_libraries(imgui PUBLIC glfw OpenGL::GL SDL2::SDL2)
    else ()
        add_library(imgui STATIC
                ${imgui_SOURCE_DIR}/imgui.cpp
                ${imgui_SOURCE_DIR}/imgui_draw.cpp
                ${imgui_SOURCE_DIR}/imgui_tables.cpp
                ${imgui_SOURCE_DIR}/imgui_widgets.cpp
        )

    endif ()

    target_include_directories(imgui PUBLIC ${imgui_SOURCE_DIR})
    set(IMBLUEPRINT_IMGUI_TARGET imgui)
endif ()


if (NOT DEFINED IMBLUEPRINT_IMNODES_TARGET)
    set(IMNODES_IMGUI_TARGET_NAME ${IMBLUEPRINT_IMGUI_TARGET})
    message("IMBLUEPRINT_IMNODES_TARGET not defined. Downloading and creating imnodes target")
    FetchContent_Declare(
            imnodes
            GIT_REPOSITORY https://github.com/gaeqs/imnodes.git
            GIT_TAG zoom2
            SOURCE_DIR ${PROJECT_SOURCE_DIR}/lib/imnodes
    )

    if (IMBLUEPRINT_EXAMPLES)
        set(IMNODES_EXAMPLES ON)
    endif ()

    FetchContent_MakeAvailable(imnodes)
    set(IMBLUEPRINT_IMNODES_TARGET imnodes)
endif ()

list(APPEND LIBRARIES ${IMBLUEPRINT_IMGUI_TARGET} ${IMBLUEPRINT_IMNODES_TARGET})

# Create version.h

if (EXISTS "${CMAKE_SOURCE_DIR}/.git")
    execute_process(
            COMMAND git rev-parse --short HEAD
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            OUTPUT_VARIABLE GIT_COMMIT
            OUTPUT_STRIP_TRAILING_WHITESPACE
            ERROR_QUIET
    )
else ()
    set(GIT_COMMIT "unknown")
endif ()

configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/cmake/version.h.in
        ${IMBLUEPRINT_GENERATED_DIR}/imblueprint/version.h
)

# Create the library
add_subdirectory(src)

if (IMBLUEPRINT_EXAMPLES)
    add_subdirectory(examples)
endif ()

# Install public headers
install(
        DIRECTORY include/imblueprint
        DESTINATION include
)

install(
        DIRECTORY ${IMBLUEPRINT_GENERATED_DIR}/imblueprint
        DESTINATION include
)