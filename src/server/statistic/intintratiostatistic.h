#pragma once
#include "statisticmanager.h"

namespace m3d
{
    class AIParam;
}

namespace ai
{
    class IntIntRatioStatistic :  public Statistic
    {
    public:
        static m3d::Class * GetBaseClass();
        static m3d::Object * CreateObject();
        void IncreaseDenominator(int);
        virtual ~IntIntRatioStatistic();
        virtual m3d::Object * Clone();
        virtual CStr GetValue() const ;
        void IncreaseNumerator(int);
        virtual m3d::Class * GetClass() const ;
        virtual void SaveToXml(m3d::cmn::XmlFile *,m3d::cmn::XmlNode *) const ;
        virtual void Zero();
        virtual m3d::AIParam GetValueAsAIParam() const ;
        virtual void LoadFromXml(m3d::cmn::XmlFile *,m3d::cmn::XmlNode const *);

    protected:
        IntIntRatioStatistic();
        IntIntRatioStatistic(IntIntRatioStatistic const &);

    private:
        int m_numerator;
        int m_denominator;
    };
}
