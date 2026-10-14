#pragma once
#include <core/stringm3d.h>

namespace m3d
{
    namespace cmn
    {
        struct XmlNode;
        class XmlFile;
    }

    struct Class;
}

namespace ai
{
    class Obj;

    class PrototypeInfo
    {
    protected:
        PrototypeInfo();
        PrototypeInfo(const ai::PrototypeInfo&);

    public:
        CStr m_className;
        CStr m_prototypeName;
        int m_prototypeId;
        int m_resourceId;
        bool m_bIsUpdating;
        bool m_bVisibleInEncyclopedia;
        bool m_bApplyAffixes;
        char Padding_13;
        unsigned int m_price;
        virtual  ~PrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode);
        virtual void PostLoad();
        virtual void RefreshFromXml(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode);
        virtual ai::Obj* CreateTargetObject() const = 0;
        bool bIsAbstract() const;
        const CStr& GetParentPrototypeName() const;
        virtual unsigned int GetBasePrice() const;
        void CopyFrom(const ai::PrototypeInfo& rhs);
        CStr GetDebugDescription() const;
        bool IsPrototypeOf(const m3d::Class* cl) const;

    protected:
        virtual void _InternalCopyFrom(const ai::PrototypeInfo& rhs);
        bool m_bIsAbstract;
        char Padding_14[3];
        CStr m_parentPrototypeName;

    private:
        m3d::Class* m_protoClassObject;
    };
}
