#include "npcmotioncontroller.h"

#include <stdexcept>

RT_CLASS_EXPORT_METHOD_DEFINE(NPCMotionController, getVehicleUnderControl)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(NPCMotionController, setVehicleUnderControl)
{
    throw std::logic_error("Not implemented");
}


namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(NPCMotionController)
        RT_CLASS_EXPORT(NPCMotionController, m3d::METHOD, getVehicleUnderControl, "", "", "")
        RT_CLASS_EXPORT(NPCMotionController, m3d::METHOD, setVehicleUnderControl, "", "", "")
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(NPCMotionController);

    NPCMotionControllerPrototypeInfo::NPCMotionControllerPrototypeInfo()
    {
    }

    Obj* NPCMotionControllerPrototypeInfo::CreateTargetObject() const
    {
        throw std::logic_error("Not implemented");
    }

    bool NPCMotionControllerPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        return ai::PrototypeInfo::LoadFromXML(xmlFile, xmlNode) != 0;
    }

    int NPCMotionController::OnEvent(Event const&)
    {
        throw std::logic_error("Not implemented");
    }

    void NPCMotionController::setVehicleUnderControl(Vehicle*)
    {
        throw std::logic_error("Not implemented");
    }

    void NPCMotionController::setVehicleUnderControl(m3d::Object*)
    {
        throw std::logic_error("Not implemented");
    }

    void NPCMotionController::RenderDebugInfo() const
    {
        throw std::logic_error("Not implemented");
    }

    void NPCMotionController::LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
    }

    void NPCMotionController::Update(float, unsigned)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* NPCMotionController::GetClass() const
    {
        throw std::logic_error("Not implemented");
    }

    Vehicle* NPCMotionController::getVehicleUnderControl() const
    {
        throw std::logic_error("Not implemented");
    }

    NPCMotionControllerPrototypeInfo const* NPCMotionController::GetPrototypeInfo() const
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* NPCMotionController::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Obj);
    }

    void NPCMotionController::SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        throw std::logic_error("Not implemented");
    }

    void NPCMotionController::SetPassedToAnotherMapStatus()
    {
        throw std::logic_error("Not implemented");
    }

    NPCMotionController::NPCMotionController(NPCMotionControllerPrototypeInfo const&)
    {
        throw std::logic_error("Not implemented");
    }

    NPCMotionController::~NPCMotionController()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* NPCMotionController::CreateObject()
    {
        throw std::logic_error("Not implemented");
    }

    CVector2 NPCMotionController::getCurrentVehiclePosition() const
    {
        throw std::logic_error("Not implemented");
    }

    void NPCMotionController::_OnObjectDie(Event const&)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* NPCMotionController::Clone()
    {
        throw std::logic_error("Not implemented");
    }

    void NPCMotionController::setDesiredVehiclePosition(CVector2 const&)
    {
        throw std::logic_error("Not implemented");
    }
}
