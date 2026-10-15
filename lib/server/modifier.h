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
        float m_timeOut;
        ai::eModifierOperation m_Operation;
        int m_magicPrototypeId;
        CStr m_PropertyName;
        int m_SenderID;
        m3d::AIParam m_Value;
        Modifier(ai::Modifier const& Mod);
        Modifier();
        ai::Modifier& operator=(ai::Modifier const& rhs);
        ~Modifier();
        void Create(CStr const& PropertyName, ai::eModifierOperation Operation, m3d::AIParam const& value);
        void LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* OwnNode);
        void SaveToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* OwnNode) const;
        void ReadFromStr(CStr const& Str);
        void Apply(CVector2* Property, CVector2 base) const;
        void Apply(CVector* Property, CVector base) const;
        void Apply(float* Property, float base) const;
        void ApplyModelNum(int* Property, int base) const;
        void ApplyModelNumList(retruxx::vector<int>* MNL) const;
        void ApplyEngineID(int* Property, int base) const;
    };
}  // namespace ai
