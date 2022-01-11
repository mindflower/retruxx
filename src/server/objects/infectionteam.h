#pragma once
#include "team.h"

namespace ai
{
    class InfectionTeamPrototypeInfo : public TeamPrototypeInfo
    {
    public:
        class Item
        {
        public:
            Item(Item const&);

        private:
            CStr m_protoName;
            unsigned int m_count;
        };

    public:
        virtual bool LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
        InfectionTeamPrototypeInfo();
        virtual void PostLoad();
        virtual Obj* CreateTargetObject() const;

    private:
        std::vector<Item> m_items;
        int m_vehiclesGeneratorProtoId;
        CStr m_vehiclesGeneratorProtoName;

    };
    class InfectionTeam :  public Team
    {
    public:
        void SetCriticalDistAndTime(float,float);
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile *,m3d::cmn::XmlNode const *);
        InfectionTeam(InfectionTeamPrototypeInfo const &);
        void SetBlindDistAndTime(float,float);
        static m3d::Class * GetBaseClass();
        void Generate(std::vector<CVector,std::allocator<CVector> > const &,unsigned int);
        virtual m3d::Class * GetClass() const ;
        virtual InfectionTeamPrototypeInfo const * GetPrototypeInfo() const ;
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile *,m3d::cmn::XmlNode *) const ;

    protected:
        virtual void _InternalPostLoad();
        virtual ~InfectionTeam();
        virtual void _TeamUpdate(float,unsigned int);

    private:
        static m3d::Object * CreateObject();
        virtual m3d::Object * Clone();

    public:
        RT_CLASS_INLINE_DECLARE(InfectionTeam);

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
