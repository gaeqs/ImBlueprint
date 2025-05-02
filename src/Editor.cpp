//
// Created by gaeqs on 2/04/25.
//

#include <imnodes.h>

#include <atomic>
#include <iostream>
#include <ranges>
#include <imblueprint/Editor.h>

namespace
{

    std::atomic_int CONTEXT_COUNT = 0;

}

namespace ImBlueprint
{
    void Editor::renderNode(Node* node)
    {
        ImNodes::BeginNode(node->getOrCreateInternalId(_uidProvider));
        ImNodes::BeginNodeTitleBar();
        node->renderTitle([this, node]() { _nodeToRemove = node; });

        for (const auto& input : node->getInputs() | std::views::values) {
            auto& style = input->getStyle();
            ImNodes::PushColorStyle(ImNodesCol_Pin, style.color);
            ImNodes::BeginInputAttribute(input->getOrCreateInternalId(_uidProvider), toImNodesShape(style.shape));
            input->render();
            ImNodes::EndInputAttribute();
            ImNodes::PopColorStyle();
        }

        ImNodes::EndNodeTitleBar();

        node->renderBody();

        for (const auto& output : node->getOutputs() | std::views::values) {
            auto& style = output->getStyle();
            ImNodes::PushColorStyle(ImNodesCol_Pin, style.color);
            ImNodes::BeginOutputAttribute(output->getOrCreateInternalId(_uidProvider), toImNodesShape(style.shape));
            output->render();
            ImNodes::EndInputAttribute();
            ImNodes::PopColorStyle();
        }

        ImNodes::EndNode();

        for (const auto& output : node->getOutputs() | std::views::values) {
            int id = output->getOrCreateInternalId(_uidProvider);
            for (auto& link : output->getLinks()) {
                int linkId = link.getOrCreateInternalId(_uidProvider);
                int inId = link.getInput()->getOrCreateInternalId(_uidProvider);
                ImNodes::Link(linkId, id, inId);
            }
        }
    }

    Editor::Editor(Editor&& other) noexcept :
        _uidProvider(std::move(other._uidProvider)),
        _nodes(std::move(other._nodes)),
        _minimap(other._minimap),
        _nodeToRemove(other._nodeToRemove)
    {
    }

    Editor& Editor::operator=(Editor&& other) noexcept
    {
        if (this == &other) {
            return *this;
        }
        _uidProvider = std::move(other._uidProvider);
        _nodes = std::move(other._nodes);
        _minimap = other._minimap;
        _nodeToRemove = other._nodeToRemove;
        return *this;
    }

    Editor::Editor() :
        _minimap(false),
        _nodeToRemove(nullptr)
    {
        if (CONTEXT_COUNT++ == 0) {
            ImNodes::CreateContext();
            ImNodes::GetIO().LinkDetachWithModifierClick.Modifier = &ImGui::GetIO().KeyCtrl;
        }
    }

    Editor::~Editor()
    {
        if (--CONTEXT_COUNT == 0) {
            ImNodes::DestroyContext();
        }
    }

    bool Editor::isShowingMinimap() const
    {
        return _minimap;
    }

    void Editor::showMinimap(bool minimap)
    {
        _minimap = minimap;
    }

    void Editor::render()
    {
        ImNodes::BeginNodeEditor();

        for (auto& node : _nodes) {
            renderNode(node.get());
        }

        if (_minimap) {
            ImNodes::MiniMap();
        }

        ImNodes::EndNodeEditor();

        int start;
        int end;
        if (ImNodes::IsLinkCreated(&start, &end)) {
            auto input = findInputWithInternalId(end);
            auto output = findOutputWithInternalId(start);
            if (input && output) {
                output.value()->addLink(*input);
            }
        }

        int linkId;
        if (ImNodes::IsLinkDestroyed(&linkId)) {
            auto link = findLinkWithInternalId(linkId);
            if (link) {
                auto [out, in] = *link;
                out->removeLink(in, true);
            }
        }

        if (_nodeToRemove != nullptr) {
            removeNode(_nodeToRemove);
            _nodeToRemove = nullptr;
        }

        if( ImNodes::IsEditorHovered() && ImGui::GetIO().MouseWheel != 0 )
        {
            float zoom = ImNodes::EditorContextGetZoom() + ImGui::GetIO().MouseWheel * 0.1f;
            ImNodes::EditorContextSetZoom( zoom, ImGui::GetMousePos() );
        }
    }

    std::optional<NodeInput*> Editor::findInputWithInternalId(int id) const
    {
        for (auto& node : _nodes) {
            for (auto& input : node->getInputs() | std::views::values) {
                if (input->getInternalId() == id) {
                    return input.get();
                }
            }
        }

        return {};
    }

    std::optional<NodeOutput*> Editor::findOutputWithInternalId(int id) const
    {
        for (auto& node : _nodes) {
            for (auto& output : node->getOutputs() | std::views::values) {
                if (output->getInternalId() == id) {
                    return output.get();
                }
            }
        }

        return {};
    }

    std::optional<std::pair<NodeOutput*, NodeInput*>> Editor::findLinkWithInternalId(int id) const
    {
        for (auto& node : _nodes) {
            for (auto& output : node->getOutputs() | std::views::values) {
                for (auto& link : output->getLinks()) {
                    if (link.getInternalId() == id) {
                        return std::make_pair(output.get(), link.getInput());
                    }
                }
            }
        }

        return {};
    }

    void Editor::removeNode(Node* node)
    {
        erase_if(_nodes, [node](auto& it) { return it.get() == node; });
    }

    ImVec2 Editor::getNodePosition(Node* node) const
    {
        ImVec2 res;
        if (node == nullptr) {
            return res;
        }
        ImNodes::GetNodeGridSpacePos(node->getOrCreateInternalId(_uidProvider));

        return res;
    }

    void Editor::setNodePosition(Node* node, ImVec2 pos)
    {
        if (node == nullptr) {
            return;
        }
        ImNodes::SetNodeGridSpacePos(node->getOrCreateInternalId(_uidProvider), pos);
    }

    void Editor::setNodeScreenPosition(Node* node, ImVec2 pos)
    {
        if (node == nullptr) {
            return;
        }
        ImNodes::SetNodeScreenSpacePos(node->getOrCreateInternalId(_uidProvider), pos);
    }

} // namespace ImBlueprint
