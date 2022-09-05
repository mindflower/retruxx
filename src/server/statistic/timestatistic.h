#pragma once
#include "statisticmanager.h"

namespace m3d
{
    class AIParam;
}

namespace ai
{
    class TimeStatistic : public Statistic
    {
    public:
        virtual CStr GetValue() const ;
        virtual m3d::AIParam GetValueAsAIParam() const ;
        static m3d::Class * GetBaseClass();
        virtual void Zero();
        static m3d::Object * CreateObject();
        virtual m3d::Class * GetClass() const ;
        virtual void SaveToXml(m3d::cmn::XmlFile *,m3d::cmn::XmlNode *) const ;
        virtual ~TimeStatistic();
        virtual void LoadFromXml(m3d::cmn::XmlFile *,m3d::cmn::XmlNode const *);
        virtual m3d::Object * Clone();
        void IncreaseByMilliseconds(__int64);

    protected:
        TimeStatistic(TimeStatistic const &);
        TimeStatistic();

    public:
        RT_CLASS_DECLARE(TimeStatistic);

    private:
        __int64 m_milliseconds;
    };
}
