#include <cassert>
#include <stdexcept>
#include <core/stringm3d.h>
#include <windows.h>

#include "math/vector.h"

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
    delete[] m_charPtr;
    m_charPtr = nullptr;
    m_allocSz = 0;
}

void CStr::realloc(int sz)
{
    //TODO: check this
    auto size = 32 * ((sz + 31) / 32);
    if (size > m_allocSz || m_charPtr == nullptr)
    {
        delete[] m_charPtr;
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
    throw std::logic_error("Not implemented");
}

CStr::CStr(CVector2 const&)
{
    throw std::logic_error("Not implemented");
}

CStr::CStr(CVector const&)
{
    throw std::logic_error("Not implemented");
}

CStr::CStr(double)
{
    throw std::logic_error("Not implemented");
}

CStr::CStr(float)
{
    throw std::logic_error("Not implemented");
}

CStr::CStr(uint64_t)
{
    throw std::logic_error("Not implemented");
}

CStr::CStr(int64_t)
{
    throw std::logic_error("Not implemented");
}

CStr::CStr(unsigned long v)
{
    char buf[136] = { 0 };
    sprintf(buf, "%lu", v);
    realloc(strlen(buf) + 1);
    strcpy(m_charPtr, buf);
}

CStr::CStr(unsigned int v)
{
    char buf[136] = { 0 };
    sprintf(buf, "%u", v);
    realloc(strlen(buf) + 1);
    strcpy(m_charPtr, buf);
}

CStr::CStr(int v)
{
    char buf[136] = { 0 };
    sprintf(buf, "%d", v);
    realloc(strlen(buf) + 1);
    strcpy(m_charPtr, buf);
}

CStr::CStr(char)
{
    throw std::logic_error("Not implemented");
}

CStr::CStr(char const*, int)
{
    throw std::logic_error("Not implemented");
}

CStr::CStr(char const* str)
{
    if (str)
    {
        auto const len = strlen(str);
        realloc(len + 1);
        strcpy(m_charPtr, str);
    }
    else
    {
        realloc(1);
        m_charPtr[0] = '\0';
    }
}

CStr::CStr(char c, int repeat)
{
    if (repeat > 0)
    {
        realloc(repeat + 1);
        memset(m_charPtr, c, repeat);
        m_charPtr[repeat] = '\0';
    }
    else
    {
        realloc(1);
        m_charPtr[0] = '\0';
    }
}

CStr::CStr(CStr const& s)
{
    if (this == &s)
    {
        return;
    }
    if (s.length() > 0)
    {
        realloc(s.length() + 1);
        strcpy(m_charPtr, s.c_str());
    }
    else
    {
        realloc(1);
        m_charPtr[0] = '\0';
    }
}

CStr::CStr()
{
    realloc(1);
    m_charPtr[0] = '\0';
}

CStr::~CStr()
{
    delete[] m_charPtr;
    m_charPtr = nullptr;
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
    if (m_charPtr)
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
    char* newCharPtr = new char[newSize];
    strcpy(newCharPtr, c_str());
    strcat(newCharPtr, a.c_str());
    delete[] m_charPtr;
    m_charPtr = newCharPtr;
    m_allocSz = newSize;
    return *this;
}

unsigned int CStr::getHashCode()
{
    throw std::logic_error("Not implemented");
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
    throw std::logic_error("Not implemented");
}

void CStr::FirstCharToLower(unsigned long)
{
    throw std::logic_error("Not implemented");
}

void CStr::FirstCharToUpper(unsigned long)
{
    throw std::logic_error("Not implemented");
}

int CStr::findOneOf(char const*, int) const
{
    throw std::logic_error("Not implemented");
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

int CStr::del(int, int)
{
    throw std::logic_error("Not implemented");
}

int CStr::format(char const*, ...)
{
    throw std::logic_error("Not implemented");
}

CStr CStr::format_(char const*, ...)
{
    throw std::logic_error("Not implemented");
}

int CStr::Write(m3d::fs::IStream&)
{
    throw std::logic_error("Not implemented");
}

int CStr::Read(m3d::fs::IStream&)
{
    throw std::logic_error("Not implemented");
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
    else
    {
        realloc(1);
        m_charPtr[0] = '\0';
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

CStr operator+(CStr lhs, CStr const& rhs)
{
    lhs += rhs;
    return lhs;
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

