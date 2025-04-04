//
// Created by gaeqs on 4/04/25.
//

#ifndef IMBLUEPRINT_LINK_H
#define IMBLUEPRINT_LINK_H

#include <functional>

#include <imblueprint/EditorElement.h>

namespace ImBlueprint
{

    class NodeInput;

    class Link : public EditorElement
    {
        NodeInput* _input;

      public:
        explicit Link(NodeInput* input);

        [[nodiscard]] NodeInput* getInput() const;

        bool operator==(const Link& other) const;

        bool operator!=(const Link& other) const;
    };

} // namespace ImBlueprint


template<>
struct std::hash<ImBlueprint::Link>
{
    std::size_t operator()(const ImBlueprint::Link& link) const noexcept
    {
        return std::hash<ImBlueprint::NodeInput*>{}(link.getInput());
    }
};

#endif // IMBLUEPRINT_LINK_H
