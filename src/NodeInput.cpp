//
// Created by gaeqs on 2/04/25.
//

#include <imgui.h>
#include <utility>
#include <imblueprint/Node.h>

#include <imblueprint/NodeInput.h>
#include <imblueprint/NodeOutput.h>

namespace ImBlueprint
{

    NodeInput::NodeInput(Node* node, std::string name, std::type_index type) :
        _node(node),
        _name(std::move(name)),
        _type(type),
        _output(nullptr)
    {
    }

    NodeInput::~NodeInput()
    {
        if (_output != nullptr) {
            _output->removeLink(this);
        }
    }

    const std::any& NodeInput::getValueAsAny() const
    {
        return _value;
    }

    void NodeInput::render()
    {
        ImGui::Text(_name.c_str());
    }

    void NodeInput::onInput(std::any value)
    {
        _value = std::move(value);
        _node->onInputChange(_name, getValueAsAny());
    }

    Node* NodeInput::getNode() const
    {
        return _node;
    }

    const std::string& NodeInput::getName() const
    {
        return _name;
    }

    std::type_index NodeInput::getType() const
    {
        return _type;
    }

    std::optional<NodeOutput*> NodeInput::getOutput() const
    {
        if (_output == nullptr) {
            return {};
        }
        return _output;
    }

    void NodeInput::setOutput(NodeOutput* output)
    {
        _output = output;
    }
} // namespace ImBlueprint