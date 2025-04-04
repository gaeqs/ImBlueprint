//
// Created by gaeqs on 2/04/25.
//

#ifndef EDITOR_H
#define EDITOR_H

#include "EditorHandler.h"

#include <memory>
#include <vector>

#include <imblueprint/Node.h>

namespace ImBlueprint
{

    class Editor
    {
        EditorHandler* _handler;
        UIDProvider _uidProvider;

        std::vector<std::unique_ptr<Node>> _nodes;
        bool _minimap;

        Node* _nodeToRemove;

        void renderNode(Node* node);

      public:
        Editor(const Editor& other) = delete;

        Editor(Editor&& other) noexcept;

        Editor& operator=(Editor&& other) noexcept;

        Editor();

        ~Editor();

        [[nodiscard]] bool isShowingMinimap() const;

        void showMinimap(bool minimap);

        void render();

        [[nodiscard]] std::optional<NodeInput*> findInputWithInternalId(int id) const;

        [[nodiscard]] std::optional<NodeOutput*> findOutputWithInternalId(int id) const;

        [[nodiscard]] std::optional<std::pair<NodeOutput*, NodeInput*>> findLinkWithInternalId(int id) const;

        template<typename T, typename... Args>
        void addNode(Args&&... args)
        {
            _nodes.emplace_back(std::make_unique<T>(std::forward<Args>(args)...));
        }

        void removeNode(Node* node);
    };

} // namespace ImBlueprint

#endif // EDITOR_H
