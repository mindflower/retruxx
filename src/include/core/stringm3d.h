#pragma once

//TODO: add static functions
class CVector;
class CVector2;
class Quaternion;

class CStr
{
public:
    class ZeroCharHolder
    {
    public:
        ZeroCharHolder();
        operator char* ();

    private:
        char m_zeroChar;
    };

public:
    static int my_stricmp(char const*, char const*);
    static int my_strcmp(char const*, char const*);
    static CStr format_(char const*, ...);

public:
    CStr(__int64);
    CStr(char const*);
    CStr(char);
    CStr(CVector const&);
    CStr(CVector2 const&);
    CStr(float);
    CStr(CStr const&);
    CStr(char, int);
    CStr(char const*, int);
    CStr(Quaternion const&);
    CStr(unsigned int);
    CStr(int);
    CStr(unsigned long);
    CStr();
    ~CStr();

    int length() const;
    void FirstCharToUpper(unsigned long);
    void erase();
    CStr& operator+=(CStr const&);
    int findOneOf(char const*, int) const;
    int del(int, int);
    int rfind(char) const;
    char const* c_str() const;
    char const& operator[](int) const;
    char& operator[](int);
    //bool ai::Obj::LessNoCaseoperator()(CStr const&, CStr const&) const;
    int findsubstr(char const*, int) const;
    bool empty() const;
    int __cdecl format(char const*, ...);
    void FirstCharToLower(unsigned long);
    CStr substr(int, int) const;
    void toLower(unsigned long);
    CStr& operator=(CStr const&);
    int find(char, int) const;

private:
    void cleanup();
    void realloc(int);

private:
    char* m_charPtr;
    int m_allocSz;
    ZeroCharHolder ZERO;
};
