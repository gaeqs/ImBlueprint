//
// Created by gaeqs on 4/04/25.
//

#include <imblueprint/UIDProvider.h>

namespace ImBlueprint
{

    UIDProvider::UIDProvider() :
        _current(0)
    {
    }

    int UIDProvider::nextUID()
    {
        return _current++;
    }
} // namespace ImBlueprint