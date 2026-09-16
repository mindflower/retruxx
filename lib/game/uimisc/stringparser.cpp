#include "stringparser.h"
#include <stdexcept>

#include "retruxx/common.h"
#include "game/m3dgame.h"
#include "game/uimisc/guihelper.h"
#include "impulses/i_impulses.h"
#include "server/objects/base/objcontainer.h"
#include "server/objects/dynamicquest.h"
#include "server/server.h"
#include <cstring>

FormatParam::ParamType FormatParam::GetType() const
{
    // RVA 0x583050
    return m_type;
}

CStr FormatParam::GetAsTString() const
{
    // RVA 0x5830A0 - the display ("T") form of a string parameter.
    if (m_type == FP_STRING)
    {
        return m_str;
    }
    return CStr();
}

int FormatParam::GetAsNum() const
{
    // RVA 0x583060
    return m_type == FP_NUMERIC ? m_num : -1;
}

FormatParam::FormatParam(FormatParam const& rhs) :
    m_type(rhs.m_type),
    m_num(rhs.m_num),
    m_str(rhs.m_str)
{
    // Inlined in the binary: a plain member-wise copy.
}

FormatParam::FormatParam(CStr const& str) :
    m_type(FP_STRING),
    m_num(0),
    m_str(str)
{
    // Inlined in the binary (see StringParser::FormatStringForDynamicQuest).
}

FormatParam::FormatParam(int num) :
    m_type(FP_NUMERIC),
    m_num(num)
{
    // Inlined in the binary (see StringParser::FormatStringForDynamicQuest).
}

FormatParam::FormatParam(StrW const& str) :
    m_type(FP_STRING),
    m_num(0),
    m_str(str.ToStr(0))
{
    // Never emitted in the binary; the wide string is stored converted.
}

CStr FormatParam::GetAsString() const
{
    // RVA 0x583070
    if (m_type == FP_STRING)
    {
        return m_str;
    }
    return CStr();
}

StringParser::StringParser()
{
}

StringParser::~StringParser()
{
}

void StringParser::FormatString(CStr& outStr, std::vector<FormatParam> const& params) const
{
    // RVA 0x581F90 - replaces "%<n><t>" (n is the 1-based parameter index) with
    // parameter n rendered according to the type letter t. Only the first
    // occurrence of each index is replaced.
    if (outStr.empty() || params.empty())
    {
        return;
    }

    for (size_t i = 0; i < params.size(); ++i)
    {
        CStr const paramPoint = CStr("%") + CStr(static_cast<int>(i + 1));
        char const* found = strstr(outStr.c_str(), paramPoint.c_str());
        if (!found)
        {
            continue;
        }
        size_t const pos = found - outStr.c_str();

        CStr const leftPart = outStr.substr(0, pos);
        CStr const rightPart = outStr.substr(pos + 3, static_cast<size_t>(-1));
        char const type = outStr.substr(pos + 2, 1).c_str()[0];
        CStr toInsert = outStr.substr(pos, 3);

        auto const& param = params[i];
        auto const mismatch = [&param]() {
            return param.GetType() == FormatParam::FP_NUMERIC
                ? CStr("PARAM_MISMATCH_") + CStr(param.GetAsNum())
                : CStr("PARAM_MISMATCH_") + param.GetAsTString();
        };

        switch (type)
        {
        case 'b': // string id, looked up
            if (param.GetType() == FormatParam::FP_NUMERIC)
            {
                toInsert = mismatch();
            }
            else if (param.GetType() == FormatParam::FP_STRING)
            {
                toInsert = M3D_APP->GetStringByStringId0(param.GetAsString());
                if (toInsert.empty())
                {
                    toInsert = CStr("VALUE_NOT_FOUND_") + param.GetAsTString();
                }
            }
            break;
        case 'd': // number
            if (param.GetType() == FormatParam::FP_NUMERIC)
            {
                toInsert = CStr(param.GetAsNum());
            }
            else if (param.GetType() == FormatParam::FP_STRING)
            {
                toInsert = mismatch();
            }
            break;
        case 'i': // prototype id -> full name
            if (param.GetType() == FormatParam::FP_NUMERIC)
            {
                CStr const name = ai::pServer->GetPrototypeFullName(param.GetAsNum());
                // NOTE: the original adds the number to the literal's address
                // instead of appending it, as in the original.
                toInsert = name.empty() ? CStr("VALUE_NOT_FOUND_" + param.GetAsNum()) : name;
            }
            else if (param.GetType() == FormatParam::FP_STRING)
            {
                toInsert = mismatch();
            }
            break;
        case 'l': // string id, looked up, no fallback
            if (param.GetType() == FormatParam::FP_NUMERIC)
            {
                toInsert = mismatch();
            }
            else if (param.GetType() == FormatParam::FP_STRING)
            {
                toInsert = M3D_APP->GetStringByStringId0(param.GetAsString());
            }
            break;
        case 'm': // impulse name -> bound keys
            if (param.GetType() == FormatParam::FP_NUMERIC)
            {
                toInsert = mismatch();
            }
            else if (param.GetType() == FormatParam::FP_STRING)
            {
                CStr const keys =
                    help::GetKeysForImpulse(M3D_APP->m_pImpulses->GetImpulseIdByName(param.GetAsString()));
                // NOTE: pointer arithmetic on the literal as in the original; for
                // a string parameter GetAsNum() is -1, pointing before the text.
                toInsert = keys.empty() ? CStr("VALUE_NOT_FOUND_" + param.GetAsNum()) : keys;
            }
            break;
        case 'n': // object name -> full name
            if (param.GetType() == FormatParam::FP_NUMERIC)
            {
                toInsert = mismatch();
            }
            else if (param.GetType() == FormatParam::FP_STRING)
            {
                toInsert = ai::theObjects->GetObjectFullName(param.GetAsString());
                if (toInsert.empty())
                {
                    toInsert = CStr("VALUE_NOT_FOUND_") + param.GetAsTString();
                }
            }
            break;
        case 'o': // object id -> full name
            if (param.GetType() == FormatParam::FP_NUMERIC)
            {
                auto* obj = ai::theObjects->GetEntityByObjId(param.GetAsNum());
                if (!obj)
                {
                    toInsert = CStr("OBJECT_NOT_FOUND_") + CStr(param.GetAsNum());
                }
                else
                {
                    toInsert = ai::theObjects->GetObjectFullName(CStr(obj->GetName()));
                    if (toInsert.empty())
                    {
                        // NOTE: the original assigns the object's name over the
                        // "VALUE_NOT_FOUND_" temporary, so the bare name is used.
                        toInsert = CStr(obj->GetName());
                    }
                }
            }
            else if (param.GetType() == FormatParam::FP_STRING)
            {
                toInsert = mismatch();
            }
            break;
        case 'p': // prototype name -> full name
            if (param.GetType() == FormatParam::FP_NUMERIC)
            {
                toInsert = mismatch();
            }
            else if (param.GetType() == FormatParam::FP_STRING)
            {
                CStr const fullname = ai::pServer->GetPrototypeFullName(param.GetAsString());
                toInsert = fullname.empty() ? CStr("VALUE_NOT_FOUND_") + param.GetAsTString() : fullname;
            }
            break;
        case 's': // plain string
            if (param.GetType() == FormatParam::FP_NUMERIC)
            {
                toInsert = mismatch();
            }
            else if (param.GetType() == FormatParam::FP_STRING)
            {
                toInsert = param.GetAsTString();
            }
            break;
        default:
            break;
        }

        outStr = leftPart + toInsert + rightPart;
    }
}

void StringParser::FormatStringForDynamicQuest(CStr& patternString, ai::DynamicQuest const* dQuest) const
{
    // RVA 0x582D90 - %1 hirer, %2 target, %3 reward.
    if (!dQuest)
    {
        return;
    }
    std::vector<FormatParam> params;
    params.push_back(FormatParam(CStr(dQuest->GetHirerName())));
    params.push_back(FormatParam(CStr(dQuest->GetTargetName())));
    params.push_back(FormatParam(dQuest->GetReward()));
    FormatString(patternString, params);
}
