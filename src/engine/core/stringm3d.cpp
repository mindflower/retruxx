#include <core/stringm3d.h>
#include <windows.h>

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
