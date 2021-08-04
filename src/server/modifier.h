#pragma once
#include <core/aiparam.h>
#include <core/stringm3d.h>

namespace ai
{
    enum eModifierOperation
    {
        MO_SET = 0x0,
        MO_BACK = 0x1,
        MO_ADD = 0x2,
        MO_SUB = 0x3,
        MO_MULT = 0x4,
        MO_DIV = 0x5,
    };

    class Modifier
    {
    public:
        Modifier();
        Modifier(Modifier const &);
        ~Modifier();
        void Create(CStr const &,eModifierOperation,m3d::AIParam const &);
        void Apply(float *,float) const ;
        void Apply(CVector2 *,CVector2) const ;
        void Apply(CVector *,CVector) const ;
        Modifier & operator=(Modifier const &);

        template<class T>
        void ApplyInBuilt(T &,T) const ;

        void SaveToXML(m3d::cmn::XmlFile *,m3d::cmn::XmlNode *) const ;
        void ApplyModelNum(int *,int) const ;
        void ApplyModelNumList(std::vector<int,std::allocator<int> > *) const ;
        void ReadFromStr(CStr const &);
        void LoadFromXML(m3d::cmn::XmlFile *,m3d::cmn::XmlNode const *);
        void ApplyEngineID(int *,int) const ;

    public:
        float m_timeOut;
        eModifierOperation m_Operation;
        int m_magicPrototypeId;
        CStr m_PropertyName;
        int m_SenderID;
        m3d::AIParam m_Value;
    };
}
