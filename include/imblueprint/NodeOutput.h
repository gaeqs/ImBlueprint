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

namespace ImBlueprint
{

    class Node;
    class NodeInput;

    class NodeOutput : public EditorElement
    {
        Node* _node;
        std::string _name;
        std::type_index _type;
        std::any _value;

        std::unordered_set<Link> _links;

      public:
        NodeOutput(Node* node, std::string name, std::type_index type);

        ~NodeOutput();

        virtual void render();

        Node* getNode() const;

        [[nodiscard]] const std::string& getName() const;

        [[nodiscard]] std::type_index getType() const;

        [[nodiscard]] const std::any& getValueAsAny() const;

        bool setValue(std::any value);

        [[nodiscard]] const std::unordered_set<Link>& getLinks() const;

        bool addLink(NodeInput* input);

        void removeLink(NodeInput* input, bool emitEmptyInput);

        bool willLinkCreateCircularDependency(const NodeInput* input) const;
    };

} // namespace ImBlueprint

#endif // IMBLUEPRINT_NODEOUTPUT_H
