#pragma once
#include "physicobj.h"
#include <ode/mass.h>

namespace ai
{
    class GeomObjPrototypeInfo : public PhysicObjPrototypeInfo
    {
    public:
        virtual Obj* CreateTargetObject() const;
        GeomObjPrototypeInfo();
        virtual ~GeomObjPrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
    };

    class GeomObj :  public PhysicObj
    {
    public:
        virtual void LoadFromXML(m3d::cmn::XmlFile *,m3d::cmn::XmlNode const *);
        Geom * GetGeom();
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile *,m3d::cmn::XmlNode const *);
        virtual void RenderDebugInfo() const ;
        virtual void UnlinkGeomsFromCollisionCells();
        virtual Geom::CellAabb GetCollisionCellAabb() const ;
        virtual void EnableGeometry(bool);
        void SetGeom(CVector,float,float,bool);
        virtual void RelinkGeomsToCollisionCells();
        virtual GeomObjPrototypeInfo const * GetPrototypeInfo() const ;
        GeomObj(GeomObjPrototypeInfo const &);
        virtual m3d::Class * GetClass() const ;
        virtual void TransferToSpace(dxSpace *);
        static m3d::Class * GetBaseClass();
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile *,m3d::cmn::XmlNode *) const ;
        virtual void DisableGeometry(bool);
        virtual void LinkGeomsToCollisionCells();
        virtual void SaveToXML(m3d::cmn::XmlFile *,m3d::cmn::XmlNode *) const ;

    protected:
        virtual void _UnlinkBodyFromGeoms();
        virtual void _LinkBodyToGeoms();
        virtual void _SetRotationToGeoms(Quaternion const &);
        virtual void _Construct();
        virtual void _SetPositionToGeoms(CVector const &);
        virtual ~GeomObj();

    private:
        static m3d::Object * CreateObject();
        virtual m3d::Object * Clone();

    public:
        RT_CLASS_INLINE_DECLARE(GeomObj);

    private:
        Geom *m_pGeom;
        dMass m_mass;
    };
}
