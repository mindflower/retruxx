#pragma once
#include <map>
#include <vector>

class CStr;
class CVector2;

namespace ai
{
    class ExternalPaths
    {
    public:
        std::vector<CStr> const& GetPathNames() const;
        bool Load(CStr const&);
        bool DelPath(CStr const&);
        bool SetPath(CStr const&, std::vector<CVector2> const&);
        bool Save(CStr const&);
        bool AddPath(CStr const&, std::vector<CVector2> const&);
        std::vector<CVector2> const& GetPath(CStr const&) const;

    private:
        std::map<CStr, std::vector<CVector2>> m_paths;
        std::vector<CVector2> m_emptyPath;
        std::vector<CStr> m_emptyNames;
    };
}
