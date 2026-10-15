#pragma once
#include "geom.h"

namespace ai
{
    class GeomTransform : public ai::Geom
    {
    public:
        static ai::GeomTransform* CreateObject(dxSpace* space, void (*movedCalback)(dxGeom*));
        virtual ~GeomTransform() override;
        void SetGeom(ai::Geom* pGeom);
        ai::Geom const* GetGeom() const;
        ai::Geom* GetGeom();

    private:
        GeomTransform(dxGeom* const geomId, void (*movedCalback)(dxGeom*));
        ai::Geom* m_innerGeom;
    };
}  // namespace ai
