#pragma once
#include <math/quaternion.h>
#include <math/vector2.h>
#include <cstdint>

namespace m3d
{
    namespace fs
    {
        class IStream;
    }
}

constexpr auto CStr_npos{ -1 };
//string class with restricted fields for dlls
class CStr
{
    class ZeroCharHolder
    {
    private:
        /* 0x0000 */ char m_zeroChar = 0;

    public:
        ZeroCharHolder();
        operator char*();
    }; /* size: 0x0001 */
private:
    /* 0x0000 */ char* m_charPtr = nullptr;
    /* 0x0004 */ int m_allocSz = 0;
    /* 0x0008 */ CStr::ZeroCharHolder ZERO;
    void cleanup();
    void realloc(int sz);

public:
    static int __fastcall my_strcmp(const char* lhs, const char* rhs);
    static int __fastcall my_stricmp(const char* lhs, const char* rhs);
    CStr(const Quaternion& q);
    CStr(const CVector2& v);
    CStr(const CVector& v);
    CStr(double);
    CStr(float v);
    CStr(uint64_t);
    CStr(int64_t v);
    CStr(unsigned long v);
    CStr(unsigned int v);
    CStr(int v);
    CStr(char v);
    CStr(const char* str, int num);
    CStr(const char* str);
    CStr(char c, int repeat);
    CStr(const CStr& s);
    CStr();
    ~CStr();
    int length() const;
    bool empty() const;
    void erase();
    char& operator[](int i);
    const char& operator[](int i) const;
    const char* c_str() const;
    CStr& operator+=(const CStr& a);
    unsigned int getHashCode();
    void toLower(unsigned long locale = -1);
    void toUpper(unsigned long);
    void FirstCharToLower(unsigned long locale);
    void FirstCharToUpper(unsigned long locale);
    int findOneOf(const char* str, int startIdx = 0) const;
    int find(char c, int startIdx = 0) const;
    int rfind(char c) const;
    CStr substr(int pos, int npos = CStr_npos) const;
    int findsubstr(const char* substr, int nStart = 0) const;
    int del(int idx, int count);
    static int __cdecl format(const char* fmt, ...);
    static CStr __cdecl format_(const char* fmt, ...);
    int Write(m3d::fs::IStream&);
    int Read(m3d::fs::IStream&);
    CStr& CStr::operator=(CStr const& rhs);
}; /* size: 0x000c */

void UnifyFileName(CStr& fileName);
void UnifyFileName0(CStr& fileName);
CStr DirectoryFromFileName(CStr const& source);
CStr NameFromFileName(CStr const& source);
CVector2 strToVec2(CStr const&);
CVector strToVec(CStr const&);
Quaternion strToQuat(CStr const&);

bool operator==(CStr const& lhs, CStr const& rhs);
bool operator!=(CStr const& lhs, CStr const& rhs);
bool operator<(CStr const& lhs, CStr const& rhs);
bool operator>(CStr const& lhs, CStr const& rhs);
CStr operator+(CStr const& lhs, CStr const& rhs);

unsigned int strToColor(CStr const& str, unsigned def);
int strToInt(CStr const& str);