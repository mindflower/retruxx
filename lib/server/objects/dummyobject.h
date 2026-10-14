#pragma once
#include "base/simplephysicobj.h"

namespace ai
{
    class DummyObjectPrototypeInfo : public ai::SimplePhysicObjPrototypeInfo
    {
    public:
        DummyObjectPrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode) override;
        virtual ai::Obj* CreateTargetObject() const override;
        bool m_DisablePhysics;
        bool m_DisableGeometry;
    };

    class DummyObject : public ai::SimplePhysicObj
    {
        friend class DummyObjectPrototypeInfo;

    protected:
        virtual ~DummyObject() override;

    protected:
        DummyObject(ai::DummyObjectPrototypeInfo const& prototypeInfo);
        virtual m3d::Object* Clone() override;
        static m3d::Object* __fastcall CreateObject();

    public:
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override;
        static m3d::Class m_classDummyObject;
        virtual ai::DummyObjectPrototypeInfo const* GetPrototypeInfo() const override;

    protected:
        static void __fastcall RegisterProperty(char const* Name, int id, ai::eGObjPropertySaveStatus saveStatus);

    public:
        virtual ai::eGObjPropertySaveStatus GetPropertySaveStatus(int id) const override;
        virtual void GetPropertiesNames(
            retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr>>& Props) const override;
        virtual void GetPropertiesIDs(
            retruxx::set<int, retruxx::less<int>, retruxx::allocator<int>>& Props) const override;
        virtual CStr GetPropertyName(int id) const override;
        virtual bool SetPropertyById(int propertyId, m3d::AIParam const& newValue) override;
        virtual int GetPropertyId(char const* PropertyName) const override;

    protected:
        static inline retruxx::
            map<CStr, int, ai::Obj::LessNoCaseCStr, retruxx::allocator<retruxx::pair<CStr const, int>>>
                m_propertiesMap;
        static inline retruxx::map<
            int,
            enum ai::eGObjPropertySaveStatus,
            retruxx::less<int>,
            retruxx::allocator<retruxx::pair<int const, enum ai::eGObjPropertySaveStatus>>>
            m_propertiesSaveStatesMap;
        virtual bool _GetPropertyDefaultInternal(int propertyId, m3d::AIParam& retVal) const override;
        virtual bool _GetPropertyInternal(int propertyId, m3d::AIParam& retVal) const override;

    public:
        virtual bool CanChildBeAdded(m3d::Class* pClass) const override;
        void SetModelName(char const* modelName);
        void SetSgNodeAndCollision(m3d::SgNode* node, ai::CollisionInfo const* collisionInfo);
        void SetModelNameUnsafe(CStr const& modelName);
        static void __fastcall Registration();

    private:
        CStr m_modelName;
    };
}  // namespace ai
