#pragma once
#include "geom.h"

namespace ai
{
    class Box : public ai::Geom
    {
    public:
        static ai::Box* CreateObject(dxSpace* space, CVector const& size, void (*movedCalback)(dxGeom*));
        CVector GetSize() const;
        void SetSize(CVector const& size);

    private:
        Box(dxGeom* const geomId, void (*movedCalback)(dxGeom*));
    };
}  // namespace ai
