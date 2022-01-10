#include <stdexcept>
#include <core/stringm3d.h>
#include <windows.h>

void UnifyFileName(CStr& fileName)
{
    for (auto& c : fileName)
    {
        if (c == '/')
        {
            c = '\\';
        }
    }
    toLower(fileName);
}

void UnifyFileName0(CStr& fileName)
{
    for (auto& c : fileName)
    {
        if (c == '\\')
        {
            c = '/';
        }
    }
    toLower(fileName);
}

CStr DirectoryFromFileName(CStr const& source)
{
    auto result = source;
    UnifyFileName0(result);
    auto pos = result.rfind('/');
    if (pos != CStr::npos)
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
    if (pos != CStr::npos)
    {
        //TODO: check this
        return result.substr(pos+1);
    }
    return result;
}

void toLower(CStr& str)
{
    if (!str.empty())
    {
        ::LCMapStringA(LOCALE_USER_DEFAULT, LCMAP_LOWERCASE, str.data(), str.size() + 1, str.data(), str.size() + 1);
    }
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
    m_allocSz = 0;
}

void CStr2::realloc(int sz)
{
    throw std::logic_error("Not implemented");
}

int CStr2::my_strcmp(char const*, char const*)
{
    throw std::logic_error("Not implemented");
}

int CStr2::my_stricmp(char const*, char const*)
{
    throw std::logic_error("Not implemented");
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

CStr2::CStr2(unsigned long)
{
    throw std::logic_error("Not implemented");
}

CStr2::CStr2(unsigned)
{
    throw std::logic_error("Not implemented");
}

CStr2::CStr2(int)
{
    throw std::logic_error("Not implemented");
}

CStr2::CStr2(unsigned char)
{
    throw std::logic_error("Not implemented");
}

CStr2::CStr2(char const*, int)
{
    throw std::logic_error("Not implemented");
}

CStr2::CStr2(char const*)
{
    throw std::logic_error("Not implemented");
}

CStr2::CStr2(char, int)
{
    throw std::logic_error("Not implemented");
}

CStr2::CStr2(CStr2 const&)
{
    throw std::logic_error("Not implemented");
}

CStr2::CStr2()
{
    throw std::logic_error("Not implemented");
}

CStr2::~CStr2()
{
    throw std::logic_error("Not implemented");
}

int CStr2::length() const
{
    throw std::logic_error("Not implemented");
}

bool CStr2::empty() const
{
    throw std::logic_error("Not implemented");
}

void CStr2::erase()
{
    throw std::logic_error("Not implemented");
}

char& CStr2::operator[](int)
{
    throw std::logic_error("Not implemented");
}

char const& CStr2::operator[](int) const
{
    throw std::logic_error("Not implemented");
}

char const* CStr2::c_str() const
{
    throw std::logic_error("Not implemented");
}

CStr& CStr2::operator+=(CStr const&)
{
    throw std::logic_error("Not implemented");
}

char* CStr2::getHashCode()
{
    throw std::logic_error("Not implemented");
}

void CStr2::toLower(unsigned long)
{
    throw std::logic_error("Not implemented");
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

int CStr2::find(char, int) const
{
    throw std::logic_error("Not implemented");
}

int CStr2::rfind(char) const
{
    throw std::logic_error("Not implemented");
}

CStr2 CStr2::substr(int, int) const
{
    throw std::logic_error("Not implemented");
}

int CStr2::findsubstr(char const*, int) const
{
    throw std::logic_error("Not implemented");
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
