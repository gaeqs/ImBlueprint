//
// Created by gaeqs on 2/04/25.
//

#include <imblueprint/NodeInput.h>

#include <utility>

namespace ImBlueprint
{

    NodeInput::NodeInput(std::string name, std::type_index type) :
        _name(std::move(name)),
        _type(type)
    {
    }

    const std::string& NodeInput::getName() const
    {
        return _name;
    }

    std::type_index NodeInput::getType() const
    {
        return _type;
    }
} // namespace ImBlueprint