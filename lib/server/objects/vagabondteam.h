#pragma once
#include "team.h"

namespace ai
{
    class VagabondTeamPrototypeInfo : public ai::TeamPrototypeInfo
    {
    public:
        VagabondTeamPrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode) override;
        CStr const& GetVehiclesGeneratorPrototypeName() const;
        std::vector<CStr, std::allocator<CStr>> const& GetWaresPrototypes() const;
        virtual ai::Obj* CreateTargetObject() const override;

    private:
        CStr m_vehiclesGeneratorPrototype;
        std::vector<CStr, std::allocator<CStr>> m_waresPrototypes;
    };

    class VagabondTeam : public ai::Team
    {
    protected:
        virtual ~VagabondTeam() override;

    private:
        VagabondTeam(ai::VagabondTeamPrototypeInfo const& prototypeInfo);
        virtual m3d::Object* Clone() override;
        static m3d::Object* __fastcall CreateObject();

    public:
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override;
        static m3d::Class m_classVagabondTeam;
        virtual VagabondTeamPrototypeInfo const* GetPrototypeInfo() const override;

        virtual void LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode) override;
        virtual void SaveToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override;
        void GenerateAndPlace(CVector const& start);

    private:
        virtual void _DoNoticeEnemy(int enemyId) override;

        int m_vehiclesGeneratorPrototypeId;
    };
}  // namespace ai
