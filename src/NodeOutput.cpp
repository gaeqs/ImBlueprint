//
// Created by gaeqs on 2/04/25.
//

#include <imgui.h>
#include <queue>
#include <ranges>
#include <imblueprint/Node.h>

#include <imblueprint/NodeInput.h>
#include <imblueprint/NodeOutput.h>

namespace ImBlueprint
{

    NodeOutput::NodeOutput(Node* node, std::string name, std::type_index type) :
        _node(node),
        _name(std::move(name)),
        _type(type)
    {
    }

    NodeOutput::~NodeOutput()
    {
        for (auto& link : _links) {
            link.getInput()->setOutput(nullptr);
            link.getInput()->onInput({});
        }
    }

    void NodeOutput::render()
    {
        ImGui::Text(_name.c_str());
    }

    Node* NodeOutput::getNode() const
    {
        return _node;
    }

    const std::string& NodeOutput::getName() const
    {
        return _name;
    }

    std::type_index NodeOutput::getType() const
    {
        return _type;
    }

    const std::any& NodeOutput::getValueAsAny() const
    {
        return _value;
    }

    bool NodeOutput::setValue(std::any value)
    {
        if (!value.has_value() || _type == value.type()) {
            _value = std::move(value);

            // Output can be modified!
            auto out = getValueAsAny();
            for (auto& link : _links) {
                link.getInput()->onInput(out);
            }
            return true;
        }
        return false;
    }

    const std::unordered_set<Link>& NodeOutput::getLinks() const
    {
        return _links;
    }

    bool NodeOutput::addLink(NodeInput* input)
    {
        if (input == nullptr || input->getType() != _type || willLinkCreateCircularDependency(input)) {
            return false;
        }

        auto oldOutput = input->getOutput();
        if (oldOutput == this) {
            return false;
        }

        if (oldOutput.has_value()) {
            oldOutput.value()->removeLink(input, false);
        }

        _links.insert(Link(input));
        input->setOutput(this);
        input->onInput(getValueAsAny());

        return true;
    }

    void NodeOutput::removeLink(NodeInput* input, bool emitEmptyInput)
    {
        if (input == nullptr) {
            return;
        }
        if (_links.erase(Link(input)) > 0) {
            input->setOutput(nullptr);
            if (emitEmptyInput) {
                input->onInput({});
            }
        }
    }

    bool NodeOutput::willLinkCreateCircularDependency(const NodeInput* input) const
    {
        std::queue<Node*> queue;
        queue.push(input->getNode());

        while (!queue.empty()) {
            auto node = queue.front();
            queue.pop();
            if (_node == node) {
                return true;
            }
            for (auto& output : node->getOutputs() | std::ranges::views::values) {
                for (auto& link : output->getLinks()) {
                    queue.push(link.getInput()->getNode());
                }
            }
        }

        return false;
    }

} // namespace ImBlueprint