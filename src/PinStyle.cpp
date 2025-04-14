//
// Created by gaeqs on 13/04/2025.
//

#include <array>
#include <imblueprint/PinStyle.h>

namespace ImBlueprint
{

    PinStyle::PinStyle() :
        shape(PinShape::FILLED_CIRCLE),
        color(ImNodes::GetStyle().Colors[ImNodesCol_Pin])
    {
    }

    PinStyle::PinStyle(PinShape shape_, uint32_t color_) :
        shape(shape_),
        color(color_)
    {
    }

    ImNodesPinShape_ toImNodesShape(PinShape shape)
    {
        switch (shape) {
            case PinShape::CIRCLE:
                return ImNodesPinShape_Circle;
            case PinShape::FILLED_CIRCLE:
                return ImNodesPinShape_CircleFilled;
            case PinShape::TRIANGLE:
                return ImNodesPinShape_Triangle;
            case PinShape::FILLED_TRIANGLE:
                return ImNodesPinShape_TriangleFilled;
            case PinShape::QUAD:
                return ImNodesPinShape_Quad;
            case PinShape::FILLED_QUAD:
                return ImNodesPinShape_QuadFilled;
        }
        return ImNodesPinShape_Circle;
    }

    PinShape pinShapeForType(const std::type_index& type)
    {
        return static_cast<PinShape>(type.hash_code() % PIN_SHAPES_AMOUNT);
    }

    uint32_t pinColorForType(const std::type_index& type)
    {
        constexpr std::array COLORS = {IM_COL32(255, 0, 0, 255),   IM_COL32(0, 255, 0, 255),
                                       IM_COL32(0, 0, 255, 255),   IM_COL32(255, 255, 0, 255),
                                       IM_COL32(255, 0, 255, 255), IM_COL32(0, 255, 255, 255),
                                       IM_COL32(255, 165, 0, 255), IM_COL32(128, 0, 128, 255),
                                       IM_COL32(0, 128, 128, 255), IM_COL32(192, 192, 192, 255)};
        return COLORS[type.hash_code() % COLORS.size()];
    }

} // namespace ImBlueprint