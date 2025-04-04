//
// Created by gaeqs on 2/04/25.
//

#ifndef EDITOR_H
#define EDITOR_H

#include <memory>
#include <imblueprint/Node.h>

#include <vector>
#include <imblueprint/EditorHandler.h>

namespace ImBlueprint
{

    class Editor
    {
        EditorHandler* _handler;

        std::vector<std::unique_ptr<Node>> _nodes;
        bool _minimap;

      public:
        Editor(const Editor& other) = delete;

        Editor(Editor&& other) noexcept;

        Editor();

        ~Editor();

        [[nodiscard]] bool isShowingMinimap() const;

        void showMinimap(bool minimap);

        void render();

        Editor& operator=(Editor&& other) noexcept;

        template<typename T, typename... Args>
        void addNode(Args&&... args)
        {
            _nodes.emplace_back(std::make_unique<T>(std::forward<Args>(args)...));
        }
    };

} // namespace ImBlueprint

#endif // EDITOR_H
