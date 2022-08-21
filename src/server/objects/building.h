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

    class BuildingPrototypeInfo : public PrototypeInfo
    {
    public:
        virtual bool LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
        BuildingPrototypeInfo();
        virtual Obj* CreateTargetObject() const;

    private:
        BuildingType m_buildingType;
    };


    class Building :  public Obj
    {
    public:
        Building(BuildingPrototypeInfo const &);
        virtual m3d::Class * GetClass() const ;
        virtual bool RemoveChild(Obj *);
        BuildingType GetBuildingType() const ;
        virtual void Remove();
        static m3d::Class * GetBaseClass();
        static BuildingType __fastcall GetBuildingTypeByName(CStr const &);
        virtual bool CanChildBeAdded(m3d::Class *) const ;
        std::vector<Npc *> const & GetNpcs() const ;
        virtual void AddChild(Obj *);
        static CStr __fastcall GetBuildingTypeName(BuildingType);
        virtual BuildingPrototypeInfo const * GetPrototypeInfo() const ;

    protected:
        virtual ~Building();

    private:
        static m3d::Object * CreateObject();
        virtual m3d::Object * Clone();

    public:
        RT_CLASS_DECLARE(Building);

    private:
        std::vector<Npc *> m_npcs;
    };
}
