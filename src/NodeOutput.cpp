//
// Created by gaeqs on 2/04/25.
//

#include <imblueprint/NodeOutput.h>

namespace ImBlueprint
{

    NodeOutput::NodeOutput(std::type_index type) :
        _type(type)
    {
    }

    bool NodeOutput::setValue(std::any value)
    {
        if (!value.has_value() || _type == value.type()) {
            _value = std::move(value);
            return true;
        }
        return false;
    }

} // namespace ImBlueprint