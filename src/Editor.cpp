//
// Created by gaeqs on 2/04/25.
//

#include <imnodes.h>

#include <atomic>
#include <map>
#include <ranges>
#include <imblueprint/Editor.h>

#include <imblueprint/EditorHandler.h>

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
            ImNodes::BeginInputAttribute(input->getOrCreateInternalId(_uidProvider));
            input->render();
            ImNodes::EndInputAttribute();
        }

        ImNodes::EndNodeTitleBar();

        node->renderBody();

        for (const auto& output : node->getOutputs() | std::views::values) {
            ImNodes::BeginOutputAttribute(output->getOrCreateInternalId(_uidProvider));
            output->render();
            ImNodes::EndInputAttribute();
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
        _handler(other._handler),
        _minimap(other._minimap),
        _nodeToRemove(nullptr)
    {
        other._handler = new EditorHandler(&other);
        _handler->editor = this;
    }

    Editor::Editor() :
        _handler(new EditorHandler(this)),
        _minimap(false)
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

        delete _handler;
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
                out->removeLink(in);
            }
        }

        if (_nodeToRemove != nullptr) {
            removeNode(_nodeToRemove);
            _nodeToRemove = nullptr;
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
        erase_if(_nodes, [node](auto& it) {
            return it.get() == node;
        });
    }


    Editor& Editor::operator=(Editor&& other) noexcept
    {
        delete _handler;
        _handler = other._handler;
        _minimap = other._minimap;

        other._handler = new EditorHandler(&other);
        _handler->editor = this;
        return *this;
    }

} // namespace ImBlueprint
