//
// Created by gaeqs on 2/04/25.
//

#ifndef NODEOUTPUT_H
#define NODEOUTPUT_H

#include <typeindex>
#include <any>
#include <string>

namespace ImBlueprint
{

    struct EditorHandler;

    class NodeOutput
    {
        EditorHandler* _handler;
        std::string _name;
        std::type_index _type;
        std::any _value;

      public:
        NodeOutput(std::string name, std::type_index type);

        [[nodiscard]] const std::string& getName() const;

        [[nodiscard]] std::type_index getType() const;

        [[nodiscard]] const std::any& getValueAsAny() const;

        bool setValue(std::any value);

        void assignHandler(EditorHandler* handler);
    };

} // namespace ImBlueprint

#endif // NODEOUTPUT_H
