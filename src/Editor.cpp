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

        auto& [tsNormal, tsHover, tsSel] = node->getTitleStyle();
        ImNodes::SetTitleRectColor(tsNormal.topLeft, tsNormal.topRight, tsNormal.bottomLeft, tsNormal.bottomRight);
        ImNodes::SetHoverTitleRectColor(tsHover.topLeft, tsHover.topRight, tsHover.bottomLeft, tsHover.bottomRight);
        ImNodes::SetSelectedTitleRectColor(tsSel.topLeft, tsSel.topRight, tsSel.bottomLeft, tsSel.bottomRight);

        ImNodes::BeginNodeTitleBar();
        node->renderTitle();
        ImNodes::EndNodeTitleBar();

        for (const auto& input : node->getInputs() | std::views::values) {
            auto& style = input->getStyle();
            ImNodes::PushColorStyle(ImNodesCol_Pin, style.color);
            ImNodes::SetupInputAttribute(input->getOrCreateInternalId(_uidProvider), input->getName(),
                                         toImNodesShape(style.shape, input->hasLink()));
            ImNodes::PopColorStyle();
        }

        for (const auto& output : node->getOutputs() | std::views::values) {
            auto& style = output->getStyle();
            ImNodes::PushColorStyle(ImNodesCol_Pin, style.color);
            ImNodes::SetupOutputAttribute(output->getOrCreateInternalId(_uidProvider), output->getName(),
                                          toImNodesShape(style.shape, output->hasLink()));
            ImNodes::PopColorStyle();
        }

        ImNodes::BeginNodeBody();
        node->renderBody();
        ImNodes::EndNodeBody();

        if (!node->getInputs().empty()) {
            ImNodes::DrawInputAttributes();
            ImGui::SameLine();
        }

        if (!node->getOutputs().empty()) {
            ImNodes::DrawOutputAttributes();
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
            // ImNodes::GetIO().LinkDetachWithModifierClick.Modifier = &ImGui::GetIO().KeyCtrl;
            // ImNodes::GetStyle().NodeCornerRounding = 8.0f;
            // ImNodes::GetStyle().Flags -= ImNodesStyleFlags_NodeOutline;
            // ImNodes::GetStyle().Colors[ImNodesCol_TitleBar] = IM_COL32(255, 0, 0, 255);

            ImNodesStyle& style = ImNodes::GetStyle();
            style.Flags |= ImNodesStyleFlags_GridLinesPrimary;
            style.GridSpacing = 32.0f;
            style.NodeBorderThickness = 4.0f;
            style.NodeCornerRounding = 16.0f;

            ImNodes::GetStyle().Colors[ImNodesCol_NodeBackground] = IM_COL32(20, 20, 28, 175);
            ImNodes::GetStyle().Colors[ImNodesCol_NodeBackgroundHovered] = IM_COL32(20, 20, 28, 175);
            ImNodes::GetStyle().Colors[ImNodesCol_NodeBackgroundSelected] = IM_COL32(20, 20, 28, 175);
            ImNodes::GetStyle().Colors[ImNodesCol_NodeOutline] = IM_COL32(80, 80, 85, 255);

            ImNodes::GetStyle().Colors[ImNodesCol_TitleBar] = IM_COL32(234, 37, 81, 255);
            ImNodes::GetStyle().Colors[ImNodesCol_TitleBarHovered] = IM_COL32(234, 60, 100, 255);
            ImNodes::GetStyle().Colors[ImNodesCol_TitleBarSelected] = IM_COL32(234, 80, 130, 255);

            ImNodes::GetStyle().Colors[ImNodesCol_Link] = IM_COL32(82, 136, 182, 200);
            ImNodes::GetStyle().Colors[ImNodesCol_LinkHovered] = IM_COL32(50, 152, 224, 255);
            ImNodes::GetStyle().Colors[ImNodesCol_LinkSelected] = IM_COL32(50, 152, 224, 255);

            ImNodes::GetStyle().Colors[ImNodesCol_Pin] = IM_COL32(180, 180, 185, 255);
            ImNodes::GetStyle().Colors[ImNodesCol_PinHovered] = IM_COL32(255, 255, 255, 255);

            ImNodes::GetStyle().Colors[ImNodesCol_BoxSelector] = IM_COL32(0, 122, 204, 50);
            ImNodes::GetStyle().Colors[ImNodesCol_BoxSelectorOutline] = IM_COL32(0, 122, 204, 150);

            ImNodes::GetStyle().Colors[ImNodesCol_GridBackground] = IM_COL32(50, 50, 55, 255);
            ImNodes::GetStyle().Colors[ImNodesCol_GridLine] = IM_COL32(52, 52, 57, 255);
            ImNodes::GetStyle().Colors[ImNodesCol_GridLinePrimary] = IM_COL32(52, 52, 57, 255);

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

        if (ImNodes::IsEditorHovered() && ImGui::GetIO().MouseWheel != 0) {
            float zoom = ImNodes::EditorContextGetZoom() + ImGui::GetIO().MouseWheel * 0.1f;
            ImNodes::EditorContextSetZoom(zoom, ImGui::GetMousePos());
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
        pos = ImNodes::ConvertToEditorContextSpace(pos);
        ImNodes::SetNodeEditorSpacePos(node->getOrCreateInternalId(_uidProvider), pos);
    }

} // namespace ImBlueprint
