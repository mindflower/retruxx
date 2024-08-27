#pragma once
#include "statisticmanager.h"

namespace m3d
{
    class AIParam;
}

namespace ai
{
    class IntStatistic : public Statistic
    {
    public:
        virtual CStr GetValue() const ;
        virtual m3d::Object * Clone();
        void Increase(int);
        static m3d::Object * CreateObject();
        virtual ~IntStatistic();
        static m3d::Class * GetBaseClass();
        virtual m3d::AIParam GetValueAsAIParam() const ;
        virtual void LoadFromXml(m3d::cmn::XmlFile *,m3d::cmn::XmlNode const *);
        virtual void Zero();
        virtual void SaveToXml(m3d::cmn::XmlFile *,m3d::cmn::XmlNode *) const ;
        virtual m3d::Class * GetClass() const ;

    protected:
        IntStatistic();
        IntStatistic(IntStatistic const &);

    public:
        RT_CLASS_DECLARE(IntStatistic);

    private:
        int m_value;
    };
}
