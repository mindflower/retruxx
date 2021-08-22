#pragma once
#include "base/obj.h"
#include "base/prototypeinfo.h"

namespace m3d
{
    class CameraPath;
}

namespace ai
{
    class PhysicObj;

    class CinematicMoverPrototypeInfo : public PrototypeInfo
    {
    public:
        CinematicMoverPrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
        virtual Obj* CreateTargetObject() const;
    };


    class CinematicMover : public Obj
    {
    public:
        static inline m3d::Class m_classCinematicMover;

    public:
        virtual m3d::Class* GetClass() const;
        CinematicMover(CinematicMoverPrototypeInfo const&);
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const;
        void SetObjAndPath(int, CStr const&, float);
        static m3d::Class* __fastcall GetBaseClass();
        virtual void Update(float, unsigned int);
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
        virtual CinematicMoverPrototypeInfo const* GetPrototypeInfo() const;

    protected:
        virtual ~CinematicMover();

    private:
        PhysicObj* _GetControlledObj() const;
        static m3d::Object* __fastcall CreateObject();
        virtual m3d::Object* Clone();
        CStr m_flyPathName;
        m3d::CameraPath* m_currentFlyPath;
        float m_currentFlyTime;
        int m_controlledObjId;
    };
}
