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
#include <imblueprint/PinStyle.h>

namespace ImBlueprint
{

    class Node;
    class NodeOutput;

    class NodeInput : public EditorElement
    {
        Node* _node;
        std::string _name;
        std::type_index _type;
        bool _multiple;
        PinStyle _style;
        std::unordered_map<NodeOutput*, std::any> _values;

      public:
        NodeInput(Node* node, std::string name, std::type_index type, bool multiple, PinStyle style);

        ~NodeInput();

        [[nodiscard]] virtual const std::any& getSingleValueAsAny() const;

        [[nodiscard]] virtual std::vector<const std::any*> getMultipleValuesAsAny() const;

        bool addLink(NodeOutput* output);

        bool removeLink(NodeOutput* output, bool emitEmptyInput);

        virtual void onInput(NodeOutput* output, std::any value);

        Node* getNode() const;

        [[nodiscard]] const std::string& getName() const;

        [[nodiscard]] std::type_index getType() const;

        [[nodiscard]] bool supportsMultipleInputs() const;

        [[nodiscard]] PinStyle& getStyle();

        [[nodiscard]] const PinStyle& getStyle() const;

        [[nodiscard]] bool hasLink() const;

        template<typename T>
        [[nodiscard]] std::optional<T> getSingleValueAs() const
        {
            auto value = std::any_cast<T>(&getSingleValueAsAny());
            if (value == nullptr) {
                return std::optional<T>();
            }
            return *value;
        }

        template<typename T>
        [[nodiscard]] std::vector<T> getMultipleValueAs() const
        {
            auto raw = getMultipleValuesAsAny();
            std::vector<T> vector;
            vector.reserve(raw.size());

            for (auto any : raw) {
                auto value = std::any_cast<T>(any);
                if (value != nullptr) {
                    vector.push_back(*value);
                }
            }

            return vector;
        }
    };

} // namespace ImBlueprint

#endif // IMBLUEPRINT_NODEINPUT_H
