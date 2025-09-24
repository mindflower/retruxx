#include "mine.h"

namespace ai
{
	RT_CLASS_EXPORTS_BEGIN(Mine)
    RT_CLASS_EXPORTS_END;
	RT_CLASS_DEFINE(Mine);

    MinePrototypeInfo::MinePrototypeInfo()
    {
        throw std::logic_error("Not implemented");
    }

    Obj* MinePrototypeInfo::CreateTargetObject() const
    {
        throw std::logic_error("Not implemented");
    }

    bool MinePrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode)
    {
        throw std::logic_error("Not implemented");
    }

    Mine::~Mine()
    {
        throw std::logic_error("Not implemented");
    }

    Mine::Mine(const MinePrototypeInfo& prototypeInfo) : Rocket(prototypeInfo)
    {
        throw std::logic_error("Not implemented");
    }

    Mine::Mine(const Mine& mine) : Rocket(mine)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* Mine::Clone()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* Mine::CreateObject()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* Mine::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Rocket);
    }

    m3d::Class* Mine::GetClass() const
    {
        throw std::logic_error("Not implemented");
    }

    const MinePrototypeInfo* Mine::GetPrototypeInfo() const
    {
        throw std::logic_error("Not implemented");
    }

    void Mine::LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode)
    {
        throw std::logic_error("Not implemented");
    }

    void Mine::SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
    {
        throw std::logic_error("Not implemented");
    }

    void Mine::Update(float elapsedTime, unsigned int workTime)
    {
        throw std::logic_error("Not implemented");
    }

    Mine::MineState Mine::getState()
    {
        throw std::logic_error("Not implemented");
    }
}
