#pragma once
#include "geom.h"

namespace ai
{
    class Box : public ai::Geom
    {
    public:
        static ai::Box* CreateObject(dxSpace* space, const CVector& size, void (*movedCalback)(dxGeom*));
        CVector GetSize() const;
        void SetSize(const CVector& size);

    private:
        Box(dxGeom* const geomId, void (*movedCalback)( dxGeom*));
    }; /* size: 0x0018 */
}
