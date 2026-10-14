#pragma once
#include "base/prototypeinfo.h"
#include "server/geomrepository.h"
#include "retruxx/common.h"

namespace ai
{
    class RepositoryObjectsGeneratorPrototypeInfo : public ai::PrototypeInfo
    {
    public:
        struct ObjectDescription
        {
            CStr prototypeName;
            int prototypeId;
        };

        using ObjectDescriptionVector = retruxx::vector<ai::RepositoryObjectsGeneratorPrototypeInfo::ObjectDescription, retruxx::allocator<ai::RepositoryObjectsGeneratorPrototypeInfo::ObjectDescription> >;

    public:
        retruxx::vector<ai::RepositoryObjectsGeneratorPrototypeInfo::ObjectDescription, retruxx::allocator<ai::RepositoryObjectsGeneratorPrototypeInfo::ObjectDescription> > m_objectDescriptions;
        RepositoryObjectsGeneratorPrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override;
        virtual void PostLoad() override;
        virtual ai::Obj* CreateTargetObject() const override;
        void Generate(unsigned int count, ai::GeomRepository* repository) const;
    };

}
