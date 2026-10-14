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
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override;
        virtual ai::Obj* CreateTargetObject() const override;
        const CStr& GetEngineModelName() const;

    private:
        CStr m_engineModelName;
    };

    class SgNodeObj : public ai::Obj
    {
        // SgNodeObjPrototypeInfo::CreateTargetObject builds the object from its prototype.
        friend class SgNodeObjPrototypeInfo;

    protected:
        virtual  ~SgNodeObj() override;

    protected:
        SgNodeObj(const ai::SgNodeObjPrototypeInfo& prototypeInfo);
        SgNodeObj(const ai::SgNodeObj&);
        virtual m3d::Object* Clone() override;
        static m3d::Object* __fastcall CreateObject();

    public:
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override;
        static m3d::Class m_classSgNodeObj;
        virtual const ai::SgNodeObjPrototypeInfo* GetPrototypeInfo() const override;

    protected:
        static void __fastcall RegisterProperty(const char* Name, int id, ai::eGObjPropertySaveStatus saveStatus);

    public:
        virtual ai::eGObjPropertySaveStatus GetPropertySaveStatus(int id) const override;
        virtual void GetPropertiesNames(retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr> >& Props) const override;
        virtual void GetPropertiesIDs(retruxx::set<int, retruxx::less<int>, retruxx::allocator<int> >& Props) const override;
        virtual CStr GetPropertyName(int id) const override;
        virtual bool SetPropertyById(int propertyId, const m3d::AIParam& newValue) override;
        virtual int GetPropertyId(const char* PropertyName) const override;

    protected:
        static inline retruxx::map<CStr, int, ai::Obj::LessNoCaseCStr, retruxx::allocator<retruxx::pair<CStr const, int> > > m_propertiesMap;
        static inline retruxx::map<int, enum ai::eGObjPropertySaveStatus, retruxx::less<int>, retruxx::allocator<retruxx::pair<int const, enum ai::eGObjPropertySaveStatus> > > m_propertiesSaveStatesMap;
        virtual bool _GetPropertyDefaultInternal(int propertyId, m3d::AIParam& retVal) const override;
        virtual bool _GetPropertyInternal(int propertyId, m3d::AIParam& retVal) const override;

    public:
        void SetSgNode();
        void SetSgNode(const CStr& modelName);
        void DeleteSgNode();
        CVector GetPosition() const;
        virtual void SetPosition(const CVector& pos);
        Quaternion GetRotation() const;
        virtual void SetRotation(const Quaternion& rot);
        virtual void SetScale(float scale);
        float GetScale() const;
        virtual void Update(float elapsedTime, unsigned int workTime) override;
        static void __fastcall Registration();

    protected:
        virtual void _InternalCreateVisualPart() override;
        m3d::SgNode* m_node;
        CStr m_modelName;
        CVector m_position;
        Quaternion m_rotation;
        float m_scale;
        bool m_needToRelink;
    };
}
