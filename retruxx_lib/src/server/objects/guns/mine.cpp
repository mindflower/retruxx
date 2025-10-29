#include "mine.h"

namespace ai
{
	RT_CLASS_EXPORTS_BEGIN(Mine)
    RT_CLASS_EXPORTS_END;
	RT_CLASS_DEFINE(Mine);

    MinePrototypeInfo::MinePrototypeInfo()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Obj* MinePrototypeInfo::CreateTargetObject() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool MinePrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Mine::~Mine()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Mine::Mine(const MinePrototypeInfo& prototypeInfo) : Rocket(prototypeInfo)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Mine::Mine(const Mine& mine) : Rocket(mine)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* Mine::Clone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* Mine::CreateObject()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* Mine::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Rocket);
    }

    m3d::Class* Mine::GetClass() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    const MinePrototypeInfo* Mine::GetPrototypeInfo() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Mine::LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Mine::SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Mine::Update(float elapsedTime, unsigned int workTime)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Mine::MineState Mine::getState()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}
