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
    Editor::Editor(Editor&& other) noexcept :
        _handler(other._handler),
        _minimap(other._minimap)
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
            node->render();
        }

        if (_minimap) {
            ImNodes::MiniMap();
        }
        ImNodes::EndNodeEditor();
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
