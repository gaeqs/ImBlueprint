//
// Created by gaeqs on 2/04/25.
//

#include <imnodes.h>

#include <imblueprint/Node.h>

#include <utility>

namespace ImBlueprint
{
    Node::Node(std::string name) :
        _name(std::move(name))
    {
    }

    Node::~Node()
    {
    }

    void Node::onInputChange(const std::string& name, const std::any& value)
    {
    }

    void Node::renderTitle(std::function<void()> closeAction)
    {
        ImGui::TextUnformatted(_name.c_str());
        if (closeAction != nullptr) {
            ImGui::SameLine(0, 100);
            if (ImGui::Button("X", ImVec2(20, 20))) {
                closeAction();
            }
        }
    }

    void Node::renderBody()
    {
        ImGui::Dummy(ImVec2(50, 20));
    }

    const std::string& Node::getName() const
    {
        return _name;
    }

    const std::map<std::string, std::unique_ptr<NodeInput>>& Node::getInputs() const
    {
        return _inputs;
    }

    const std::map<std::string, std::unique_ptr<NodeOutput>>& Node::getOutputs() const
    {
        return _outputs;
    }

    void Node::sendOutput(const std::string& name, std::any value) const
    {
        auto it = _outputs.find(name);
        if (it != _outputs.end()) {
            it->second->setValue(std::move(value));
        }
    }
} // namespace ImBlueprint
