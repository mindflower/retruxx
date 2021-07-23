#pragma once
#include "team.h"

namespace ai
{
    class CaravanTeamPrototypeInfo : public TeamPrototypeInfo
    {
    public:
        CaravanTeamPrototypeInfo();
        std::vector<CStr, std::allocator<CStr> > const& GetWaresPrototypes() const;
        virtual Obj* CreateTargetObject() const;
        virtual void PostLoad();
        virtual bool LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);

    private:
        int m_tradersGeneratorPrototypeId;
        int m_guardsGeneratorPrototypeId;
        CStr m_tradersGeneratorPrototypeName;
        CStr m_guardsGeneratorPrototypeName;
        std::vector<CStr> m_waresPrototypes;
    };

    class CaravanTeam : public Team
    {
    public:
        static m3d::Class m_classCaravanTeam;

    public:
        void SetWaitingPlayerToMoveout();
        virtual CaravanTeamPrototypeInfo const* GetPrototypeInfo() const;
        virtual m3d::Class* GetClass() const;
        virtual int OnEvent(Event const&);
        virtual void LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
        CaravanTeam(CaravanTeamPrototypeInfo const&);
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
        virtual void SaveToXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const;
        void GenerateAndPlace(CVector const&);
        static m3d::Class* __fastcall GetBaseClass();
        virtual void Remove();
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const;

    protected:
        virtual void _DoNoticeEnemy(int);
        virtual void _TeamUpdate(float, unsigned int);
        virtual void _DoUnderAttack(int);
        virtual void _DoPosUnreachable();
        virtual ~CaravanTeam();
    private:

        void _RemoveChildrenWhenPlayerIsFarEnough();
        static m3d::Object* __fastcall CreateObject();
        std::vector<int, std::allocator<int> > _GenerateWithVehicleGenerator(int, CVector const&);
        bool _HasAvailableGuards() const;
        void _RemoveUnlessChildrenExist();
        void _OnEnemyDestroyed(Event const&);
        void _EnsureGuardsAreInSeparateTeam();
        virtual m3d::Object* Clone();

    private:
        int m_guardTeamId;
        std::vector<int> m_guardVehiclesIds;
        bool m_waitingForPlayerToMoveout;
        int m_curAttackerId;
    };
}
