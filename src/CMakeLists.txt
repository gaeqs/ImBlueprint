# Generate global header
file(GLOB_RECURSE IMBLUEPRINT_PUBLIC_HEADERS
        RELATIVE ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/include/imblueprint/*.h
)

file(WRITE ${IMBLUEPRINT_SINGLE_HEADER_PATH} "// This file is auto-generated by CMake. Do not edit.\n#pragma once\n\n")

foreach (header IN LISTS IMBLUEPRINT_PUBLIC_HEADERS)
    file(APPEND ${IMBLUEPRINT_SINGLE_HEADER_PATH} "#include <${header}>\n")
endforeach ()

if (NOT IMBLUEPRINT_BUILD_SHARED)
    add_library(imblueprint STATIC)
else ()
    add_library(imblueprint SHARED)
endif ()

target_sources(imblueprint PRIVATE
        Editor.cpp
        Node.cpp
        NodeInput.cpp
        NodeOutput.cpp
        UIDProvider.cpp
        EditorElement.cpp
        Link.cpp
        PinStyle.cpp
)

target_include_directories(imblueprint PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${IMBLUEPRINT_GENERATED_DIR}>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(imblueprint PUBLIC ${LIBRARIES})

set_target_properties(imblueprint PROPERTIES
        VERSION ${PROJECT_VERSION}
        SUBVERSION ${PROJECT_VERSION_MAJOR}
)

install(TARGETS imblueprint
        EXPORT ImBlueprintTargets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
)