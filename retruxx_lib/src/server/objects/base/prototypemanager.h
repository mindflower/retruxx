#pragma once
#include <vector>
#include <core/containers.h>

namespace m3d
{
    namespace cmn
    {
        class XmlFile;
        struct XmlNode;
    }
}

namespace ai
{
    class PrototypeInfo;

    class PrototypeManager
    {
    public:
        PrototypeManager();
        void Clear();
        unsigned int GetPrototypeFullNameLocalizedForm(int) const ;
        void LoadFromXmlFile(CStr const &);
        int GetMatrixNum(int) const ;
        void RefreshFromXmlFile(CStr const &);
        CStr GetPrototypeFullName(CStr const &) const ;
        CStr GetPrototypeFullName(int) const ;
        CStr GetPrototypeName(int) const ;
        int GetPrototypeId(CStr const &) const ;
        ~PrototypeManager();
        void GetPrototypeIdsByResourceId(int,std::vector<int> &) const ;
        PrototypeInfo const * GetPrototypeInfo(CStr const &) const ;
        PrototypeInfo const * GetPrototypeInfo(int) const ;
        int GetNumOfPrototypes() const ;

    private:
        void _LoadGameObjectsFolderFromXML(CStr const &,bool (__fastcall*)(m3d::cmn::XmlFile *,m3d::cmn::XmlNode const *));
        PrototypeInfo * _InternalGetPrototypeInfo(CStr const &);
        static bool _RefreshPrototype(m3d::cmn::XmlFile *,m3d::cmn::XmlNode const *);
        void _LoadFromFolder(m3d::cmn::XmlFile *,m3d::cmn::XmlNode const *,CStr const &,bool (__fastcall*)(m3d::cmn::XmlFile *,m3d::cmn::XmlNode const *));
        static bool _ReadNewPrototype(m3d::cmn::XmlFile *,m3d::cmn::XmlNode const *);

    private:
        m3d::CStrHash<CStr> m_prototypeFullNames;
        m3d::CStrHash<unsigned int> m_prototypeFullNamesLocalizedForms;
        m3d::CStrHash<int> m_prototypeNamesToIds;
        std::vector<PrototypeInfo *> m_prototypes;
        int m_loadingLock = 0;
    };
}
