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
            /* 0x000c */ unsigned int m_count;
            Item(const CStr& protoName, int count);
        }; /* size: 0x0010 */

        using ItemVector = retruxx::vector<ai::InfectionTeamPrototypeInfo::Item, retruxx::allocator<ai::InfectionTeamPrototypeInfo::Item> >;

    public:
        retruxx::vector<ai::InfectionTeamPrototypeInfo::Item, retruxx::allocator<ai::InfectionTeamPrototypeInfo::Item> > m_items;
        /* 0x0070 */ int m_vehiclesGeneratorProtoId;
        InfectionTeamPrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0x04 */;
        virtual void PostLoad() override /* 0x08 */;
        virtual ai::Obj* CreateTargetObject() const override /* 0x10 */;

    private:
        CStr m_vehiclesGeneratorProtoName;
    }; /* size: 0x0080 */

    class InfectionTeam : public ai::Team
    {
    protected:
        virtual  ~InfectionTeam() override /* 0x00 */;

    private:
        InfectionTeam(const ai::InfectionTeamPrototypeInfo& prototypeInfo);
        virtual m3d::Object* Clone() override /* 0x00 */;
        static m3d::Object* __fastcall CreateObject();

    public:
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override /* 0x00 */;
        static m3d::Class m_classInfectionTeam;
        virtual const ai::InfectionTeamPrototypeInfo* GetPrototypeInfo() const override /* 0x4c */;
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0xb0 */;
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override /* 0xb8 */;
        void Generate(const retruxx::vector<CVector, retruxx::allocator<CVector> >& points, unsigned int restSchwarz);
        void SetCriticalDistAndTime(float criticalDist, float criticalTime);
        void SetBlindDistAndTime(float blindDist, float blindTime);

    protected:
        virtual void _InternalPostLoad() override /* 0xfc */;
        virtual void _TeamUpdate(float elapsedTime, unsigned int workTime) override /* 0x110 */;

    private:
        /* 0x0168 */ bool m_bGenerated;
        /* 0x0169 */ char Padding_274[3];
        /* 0x016c */ float m_criticalTeamDist;
        /* 0x0170 */ float m_criticalTeamTime;
        /* 0x0174 */ float m_timeBeyondCriticalDist;
        /* 0x0178 */ float m_blindTeamDist;
        /* 0x017c */ float m_blindTeamTime;
        /* 0x0180 */ float m_timeBeyondBlindDist;
    }; /* size: 0x0184 */
}
