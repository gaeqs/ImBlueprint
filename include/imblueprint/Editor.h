//
// Created by gaeqs on 2/04/25.
//

#ifndef EDITOR_H
#define EDITOR_H

namespace ImBlueprint
{

    class Editor
    {
        bool _minimap;

      public:
        Editor();

        ~Editor();

        [[nodiscard]] bool isShowingMinimap() const;

        void showMinimap(bool minimap);

        void render() const;
    };

} // namespace ImBlueprint

#endif // EDITOR_H
