//
// Created by gaeqs on 2/04/25.
//

#ifndef IMBLUEPRINT_NODEINPUT_H
#define IMBLUEPRINT_NODEINPUT_H

#include <any>
#include <optional>
#include <string>
#include <typeindex>

#include <imblueprint/EditorElement.h>

namespace ImBlueprint
{

    class Node;
    class NodeOutput;

    class NodeInput : public EditorElement
    {
        Node* _node;
        std::string _name;
        std::type_index _type;
        std::any _value;
        NodeOutput* _output;

      public:
        NodeInput(Node* node, std::string name, std::type_index type);

        ~NodeInput();

        virtual void render();

        [[nodiscard]] virtual const std::any& getValueAsAny() const;

        virtual void onInput(std::any value);

        Node* getNode() const;

        [[nodiscard]] const std::string& getName() const;

        [[nodiscard]] std::type_index getType() const;

        [[nodiscard]] std::optional<NodeOutput*> getOutput() const;

        void setOutput(NodeOutput* output);

        template<typename T>
        [[nodiscard]] std::optional<T> getValueAs() const
        {
            auto value = std::any_cast<T>(&getValueAsAny());
            if (value == nullptr) {
                return {};
            }
            return *value;
        }
    };

} // namespace ImBlueprint

#endif // IMBLUEPRINT_NODEINPUT_H
