#include "stringparser.h"
#include <stdexcept>

FormatParam::ParamType FormatParam::GetType() const
{
    throw std::logic_error("Not implemented");
}

CStr FormatParam::GetAsTString() const
{
    throw std::logic_error("Not implemented");
}

int FormatParam::GetAsNum() const
{
    throw std::logic_error("Not implemented");
}

FormatParam::FormatParam(FormatParam const&)
{
    throw std::logic_error("Not implemented");
}

FormatParam::FormatParam(CStr const&)
{
    throw std::logic_error("Not implemented");
}

FormatParam::FormatParam(int)
{
    throw std::logic_error("Not implemented");
}

FormatParam::FormatParam(StrW const&)
{
    throw std::logic_error("Not implemented");
}

CStr FormatParam::GetAsString() const
{
    throw std::logic_error("Not implemented");
}

StringParser::StringParser()
{
    throw std::logic_error("Not implemented");
}

StringParser::~StringParser()
{
    throw std::logic_error("Not implemented");
}

void StringParser::FormatString(CStr&, std::vector<FormatParam> const&) const
{
    throw std::logic_error("Not implemented");
}

void StringParser::FormatStringForDynamicQuest(CStr&, ai::DynamicQuest const*) const
{
    throw std::logic_error("Not implemented");
}
