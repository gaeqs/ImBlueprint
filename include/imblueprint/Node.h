//
// Created by gaeqs on 2/04/25.
//

#ifndef IMBLUEPRINT_NODE_H
#define IMBLUEPRINT_NODE_H

#include <functional>
#include <memory>
#include <string>
#include <map>

#include <imblueprint/EditorElement.h>
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
                return it->second->getSingleValueAs<T>();
            }
            return {};
        }

        template<typename T>
        std::optional<std::vector<T>> getMultipleInputs(const std::string& name) const
        {
            auto it = _inputs.find(name);
            if (it != _inputs.end()) {
                return it->second->getMultipleValueAs<T>();
            }
            return {};
        }

        template<typename T>
        bool defineInput(const std::string& name, bool multiple, PinShape shape = pinShapeForType(typeid(T)))
        {
            return _inputs.insert({name, std::make_unique<NodeInput>(this, name, typeid(T), multiple, shape)}).second;
        }

        template<typename T>
        bool defineOutput(const std::string& name, PinShape shape = pinShapeForType(typeid(T)))
        {
            return _outputs.insert({name, std::make_unique<NodeOutput>(this, name, typeid(T), shape)}).second;
        }

        template<typename T>
        bool defineOutput(const std::string& name, T initialValue, PinShape shape = pinShapeForType(typeid(T)))
        {
            auto [it, ok] = _outputs.insert({name, std::make_unique<NodeOutput>(this, name, typeid(T), shape)});
            if (ok) {
                it->second->setValue(initialValue);
            }
            return ok;
        }
    };

} // namespace ImBlueprint

#endif // IMBLUEPRINT_NODE_H
