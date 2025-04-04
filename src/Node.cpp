//
// Created by gaeqs on 2/04/25.
//

#include <atomic>
#include <imnodes.h>

#include <imblueprint/Node.h>

#include <utility>

namespace
{

    std::atomic_int UID_GENERATOR = 0;

}

namespace ImBlueprint
{
    Node::Node(std::string name) :
        _uid(UID_GENERATOR++),
        _name(std::move(name))
    {
    }

    Node::~Node()
    {
    }

    void Node::renderTitle()
    {
        ImNodes::BeginNodeTitleBar();
        ImGui::TextUnformatted(_name.c_str());
        ImNodes::EndNodeTitleBar();
    }

    void Node::renderBody()
    {
    }

    void Node::renderInputs()
    {
        int i = 0;
        for (auto& [name, input] : _inputs) {
            ImNodes::BeginInputAttribute(i++);
            ImGui::Text(name.c_str());
            ImNodes::EndInputAttribute();
        }
    }

    void Node::renderOutputs()
    {
        int i = _inputs.size();
        for (auto& [name, input] : _outputs) {
            ImNodes::BeginOutputAttribute(i++);
            ImGui::Text(name.c_str());
            ImNodes::EndOutputAttribute();
        }
    }

    void Node::render()
    {
        ImNodes::BeginNode(_uid);

        renderTitle();
        renderBody();
        renderInputs();
        renderOutputs();

        ImNodes::EndNode();
    }

    int Node::getUID() const
    {
        return _uid;
    }

    const std::string& Node::getName() const
    {
        return _name;
    }

    const std::unordered_map<std::string, NodeInput>& Node::getInputs() const
    {
        return _inputs;
    }

    const std::unordered_map<std::string, NodeOutput>& Node::getOutputs() const
    {
        return _outputs;
    }
} // namespace ImBlueprint
