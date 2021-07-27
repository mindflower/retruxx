#pragma once

class StringParser
{
public:
    StringParser();
    ~StringParser();
    void FormatString(CStr &,class std::vector<class FormatParam,class std::allocator<class FormatParam> > const &) const ;
    void FormatStringForDynamicQuest(CStr &,class ai::DynamicQuest const *) const ;
protected:
private:
};
