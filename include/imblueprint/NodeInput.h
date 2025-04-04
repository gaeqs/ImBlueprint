//
// Created by gaeqs on 2/04/25.
//

#ifndef NODEINPUT_H
#define NODEINPUT_H
#include <string>
#include <typeindex>

namespace ImBlueprint
{

    class NodeInput
    {
        std::string _name;
        std::type_index _type;

    public:

        NodeInput(std::string name, std::type_index type);

        [[nodiscard]] const std::string& getName() const;

        [[nodiscard]] std::type_index getType() const;
    };

} // namespace ImBlueprint

#endif // NODEINPUT_H
