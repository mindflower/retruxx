#pragma once
#include "team.h"

struct CVector;

namespace ai
{
    class CaravanTeamPrototypeInfo : public ai::TeamPrototypeInfo
    {
    public:
        int m_tradersGeneratorPrototypeId;
        int m_guardsGeneratorPrototypeId;

        CaravanTeamPrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode) override;
        virtual void PostLoad() override;
        virtual Obj* CreateTargetObject() const override;
        retruxx::vector<CStr> const& GetWaresPrototypes() const;

    private:
        CStr m_tradersGeneratorPrototypeName;
        CStr m_guardsGeneratorPrototypeName;
        retruxx::vector<CStr> m_waresPrototypes;
    };

    class CaravanTeam : public ai::Team
    {
    protected:
        virtual ~CaravanTeam() override;

    private:
        CaravanTeam(ai::CaravanTeamPrototypeInfo const& prototypeInfo);
        virtual m3d::Object* Clone() override;
        static m3d::Object* __fastcall CreateObject();

    public:
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override;
        static m3d::Class m_classCaravanTeam;
        virtual ai::CaravanTeamPrototypeInfo const* GetPrototypeInfo() const override;

        virtual int OnEvent(ai::Event const& evn) override;
        virtual void Remove() override;
        virtual void LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode) override;
        virtual void SaveToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override;
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode) override;
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override;
        void GenerateAndPlace(CVector const& start);
        void SetWaitingPlayerToMoveout();

    protected:
        virtual void _TeamUpdate(float elapsedTime, unsigned int workTime) override;
        virtual void _DoNoticeEnemy(int objId) override;
        virtual void _DoUnderAttack(int attackerId) override;
        virtual void _DoPosUnreachable() override;

    private:
        int m_guardTeamId;
        retruxx::vector<int> m_guardVehiclesIds;
        bool m_waitingForPlayerToMoveout;
        int m_curAttackerId;

        retruxx::vector<int> _GenerateWithVehicleGenerator(int protoId, CVector const& pos);
        bool _HasAvailableGuards() const;
        void _EnsureGuardsAreInSeparateTeam();
        void _RemoveChildrenWhenPlayerIsFarEnough();
        void _RemoveUnlessChildrenExist();
        void _OnEnemyDestroyed(ai::Event const& evn);
    };
}  // namespace ai
