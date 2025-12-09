#pragma once
#include <core/stringw.h>
#include <vector>

namespace ai
{
    class DynamicQuest;
}

class FormatParam
{
public:
    enum ParamType
    {
        FP_NUMERIC = 0x0,
        FP_STRING = 0x1,
    };

public:
    ParamType GetType() const;
    CStr GetAsTString() const;
    int GetAsNum() const;
    FormatParam(FormatParam const&);
    FormatParam(CStr const&);
    FormatParam(int);
    FormatParam(StrW const&);
    CStr GetAsString() const;

private:
    ParamType m_type;
    int m_num;
    CStr m_str;
};

class StringParser
{
public:
    StringParser();
    ~StringParser();
    void FormatString(CStr &,std::vector<FormatParam> const &) const ;
    void FormatStringForDynamicQuest(CStr &, ai::DynamicQuest const *) const ;
};
