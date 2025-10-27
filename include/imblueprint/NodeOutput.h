//
// Created by gaeqs on 2/04/25.
//

#ifndef IMBLUEPRINT_NODEOUTPUT_H
#define IMBLUEPRINT_NODEOUTPUT_H

#include <typeindex>
#include <any>
#include <string>
#include <unordered_set>

#include <imblueprint/Link.h>
#include <imblueprint/EditorElement.h>
#include <imblueprint/PinStyle.h>

namespace ImBlueprint
{

    class Node;
    class NodeInput;

    class NodeOutput : public EditorElement
    {
        Node* _node;
        std::string _name;
        std::type_index _type;
        PinStyle _style;
        std::any _value;

        std::unordered_set<Link> _links;

      public:
        NodeOutput(Node* node, std::string name, std::type_index type, PinStyle style);

        ~NodeOutput() override;

        Node* getNode() const;

        [[nodiscard]] const std::string& getName() const;

        [[nodiscard]] std::type_index getType() const;

        [[nodiscard]] PinStyle& getStyle();

        [[nodiscard]] const PinStyle& getStyle() const;

        [[nodiscard]] const std::any& getValueAsAny() const;

        bool setValue(std::any value);

        [[nodiscard]] const std::unordered_set<Link>& getLinks() const;

        bool addLink(NodeInput* input);

        void removeLink(NodeInput* input, bool emitEmptyInput);

        [[nodiscard]] bool hasLink() const;

        bool willLinkCreateCircularDependency(const NodeInput* input) const;
    };

} // namespace ImBlueprint

#endif // IMBLUEPRINT_NODEOUTPUT_H
