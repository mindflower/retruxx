#pragma once
#include "base/obj.h"
#include "base/prototypeinfo.h"

namespace ai
{
    class Npc;

    enum BuildingType
    {
        ADMINISTRATION = 0x0,
        BAR = 0x1,
        SHOP = 0x2,
        WORKSHOP = 0x3,
        GARAGE = 0x4,
        NUM_BUILDINGTYPES = 0x5,
        INVALID_BUILDINGTYPE = 0x5,
    };

    class BuildingPrototypeInfo : public ai::PrototypeInfo
    {
    public:
        BuildingPrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode) override;
        virtual ai::Obj* CreateTargetObject() const override;
        ai::BuildingType m_buildingType;
    };

    class Building : public ai::Obj
    {
        // BuildingPrototypeInfo::CreateTargetObject constructs a plain Building.
        friend class BuildingPrototypeInfo;

    protected:
        virtual ~Building() override;

    protected:
        Building(ai::BuildingPrototypeInfo const& prototypeInfo);
        Building(ai::Building const&);
        virtual m3d::Object* Clone() override;
        static m3d::Object* __fastcall CreateObject();

    public:
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override;
        static m3d::Class m_classBuilding;
        virtual ai::BuildingPrototypeInfo const* GetPrototypeInfo() const override;

        using BuildingTypeSet = retruxx::
            set<enum ai::BuildingType, retruxx::less<enum ai::BuildingType>, retruxx::allocator<enum ai::BuildingType>>;

    public:
        virtual void Remove() override;
        virtual void AddChild(ai::Obj* pObj) override;
        virtual bool CanChildBeAdded(m3d::Class* pClass) const override;
        virtual bool RemoveChild(ai::Obj* pChild) override;

        using NpcVector = retruxx::vector<ai::Npc*, retruxx::allocator<ai::Npc*>>;

    public:
        retruxx::vector<ai::Npc*, retruxx::allocator<ai::Npc*>> const& GetNpcs() const;
        ai::BuildingType GetBuildingType() const;
        static CStr __fastcall GetBuildingTypeName(ai::BuildingType buildingType);
        static ai::BuildingType __fastcall GetBuildingTypeByName(CStr const& buildingTypeName);

    private:
        static inline CStr const m_buildingTypeNames[5] = {"Administration", "Bar", "Shop", "Workshop", "Garage"};
        retruxx::vector<ai::Npc*, retruxx::allocator<ai::Npc*>> m_npcs;
    };
}  // namespace ai
