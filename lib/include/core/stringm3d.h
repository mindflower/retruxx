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
}  // namespace m3d

auto constexpr CStr_npos{-1};
//string class with restricted fields for dlls
class CStr
{
    class ZeroCharHolder
    {
    private:
        char m_zeroChar = 0;

    public:
        ZeroCharHolder();
        operator char*();
    };

private:
    char* m_charPtr = nullptr;
    int32_t m_allocSz = 0;
    CStr::ZeroCharHolder ZERO;
    void cleanup();
    void realloc(int sz);

public:
    static int __fastcall my_strcmp(char const* lhs, char const* rhs);
    static int __fastcall my_stricmp(char const* lhs, char const* rhs);
    explicit CStr(Quaternion const& q);
    explicit CStr(CVector2 const& v);
    explicit CStr(CVector const& v);
    explicit CStr(double);
    explicit CStr(float v);
    explicit CStr(uint64_t);
    explicit CStr(int64_t v);
    explicit CStr(unsigned long v);
    explicit CStr(unsigned int v);
    explicit CStr(int v);
    explicit CStr(char const* str, int num);
    explicit CStr(char c, int repeat);
    explicit CStr(char v);
    CStr(char const* str);
    CStr(CStr const& s);
    CStr();
    ~CStr();
    int length() const;
    bool empty() const;
    void erase();
    char& operator[](int i);
    char const& operator[](int i) const;
    char const* c_str() const;
    CStr& operator+=(CStr const& a);
    unsigned int getHashCode();
    void toLower(unsigned long locale = -1);
    void toUpper(unsigned long);
    void FirstCharToLower(unsigned long locale);
    void FirstCharToUpper(unsigned long locale);
    int findOneOf(char const* str, int startIdx = 0) const;
    int find(char c, int startIdx = 0) const;
    int rfind(char c) const;
    CStr substr(int pos, int npos = CStr_npos) const;
    int findsubstr(char const* substr, int nStart = 0) const;
    int del(int idx, int count);
    int format(char const* fmt, ...);
    static CStr format_(char const* fmt, ...);
    int Write(m3d::fs::IStream&);
    int Read(m3d::fs::IStream&);
    CStr& CStr::operator=(CStr const& rhs);
};

// Embedded in CVar and read by the original driver DLLs, so the layout is fixed.

// Didnt true for x64
//static_assert(sizeof(CStr) == 0x000c);

void UnifyFileName(CStr& fileName);
void UnifyFileName0(CStr& fileName);
CStr DirectoryFromFileName(CStr const& source);
CStr NameFromFileName(CStr const& source);
CVector2 strToVec2(CStr const&);
CVector strToVec(CStr const&);
Quaternion strToQuat(CStr const&);
bool strToBool(CStr const& str);

bool operator==(CStr const& lhs, CStr const& rhs);
bool operator!=(CStr const& lhs, CStr const& rhs);
bool operator<(CStr const& lhs, CStr const& rhs);
bool operator>(CStr const& lhs, CStr const& rhs);
CStr operator+(CStr const& lhs, CStr const& rhs);

unsigned int strToColor(CStr const& str, unsigned def);
int strToInt(CStr const& str);
float strToFloat(CStr const& str);
CStr vecToStr(CVector const& v);