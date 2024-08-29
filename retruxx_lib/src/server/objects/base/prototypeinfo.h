#pragma once
#include <core/stringm3d.h>

namespace m3d
{
    namespace cmn
    {
        class XmlNode;
        class XmlFile;
    }

    struct Class;
}

namespace ai
{
    class PrototypeInfo
    {
    public:
        virtual ~PrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
        virtual void PostLoad();
        virtual void RefreshFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
        virtual unsigned int GetBasePrice() const ;

        PrototypeInfo();
        bool bIsAbstract() const ;
        void CopyFrom(class PrototypeInfo const &);
        CStr GetDebugDescription() const ;
        bool IsPrototypeOf(m3d::Class const *) const ;
        CStr const & GetParentPrototypeName() const ;

    protected:
        virtual void _InternalCopyFrom(PrototypeInfo const&);

    public:
        CStr m_className;
        CStr m_prototypeName;
        int m_prototypeId;
        int m_resourceId;
        bool m_bIsUpdating;
        bool m_bVisibleInEncyclopedia;
        bool m_bApplyAffixes;
        unsigned int m_price;

    protected:
        bool m_bIsAbstract;
        CStr m_parentPrototypeName;

    private:
        m3d::Class *m_protoClassObject;
    };
}
