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
        /* 0x0004 */ CStr m_className;
        /* 0x0010 */ CStr m_prototypeName;
        /* 0x001c */ int m_prototypeId;
        /* 0x0020 */ int m_resourceId;
        /* 0x0024 */ bool m_bIsUpdating;
        /* 0x0025 */ bool m_bVisibleInEncyclopedia;
        /* 0x0026 */ bool m_bApplyAffixes;
        /* 0x0027 */ char Padding_13;
        /* 0x0028 */ unsigned int m_price;
        virtual  ~PrototypeInfo() /* 0x00 */;
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) /* 0x04 */;
        virtual void PostLoad() /* 0x08 */;
        virtual void RefreshFromXml(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) /* 0x0c */;
        virtual ai::Obj* CreateTargetObject() const = 0 /* 0x10 */;
        bool bIsAbstract() const;
        const CStr& GetParentPrototypeName() const;
        virtual unsigned int GetBasePrice() const /* 0x14 */;
        void CopyFrom(const ai::PrototypeInfo& rhs);
        CStr GetDebugDescription() const;
        bool IsPrototypeOf(const m3d::Class* cl) const;

    protected:
        virtual void _InternalCopyFrom(const ai::PrototypeInfo& rhs) /* 0x18 */;
        /* 0x002c */ bool m_bIsAbstract;
        /* 0x002d */ char Padding_14[3];
        /* 0x0030 */ CStr m_parentPrototypeName;

    private:
        /* 0x003c */ m3d::Class* m_protoClassObject;
    }; /* size: 0x0040 */

    static_assert(sizeof(PrototypeInfo) == 0x0040);
}
