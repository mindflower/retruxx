#pragma once
#include "building.h"

namespace ai
{
    class BarPrototypeInfo : public ai::BuildingPrototypeInfo
    {
    protected:
        virtual void _InternalCopyFrom(const ai::PrototypeInfo& rhs) override /* 0x00 */;

    public:
        BarPrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0x04 */;
        virtual ai::Obj* CreateTargetObject() const override /* 0x10 */;
        /* 0x0044 */ bool m_bWithBarman;
    }; /* size: 0x0048 */

    static_assert(sizeof(BarPrototypeInfo) == 0x0048);

    class Bar : public ai::Building
    {
    protected:
        virtual  ~Bar() override /* 0x00 */;

    private:
        Bar(const ai::BarPrototypeInfo& prototypeInfo);
        Bar(const ai::Bar&);
        virtual m3d::Object* Clone() override /* 0x00 */;
        static m3d::Object* __fastcall CreateObject();

    public:
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override /* 0x00 */;
        static m3d::Class m_classBar;
        virtual const ai::BarPrototypeInfo* GetPrototypeInfo() const override /* 0x4c */;
        bool bWithBarman() const;
        virtual void CreateChildren() override /* 0x00 */;
        virtual bool CanChildBeAdded(m3d::Class* pClass) const override /* 0x98 */;
        virtual void AddChild(ai::Obj* pObj) override /* 0x94 */;
        virtual bool RemoveChild(ai::Obj* pChild) override /* 0x9c */;
        virtual void Update(float elapsedTime, unsigned int workTime) override /* 0x00 */;

    protected:
        /* 0x00d0 */ int m_barmanId;

    private:
        void CreateBarman();
    }; /* size: 0x00d4 */

    static_assert(sizeof(Bar) == 0x00d4);
}
