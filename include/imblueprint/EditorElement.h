//
// Created by gaeqs on 4/04/25.
//

#ifndef EDITORELEMENT_H
#define EDITORELEMENT_H

#include <optional>

#include <imblueprint/UIDProvider.h>

namespace ImBlueprint
{

    class EditorElement
    {
        mutable std::optional<int> _internalId;

      public:
        std::optional<int> getInternalId() const;

        int getOrCreateInternalId(UIDProvider& provider) const;
    };

} // namespace ImBlueprint

#endif // EDITORELEMENT_H
