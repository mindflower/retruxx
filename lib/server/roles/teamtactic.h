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
        virtual ~TeamTactic() override = 0;

        TeamTactic(ai::TeamTacticPrototypeInfo const& prototypeInfo);
        TeamTactic(ai::TeamTactic const&);
        virtual m3d::Object* Clone() override;
        static m3d::Object* CreateObject();

    public:
        static m3d::Class* GetBaseClass();
        virtual m3d::Class* GetClass() const override;
        static m3d::Class m_classTeamTactic;
        virtual ai::TeamTacticPrototypeInfo const* GetPrototypeInfo() const override;
        virtual float FitAgainstVehicle(ai::Team const*, ai::Vehicle const*) = 0;
        virtual float FitAgainstTeam(ai::Team const*, ai::Team const*) = 0;
        virtual float FitAgainstObj(ai::Team const*, ai::Obj const*) = 0;
        virtual void AssignAgainstVehicle(ai::Team*, ai::Vehicle const*) = 0;
        virtual void AssignAgainstTeam(ai::Team*, ai::Team const*) = 0;
        virtual void AssignAgainstObj(ai::Team*, ai::Obj const*) = 0;
    };

    class TeamTacticWithRolesPrototypeInfo : public ai::TeamTacticPrototypeInfo
    {
    public:
        TeamTacticWithRolesPrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode) override;
        virtual void PostLoad() override;
        virtual ai::Obj* CreateTargetObject() const override;
        std::vector<int, std::allocator<int>> const& GetPrototypeIds() const;

    private:
        std::vector<CStr, std::allocator<CStr>> m_rolePrototypeNames;
        std::vector<int, std::allocator<int>> m_rolePrototypeIds;
    };

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
        void GetRolePrototypeIdsEx(int, std::vector<int, std::allocator<int>>&) const;
        static m3d::Class* GetBaseClass();
        virtual float FitAgainstTeam(Team const*, Team const*);
        virtual float FitAgainstObj(Team const*, Obj const*);

    protected:
        virtual ~TeamTacticWithRoles();

    private:
        static m3d::Object* CreateObject();
        std::vector<int, std::allocator<int>> const& GetRolePrototypeIds() const;
        virtual m3d::Object* Clone();

    public:
        RT_CLASS_DECLARE(TeamTacticWithRoles);
    };
}  // namespace ai
