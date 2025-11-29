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
            /* 0x000c */ int prototypeId;
        }; /* size: 0x0010 */

        using ObjectDescriptionVector = retruxx::vector<ai::RepositoryObjectsGeneratorPrototypeInfo::ObjectDescription, retruxx::allocator<ai::RepositoryObjectsGeneratorPrototypeInfo::ObjectDescription> >;

    public:
        retruxx::vector<ai::RepositoryObjectsGeneratorPrototypeInfo::ObjectDescription, retruxx::allocator<ai::RepositoryObjectsGeneratorPrototypeInfo::ObjectDescription> > m_objectDescriptions;
        RepositoryObjectsGeneratorPrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0x04 */;
        virtual void PostLoad() override /* 0x08 */;
        virtual ai::Obj* CreateTargetObject() const override /* 0x10 */;
        void Generate(unsigned int count, ai::GeomRepository* repository) const;
    }; /* size: 0x0050 */

}
