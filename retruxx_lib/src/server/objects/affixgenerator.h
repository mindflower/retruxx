#pragma once
#include "base/prototypeinfo.h"
#include "thirdparty/containers.h"

namespace ai
{
    class AffixGeneratorPrototypeInfo : public PrototypeInfo
    {
    protected:
        virtual void _InternalCopyFrom(const PrototypeInfo& rhs) override /* 0x18 */;

        struct AffixDescription
        {
            CStr m_affixName;
        }; /* size: 0x000c */

        using AffixDescriptionVector = retruxx::vector<AffixDescription>;

    public:
        AffixGeneratorPrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0x04 */;
        virtual void PostLoad() override /* 0x08 */;
        virtual Obj* CreateTargetObject() const override /* 0x10 */;
        void GenerateAffixesForObj(Obj* obj, unsigned int desiredNumAffixes) const;

    private:
        retruxx::vector<AffixDescription> m_affixDescriptions;
    }; /* size: 0x0050 */
}
