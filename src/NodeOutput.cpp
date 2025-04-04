//
// Created by gaeqs on 2/04/25.
//

#include <imblueprint/NodeOutput.h>

namespace ImBlueprint
{

    NodeOutput::NodeOutput(std::string name, std::type_index type) :
        _handler(nullptr),
        _name(std::move(name)),
        _type(type)
    {
    }

    const std::string& NodeOutput::getName() const
    {
        return _name;
    }

    std::type_index NodeOutput::getType() const
    {
        return _type;
    }

    const std::any& NodeOutput::getValueAsAny() const
    {
        return _value;
    }

    bool NodeOutput::setValue(std::any value)
    {
        if (!value.has_value() || _type == value.type()) {
            _value = std::move(value);
            return true;
        }
        return false;
    }

    void NodeOutput::assignHandler(EditorHandler* handler)
    {
        _handler = handler;
    }

} // namespace ImBlueprint