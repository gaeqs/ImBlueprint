//
// Created by gaeqs on 2/04/25.
//

#include <imnodes.h>

#include <imblueprint/Node.h>

#include <utility>

namespace ImBlueprint
{
    NodeTitleStyleEntry::NodeTitleStyleEntry(uint32_t color) :
        topLeft(color),
        topRight(color),
        bottomLeft(color),
        bottomRight(color)
    {
    }

    NodeTitleStyleEntry::NodeTitleStyleEntry(uint32_t topLeft_, uint32_t topRight_, uint32_t bottomLeft_,
                                             uint32_t bottomRight_) :
        topLeft(topLeft_),
        topRight(topRight_),
        bottomLeft(bottomLeft_),
        bottomRight(bottomRight_)
    {
    }

    NodeTitleStyle::NodeTitleStyle() :
        normal(ImNodes::GetStyle().Colors[ImNodesCol_TitleBar]),
        hover(ImNodes::GetStyle().Colors[ImNodesCol_TitleBarHovered]),
        selected(ImNodes::GetStyle().Colors[ImNodesCol_TitleBarSelected])
    {
    }

    NodeTitleStyle::NodeTitleStyle(NodeTitleStyleEntry normal, NodeTitleStyleEntry hover, NodeTitleStyleEntry selected) :
        normal(normal),
        hover(hover),
        selected(selected)
    {
    }

    void Node::setTitleStyle(NodeTitleStyle titleStyle)
    {
        _titleStyle = std::move(titleStyle);
    }

    Node::Node(std::string name) :
        _name(std::move(name))
    {
    }

    Node::Node(std::string name, NodeTitleStyle titleStyle) :
        _name(std::move(name)),
        _titleStyle(std::move(titleStyle))
    {
    }

    void Node::onInputChange(const std::string& name, const std::any& value)
    {
    }

    void Node::renderTitle()
    {
        ImNodes::DynamicText(_name);
    }

    void Node::renderBody()
    {
    }

    const std::string& Node::getName() const
    {
        return _name;
    }

    const NodeTitleStyle& Node::getTitleStyle() const
    {
        return _titleStyle;
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
        if (auto it = _outputs.find(name); it != _outputs.end()) {
            it->second->setValue(std::move(value));
        }
    }
} // namespace ImBlueprint
