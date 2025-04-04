//
// Created by gaeqs on 2/04/25.
//

#ifndef NODE_H
#define NODE_H

#include "EditorElement.h"

#include <functional>
#include <memory>
#include <string>
#include <map>

#include <imblueprint/NodeInput.h>
#include <imblueprint/NodeOutput.h>

namespace ImBlueprint
{

    class Node : public EditorElement
    {
        std::string _name;

        std::map<std::string, std::unique_ptr<NodeInput>> _inputs;
        std::map<std::string, std::unique_ptr<NodeOutput>> _outputs;

      public:
        Node(const Node& other) = delete;

        explicit Node(std::string name);

        virtual ~Node();

        virtual void onInputChange(const std::string& name, const std::any& value);

        virtual void renderTitle(std::function<void()> closeAction);

        virtual void renderBody();

        [[nodiscard]] const std::string& getName() const;

        [[nodiscard]] const std::map<std::string, std::unique_ptr<NodeInput>>& getInputs() const;

        [[nodiscard]] const std::map<std::string, std::unique_ptr<NodeOutput>>& getOutputs() const;

        void sendOutput(const std::string& name, std::any value) const;

        template<typename T>
        std::optional<T> getInput(const std::string& name) const
        {
            auto it = _inputs.find(name);
            if (it != _inputs.end()) {
                return it->second->getValueAs<T>();
            }
            return {};
        }

        template<typename T>
        void defineInput(const std::string& name)
        {
            _inputs.insert({name, std::make_unique<NodeInput>(this, name, typeid(T))});
        }

        template<typename T>
        void defineOutput(const std::string& name)
        {
            _outputs.insert({name, std::make_unique<NodeOutput>(this, name, typeid(T))});
        }

        template<typename T>
        void defineOutput(const std::string& name, T initialValue)
        {
            auto [it, ok] = _outputs.insert({name, std::make_unique<NodeOutput>(this, name, typeid(T))});
            if (ok) {
                it->second->setValue(initialValue);
            }
        }
    };

} // namespace ImBlueprint

#endif // NODE_H
