//
// Created by gaeqs on 4/04/25.
//

#ifndef UIDPROVIDER_H
#define UIDPROVIDER_H

namespace ImBlueprint
{

    class UIDProvider
    {
        int _current;

      public:
        UIDProvider(const UIDProvider&) = delete;

        UIDProvider();

        int nextUID();
    };

} // namespace ImBlueprint

#endif // UIDPROVIDER_H
