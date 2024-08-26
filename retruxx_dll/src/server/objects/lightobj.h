#pragma once
#include "sgnodeobj.h"

namespace ai
{
    class LightObjPrototypeInfo : public SgNodeObjPrototypeInfo
    {
    public:
        virtual bool LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
        virtual ai::Obj* CreateTargetObject() const;
        LightObjPrototypeInfo();
    };

    class LightObj :  public SgNodeObj
    {
    public:
        virtual m3d::Class * GetClass() const ;
        virtual void GetPropertiesNames(std::set<CStr,std::less<CStr>,std::allocator<CStr> > &) const ;
        static void __fastcall Registration();
        LightObj(LightObjPrototypeInfo const &);
        virtual void GetPropertiesIDs(std::set<int,std::less<int>,std::allocator<int> > &) const ;
        virtual bool SetPropertyById(int,m3d::AIParam const &);
        virtual eGObjPropertySaveStatus GetPropertySaveStatus(int) const ;
        static m3d::Class * GetBaseClass();
        virtual LightObjPrototypeInfo const * GetPrototypeInfo() const ;
        virtual CStr GetPropertyName(int) const ;
        virtual int GetPropertyId(char const *) const ;

    protected:
        virtual bool _GetPropertyDefaultInternal(int,m3d::AIParam &) const ;
        virtual ~LightObj();
        virtual bool _GetPropertyInternal(int,m3d::AIParam &) const ;

    private:
        static m3d::Object * CreateObject();
        virtual m3d::Object * Clone();

    public:
        RT_CLASS_DECLARE(LightObj);
    };
}
