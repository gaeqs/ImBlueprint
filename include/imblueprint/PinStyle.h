//
// Created by gaeqs on 13/04/2025.
//

#ifndef IMBLUEPRINT_PINSHAPE_H
#define IMBLUEPRINT_PINSHAPE_H

#include "imnodes.h"

#include <cstdint>
#include <optional>

#include <typeindex>

namespace ImBlueprint
{

    constexpr size_t PIN_SHAPES_AMOUNT = 6;

    enum class PinShape : size_t
    {
        CIRCLE = 0,
        FILLED_CIRCLE = 1,
        TRIANGLE = 2,
        FILLED_TRIANGLE = 3,
        QUAD = 4,
        FILLED_QUAD = 5
    };

    struct PinStyle
    {
        PinShape shape;
        uint32_t color;

        PinStyle();

        PinStyle(PinShape shape, uint32_t color);
    };

    ImNodesPinShape_ toImNodesShape(PinShape shape);

    PinShape pinShapeForType(const std::type_index& type);

    uint32_t pinColorForType(const std::type_index& type);

} // namespace ImBlueprint

#endif // IMBLUEPRINT_PINSHAPE_H
