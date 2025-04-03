//
// Created by gaeqs on 2/04/25.
//

#ifndef NODEOUTPUT_H
#define NODEOUTPUT_H

#include <typeindex>
#include <any>

namespace ImBlueprint
{

    class NodeOutput
    {
        std::type_index _type;
        std::any _value;

      public:
        explicit NodeOutput(std::type_index type);

        bool setValue(std::any value);
    };

} // namespace ImBlueprint

#endif // NODEOUTPUT_H
