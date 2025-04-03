//
// Created by gaeqs on 2/04/25.
//

#include <imnodes.h>

#include <atomic>
#include <imblueprint/Editor.h>

namespace
{

    std::atomic_int CONTEXT_COUNT = 0;

}

namespace ImBlueprint
{
    Editor::Editor() :
        _minimap(false)
    {
        if (CONTEXT_COUNT++ == 0) {
            ImNodes::CreateContext();
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

    void Editor::render() const
    {
        ImNodes::BeginNodeEditor();

        ImNodes::BeginNode(0);

        ImNodes::BeginNodeTitleBar();
        ImGui::TextUnformatted("Dataset");
        ImNodes::EndNodeTitleBar();

        ImGui::Text("Hello world");
        ImGui::Text("Hello world");
        ImGui::Text("Hello world");

        ImNodes::BeginOutputAttribute(1);
        // in between Begin|EndAttribute calls, you can call ImGui
        // UI functions
        ImGui::Text("output pin");
        ImNodes::EndOutputAttribute();

        ImNodes::EndNode();

        ImNodes::BeginNode(1);

        ImNodes::BeginNodeTitleBar();
        ImGui::TextUnformatted("Input");
        ImNodes::EndNodeTitleBar();

        ImGui::Text("Hello world");
        ImGui::Text("Hello world");
        ImGui::Text("Hello world");

        ImNodes::BeginInputAttribute(2, ImNodesPinShape_QuadFilled);
        // in between Begin|EndAttribute calls, you can call ImGui
        // UI functions
        ImGui::Text("input pin");
        ImNodes::EndOutputAttribute();

        ImNodes::EndNode();

        if (_minimap) {
            ImNodes::MiniMap();
        }
        ImNodes::EndNodeEditor();
    }

} // namespace ImBlueprint
