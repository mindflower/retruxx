#pragma once
#include <core/clazz.h>

namespace ai
{
    class Statistic :  public m3d::Object
    {
    public:
        virtual m3d::Class * GetRtClass() const ;
        static m3d::Class * __fastcall GetBaseClass();
        bool GetGlobalFlag() const ;
        virtual void LoadFromXml(m3d::cmn::XmlFile *,m3d::cmn::XmlNode const *);
        void SetGlobalFlag(bool);
        virtual ~Statistic();
        virtual void SaveToXml(m3d::cmn::XmlFile *,m3d::cmn::XmlNode *) const ;

    protected:
        Statistic();

    private:
        bool m_bGlobalFlag;
    };
}
