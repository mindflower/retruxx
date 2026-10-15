#pragma once
#include <core/stringm3d.h>

namespace m3d
{
    namespace cmn
    {
        struct XmlNode;
        class XmlFile;
    }  // namespace cmn

    struct Class;
}  // namespace m3d

namespace ai
{
    class Obj;

    class PrototypeInfo
    {
    protected:
        PrototypeInfo();
        PrototypeInfo(ai::PrototypeInfo const&);

    public:
        CStr m_className;
        CStr m_prototypeName;
        int m_prototypeId;
        int m_resourceId;
        bool m_bIsUpdating;
        bool m_bVisibleInEncyclopedia;
        bool m_bApplyAffixes;
        unsigned int m_price;
        virtual ~PrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode);
        virtual void PostLoad();
        virtual void RefreshFromXml(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode);
        virtual ai::Obj* CreateTargetObject() const = 0;
        bool bIsAbstract() const;
        CStr const& GetParentPrototypeName() const;
        virtual unsigned int GetBasePrice() const;
        void CopyFrom(ai::PrototypeInfo const& rhs);
        CStr GetDebugDescription() const;
        bool IsPrototypeOf(m3d::Class const* cl) const;

    protected:
        virtual void _InternalCopyFrom(ai::PrototypeInfo const& rhs);
        bool m_bIsAbstract;
        CStr m_parentPrototypeName;

    private:
        m3d::Class* m_protoClassObject;
    };
}  // namespace ai
