#pragma once
#include "base/objprefab.h"
#include <server/components/numericinrange.h>

namespace ai
{
    class BarricadePrototypeInfo : public ObjPrefabPrototypeInfo
    {
    public:
        BarricadePrototypeInfo();
        virtual Obj* CreateTargetObject() const;
        virtual bool LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);

    protected:
        virtual void _InternalCopyFrom(PrototypeInfo const&);

    private:
        float m_probability;
    };

    class Barricade : public ObjPrefab
    {
    public:
        virtual m3d::Class * GetClass() const ;
        virtual int GetPropertyId(char const *) const ;
        virtual eGObjPropertySaveStatus GetPropertySaveStatus(int) const ;
        virtual CStr GetPropertyName(int) const ;
        virtual BarricadePrototypeInfo const * GetPrototypeInfo() const ;
        virtual void GetPropertiesIDs(std::set<int,std::less<int>,std::allocator<int> > &) const ;
        virtual void LoadFromXML(m3d::cmn::XmlFile *,m3d::cmn::XmlNode const *);
        virtual void GetPropertiesNames(std::set<CStr,std::less<CStr>,std::allocator<CStr> > &) const ;
        static void __fastcall Registration();
        Barricade(BarricadePrototypeInfo const &);
        static m3d::Class * __fastcall GetBaseClass();
        virtual bool SetPropertyById(int,m3d::AIParam const &);

    protected:
        static void __fastcall RegisterProperty(char const *,int,eGObjPropertySaveStatus);
        virtual bool _GetPropertyDefaultInternal(int,m3d::AIParam &) const ;
        virtual ~Barricade();
        virtual bool _GetPropertyInternal(int,m3d::AIParam &) const ;

    private:
        static m3d::Object * __fastcall CreateObject();
        virtual m3d::Object * Clone();

    private:
        NumericInRange<float> m_probability;
    };
}
