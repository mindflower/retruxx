#include "prototypemanager.h"

namespace ai
{
    PrototypeManager* thePrototypeManager = nullptr;

    PrototypeManager::PrototypeManager()
    {
    }

    void PrototypeManager::Clear()
    {
        throw retruxx::logic_error("Not implemented");
    }

    unsigned PrototypeManager::GetPrototypeFullNameLocalizedForm(int) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void PrototypeManager::LoadFromXmlFile(CStr const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    int PrototypeManager::GetMatrixNum(int) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void PrototypeManager::RefreshFromXmlFile(CStr const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    CStr PrototypeManager::GetPrototypeFullName(CStr const&) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    CStr PrototypeManager::GetPrototypeFullName(int) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    CStr PrototypeManager::GetPrototypeName(int) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    int PrototypeManager::GetPrototypeId(CStr const&) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    PrototypeManager::~PrototypeManager()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void PrototypeManager::GetPrototypeIdsByResourceId(int, retruxx::vector<int, retruxx::allocator<int>>&) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    PrototypeInfo const* PrototypeManager::GetPrototypeInfo(CStr const&) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    PrototypeInfo const* PrototypeManager::GetPrototypeInfo(int) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    int PrototypeManager::GetNumOfPrototypes() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void PrototypeManager::_LoadGameObjectsFolderFromXML(CStr const&, bool(__fastcall *)(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*))
    {
        throw retruxx::logic_error("Not implemented");
    }

    PrototypeInfo* PrototypeManager::_InternalGetPrototypeInfo(CStr const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool PrototypeManager::_RefreshPrototype(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void PrototypeManager::_LoadFromFolder(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*, CStr const&, bool(__fastcall *)(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*))
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool PrototypeManager::_ReadNewPrototype(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw retruxx::logic_error("Not implemented");
    }
}
