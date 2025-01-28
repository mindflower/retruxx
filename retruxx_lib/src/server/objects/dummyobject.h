#pragma once
#include "base/simplephysicobj.h"

namespace ai
{
    class DummyObjectPrototypeInfo : public ai::SimplePhysicObjPrototypeInfo
    {
    public:
        DummyObjectPrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0x04 */;
        virtual ai::Obj* CreateTargetObject() const override /* 0x00 */;
        /* 0x0080 */ bool m_DisablePhysics;
        /* 0x0081 */ bool m_DisableGeometry;
    }; /* size: 0x0084 */

    static_assert(sizeof(DummyObjectPrototypeInfo) == 0x0084);

    class DummyObject : public ai::SimplePhysicObj
    {
    protected:
        virtual  ~DummyObject() override /* 0x00 */;

    protected:
        DummyObject(const ai::DummyObjectPrototypeInfo& prototypeInfo);
        DummyObject(const ai::DummyObject&);
        virtual m3d::Object* Clone() override /* 0x00 */;
        static m3d::Object* __fastcall CreateObject();

    public:
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override /* 0x00 */;
        static m3d::Class m_classDummyObject;
        virtual const ai::DummyObjectPrototypeInfo* GetPrototypeInfo() const override /* 0x00 */;

    protected:
        static void __fastcall RegisterProperty(const char* Name, int id, ai::eGObjPropertySaveStatus saveStatus);

    public:
        virtual ai::eGObjPropertySaveStatus GetPropertySaveStatus(int id) const override /* 0x00 */;
        virtual void GetPropertiesNames(retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr> >& Props) const override /* 0x00 */;
        virtual void GetPropertiesIDs(retruxx::set<int, retruxx::less<int>, retruxx::allocator<int> >& Props) const override /* 0x00 */;
        virtual CStr GetPropertyName(int id) const override /* 0x00 */;
        virtual bool SetPropertyById(int propertyId, const m3d::AIParam& newValue) override /* 0x00 */;
        virtual int GetPropertyId(const char* PropertyName) const override /* 0x00 */;

    protected:
        static inline retruxx::map<CStr, int, ai::Obj::LessNoCaseCStr, retruxx::allocator<retruxx::pair<CStr const, int> > > m_propertiesMap;
        static inline retruxx::map<int, enum ai::eGObjPropertySaveStatus, retruxx::less<int>, retruxx::allocator<retruxx::pair<int const, enum ai::eGObjPropertySaveStatus> > > m_propertiesSaveStatesMap;
        virtual bool _GetPropertyDefaultInternal(int propertyId, m3d::AIParam& retVal) const override /* 0x00 */;
        virtual bool _GetPropertyInternal(int propertyId, m3d::AIParam& retVal) const override /* 0x00 */;

    public:
        virtual bool CanChildBeAdded(m3d::Class* pClass) const override /* 0x00 */;
        void SetModelName(const char* modelName);
        void SetSgNodeAndCollision(m3d::SgNode* node, const ai::CollisionInfo* collisionInfo);
        void SetModelNameUnsafe(const CStr& modelName);
        static void __fastcall Registration();

    private:
        /* 0x0144 */ CStr m_modelName;
    }; /* size: 0x0150 */

    static_assert(sizeof(DummyObject) == 0x0150);
}
