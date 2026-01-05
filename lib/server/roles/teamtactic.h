#pragma once
#include <server/objects/base/obj.h>
#include <server/objects/base/prototypeinfo.h>

namespace ai
{
    class Vehicle;
    class Team;

    class TeamTacticPrototypeInfo : public PrototypeInfo
    {
    public:
        virtual Obj* CreateTargetObject() const;
        TeamTacticPrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
    };

    class TeamTactic : public ai::Obj
    {
    protected:
        virtual ~TeamTactic() override = 0 /* 0x00 */;

        TeamTactic(ai::TeamTacticPrototypeInfo const& prototypeInfo);
        TeamTactic(ai::TeamTactic const&);
        virtual m3d::Object* Clone() override /* 0x00 */;
        static m3d::Object* __fastcall CreateObject();

    public:
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override /* 0x00 */;
        static m3d::Class m_classTeamTactic;
        virtual ai::TeamTacticPrototypeInfo const* GetPrototypeInfo() const override /* 0x4c */;
        virtual float FitAgainstVehicle(ai::Team const*, ai::Vehicle const*) = 0 /* 0x110 */;
        virtual float FitAgainstTeam(ai::Team const*, ai::Team const*) = 0 /* 0x114 */;
        virtual float FitAgainstObj(ai::Team const*, ai::Obj const*) = 0 /* 0x118 */;
        virtual void AssignAgainstVehicle(ai::Team*, ai::Vehicle const*) = 0 /* 0x11c */;
        virtual void AssignAgainstTeam(ai::Team*, ai::Team const*) = 0 /* 0x120 */;
        virtual void AssignAgainstObj(ai::Team*, ai::Obj const*) = 0 /* 0x124 */;
    }; /* size: 0x00c0 */

   class TeamTacticWithRolesPrototypeInfo : public ai::TeamTacticPrototypeInfo
    {
    public:
        TeamTacticWithRolesPrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode) override /* 0x04 */;
        virtual void PostLoad() override /* 0x00 */;
        virtual ai::Obj* CreateTargetObject() const override /* 0x10 */;
        std::vector<int, std::allocator<int>> const& GetPrototypeIds() const;

    private:
        /* 0x0040 */ std::vector<CStr, std::allocator<CStr>> m_rolePrototypeNames;
        /* 0x0050 */ std::vector<int, std::allocator<int>> m_rolePrototypeIds;
    }; /* size: 0x0060 */

    class TeamTacticWithRoles : public TeamTactic
    {
    public:
        virtual void AssignAgainstObj(Team*, Obj const*);
        TeamTacticWithRoles(TeamTacticWithRolesPrototypeInfo const&);
        virtual m3d::Class* GetClass() const;
        virtual TeamTacticWithRolesPrototypeInfo const* GetPrototypeInfo() const;
        virtual void AssignAgainstVehicle(Team*, Vehicle const*);
        virtual void AssignAgainstTeam(Team*, Team const*);
        virtual float FitAgainstVehicle(Team const*, Vehicle const*);
        void GetRolePrototypeIdsEx(int, std::vector<int, std::allocator<int> >&) const;
        static m3d::Class* GetBaseClass();
        virtual float FitAgainstTeam(Team const*, Team const*);
        virtual float FitAgainstObj(Team const*, Obj const*);

    protected:
        virtual ~TeamTacticWithRoles();

    private:
        static m3d::Object* CreateObject();
        std::vector<int, std::allocator<int> > const& GetRolePrototypeIds() const;
        virtual m3d::Object* Clone();

    public:
        RT_CLASS_DECLARE(TeamTacticWithRoles);
    };
}
