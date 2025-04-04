//
// Created by gaeqs on 2/04/25.
//

#ifndef NODE_H
#define NODE_H

#include <string>
#include <unordered_map>

#include <imblueprint/NodeInput.h>
#include <imblueprint/NodeOutput.h>

namespace ImBlueprint
{

    class Node
    {
        int _uid;
        std::string _name;

        std::unordered_map<std::string, NodeInput> _inputs;
        std::unordered_map<std::string, NodeOutput> _outputs;

      public:
        explicit Node(std::string name);

        virtual ~Node();

        virtual void renderTitle();

        virtual void renderBody();

        virtual void renderInputs();

        virtual void renderOutputs();

        void render();

        [[nodiscard]] int getUID() const;

        [[nodiscard]] const std::string& getName() const;

        [[nodiscard]] const std::unordered_map<std::string, NodeInput>& getInputs() const;

        [[nodiscard]] const std::unordered_map<std::string, NodeOutput>& getOutputs() const;

        template<typename T>
        void defineInput(const std::string& name)
        {
            _inputs.insert({name, NodeInput(name, typeid(T))});
        }

        template<typename T>
        void defineOutput(const std::string& name)
        {
            _outputs.insert({name, NodeOutput(name, typeid(T))});
        }
    };

} // namespace ImBlueprint

#endif // NODE_H
