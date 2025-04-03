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

    void Node::renderTitle()
    {
        ImNodes::BeginNodeTitleBar();
        ImGui::TextUnformatted(_name.c_str());
        ImNodes::EndNodeTitleBar();
    }

    void Node::renderBody()
    {
    }

    void Node::render(int id)
    {
        ImNodes::BeginNode(id);

        renderTitle();
        renderBody();

        ImNodes::EndNode();
    }
} // namespace ImBlueprint
