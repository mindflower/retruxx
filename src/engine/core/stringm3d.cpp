#include <cassert>
#include <stdexcept>
#include <core/stringm3d.h>
#include <windows.h>

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

CStr2::ZeroCharHolder::ZeroCharHolder() :
    m_zeroChar(0)
{
}

CStr2::ZeroCharHolder::operator char*()
{
    return &m_zeroChar;
}

void CStr2::cleanup()
{
    delete[] m_charPtr;
    m_charPtr = nullptr;
    m_allocSz = 0;
}

void CStr2::realloc(int sz)
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

int CStr2::my_strcmp(char const* lhs, char const* rhs)
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

int CStr2::my_stricmp(char const* lhs, char const* rhs)
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

CStr2::CStr2(Quaternion const&)
{
    throw std::logic_error("Not implemented");
}

CStr2::CStr2(CVector2 const&)
{
    throw std::logic_error("Not implemented");
}

CStr2::CStr2(CVector const&)
{
    throw std::logic_error("Not implemented");
}

CStr2::CStr2(double)
{
    throw std::logic_error("Not implemented");
}

CStr2::CStr2(float)
{
    throw std::logic_error("Not implemented");
}

CStr2::CStr2(unsigned long long)
{
    throw std::logic_error("Not implemented");
}

CStr2::CStr2(long long)
{
    throw std::logic_error("Not implemented");
}

CStr2::CStr2(unsigned long v)
{
    char buf[136] = { 0 };
    sprintf(buf, "%lu", v);
    realloc(strlen(buf) + 1);
    strcpy(m_charPtr, buf);
}

CStr2::CStr2(unsigned v)
{
    char buf[136] = { 0 };
    sprintf(buf, "%u", v);
    realloc(strlen(buf) + 1);
    strcpy(m_charPtr, buf);
}

CStr2::CStr2(int v)
{
    char buf[136] = { 0 };
    sprintf(buf, "%d", v);
    realloc(strlen(buf) + 1);
    strcpy(m_charPtr, buf);
}

CStr2::CStr2(unsigned char)
{
    throw std::logic_error("Not implemented");
}

CStr2::CStr2(char const*, int)
{
    throw std::logic_error("Not implemented");
}

CStr2::CStr2(char const* str)
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

CStr2::CStr2(char c, int repeat)
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

CStr2::CStr2(CStr2 const& s)
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

CStr2::CStr2()
{
    realloc(1);
    m_charPtr[0] = '\0';
}

CStr2::~CStr2()
{
    delete[] m_charPtr;
    m_charPtr = nullptr;
}

int CStr2::length() const
{
    if (m_charPtr)
    {
        return strlen(m_charPtr);
    }
    return 0;
}

bool CStr2::empty() const
{
    return length() == 0;
}

void CStr2::erase()
{
    throw std::logic_error("Not implemented");
}

char& CStr2::operator[](int i)
{
    return m_charPtr[i];
}

char const& CStr2::operator[](int i) const
{
    return m_charPtr[i];
}

const char* CStr2::c_str() const
{
    return m_charPtr;
}

CStr& CStr2::operator+=(CStr const& a)
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

char* CStr2::getHashCode()
{
    throw std::logic_error("Not implemented");
}

void CStr2::toLower(unsigned long locale)
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

void CStr2::toUpper(unsigned long)
{
    throw std::logic_error("Not implemented");
}

void CStr2::FirstCharToLower(unsigned long)
{
    throw std::logic_error("Not implemented");
}

void CStr2::FirstCharToUpper(unsigned long)
{
    throw std::logic_error("Not implemented");
}

int CStr2::findOneOf(char const*, int) const
{
    throw std::logic_error("Not implemented");
}

int CStr2::find(char c, int startIdx) const
{
    assert(m_charPtr);
    std::string_view const view(m_charPtr);
    auto const res = view.find(c, startIdx);
    return res == std::string_view::npos ? CStr_npos : static_cast<int>(res);
}

int CStr2::rfind(char c) const
{
    assert(m_charPtr);
    std::string_view const view(m_charPtr);
    auto const res = view.rfind(c);
    return res == std::string_view::npos ? CStr_npos : static_cast<int>(res);
}

CStr2 CStr2::substr(int pos, int endpos) const
{
    //TODO: check this
    assert(m_charPtr);
    std::string_view const view(m_charPtr);
    if (endpos == CStr_npos)
    {
        return view.substr(pos).data();
    }
    return view.substr(pos, endpos - pos).data();
}

int CStr2::findsubstr(char const* substr, int offset) const
{
    assert(m_charPtr);
    std::string_view const view(m_charPtr);
    return view.find(substr, offset);
}

int CStr2::del(int, int)
{
    throw std::logic_error("Not implemented");
}

int CStr2::format(char const*, ...)
{
    throw std::logic_error("Not implemented");
}

CStr2 CStr2::format_(char const*, ...)
{
    throw std::logic_error("Not implemented");
}

int CStr2::Write(m3d::fs::IStream&)
{
    throw std::logic_error("Not implemented");
}

int CStr2::Read(m3d::fs::IStream&)
{
    throw std::logic_error("Not implemented");
}

CStr& CStr2::operator=(CStr const& rhs)
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



