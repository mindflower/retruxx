#pragma once
#include "statisticmanager.h"
#include <map>

namespace m3d
{
    class AIParam;
}

namespace ai
{
    class FavoriteStringStatistic :  public Statistic
    {
    public:
        static m3d::Class * GetBaseClass();
        virtual m3d::Object * Clone();
        virtual void Zero();
        virtual CStr GetValue() const ;
        virtual void SaveToXml(m3d::cmn::XmlFile *,m3d::cmn::XmlNode *) const ;
        virtual void LoadFromXml(m3d::cmn::XmlFile *,m3d::cmn::XmlNode const *);
        virtual m3d::AIParam GetValueAsAIParam() const ;
        static m3d::Object * CreateObject();
        virtual ~FavoriteStringStatistic();
        virtual m3d::Class * GetClass() const ;
        void Increase(CStr const &);

    protected:
        FavoriteStringStatistic();
        FavoriteStringStatistic(FavoriteStringStatistic const &);

    public:
        RT_CLASS_DECLARE(FavoriteStringStatistic);

    private:
        std::map<CStr,unsigned int> m_counters;
    };
}
