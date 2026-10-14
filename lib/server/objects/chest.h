#pragma once
#include "base/simplephysicobj.h"

namespace ai
{
    class ChestPrototypeInfo : public ai::SimplePhysicObjPrototypeInfo
    {
    public:
        float m_LifeTime;
        bool m_WithLifeTime;
        ChestPrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override;
        virtual ai::Obj* CreateTargetObject() const override;
    };

    class Chest : public ai::SimplePhysicObj
    {
        friend class ChestPrototypeInfo;
    protected:
        virtual  ~Chest() override;

    private:
        Chest(const ai::ChestPrototypeInfo& prototypeInfo);
        virtual m3d::Object* Clone() override;
        static m3d::Object* __fastcall CreateObject();

    public:
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override;
        static m3d::Class m_classChest;
        virtual const ai::ChestPrototypeInfo* GetPrototypeInfo() const override;
        virtual void Update(float elapsedTime, unsigned int workTime) override;
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override;
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override;
        ai::GeomRepository* GetRepository();
        void FillFromGroundRepository(ai::GeomRepository* repository, bool takeDroppedObjects);
        virtual void SetPositionSelf(const CVector& pos) override;
        virtual void RenderDebugInfo() const override;
        virtual bool CanChildBeAdded(m3d::Class* pClass) const override;
        virtual void AddChild(ai::Obj* pObj) override;
        virtual bool RemoveChild(ai::Obj* pObj) override;
        bool RemoveChildUnsafe(ai::Obj* pObj);

    private:
        ai::GeomRepository* m_repository;
        float m_LifeTime;
    };
}
