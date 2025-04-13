//
// Created by gaeqs on 2/04/25.
//

#include <imgui.h>
#include <utility>
#include <ranges>

#include <imblueprint/Node.h>
#include <imblueprint/NodeInput.h>
#include <imblueprint/NodeOutput.h>

namespace ImBlueprint
{

    NodeInput::NodeInput(Node* node, std::string name, std::type_index type, bool multiple, PinShape shape) :
        _node(node),
        _name(std::move(name)),
        _type(type),
        _multiple(multiple),
        _shape(shape)
    {
    }

    NodeInput::~NodeInput()
    {
        std::vector<NodeOutput*> outputs;
        for (auto other : _values | std::views::keys) {
            outputs.push_back(other);
        }
        for (auto out : outputs) {
            out->removeLink(this, false);
        }
    }

    const std::any& NodeInput::getSingleValueAsAny() const
    {
        if (_values.empty()) {
            static auto EMPTY = std::any();
            return EMPTY;
        }
        return _values.begin()->second;
    }

    std::vector<const std::any*> NodeInput::getMultipleValuesAsAny() const
    {
        std::vector<const std::any*> vector;
        vector.reserve(_values.size());
        for (auto& value : _values | std::views::values) {
            vector.push_back(&value);
        }
        return vector;
    }

    bool NodeInput::addLink(NodeOutput* output)
    {
        if (output == nullptr || _values.contains(output)) {
            return false;
        }

        if (!_multiple) {
            std::vector<NodeOutput*> outputs;
            for (auto other : _values | std::views::keys) {
                outputs.push_back(other);
            }
            for (auto out : outputs) {
                out->removeLink(this, false);
            }
        }
        auto [it, ok] = _values.insert({output, std::any()});
        if (ok) {
            onInput(output, output->getValueAsAny());
        }
        return ok;
    }

    bool NodeInput::removeLink(NodeOutput* output, bool emitEmptyInput)
    {
        if (output == nullptr) {
            return false;
        }

        auto it = _values.find(output);
        if (it == _values.end()) {
            return false;
        }
        _values.erase(it);
        if (emitEmptyInput) {
            _node->onInputChange(_name, getSingleValueAsAny());
        }
        return true;
    }

    void NodeInput::render()
    {
        ImGui::Text(_name.c_str());
    }

    void NodeInput::onInput(NodeOutput* output, std::any value)
    {
        if (auto it = _values.find(output); it != _values.end()) {
            it->second = std::move(value);
            _node->onInputChange(_name, getSingleValueAsAny());
        }
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

    bool NodeInput::supportsMultipleInputs() const
    {
        return _multiple;
    }

    PinShape NodeInput::getShape() const
    {
        return _shape;
    }
} // namespace ImBlueprint