#pragma once
#include "team.h"

namespace ai
{
    class InfectionTeamPrototypeInfo : public ai::TeamPrototypeInfo
    {
    public:
        struct Item
        {
            CStr m_protoName;
            unsigned int m_count;
            Item(const CStr& protoName, int count);
        };

        using ItemVector = retruxx::vector<ai::InfectionTeamPrototypeInfo::Item, retruxx::allocator<ai::InfectionTeamPrototypeInfo::Item> >;

    public:
        retruxx::vector<ai::InfectionTeamPrototypeInfo::Item, retruxx::allocator<ai::InfectionTeamPrototypeInfo::Item> > m_items;
        int m_vehiclesGeneratorProtoId;
        InfectionTeamPrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override;
        virtual void PostLoad() override;
        virtual ai::Obj* CreateTargetObject() const override;

    private:
        CStr m_vehiclesGeneratorProtoName;
    };

    class InfectionTeam : public ai::Team
    {
        friend class InfectionTeamPrototypeInfo;
    protected:
        virtual  ~InfectionTeam() override;

    private:
        InfectionTeam(const ai::InfectionTeamPrototypeInfo& prototypeInfo);
        virtual m3d::Object* Clone() override;
        static m3d::Object* __fastcall CreateObject();

    public:
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override;
        static m3d::Class m_classInfectionTeam;
        virtual const ai::InfectionTeamPrototypeInfo* GetPrototypeInfo() const override;
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override;
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override;
        void Generate(const retruxx::vector<CVector, retruxx::allocator<CVector> >& points, unsigned int restSchwarz);
        void SetCriticalDistAndTime(float criticalDist, float criticalTime);
        void SetBlindDistAndTime(float blindDist, float blindTime);

    protected:
        virtual void _InternalPostLoad() override;
        virtual void _TeamUpdate(float elapsedTime, unsigned int workTime) override;

    private:
        bool m_bGenerated;
        float m_criticalTeamDist;
        float m_criticalTeamTime;
        float m_timeBeyondCriticalDist;
        float m_blindTeamDist;
        float m_blindTeamTime;
        float m_timeBeyondBlindDist;
    };
}
