#pragma once
#include "team.h"

namespace ai
{
    class VagabondTeamPrototypeInfo : public TeamPrototypeInfo
    {
    public:
        std::vector<CStr, std::allocator<CStr> > const& GetWaresPrototypes() const;
        VagabondTeamPrototypeInfo();
        virtual Obj* CreateTargetObject() const;
        CStr const& GetVehiclesGeneratorPrototypeName() const;
        virtual bool LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);

    private:
        CStr m_vehiclesGeneratorPrototype;
        std::vector<CStr> m_waresPrototypes;

    };
    class VagabondTeam : public Team
    {
    public:
        VagabondTeam(VagabondTeamPrototypeInfo const &);
        virtual VagabondTeamPrototypeInfo const * GetPrototypeInfo() const ;
        void GenerateAndPlace(CVector const &);
        static m3d::Class * GetBaseClass();
        virtual void LoadFromXML(m3d::cmn::XmlFile *,m3d::cmn::XmlNode const *);
        virtual void SaveToXML(m3d::cmn::XmlFile *,m3d::cmn::XmlNode *) const ;
        virtual m3d::Class * GetClass() const ;

    protected:
        virtual ~VagabondTeam();

    private:
        virtual m3d::Object * Clone();
        static m3d::Object * CreateObject();
        virtual void _DoNoticeEnemy(int);

    public:
        RT_CLASS_INLINE_DECLARE(VagabondTeam);

    private:
        int m_vehiclesGeneratorPrototypeId;
    };
}
