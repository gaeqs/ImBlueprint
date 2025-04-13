//
// Created by gaeqs on 13/04/2025.
//

#include <imblueprint/PinShape.h>

namespace ImBlueprint
{

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

} // namespace ImBlueprint