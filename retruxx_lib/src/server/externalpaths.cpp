#include "externalpaths.h"

namespace ai
{
    bool ExternalPaths::Load(const CStr& fileName)
    {
        // TODO: implement ExternalPaths::Load
        //RETRUXX_NOT_IMPLEMENTED;
        return false;
    }

    bool ExternalPaths::Save(const CStr& filename)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    const retruxx::vector<CVector2, retruxx::allocator<CVector2>>& ExternalPaths::GetPath(const CStr& pathName) const
    {
        // TODO: implement ExternalPaths::GetPath
        return m_emptyPath;
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool ExternalPaths::SetPath(const CStr& pathName,
        const retruxx::vector<CVector2, retruxx::allocator<CVector2>>& thePath)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool ExternalPaths::AddPath(const CStr& pathName,
        const retruxx::vector<CVector2, retruxx::allocator<CVector2>>& thePath)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool ExternalPaths::DelPath(const CStr& pathName)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    const retruxx::vector<CStr, retruxx::allocator<CStr>>& ExternalPaths::GetPathNames() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}
