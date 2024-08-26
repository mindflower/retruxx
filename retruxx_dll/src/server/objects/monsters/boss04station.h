#pragma once
#include <server/objects/base/complexphysicobj.h>

namespace ai
{
    class Boss04StationPrototypeInfo : public ComplexPhysicObjPrototypeInfo
    {
    public:
        virtual bool LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
        Boss04StationPrototypeInfo();
        virtual void PostLoad();
        virtual Obj* CreateTargetObject() const;
    };

    class Boss04Station : public ComplexPhysicObj
    {
    public:
        void EvaluateToDead();
        Boss04Station(Boss04StationPrototypeInfo const &);
        virtual void Update(float,unsigned int);
        virtual void InflictDamage(DamageInfo const &);
        virtual void GetPropertiesNames(std::set<CStr,std::less<CStr>,std::allocator<CStr> > &) const ;
        static void __fastcall Registration();
        bool bDestroyed() const ;
        static m3d::Class * GetBaseClass();
        virtual bool SetPropertyById(int,m3d::AIParam const &);
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile *,m3d::cmn::XmlNode const *);
        virtual void SaveToXML(m3d::cmn::XmlFile *,m3d::cmn::XmlNode *) const ;
        virtual m3d::Class * GetClass() const ;
        virtual int GetPropertyId(char const *) const ;
        virtual eGObjPropertySaveStatus GetPropertySaveStatus(int) const ;
        virtual CStr GetPropertyName(int) const ;
        virtual Boss04StationPrototypeInfo const * GetPrototypeInfo() const ;
        virtual void GetPropertiesIDs(std::set<int,std::less<int>,std::allocator<int> > &) const ;
        virtual void LoadFromXML(m3d::cmn::XmlFile *,m3d::cmn::XmlNode const *);
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile *,m3d::cmn::XmlNode *) const ;

    protected:
        virtual bool _GetPropertyDefaultInternal(int,m3d::AIParam &) const ;
        virtual bool _GetPropertyInternal(int,m3d::AIParam &) const ;
        virtual void _InternalPostLoad();
        virtual void _InternalCreateVisualPart();
        virtual ~Boss04Station();

    private:
        static m3d::Object * CreateObject();
        virtual m3d::Object * Clone();

    public:
        RT_CLASS_DECLARE(Boss04Station);

    private:
        bool m_bDestroyed;
    };
}
