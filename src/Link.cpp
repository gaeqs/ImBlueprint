//
// Created by gaeqs on 4/04/25.
//

#include <imblueprint/Link.h>

namespace ImBlueprint
{

    Link::Link(NodeInput* input) :
        _input(input)
    {
    }

    NodeInput* Link::getInput() const
    {
        return _input;
    }

    bool Link::operator==(const Link& other) const
    {
        return _input == other._input;
    }

    bool Link::operator!=(const Link& other) const
    {
        return _input != other._input;
    }
} // namespace ImBlueprint