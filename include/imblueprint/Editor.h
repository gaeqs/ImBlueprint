//
// Created by gaeqs on 2/04/25.
//

#ifndef IMBLUEPRINT_EDITOR_H
#define IMBLUEPRINT_EDITOR_H

#include <imgui.h>

#include <memory>
#include <vector>
#include <ranges>

#include <imblueprint/Node.h>

namespace ImBlueprint
{

    class Editor
    {
        mutable UIDProvider _uidProvider;

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

        auto getNodes() const
        {
            return _nodes | std::ranges::views::transform([](auto& it) -> const Node* { return it.get(); });
        }

        auto getNodes()
        {
            return _nodes | std::ranges::views::transform([](auto& it) -> Node* { return it.get(); });
        }

        template<typename T, typename... Args>
        Node* addNode(Args&&... args)
        {
            return _nodes.emplace_back(std::make_unique<T>(std::forward<Args>(args)...)).get();
        }

        void removeNode(Node* node);

        [[nodiscard]] ImVec2 getNodePosition(Node* node) const;

        void setNodePosition(Node* node, ImVec2 pos);
    };

} // namespace ImBlueprint

#endif // IMBLUEPRINT_EDITOR_H
