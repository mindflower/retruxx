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

    class TeamTactic : public Obj
    {
    public:
        static m3d::Class* GetBaseClass();
        TeamTactic(TeamTacticPrototypeInfo const&);
        virtual TeamTacticPrototypeInfo const* GetPrototypeInfo() const;
        virtual m3d::Class* GetClass() const;

    public:
        RT_CLASS_DECLARE(TeamTactic);

    protected:
        virtual ~TeamTactic();
    private:
        static m3d::Object* CreateObject();
        virtual m3d::Object* Clone();
    };

    class TeamTacticWithRolesPrototypeInfo : public TeamTacticPrototypeInfo
    {
    public:
        TeamTacticWithRolesPrototypeInfo();
        virtual Obj* CreateTargetObject() const;
        std::vector<int> const& GetPrototypeIds() const;
        virtual bool LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
        virtual void PostLoad();

    private:
        std::vector<CStr> m_rolePrototypeNames;
        std::vector<int> m_rolePrototypeIds;
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
