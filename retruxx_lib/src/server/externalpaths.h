#pragma once
#include <thirdparty/containers.h>
#include <core/stringm3d.h>

namespace ai
{
    class ExternalPaths
    {
    public:
        bool Load(const CStr& fileName);
        bool Save(const CStr& filename);
        const retruxx::vector<CVector2, retruxx::allocator<CVector2> >& GetPath(const CStr& pathName) const;
        bool SetPath(const CStr& pathName, const retruxx::vector<CVector2, retruxx::allocator<CVector2> >& thePath);
        bool AddPath(const CStr& pathName, const retruxx::vector<CVector2, retruxx::allocator<CVector2> >& thePath);
        bool DelPath(const CStr& pathName);
        const retruxx::vector<CStr, retruxx::allocator<CStr> >& GetPathNames() const;

        using PathMap = retruxx::map<CStr, retruxx::vector<CVector2, retruxx::allocator<CVector2> >, retruxx::less<CStr>, retruxx::allocator<retruxx::pair<CStr const, retruxx::vector<CVector2, retruxx::allocator<CVector2> > > > >;

    private:
        retruxx::map<CStr, retruxx::vector<CVector2, retruxx::allocator<CVector2> >, retruxx::less<CStr>, retruxx::allocator<retruxx::pair<CStr const, retruxx::vector<CVector2, retruxx::allocator<CVector2> > > > > m_paths;
        retruxx::vector<CVector2, retruxx::allocator<CVector2> > m_emptyPath;
        retruxx::vector<CStr, retruxx::allocator<CStr> > m_emptyNames;
    }; /* size: 0x002c */
}
