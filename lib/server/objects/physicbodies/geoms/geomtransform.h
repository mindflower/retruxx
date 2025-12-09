#pragma once
#include "geom.h"

namespace ai
{
    class GeomTransform : public ai::Geom
    {
    public:
        static ai::GeomTransform* CreateObject(dxSpace* space, void (*movedCalback)(dxGeom*));
        virtual  ~GeomTransform() override /* 0x00 */;
        void SetGeom(ai::Geom* pGeom);
        const ai::Geom* GetGeom() const;
        ai::Geom* GetGeom();

    private:
        GeomTransform(dxGeom* const geomId, void (*movedCalback)(dxGeom*));
        /* 0x0018 */ ai::Geom* m_innerGeom;
    }; /* size: 0x001c */
}
