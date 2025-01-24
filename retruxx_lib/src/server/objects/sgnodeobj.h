#pragma once
#include "base/obj.h"
#include "base/prototypeinfo.h"
#include <math/quaternion.h>
#include <math/vector.h>

namespace ai
{
    class SgNodeObjPrototypeInfo : public ai::PrototypeInfo
    {
    public:
        SgNodeObjPrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0x04 */;
        virtual ai::Obj* CreateTargetObject() const override /* 0x10 */;
        const CStr& GetEngineModelName() const;

    private:
        /* 0x0040 */ CStr m_engineModelName;
    }; /* size: 0x004c */

    static_assert(sizeof(SgNodeObjPrototypeInfo) == 0x004c);

    class SgNodeObj : public ai::Obj
    {
    protected:
        virtual  ~SgNodeObj() override /* 0x00 */;

    protected:
        SgNodeObj(const ai::SgNodeObjPrototypeInfo& prototypeInfo);
        SgNodeObj(const ai::SgNodeObj&);
        virtual m3d::Object* Clone() override /* 0x00 */;
        static m3d::Object* __fastcall CreateObject();

    public:
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override /* 0x00 */;
        static m3d::Class m_classSgNodeObj;
        virtual const ai::SgNodeObjPrototypeInfo* GetPrototypeInfo() const override /* 0x4c */;

    protected:
        static void __fastcall RegisterProperty(const char* Name, int id, ai::eGObjPropertySaveStatus saveStatus);

    public:
        virtual ai::eGObjPropertySaveStatus GetPropertySaveStatus(int id) const override /* 0x58 */;
        virtual void GetPropertiesNames(retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr> >& Props) const override /* 0x5c */;
        virtual void GetPropertiesIDs(retruxx::set<int, retruxx::less<int>, retruxx::allocator<int> >& Props) const override /* 0x60 */;
        virtual CStr GetPropertyName(int id) const override /* 0x78 */;
        virtual bool SetPropertyById(int propertyId, const m3d::AIParam& newValue) override /* 0x7c */;
        virtual int GetPropertyId(const char* PropertyName) const override /* 0x74 */;

    protected:
        static retruxx::map<CStr, int, ai::Obj::LessNoCaseCStr, retruxx::allocator<retruxx::pair<CStr const, int> > > m_propertiesMap;
        static retruxx::map<int, enum ai::eGObjPropertySaveStatus, retruxx::less<int>, retruxx::allocator<retruxx::pair<int const, enum ai::eGObjPropertySaveStatus> > > m_propertiesSaveStatesMap;
        virtual bool _GetPropertyDefaultInternal(int propertyId, m3d::AIParam& retVal) const override /* 0x10c */;
        virtual bool _GetPropertyInternal(int propertyId, m3d::AIParam& retVal) const override /* 0x108 */;

    public:
        void SetSgNode();
        void SetSgNode(const CStr& modelName);
        void DeleteSgNode();
        CVector GetPosition() const;
        virtual void SetPosition(const CVector& pos) /* 0x110 */;
        Quaternion GetRotation() const;
        virtual void SetRotation(const Quaternion& rot) /* 0x114 */;
        virtual void SetScale(float scale) /* 0x118 */;
        float GetScale() const;
        virtual void Update(float elapsedTime, unsigned int workTime) override /* 0x80 */;
        static void __fastcall Registration();

    protected:
        virtual void _InternalCreateVisualPart() override /* 0x100 */;
        /* 0x00c0 */ m3d::SgNode* m_node;
        /* 0x00c4 */ CStr m_modelName;
        /* 0x00d0 */ CVector m_position;
        /* 0x00dc */ Quaternion m_rotation;
        /* 0x00ec */ float m_scale;
        /* 0x00f0 */ bool m_needToRelink;
    }; /* size: 0x00f4 */

    static_assert(sizeof(SgNodeObj) == 0x00f4);
}
