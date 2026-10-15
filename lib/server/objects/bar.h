#pragma once
#include "building.h"

namespace ai
{
    class BarPrototypeInfo : public ai::BuildingPrototypeInfo
    {
    protected:
        virtual void _InternalCopyFrom(ai::PrototypeInfo const& rhs) override;

    public:
        BarPrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode) override;
        virtual ai::Obj* CreateTargetObject() const override;
        bool m_bWithBarman;
    };

    class Bar : public ai::Building
    {
        friend class BarPrototypeInfo;

    protected:
        virtual ~Bar() override;

    private:
        Bar(ai::BarPrototypeInfo const& prototypeInfo);
        Bar(ai::Bar const&);
        virtual m3d::Object* Clone() override;
        static m3d::Object* CreateObject();

    public:
        static m3d::Class* GetBaseClass();
        virtual m3d::Class* GetClass() const override;
        static m3d::Class m_classBar;
        virtual ai::BarPrototypeInfo const* GetPrototypeInfo() const override;
        bool bWithBarman() const;
        virtual void CreateChildren() override;
        virtual bool CanChildBeAdded(m3d::Class* pClass) const override;
        virtual void AddChild(ai::Obj* pObj) override;
        virtual bool RemoveChild(ai::Obj* pChild) override;
        virtual void Update(float elapsedTime, unsigned int workTime) override;

    protected:
        int m_barmanId;

    private:
        void CreateBarman();
    };
}  // namespace ai
