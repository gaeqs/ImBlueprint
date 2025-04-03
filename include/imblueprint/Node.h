//
// Created by gaeqs on 2/04/25.
//

#ifndef NODE_H
#define NODE_H

#include <string>

namespace ImBlueprint
{

    class Node
    {
        std::string _name;

      public:
        explicit Node(std::string name);

        virtual ~Node();

        virtual void renderTitle();

        virtual void renderBody();

        void render(int id);
    };

} // namespace ImBlueprint

#endif // NODE_H
