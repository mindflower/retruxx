#pragma once
#include "base/prototypeinfo.h"
#include <vector>

namespace ai
{
    class GeomRepository;
    class Obj;

    class RepositoryObjectsGeneratorPrototypeInfo : public PrototypeInfo
    {
    public:
        class ObjectDescription
        {
        private:
            CStr prototypeName;
            int prototypeId;
        };

    public:
        virtual Obj* CreateTargetObject() const;
        RepositoryObjectsGeneratorPrototypeInfo();
        virtual void PostLoad();
        void Generate(unsigned int, GeomRepository*) const;
        virtual bool LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);

    private:
        std::vector<ObjectDescription> m_objectDescriptions;
    };

}
