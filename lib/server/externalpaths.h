#pragma once
#include "retruxx/common.h"
#include <core/stringm3d.h>

namespace ai
{
    class ExternalPaths
    {
    public:
        bool Load(CStr const& fileName);
        bool Save(CStr const& filename);
        retruxx::vector<CVector2, retruxx::allocator<CVector2>> const& GetPath(CStr const& pathName) const;
        bool SetPath(CStr const& pathName, retruxx::vector<CVector2, retruxx::allocator<CVector2>> const& thePath);
        bool AddPath(CStr const& pathName, retruxx::vector<CVector2, retruxx::allocator<CVector2>> const& thePath);
        bool DelPath(CStr const& pathName);
        retruxx::vector<CStr, retruxx::allocator<CStr>> const& GetPathNames() const;

        using PathMap = retruxx::map<
            CStr,
            retruxx::vector<CVector2, retruxx::allocator<CVector2>>,
            retruxx::less<CStr>,
            retruxx::allocator<retruxx::pair<CStr const, retruxx::vector<CVector2, retruxx::allocator<CVector2>>>>>;

    private:
        retruxx::map<
            CStr,
            retruxx::vector<CVector2, retruxx::allocator<CVector2>>,
            retruxx::less<CStr>,
            retruxx::allocator<retruxx::pair<CStr const, retruxx::vector<CVector2, retruxx::allocator<CVector2>>>>>
            m_paths;
        retruxx::vector<CVector2, retruxx::allocator<CVector2>> m_emptyPath;
        retruxx::vector<CStr, retruxx::allocator<CStr>> m_emptyNames;
    };
}  // namespace ai
