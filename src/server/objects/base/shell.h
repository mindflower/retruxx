#pragma once
#include "simplephysicobj.h"

namespace ai
{
    class Gun;
    class GunPrototypeInfo;

    class ShellPrototypeInfo : public ai::SimplePhysicObjPrototypeInfo
    {
    public:
        virtual bool LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
        ShellPrototypeInfo();
    };

    class Shell : public SimplePhysicObj
    {
    public:
        Gun * GetGun() const ;
        GunPrototypeInfo const * GetGunPrototypeInfo() const ;
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile *,m3d::cmn::XmlNode const *);
        static m3d::Class * GetBaseClass();
        int GetGunObjId() const ;
        virtual m3d::Class * GetClass() const ;
        int GetEmittedObjId() const ;
        virtual ShellPrototypeInfo const * GetPrototypeInfo() const ;
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile *,m3d::cmn::XmlNode *) const ;
        void SetGunObjId(int);
        Shell(ShellPrototypeInfo const &);
        Obj * GetEmittedObj() const ;

    protected:
        virtual ~Shell();

    protected:
        static m3d::Object * CreateObject();
        virtual m3d::Object * Clone();

    public:
        RT_CLASS_DECLARE(Shell);

    private:
        int m_gunObjId;
        int m_emittedObjId;
        int m_gunPrototypeId;
    };
}
