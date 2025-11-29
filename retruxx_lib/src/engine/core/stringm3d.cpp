#include <cassert>
#include <stdexcept>
#include <core/stringm3d.h>
#include <windows.h>

#include "math/vector.h"
#include "retruxx/common.h"

void UnifyFileName(CStr& fileName)
{
    for (size_t i = 0; i < fileName.length(); ++i)
    {
        if (fileName[i] == '/')
        {
            fileName[i] = '\\';
        }
    }
    fileName.toLower();
}

void UnifyFileName0(CStr& fileName)
{
    for (size_t i = 0; i < fileName.length(); ++i)
    {
        if (fileName[i] == '\\')
        {
            fileName[i] = '/';
        }
    }
    fileName.toLower();
}

CStr DirectoryFromFileName(CStr const& source)
{
    auto result = source;
    UnifyFileName0(result);
    auto pos = result.rfind('/');
    if (pos != CStr_npos)
    {
        //TODO: check this
        return result.substr(0, pos);
    }
    return result;
}

CStr NameFromFileName(CStr const& source)
{
    auto result = source;
    UnifyFileName0(result);
    auto pos = result.rfind('/');
    if (pos != CStr_npos)
    {
        //TODO: check this
        return result.substr(pos+1);
    }
    return result;
}

CVector2 strToVec2(CStr const& str)
{
    CVector2 res;
    if (!str.empty())
    {
        sscanf(str.c_str(), "%f %f", &res.x, &res.y);
    }
    return res;
}

CVector strToVec(CStr const& str)
{
    CVector res;
    if (!str.empty())
    {
        sscanf(str.c_str(), "%f %f %f", &res.x, &res.y, &res.z);
    }
    return res;
}

Quaternion strToQuat(CStr const& str)
{
    Quaternion res;
    if (!str.empty())
    {
        sscanf(str.c_str(), "%f %f %f %f", &res.x, &res.y, &res.z, &res.w);
    }
    return res;
}

bool strToBool(CStr const& str)
{
    int v = 0;
    if (str.empty())
        return 0;
    sscanf(str.c_str(), "%d", &v);
    return v != 0;
}

CStr::ZeroCharHolder::ZeroCharHolder() :
    m_zeroChar(0)
{
}

CStr::ZeroCharHolder::operator char*()
{
    return &m_zeroChar;
}

void CStr::cleanup()
{
    if (m_charPtr != ZERO)
    {
        delete[] m_charPtr;
        m_charPtr = ZERO;
        m_allocSz = 0;
    }
}

void CStr::realloc(int sz)
{
    //TODO: check this
    auto size = 32 * ((sz + 31) / 32);
    if (size > m_allocSz || m_charPtr == ZERO)
    {
        cleanup();
        m_charPtr = new char[size];
        m_allocSz = size;
        m_charPtr[0] = '\0';
    }
    else
    {
        m_charPtr[0] = '\0';
    }
}

int CStr::my_strcmp(char const* lhs, char const* rhs)
{
    auto result = 0;
    if (lhs)
    {
        if (rhs)
        {
            result = strcmp(lhs, rhs);
        }
        else
        {
            result = *lhs != 0;
        }
    }
    else if (rhs)
    {
        result = -(*rhs != 0);
    }
    return result;
}

int CStr::my_stricmp(char const* lhs, char const* rhs)
{
    auto result = 0;
    if (lhs)
    {
        if (rhs)
        {
            result = stricmp(lhs, rhs);
        }
        else
        {
            result = *lhs != 0;
        }
    }
    else if (rhs)
    {
        result = -(*rhs != 0);
    }
    return result;
}

CStr::CStr(Quaternion const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

CStr::CStr(CVector2 const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

CStr::CStr(CVector const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

CStr::CStr(double)
{
    RETRUXX_NOT_IMPLEMENTED;
}

CStr::CStr(float)
{
    RETRUXX_NOT_IMPLEMENTED;
}

CStr::CStr(uint64_t)
{
    RETRUXX_NOT_IMPLEMENTED;
}

CStr::CStr(int64_t)
{
    RETRUXX_NOT_IMPLEMENTED;
}

CStr::CStr(unsigned long v)
{
    char buf[136] = { 0 };

    m_charPtr = ZERO;
    m_allocSz = 0;

    sprintf(buf, "%lu", v);
    realloc(strlen(buf) + 1);
    strcpy(m_charPtr, buf);
}

CStr::CStr(unsigned int v)
{
    char buf[136] = { 0 };

    m_charPtr = ZERO;
    m_allocSz = 0;

    sprintf(buf, "%u", v);
    realloc(strlen(buf) + 1);
    strcpy(m_charPtr, buf);
}

CStr::CStr(int v)
{
    char buf[136] = { 0 };

    m_charPtr = ZERO;
    m_allocSz = 0;

    sprintf(buf, "%d", v);
    realloc(strlen(buf) + 1);
    strcpy(m_charPtr, buf);
}

CStr::CStr(char)
{
    RETRUXX_NOT_IMPLEMENTED;
}

CStr::CStr(char const* str, int num)
{
    m_charPtr = ZERO;
    m_allocSz = 0;

    if (str)
    {
        realloc(num + 1);
        strncpy(m_charPtr, str, num);
        m_charPtr[num] = '\0';
    }
}

CStr::CStr(char const* str)
{
    m_charPtr = ZERO;
    m_allocSz = 0;
    if (str)
    {
        realloc(strlen(str) + 1);
        strcpy(m_charPtr, str);
    }
}

CStr::CStr(char c, int repeat)
{
    m_charPtr = ZERO;
    m_allocSz = 0;
    if (repeat > 0)
    {
        realloc(repeat + 1);
        memset(m_charPtr, c, repeat);
        m_charPtr[repeat] = '\0';
    }
}

CStr::CStr(CStr const& s)
{
    m_charPtr = ZERO;
    m_allocSz = 0;
    if (const auto len = s.length(); len > 0)
    {
        realloc(len + 1);
        strcpy(m_charPtr, s.c_str());
    }
}

CStr::CStr()
{
    m_charPtr = ZERO;
}

CStr::~CStr()
{
    cleanup();
}

int CStr::length() const
{
    if (m_charPtr)
    {
        return strlen(m_charPtr);
    }
    return 0;
}

bool CStr::empty() const
{
    return length() == 0;
}

void CStr::erase()
{
    if (m_charPtr != ZERO)
    {
        m_charPtr[0] = '\0';
    }
}

char& CStr::operator[](int i)
{
    return m_charPtr[i];
}

char const& CStr::operator[](int i) const
{
    return m_charPtr[i];
}

const char* CStr::c_str() const
{
    return m_charPtr;
}

CStr& CStr::operator+=(CStr const& a)
{
    auto const newSize = length() + a.length() + 1;
    if (newSize == 1)
    {
        cleanup();
        return *this;
    }

    char* newCharPtr = new char[newSize];
    strcpy(newCharPtr, c_str());
    strcat(newCharPtr, a.c_str());
    
    cleanup();

    m_charPtr = newCharPtr;
    m_allocSz = newSize;
    return *this;
}

unsigned int CStr::getHashCode()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void CStr::toLower(unsigned long locale)
{
    if (locale == -1)
    {
        locale = LOCALE_USER_DEFAULT;
    }
    if (!empty())
    {
        ::LCMapStringA(locale, LCMAP_LOWERCASE, m_charPtr, length() + 1, m_charPtr, length() + 1);
    }
}

void CStr::toUpper(unsigned long)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void CStr::FirstCharToLower(unsigned long)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void CStr::FirstCharToUpper(unsigned long)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int CStr::findOneOf(char const* str, int startIdx) const
{
    assert(m_charPtr);
    if (startIdx < 0)
    {
        return -1;
    }

    auto len = length();
    if (startIdx >= len)
    {
        return -1;
    }

    auto result = strcspn(&m_charPtr[startIdx], str);

    auto v7 = this->m_charPtr;
    if (this->m_charPtr)
        v7 = (char*)strlen(v7);

    if ((char*)result == &v7[-startIdx])
        return -1;
    return result;
}

int CStr::find(char c, int startIdx) const
{
    assert(m_charPtr);
    std::string_view const view(m_charPtr);
    auto const res = view.find(c, startIdx);
    return res == std::string_view::npos ? CStr_npos : static_cast<int>(res);
}

int CStr::rfind(char c) const
{
    assert(m_charPtr);
    std::string_view const view(m_charPtr);
    auto const res = view.rfind(c);
    return res == std::string_view::npos ? CStr_npos : static_cast<int>(res);
}

CStr CStr::substr(int pos, int endpos) const
{
    //TODO: check this
    assert(m_charPtr);
    std::string const str(m_charPtr);
    if (endpos == CStr_npos)
    {
        return str.substr(pos).c_str();
    }
    return str.substr(pos, endpos - pos).c_str();
}

int CStr::findsubstr(char const* substr, int offset) const
{
    assert(m_charPtr);
    std::string_view const view(m_charPtr);
    return view.find(substr, offset);
}

int CStr::del(int idx, int count)
{
    // Validate parameters
    assert(idx >= 0 && count > 0);

    if (!m_charPtr)
        return 0;

    int length = strlen(m_charPtr);

    // Validate index
    assert(idx < length);

    // Adjust count if it would exceed string bounds
    if (idx + count > length)
    {
        count = length - idx;
    }

    // Calculate positions
    char* deletionStart = m_charPtr + idx;
    char* copySource = deletionStart + count;

    // Shift characters to overwrite the deleted portion
    memmove(deletionStart, copySource, strlen(copySource) + 1); // +1 for null terminator

    return strlen(m_charPtr);
}

int CStr::format(char const*, ...)
{
    RETRUXX_NOT_IMPLEMENTED;
}

CStr CStr::format_(char const*, ...)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int CStr::Write(m3d::fs::IStream&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int CStr::Read(m3d::fs::IStream&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

CStr& CStr::operator=(CStr const& rhs)
{
    if (this == &rhs)
    {
        return *this;
    }
    if (rhs.length() > 0)
    {
        realloc(rhs.length() + 1);
        strcpy(m_charPtr, rhs.c_str());
    }
    else if (m_charPtr != ZERO)
    {
        cleanup();
    }
    return *this;
}

bool operator==(CStr const& lhs, CStr const& rhs)
{
    return std::string_view(lhs.c_str()) == std::string_view(rhs.c_str());
}

bool operator!=(CStr const& lhs, CStr const& rhs)
{
    return std::string_view(lhs.c_str()) != std::string_view(rhs.c_str());
}

bool operator<(CStr const& lhs, CStr const& rhs)
{
    return std::string_view(lhs.c_str()) < std::string_view(rhs.c_str());
}

bool operator>(CStr const& lhs, CStr const& rhs)
{
    return std::string_view(lhs.c_str()) > std::string_view(rhs.c_str());
}

CStr operator+(CStr const& lhs, CStr const& rhs)
{
    CStr newS(lhs);
    newS += rhs;
    return newS;
}

unsigned int strToColor(CStr const& str, unsigned def)
{
    if (!str.empty())
    {
        int colorArr[4] = { 0 };
        if (sscanf_s(str.c_str(), "%d %d %d %d", &colorArr[0], &colorArr[1], &colorArr[2], &colorArr[3]) == 4)
        {
            for (auto& elem : colorArr)
            {
                if (elem > 255)
                {
                    elem = 255;
                }
                if (elem < 0)
                {
                    elem = 0;
                }
            }
            unsigned int result = colorArr[2] | ((colorArr[1] | ((colorArr[0] | (colorArr[3] << 8)) << 8)) << 8);
            return result;
        }
    }
    return def;
}

int strToInt(CStr const& str)
{
    if (str.empty())
    {
        return 0;
    }
    int v = 0;
    sscanf(str.c_str(), "%d", &v);
    return v;
}

float strToFloat(CStr const& str)
{
    if (str.empty())
    {
        return 0;
    }
    float v = 0.0;
    sscanf(str.c_str(), "%f", &v);
    return v;
}

