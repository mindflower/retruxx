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
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0x04 */;
        virtual ai::Obj* CreateTargetObject() const override /* 0x10 */;
        /* 0x0040 */ ai::BuildingType m_buildingType;
    }; /* size: 0x0044 */

    static_assert(sizeof(BuildingPrototypeInfo) == 0x0044);

    class Building : public ai::Obj
    {
    protected:
        virtual  ~Building() override /* 0x00 */;

    protected:
        Building(const ai::BuildingPrototypeInfo& prototypeInfo);
        Building(const ai::Building&);
        virtual m3d::Object* Clone() override /* 0x00 */;
        static m3d::Object* __fastcall CreateObject();

    public:
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override /* 0x00 */;
        static m3d::Class m_classBuilding;
        virtual const ai::BuildingPrototypeInfo* GetPrototypeInfo() const override /* 0x4c */;

        using BuildingTypeSet = retruxx::set<enum ai::BuildingType, retruxx::less<enum ai::BuildingType>, retruxx::allocator<enum ai::BuildingType> >;

    public:
        virtual void Remove() override /* 0x54 */;
        virtual void AddChild(ai::Obj* pObj) override /* 0x94 */;
        virtual bool CanChildBeAdded(m3d::Class* pClass) const override /* 0x98 */;
        virtual bool RemoveChild(ai::Obj* pChild) override /* 0x9c */;

        using NpcVector = retruxx::vector<ai::Npc*, retruxx::allocator<ai::Npc*> >;

    public:
        const retruxx::vector<ai::Npc*, retruxx::allocator<ai::Npc*> >& GetNpcs() const;
        ai::BuildingType GetBuildingType() const;
        static CStr __fastcall GetBuildingTypeName(ai::BuildingType buildingType);
        static ai::BuildingType __fastcall GetBuildingTypeByName(const CStr& buildingTypeName);

    private:
        static const CStr m_buildingTypeNames[5];
        /* 0x00c0 */ retruxx::vector<ai::Npc*, retruxx::allocator<ai::Npc*> > m_npcs;
    }; /* size: 0x00d0 */

    static_assert(sizeof(Building) == 0x00d0);
}
