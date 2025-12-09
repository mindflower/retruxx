#include "stringparser.h"
#include <stdexcept>

#include "retruxx/common.h"

FormatParam::ParamType FormatParam::GetType() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

CStr FormatParam::GetAsTString() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int FormatParam::GetAsNum() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

FormatParam::FormatParam(FormatParam const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

FormatParam::FormatParam(CStr const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

FormatParam::FormatParam(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

FormatParam::FormatParam(StrW const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

CStr FormatParam::GetAsString() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

StringParser::StringParser()
{
}

StringParser::~StringParser()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void StringParser::FormatString(CStr&, std::vector<FormatParam> const&) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void StringParser::FormatStringForDynamicQuest(CStr&, ai::DynamicQuest const*) const
{
    RETRUXX_NOT_IMPLEMENTED;
}
