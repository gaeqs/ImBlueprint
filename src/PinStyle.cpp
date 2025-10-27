//
// Created by gaeqs on 13/04/2025.
//

#include <array>
#include <imblueprint/PinStyle.h>

namespace ImBlueprint
{

    PinStyle::PinStyle() :
        shape(PinShape::CIRCLE),
        color(ImNodes::GetStyle().Colors[ImNodesCol_Pin])
    {
    }

    PinStyle::PinStyle(PinShape shape_, uint32_t color_) :
        shape(shape_),
        color(color_)
    {
    }

    ImNodesPinShape_ toImNodesShape(PinShape shape, bool linked)
    {
        switch (shape) {
            case PinShape::CIRCLE:
                return linked ? ImNodesPinShape_CircleFilled : ImNodesPinShape_Circle;
                return ImNodesPinShape_CircleFilled;
            case PinShape::TRIANGLE:
                return linked ? ImNodesPinShape_TriangleFilled : ImNodesPinShape_Triangle;
        }
        return ImNodesPinShape_Circle;
    }

    uint32_t pinColorForType(const std::type_index& type)
    {
        constexpr std::array COLORS = {
            0xFFEA2551,
            0xFFE5C224,
            0xFF25E825,
            0xFF24E5E5,
            0xFF3424E2,
            0xFFBD23E0,
        };
        return COLORS[(type.hash_code() + 5) % COLORS.size()];
    }

} // namespace ImBlueprint