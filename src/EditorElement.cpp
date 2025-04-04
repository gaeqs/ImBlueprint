//
// Created by gaeqs on 4/04/25.
//

#include <imblueprint/EditorElement.h>

namespace ImBlueprint
{

    std::optional<int> EditorElement::getInternalId() const
    {
        return _internalId;
    }

    int EditorElement::getOrCreateInternalId(UIDProvider& provider) const
    {
        if (!_internalId.has_value()) {
            _internalId = provider.nextUID();
        }

        return _internalId.value();
    }
} // namespace ImBlueprint