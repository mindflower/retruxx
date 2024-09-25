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
        /* 0x0000 */ float m_timeOut;
        /* 0x0004 */ ai::eModifierOperation m_Operation;
        /* 0x0008 */ int m_magicPrototypeId;
        /* 0x000c */ CStr m_PropertyName;
        /* 0x0018 */ int m_SenderID;
        /* 0x001c */ m3d::AIParam m_Value;
        Modifier(const ai::Modifier& Mod);
        Modifier();
        ~Modifier();
        void Create(const CStr& PropertyName, ai::eModifierOperation Operation, const m3d::AIParam& value);
        void LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* OwnNode);
        void SaveToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* OwnNode) const;
        void ReadFromStr(const CStr& Str);
        void Apply(CVector2* Property, CVector2 base) const;
        void Apply(CVector* Property, CVector base) const;
        void Apply(float* Property, float base) const;
        void ApplyModelNum(int* Property, int base) const;
        void ApplyModelNumList(oldstd::vector<int>* MNL) const;
        void ApplyEngineID(int* Property, int base) const;
    }; /* size: 0x0038 */
}
