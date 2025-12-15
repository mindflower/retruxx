#include "mine.h"

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(Mine)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(Mine);

    MinePrototypeInfo::MinePrototypeInfo()
    {
        m_TTL = 100.0f;
        m_TimeForActivation = 0.0f;
    }

    Obj* MinePrototypeInfo::CreateTargetObject() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool MinePrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        bool const result = RocketPrototypeInfo::LoadFromXML(xmlFile, xmlNode);
        if (result)
        {
            m3d::SafeFloatAttrib(m_TTL, xmlNode, "TTL");
            m3d::SafeFloatAttrib(m_TimeForActivation, xmlNode, "TimeForActivation");
        }
        return result;
    }

    Mine::~Mine()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Mine::Mine(MinePrototypeInfo const& prototypeInfo) : Rocket(prototypeInfo)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Mine::Mine(Mine const& mine) : Rocket(mine)
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

    MinePrototypeInfo const* Mine::GetPrototypeInfo() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Mine::LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
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
}  // namespace ai
