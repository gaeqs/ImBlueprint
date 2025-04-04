//
// Created by gaeqs on 4/04/25.
//

#ifndef IMBLUEPRINT_UIDPROVIDER_H
#define IMBLUEPRINT_UIDPROVIDER_H

namespace ImBlueprint
{

    class UIDProvider
    {
        int _current;

      public:
        UIDProvider();

        int nextUID();
    };

} // namespace ImBlueprint

#endif // IMBLUEPRINT_UIDPROVIDER_H
