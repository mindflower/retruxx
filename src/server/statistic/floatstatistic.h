#pragma once
#include "statisticmanager.h"

namespace m3d
{
    class AIParam;
}

namespace ai
{
    class FloatStatistic : public Statistic
    {
    public:
        virtual CStr GetValue() const ;
        void Increase(float);
        virtual m3d::Object * Clone();
        virtual void LoadFromXml(m3d::cmn::XmlFile *,m3d::cmn::XmlNode const *);
        static m3d::Object * CreateObject();
        virtual ~FloatStatistic();
        static m3d::Class * GetBaseClass();
        virtual m3d::AIParam GetValueAsAIParam() const ;
        virtual void Zero();
        virtual void SaveToXml(m3d::cmn::XmlFile *,m3d::cmn::XmlNode *) const ;
        virtual m3d::Class * GetClass() const ;

    protected:
        FloatStatistic(FloatStatistic const &);
        FloatStatistic();

    public:
        RT_CLASS_INLINE_DECLARE(FloatStatistic);

    private:
        float m_value;
    };
}
