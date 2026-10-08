#include "modifier.h"
#include <stdexcept>
#include <config.h>
#include <core/ini.h>
#include <core/kernel.h>
#include <core/log.h>
#include <math/vector.h>
#include <math/vector2.h>

#include <algorithm>
#include <cstdio>
#include <cstdlib>

namespace ai
{
    namespace
    {
        // Property names are matched case-insensitively, so everything that
        // stores one folds it to lower case on the way in.
        void NormalisePropertyName(CStr& name)
        {
            name.toLower(0x400u);
        }
    }  // namespace

    Modifier::Modifier()
    {
        // RVA 0x7DEE40 - NOTE: m_timeOut is deliberately left uninitialised by
        // the shipped constructor.
        this->m_Operation = MO_ADD;
        this->m_magicPrototypeId = -1;
        this->m_SenderID = -1;
    }

    Modifier::Modifier(Modifier const& Mod)
    {
        // RVA 0x7DEE90
        m_timeOut = Mod.m_timeOut;
        m_Operation = Mod.m_Operation;
        m_magicPrototypeId = Mod.m_magicPrototypeId;
        m_PropertyName = Mod.m_PropertyName;
        m_SenderID = Mod.m_SenderID;
        m_Value = Mod.m_Value;
        NormalisePropertyName(m_PropertyName);
    }

    Modifier& Modifier::operator=(Modifier const& rhs)
    {
        // RVA 0x7DCA90 - NOTE: the shipped assignment operator copies every
        // member except m_PropertyName, which keeps whatever the destination
        // already had. The copy constructor does copy it.
        m_timeOut = rhs.m_timeOut;
        m_Operation = rhs.m_Operation;
        m_magicPrototypeId = rhs.m_magicPrototypeId;
        m_SenderID = rhs.m_SenderID;
        m_Value = rhs.m_Value;
        return *this;
    }

    Modifier::~Modifier()
    {
        // RVA 0x7DEF20
    }

    void Modifier::Create(CStr const& PropertyName, eModifierOperation Operation, m3d::AIParam const& value)
    {
        // RVA 0x7DCA10
        m_Operation = Operation;
        m_PropertyName = PropertyName;
        NormalisePropertyName(m_PropertyName);
        m_Value = value;
    }

    void Modifier::ReadFromStr(CStr const& Str)
    {
        // RVA 0x7DCAE0 - the textual form is an operator character followed by
        // the value, e.g. "-10" or "= some string". A leading character that is
        // not a known operator, or one not followed by a separator, means the
        // whole string is the value and the operation defaults to add.
        char const* p = Str.c_str();
        while (*p == ' ')
        {
            ++p;
        }

        bool noOperator = false;
        char const second = p[1];
        if (second != ' ' && second != '\0')
        {
            noOperator = true;
        }
        else
        {
            switch (*p)
            {
            case '*': m_Operation = MO_MULT; break;
            case '+': m_Operation = MO_ADD; break;
            case '-': m_Operation = MO_SUB; break;
            case '/': m_Operation = MO_DIV; break;
            case '=': m_Operation = MO_SET; break;
            case '^': m_Operation = MO_BACK; break;
            default:  noOperator = true; break;
            }

            // Step over the operator and, if there is one, the separator.
            if (*++p)
            {
                ++p;
            }
        }

        if (noOperator)
        {
            m_Operation = MO_ADD;
        }

        m_Value.ReadFromString(CStr(p));

        // A bare string can only sensibly be assigned.
        if (noOperator && m_Value.GetType() == m3d::AIPARAM_STRING)
        {
            m_Operation = MO_SET;
        }
    }

    void Modifier::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* OwnNode)
    {
        // RVA 0x7DCC10 - NOTE: the shipped code reads the "Type" attribute and
        // throws the result away.
        OwnNode->GetAttribute("Type");

        m3d::SafeFloatAttrib(m_timeOut, OwnNode, "TimeOut");
        m_PropertyName = CStr(OwnNode->GetAttribute("PropertyName"));
        m3d::SafeIntAttrib(m_SenderID, OwnNode, "SenderID");
        m3d::SafeIntAttrib(m_magicPrototypeId, OwnNode, "MagicPrototypeId");

        char const* op = OwnNode->GetAttribute("Operation");
        m_Operation = MO_ADD;
        if (op)
        {
            if (!_stricmp(op, "MO_SET"))
            {
                m_Operation = MO_SET;
            }
            else if (!_stricmp(op, "MO_BACK"))
            {
                m_Operation = MO_BACK;
            }
            else if (!_stricmp(op, "MO_SUB"))
            {
                m_Operation = MO_SUB;
            }
            else if (!_stricmp(op, "MO_MULT"))
            {
                m_Operation = MO_MULT;
            }
            else if (!_stricmp(op, "MO_DIV"))
            {
                m_Operation = MO_DIV;
            }
        }

        m_Value.LoadFromXML(xmlFile, OwnNode);
    }

    void Modifier::SaveToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* OwnNode) const
    {
        // RVA 0x7DCD90
        char c[52];

        sprintf(c, "%.3f", m_timeOut);
        OwnNode->SetAttribute("TimeOut", c);
        OwnNode->SetAttribute("PropertyName", m_PropertyName.c_str());
        sprintf(c, "%u", m_SenderID);
        OwnNode->SetAttribute("SenderID", c);
        sprintf(c, "%u", m_magicPrototypeId);
        OwnNode->SetAttribute("MagicPrototypeId", c);

        CStr operationValue("MO_ADD");
        switch (m_Operation)
        {
        case MO_SET:  operationValue = CStr("MO_SET"); break;
        case MO_BACK: operationValue = CStr("MO_BACK"); break;
        case MO_ADD:  operationValue = CStr("MO_ADD"); break;
        case MO_SUB:  operationValue = CStr("MO_SUB"); break;
        case MO_MULT: operationValue = CStr("MO_MULT"); break;
        case MO_DIV:  operationValue = CStr("MO_DIV"); break;
        default: break;
        }
        OwnNode->SetAttribute("Operation", operationValue.c_str());

        m_Value.SaveToXML(xmlFile, OwnNode);
    }

    void Modifier::Apply(float* Property, float base) const
    {
        // RVA 0x7DCF80 - NOTE: MO_MULT and MO_DIV do not multiply or divide.
        // They add or subtract the value scaled by the base, so a modifier of
        // "*0.1" raises the property by a tenth of its unmodified value.
        switch (m_Value.GetType())
        {
        case m3d::AIPARAM_ID:
        case m3d::AIPARAM_FLOAT:
        case m3d::AIPARAM_STRING:
        {
            float const value = m_Value.GetAsFloat();
            switch (m_Operation)
            {
            case MO_SET:  *Property = value; break;
            case MO_BACK: *Property = base; break;
            case MO_ADD:  *Property = *Property + value; break;
            case MO_SUB:  *Property = *Property - value; break;
            case MO_MULT: *Property = value * base + *Property; break;
            case MO_DIV:  *Property = *Property - value * base; break;
            default:
            {
                M3D_LOG_INFO(CStr("Incorrect operation of Modifier ") + m_PropertyName);
                break;
            }
            }
            break;
        }

        case m3d::AIPARAM_RANGE:
        {
            // A range draws a fresh random value for every application.
            CVector2 const value = m_Value.GetAsRange();
            switch (m_Operation)
            {
            case MO_SET:  *Property = value.randomValue(); break;
            case MO_BACK: *Property = base; break;
            case MO_ADD:  *Property = value.randomValue() + *Property; break;
            case MO_SUB:  *Property = *Property - value.randomValue(); break;
            case MO_MULT: *Property = value.randomValue() * base + *Property; break;
            case MO_DIV:  *Property = *Property - value.randomValue() * base; break;
            default:
            {
                M3D_LOG_INFO(CStr("Incorrect operation of Modifier ") + m_PropertyName);
                break;
            }
            }
            break;
        }

        case m3d::AIPARAM_VECTOR:
        {
            M3D_LOG_INFO(
                CStr("Incorrect Modifier ") + m_PropertyName +
                CStr(". Couldn't apply CVector to float property"));
            break;
        }

        default:
            break;
        }
    }

    void Modifier::Apply(CVector* Property, CVector base) const
    {
        // RVA 0x7DD330
        switch (m_Value.GetType())
        {
        case m3d::AIPARAM_VECTOR:
        case m3d::AIPARAM_STRING:
        {
            CVector const value = m_Value.GetAsVector();
            switch (m_Operation)
            {
            case MO_SET:
                Property->x = value.x;
                Property->y = value.y;
                Property->z = value.z;
                break;
            case MO_BACK:
                *Property = base;
                break;
            case MO_ADD:
                Property->x = Property->x + value.x;
                Property->y = Property->y + value.y;
                Property->z = Property->z + value.z;
                break;
            case MO_SUB:
                Property->x = Property->x - value.x;
                Property->y = Property->y - value.y;
                Property->z = Property->z - value.z;
                break;
            default:
            {
                // NOTE: a vector valued modifier has no scaling operation, so
                // MO_MULT and MO_DIV land here and do nothing but log.
                M3D_LOG_INFO(CStr("Incorrect operation of Modifier ") + m_PropertyName);
                break;
            }
            }
            break;
        }

        case m3d::AIPARAM_ID:
        case m3d::AIPARAM_FLOAT:
        {
            // A scalar only makes sense as a proportion of the base value, so
            // here it is MO_MULT and MO_DIV that are the supported operations.
            float const value = m_Value.GetAsFloat();
            if (m_Operation == MO_MULT)
            {
                Property->x = Property->x + base.x * value;
                Property->y = Property->y + base.y * value;
                Property->z = Property->z + base.z * value;
            }
            else if (m_Operation == MO_DIV)
            {
                Property->x = Property->x - base.x * value;
                Property->y = Property->y - base.y * value;
                Property->z = Property->z - base.z * value;
            }
            else
            {
                M3D_LOG_INFO(CStr("Incorrect operation of Modifier ") + m_PropertyName);
            }
            break;
        }

        case m3d::AIPARAM_RANGE:
        {
            CVector2 const range = m_Value.GetAsRange();
            if (m_Operation == MO_MULT)
            {
                // One draw is shared by all three components.
                float const value = range.randomValue();
                Property->x = Property->x + base.x * value;
                Property->y = Property->y + base.y * value;
                Property->z = Property->z + base.z * value;
            }
            else if (m_Operation == MO_DIV)
            {
                float const value = range.randomValue();
                Property->x = Property->x - base.x * value;
                Property->y = Property->y - base.y * value;
                Property->z = Property->z - base.z * value;
            }
            else
            {
                M3D_LOG_INFO(CStr("Incorrect operation of Modifier ") + m_PropertyName);
            }
            break;
        }

        default:
            break;
        }
    }

    void Modifier::Apply(CVector2* Property, CVector2 base) const
    {
        // RVA 0x7DD850
        switch (m_Value.GetType())
        {
        case m3d::AIPARAM_ID:
        case m3d::AIPARAM_FLOAT:
        {
            // A scalar is applied to both components alike.
            float const value = m_Value.GetAsFloat();
            switch (m_Operation)
            {
            case MO_SET:
                Property->x = value;
                Property->y = value;
                break;
            case MO_BACK:
                *Property = base;
                break;
            case MO_ADD:
                Property->x = Property->x + value;
                Property->y = Property->y + value;
                break;
            case MO_SUB:
                Property->x = Property->x - value;
                Property->y = Property->y - value;
                break;
            case MO_MULT:
                Property->x = Property->x + base.x * value;
                Property->y = Property->y + base.y * value;
                break;
            case MO_DIV:
                Property->x = Property->x - base.x * value;
                Property->y = Property->y - base.y * value;
                break;
            default:
                break;
            }
            break;
        }

        case m3d::AIPARAM_STRING:
        case m3d::AIPARAM_RANGE:
        {
            // Here the pair is the value itself rather than something to draw
            // a random number from.
            CVector2 const value = m_Value.GetAsRange();
            switch (m_Operation)
            {
            case MO_SET:
                Property->x = value.x;
                Property->y = value.y;
                break;
            case MO_BACK:
                *Property = base;
                break;
            case MO_ADD:
                Property->x = Property->x + value.x;
                Property->y = Property->y + value.y;
                break;
            case MO_SUB:
                Property->x = Property->x - value.x;
                Property->y = Property->y - value.y;
                break;
            case MO_MULT:
                Property->x = Property->x + value.x * base.x;
                Property->y = Property->y + value.y * base.y;
                break;
            case MO_DIV:
                Property->x = Property->x - value.x * base.x;
                Property->y = Property->y - value.y * base.y;
                break;
            default:
            {
                M3D_LOG_INFO(CStr("Incorrect operation of Modifier ") + m_PropertyName);
                break;
            }
            }
            break;
        }

        case m3d::AIPARAM_VECTOR:
        {
            M3D_LOG_INFO(
                CStr("Incorrect Modifier ") + m_PropertyName +
                CStr(". Couldn't apply CVector to CVector2 property"));
            break;
        }

        default:
            break;
        }
    }

    void Modifier::ApplyEngineID(int* Property, int base) const
    {
        // RVA 0x7DDD00 - the value names an engine model; only assignment and
        // restoring the base make sense for an index.
        switch (m_Value.GetType())
        {
        case m3d::AIPARAM_VECTOR:
        {
            M3D_LOG_INFO(
                CStr("Incorrect Modifier ") + m_PropertyName +
                CStr(". Couldn't apply CVector to Index property"));
            break;
        }

        case m3d::AIPARAM_FLOAT:
        {
            M3D_LOG_INFO(
                CStr("Incorrect Modifier ") + m_PropertyName +
                CStr(". Couldn't apply float to Index property"));
            break;
        }

        case m3d::AIPARAM_RANGE:
        {
            M3D_LOG_INFO(
                CStr("Incorrect Modifier ") + m_PropertyName +
                CStr(". Couldn't apply Range to Index property"));
            break;
        }

        default:
        {
            CStr const value = m_Value.GetAsStr();
            int const id = M3D_ENGINE_CFG.GetModelIdByName(value);
            if (id == -1)
            {
                M3D_LOG_INFO(
                    CStr("Incorrect Modifier ") + m_PropertyName +
                    CStr(". Couldn't find engine model <") + value + CStr(">"));
                return;
            }

            if (m_Operation == MO_SET)
            {
                *Property = id;
            }
            else if (m_Operation == MO_BACK)
            {
                // Only put the base back if this modifier is the one in force.
                if (*Property == id)
                {
                    *Property = base;
                }
            }
            else
            {
                M3D_LOG_INFO(CStr("Incorrect operation of Modifier ") + m_PropertyName);
            }
            break;
        }
        }
    }

    void Modifier::ApplyModelNum(int* Property, int base) const
    {
        // RVA 0x7DE350 - as ApplyEngineID, but the index comes from the value's
        // own id list rather than from the engine's model table.
        switch (m_Value.GetType())
        {
        case m3d::AIPARAM_VECTOR:
        {
            M3D_LOG_INFO(
                CStr("Incorrect Modifier ") + m_PropertyName +
                CStr(". Couldn't apply CVector to Index property"));
            break;
        }

        case m3d::AIPARAM_FLOAT:
        {
            M3D_LOG_INFO(
                CStr("Incorrect Modifier ") + m_PropertyName +
                CStr(". Couldn't apply float to Index property"));
            break;
        }

        case m3d::AIPARAM_RANGE:
        {
            M3D_LOG_INFO(
                CStr("Incorrect Modifier ") + m_PropertyName +
                CStr(". Couldn't apply Range to Index property"));
            break;
        }

        case m3d::AIPARAM_STRING:
        case m3d::AIPARAM_ID_LIST:
        case m3d::AIPARAM_STRING_LIST:
        {
            retruxx::vector<int> const List = m_Value.GetAsIdList();
            if (List.empty() || List[0] == -1)
            {
                M3D_LOG_INFO(
                    CStr("Incorrect Modifier ") + m_PropertyName + CStr(". Couldn't find <") +
                    m_Value.GetAsStr() + CStr(">"));
                return;
            }

            int const id = List[0];
            if (m_Operation == MO_SET)
            {
                *Property = id;
            }
            else if (m_Operation == MO_BACK)
            {
                if (*Property == id)
                {
                    *Property = base;
                }
            }
            else
            {
                M3D_LOG_INFO(CStr("Incorrect operation of Modifier ") + m_PropertyName);
            }
            break;
        }

        default:
            break;
        }
    }

    void Modifier::ApplyModelNumList(retruxx::vector<int>* MNL) const
    {
        // RVA 0x7DE8E0 - set replaces the list outright, add unions into it and
        // back/sub removes the named entries from it.
        switch (m_Value.GetType())
        {
        case m3d::AIPARAM_VECTOR:
        {
            M3D_LOG_INFO(
                CStr("Incorrect Modifier ") + m_PropertyName +
                CStr(". Couldn't apply CVector to Index property"));
            break;
        }

        case m3d::AIPARAM_FLOAT:
        {
            M3D_LOG_INFO(
                CStr("Incorrect Modifier ") + m_PropertyName +
                CStr(". Couldn't apply float to Index property"));
            break;
        }

        case m3d::AIPARAM_RANGE:
        {
            M3D_LOG_INFO(
                CStr("Incorrect Modifier ") + m_PropertyName +
                CStr(". Couldn't apply Range to Index property"));
            break;
        }

        case m3d::AIPARAM_STRING:
        case m3d::AIPARAM_ID_LIST:
        case m3d::AIPARAM_STRING_LIST:
        {
            retruxx::vector<int> const List = m_Value.GetAsIdList();

            if (m_Operation == MO_SET)
            {
                *MNL = List;
                return;
            }

            if (m_Operation == MO_BACK || m_Operation == MO_SUB)
            {
                for (unsigned i = 0; i < List.size(); ++i)
                {
                    if (List[i] == -1)
                    {
                        continue;
                    }
                    for (unsigned j = 0; j < MNL->size();)
                    {
                        if ((*MNL)[j] == List[i])
                        {
                            MNL->erase(MNL->begin() + j);
                        }
                        else
                        {
                            ++j;
                        }
                    }
                }
                return;
            }

            if (m_Operation != MO_ADD)
            {
                M3D_LOG_INFO(
                    CStr("Incorrect operation of Modifier ") + m_PropertyName +
                    CStr(". No mult or div for PrototypeIdVector"));
                return;
            }

            // Add: append anything not already present.
            for (unsigned i = 0; i < List.size(); ++i)
            {
                if (List[i] == -1)
                {
                    continue;
                }
                if (std::find(MNL->begin(), MNL->end(), List[i]) == MNL->end())
                {
                    MNL->push_back(List[i]);
                }
            }
            break;
        }

        default:
            break;
        }
    }
}
