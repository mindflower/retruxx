#pragma once
#include "base/prototypeinfo.h"
#include "retruxx/common.h"

namespace ai
{
    class AffixGeneratorPrototypeInfo : public PrototypeInfo
    {
    protected:
        virtual void _InternalCopyFrom(PrototypeInfo const& rhs) override;

        struct AffixDescription
        {
            CStr m_affixName;
        };

        using AffixDescriptionVector = retruxx::vector<AffixDescription>;

    public:
        AffixGeneratorPrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode) override;
        virtual void PostLoad() override;
        virtual Obj* CreateTargetObject() const override;
        void GenerateAffixesForObj(Obj* obj, unsigned int desiredNumAffixes) const;

    private:
        retruxx::vector<AffixDescription> m_affixDescriptions;
    };
}  // namespace ai
