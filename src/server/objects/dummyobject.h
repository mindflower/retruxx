#pragma once
#include "base/simplephysicobj.h"

namespace ai
{
    class DummyObjectPrototypeInfo : public SimplePhysicObjPrototypeInfo
    {
    public:
        DummyObjectPrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
        virtual Obj* CreateTargetObject() const;

    private:
        bool m_DisablePhysics;
        bool m_DisableGeometry;

    };

    class DummyObject :  public SimplePhysicObj
    {
    public:
        static void __fastcall Registration();
        virtual eGObjPropertySaveStatus GetPropertySaveStatus(int) const ;
        virtual int GetPropertyId(char const *) const ;
        void SetModelName(char const *);
        virtual bool SetPropertyById(int,m3d::AIParam const &);
        static m3d::Class * GetBaseClass();
        virtual m3d::Class * GetClass() const ;
        virtual CStr GetPropertyName(int) const ;
        virtual DummyObjectPrototypeInfo const * GetPrototypeInfo() const ;
        void SetModelNameUnsafe(CStr const &);
        void SetSgNodeAndCollision(m3d::SgNode *,CollisionInfo const *);
        virtual void GetPropertiesIDs(std::set<int,std::less<int>,std::allocator<int> > &) const ;
        virtual void GetPropertiesNames(std::set<CStr,std::less<CStr>,std::allocator<CStr> > &) const ;
        DummyObject(DummyObjectPrototypeInfo const &);
        virtual bool CanChildBeAdded(m3d::Class *) const ;

    protected:
        virtual bool _GetPropertyDefaultInternal(int,m3d::AIParam &) const ;
        virtual bool _GetPropertyInternal(int,m3d::AIParam &) const ;
        static void __fastcall RegisterProperty(char const *,int,eGObjPropertySaveStatus);
        virtual ~DummyObject();

    private:
        virtual m3d::Object * Clone();
        static m3d::Object * CreateObject();

    public:
        RT_CLASS_DECLARE(DummyObject);

    private:
        CStr m_modelName;
    };
}
