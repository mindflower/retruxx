#pragma once
#include "gun.h"

namespace ai
{
    class RocketLauncherPrototypeInfo : public GunPrototypeInfo
    {
    public:
        virtual Obj* CreateTargetObject() const;
        virtual bool LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
        RocketLauncherPrototypeInfo();
        virtual void PostLoad();

    private:
        bool m_withAngleLimit;
    };

    class RocketLauncher :  public Gun
    {
    public:
        virtual void GetPropertiesNames(std::set<CStr,std::less<CStr>,std::allocator<CStr> > &) const ;
        RocketLauncher(RocketLauncherPrototypeInfo const &);
        static m3d::Class * GetBaseClass();
        virtual RocketLauncherPrototypeInfo const * GetPrototypeInfo() const ;
        virtual bool isLookAtPoint(CVector const &,float) const ;
        virtual bool SetPropertyById(int,m3d::AIParam const &);
        virtual bool CanLookAtTarget() const ;
        virtual int GetPropertyId(char const *) const ;
        virtual m3d::Class * GetClass() const ;
        static void __fastcall Registration();
        virtual void GetPropertiesIDs(std::set<int,std::less<int>,std::allocator<int> > &) const ;
        virtual CStr GetPropertyName(int) const ;
        virtual eGObjPropertySaveStatus GetPropertySaveStatus(int) const ;

    protected:
        virtual bool _GetPropertyDefaultInternal(int,m3d::AIParam &) const ;
        virtual bool _GetPropertyInternal(int,m3d::AIParam &) const ;
        virtual void _LaunchShells();
        virtual ~RocketLauncher();

    private:
        static m3d::Object * CreateObject();
        virtual m3d::Object * Clone();
    };
}
