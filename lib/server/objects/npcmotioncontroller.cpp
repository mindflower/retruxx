#include "npcmotioncontroller.h"

#include <stdexcept>

RT_CLASS_EXPORT_METHOD_DEFINE(NPCMotionController, getVehicleUnderControl)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(NPCMotionController, setVehicleUnderControl)
{
    RETRUXX_NOT_IMPLEMENTED;
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
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool NPCMotionControllerPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        return ai::PrototypeInfo::LoadFromXML(xmlFile, xmlNode) != 0;
    }

    int NPCMotionController::OnEvent(Event const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void NPCMotionController::setVehicleUnderControl(Vehicle*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void NPCMotionController::setVehicleUnderControl(m3d::Object*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void NPCMotionController::RenderDebugInfo() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void NPCMotionController::LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void NPCMotionController::Update(float, unsigned)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* NPCMotionController::GetClass() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Vehicle* NPCMotionController::getVehicleUnderControl() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    NPCMotionControllerPrototypeInfo const* NPCMotionController::GetPrototypeInfo() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* NPCMotionController::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Obj);
    }

    void NPCMotionController::SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void NPCMotionController::SetPassedToAnotherMapStatus()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    NPCMotionController::NPCMotionController(NPCMotionControllerPrototypeInfo const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    NPCMotionController::~NPCMotionController()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* NPCMotionController::CreateObject()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CVector2 NPCMotionController::getCurrentVehiclePosition() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void NPCMotionController::_OnObjectDie(Event const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* NPCMotionController::Clone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void NPCMotionController::setDesiredVehiclePosition(CVector2 const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}
