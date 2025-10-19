#include "externalpaths.h"

namespace ai
{
    bool ExternalPaths::Load(const CStr& fileName)
    {
        // TODO: implement ExternalPaths::Load
        //throw std::logic_error("Not implemented");
        return false;
    }

    bool ExternalPaths::Save(const CStr& filename)
    {
        throw std::logic_error("Not implemented");
    }

    const retruxx::vector<CVector2, retruxx::allocator<CVector2>>& ExternalPaths::GetPath(const CStr& pathName) const
    {
        // TODO: implement ExternalPaths::GetPath
        return m_emptyPath;
        throw std::logic_error("Not implemented");
    }

    bool ExternalPaths::SetPath(const CStr& pathName,
        const retruxx::vector<CVector2, retruxx::allocator<CVector2>>& thePath)
    {
        throw std::logic_error("Not implemented");
    }

    bool ExternalPaths::AddPath(const CStr& pathName,
        const retruxx::vector<CVector2, retruxx::allocator<CVector2>>& thePath)
    {
        throw std::logic_error("Not implemented");
    }

    bool ExternalPaths::DelPath(const CStr& pathName)
    {
        throw std::logic_error("Not implemented");
    }

    const retruxx::vector<CStr, retruxx::allocator<CStr>>& ExternalPaths::GetPathNames() const
    {
        throw std::logic_error("Not implemented");
    }
}
