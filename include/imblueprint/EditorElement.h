//
// Created by gaeqs on 4/04/25.
//

#ifndef IMBLUEPRINT_EDITORELEMENT_H
#define IMBLUEPRINT_EDITORELEMENT_H

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

#endif // IMBLUEPRINT_EDITORELEMENT_H
