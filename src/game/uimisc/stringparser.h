#pragma once

class StringParser
{
public:
    StringParser();
    ~StringParser();
    void FormatString(class CStr &,class std::vector<class FormatParam,class std::allocator<class FormatParam> > const &) const ;
    void FormatStringForDynamicQuest(class CStr &,class ai::DynamicQuest const *) const ;
protected:
private:
};
