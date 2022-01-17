#pragma once
#include <math/quaternion.h>
#include <math/vector2.h>

class CStr2;
using CStr = CStr2;

void UnifyFileName(CStr& fileName);
void UnifyFileName0(CStr& fileName);
CStr DirectoryFromFileName(CStr const& source);
CStr NameFromFileName(CStr const& source);
CVector2 strToVec2(CStr const&);

namespace m3d
{
    namespace fs
    {
        class IStream;
    }
}

constexpr auto CStr_npos{-1};
//string class with restricted fields for dlls
class CStr2
{
public:
    class ZeroCharHolder
    {
        char m_zeroChar = 0;

    public:
        ZeroCharHolder();
        operator char* ();
       
    };

public:
    char* m_charPtr = nullptr;
    int m_allocSz = 0;
    ZeroCharHolder ZERO;
    //static constexpr auto npos{ -1 };

private:
    void cleanup();
    void realloc(int);

public:
    static int my_strcmp(char const*, char const*);
    static int my_stricmp(char const*, char const*);

public:
    explicit CStr2(Quaternion const&);
    explicit CStr2(CVector2 const&);
    explicit CStr2(CVector const&);
    explicit CStr2(double);
    explicit CStr2(float);
    explicit CStr2(unsigned long long);
    explicit CStr2(long long);
    explicit CStr2(unsigned long);
    explicit CStr2(unsigned int);
    explicit CStr2(int);
    explicit CStr2(unsigned char);
    explicit CStr2(char const*, int);
    CStr2(char const*);
    explicit CStr2(char, int);
    CStr2(CStr2 const&);
    CStr2();

    ~CStr2();

    int length() const;
    bool empty() const;
    void erase();

    char& operator[](int);
    char const& operator[](int) const;

    const char* c_str() const;
    CStr2& operator+=(CStr2 const&);
    char* getHashCode();            //TODO: what the heck
    void toLower(unsigned long locale = -1);
    void toUpper(unsigned long);
    void FirstCharToLower(unsigned long);
    void FirstCharToUpper(unsigned long);
    int findOneOf(char const*, int) const;
    int find(char c, int startIdx = 0) const;
    int rfind(char) const;
    CStr2 substr(int pos, int endpos = CStr_npos) const;
    int findsubstr(char const*, int offset = 0) const;
    int del(int, int);
    int format(char const*, ...);
    static CStr2 format_(char const*, ...);
    int Write(m3d::fs::IStream&);
    int Read(m3d::fs::IStream&);

    CStr& operator=(CStr const& rhs);
    //bool operator==(CStr2 const&) const;
    //CStr2& operator+(CStr2 const&);
    //CStr2& operator+(int);
};

bool operator==(CStr const& lhs, CStr const& rhs);
bool operator!=(CStr const& lhs, CStr const& rhs);
bool operator<(CStr const& lhs, CStr const& rhs);
bool operator>(CStr const& lhs, CStr const& rhs);
CStr operator+(CStr lhs, CStr const& rhs);