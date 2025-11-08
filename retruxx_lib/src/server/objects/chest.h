#pragma once
#include "base/simplephysicobj.h"

namespace ai
{
    class ChestPrototypeInfo : public ai::SimplePhysicObjPrototypeInfo
    {
    public:
        /* 0x0080 */ float m_LifeTime;
        /* 0x0084 */ bool m_WithLifeTime;
        ChestPrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0x04 */;
        virtual ai::Obj* CreateTargetObject() const override /* 0x00 */;
    }; /* size: 0x0088 */

    class Chest : public ai::SimplePhysicObj
    {
    protected:
        virtual  ~Chest() override /* 0x00 */;

    private:
        Chest(const ai::ChestPrototypeInfo& prototypeInfo);
        virtual m3d::Object* Clone() override /* 0x00 */;
        static m3d::Object* __fastcall CreateObject();

    public:
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override /* 0x00 */;
        static m3d::Class m_classChest;
        virtual const ai::ChestPrototypeInfo* GetPrototypeInfo() const override /* 0x00 */;
        virtual void Update(float elapsedTime, unsigned int workTime) override /* 0x00 */;
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0x00 */;
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override /* 0x00 */;
        ai::GeomRepository* GetRepository();
        void FillFromGroundRepository(ai::GeomRepository* repository, bool takeDroppedObjects);
        virtual void SetPositionSelf(const CVector& pos) override /* 0x00 */;
        virtual void RenderDebugInfo() const override /* 0x00 */;
        virtual bool CanChildBeAdded(m3d::Class* pClass) const override /* 0x00 */;
        virtual void AddChild(ai::Obj* pObj) override /* 0x00 */;
        virtual bool RemoveChild(ai::Obj* pObj) override /* 0x00 */;
        bool RemoveChildUnsafe(ai::Obj* pObj);

    private:
        /* 0x0144 */ ai::GeomRepository* m_repository;
        /* 0x0148 */ float m_LifeTime;
    }; /* size: 0x014c */
}
