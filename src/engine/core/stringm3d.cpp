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
    if (!fileName.empty())
    {
        ::LCMapStringA(0x400, 0x100, fileName.data(), fileName.size(), fileName.data(), fileName.size());
    }
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
