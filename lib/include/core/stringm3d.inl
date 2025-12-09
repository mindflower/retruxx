#pragma once

class CStr
{
public:
    CStr(__int64);
    CStr(char const *);
    CStr(char);
    CStr(struct CVector const &);
    CStr(struct CVector2 const &);
    CStr(float);
    CStr(class CStr const &);
    CStr(char,int);
    CStr(char const *,int);
    CStr(struct Quaternion const &);
    CStr(unsigned int);
    CStr(int);
    CStr(unsigned long);
    CStr();
    int length() const ;
    void FirstCharToUpper(unsigned long);
    void erase();
    class CStr & operator+=(class CStr const &);
    ~CStr();
    int findOneOf(char const *,int) const ;
    int del(int,int);
    int rfind(char) const ;
    char const * c_str() const ;
    char const & operator[](int) const ;
    char & operator[](int);
    bool ai::Obj::LessNoCaseoperator()(class CStr const &,class CStr const &) const ;
    int findsubstr(char const *,int) const ;
    bool empty() const ;
    static int __fastcall my_stricmp(char const *,char const *);
    static int __fastcall my_strcmp(char const *,char const *);
    static class CStr __cdecl format_(char const *,...);
    int __cdecl format(char const *,...);
    void FirstCharToLower(unsigned long);
    class CStr substr(int,int) const ;
    void toLower(unsigned long);
    class CStr & operator=(class CStr const &);
    int find(char,int) const ;
protected:
private:
    void cleanup();
    void realloc(int);
    char *m_charPtr;
    int m_allocSz;
    CStr::ZeroCharHolder ZERO;
};
