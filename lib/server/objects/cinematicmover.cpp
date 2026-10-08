#include "cinematicmover.h"

#include <stdexcept>

#include "cinematic.h"
#include "m3dapp.h"
#include "core/kernel.h"
#include "base/objcontainer.h"
#include "base/physicobj.h"
#include "base/prototypemanager.h"

RT_CLASS_EXPORT_METHOD_DEFINE(CinematicMover, SetObjAndPath)
{
    // RVA 0x7FACA0 - the arguments are read in the order path, time, object id.
    auto* mover = static_cast<ai::CinematicMover*>(context->asObject(0, "CinematicMover"));
    CStr const pathName = context->asString(2);
    float const totalTime = context->asFloat(3);
    int const objId = context->asInt(1);
    mover->SetObjAndPath(objId, pathName, totalTime);
    return 1;
}

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(CinematicMover)
        RT_CLASS_EXPORT(CinematicMover, m3d::METHOD, SetObjAndPath, "", "", "")
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(CinematicMover);

    CinematicMoverPrototypeInfo::CinematicMoverPrototypeInfo()
    {
    }

    bool CinematicMoverPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        return ai::PrototypeInfo::LoadFromXML(xmlFile, xmlNode) != 0;
    }

    Obj* CinematicMoverPrototypeInfo::CreateTargetObject() const
    {
        // RVA 0x7FA4B0
        return new CinematicMover(*this);
    }

    m3d::Class* CinematicMover::GetClass() const
    {
        // RVA 0x7FA060
        return RT_CLASS_LOCAL(CinematicMover);
    }

    CinematicMover::CinematicMover(CinematicMoverPrototypeInfo const& prototypeInfo) :
        Obj(prototypeInfo),
        m_currentFlyPath(nullptr),
        m_currentFlyTime(0.0f),
        m_controlledObjId(-1)
    {
        // RVA 0x7FA0E0
    }

    void CinematicMover::SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
    {
        // RVA 0x7FA4E0
        Obj::SaveRuntimeValues(xmlFile, xmlNode);
        xmlNode->SetAttribute("FlyPathName", m_flyPathName.c_str());
        xmlNode->SetAttribute("CurrentFlyTime", CStr(m_currentFlyTime).c_str());
        xmlNode->SetAttribute("ControlledObjId", CStr(m_controlledObjId).c_str());
        if (m_currentFlyPath)
        {
            ref_ptr flyPathNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, "CurrentFlyPath");
            m_currentFlyPath->SaveToXmlRuntime(xmlFile, flyPathNode);
            xmlNode->AddChild(flyPathNode);
        }
    }

    void CinematicMover::SetObjAndPath(int controlledObjId, CStr const& cinematicPathName, float totalTime)
    {
        // RVA 0x7FA950 - flies the object along a named camera path from camera_paths.xml, starting
        // from where the object is now and taking totalTime seconds.
        delete m_currentFlyPath;
        m_controlledObjId = controlledObjId;
        m_currentFlyPath = nullptr;
        m_currentFlyTime = 0.0f;
        m_flyPathName = cinematicPathName;

        PhysicObj* controlledObj = _GetControlledObj();
        if (!controlledObj)
        {
            return;
        }
        if (!controlledObj->IsKindOf(RT_CLASS_LOCAL(PhysicObj)))
        {
            SYS_ERROR("IS_KIND_OF( controlledObj, PhysicObj )");
        }

        m3d::Cinematic* cinematic = M3D_APP->m_cinematic;
        cinematic->Load("camera_paths.xml");
        delete m_currentFlyPath;
        m_currentFlyPath = new m3d::CameraPath(cinematic->GetPathByName(m_flyPathName));

        // The object's current placement becomes the path's first point.
        m3d::CameraPathState state;
        state.m_rotation = controlledObj->GetRotation();
        state.m_point = controlledObj->GetPosition();
        state.m_zoom = 1.0f;
        state.m_speed = 1.0f;
        // NOTE: the shipped code leaves the state's flyTime uninitialized; CalcFlyTimes recomputes it.
        m_currentFlyPath->insert(0, state);
        m_currentFlyPath->SetFullTime(totalTime);
        m_currentFlyPath->CalcFlyTimes(1, true);
    }

    m3d::Class* CinematicMover::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Obj);
    }

    void CinematicMover::Update(float elapsedTime, unsigned workTime)
    {
        // RVA 0x7FA670 - NOTE: m_currentFlyPath is not checked for null.
        Obj::Update(elapsedTime, workTime);
        PhysicObj* controlledObj = _GetControlledObj();
        if (!controlledObj)
        {
            return;
        }
        CVector pos = controlledObj->GetPosition();
        Quaternion rot = controlledObj->GetRotation();
        float zoom;
        m_currentFlyPath->GetCameraForTime(m_currentFlyTime, pos, rot, zoom);
        controlledObj->SetPosition(pos);
        controlledObj->SetRotation(rot);
        m_currentFlyTime = m_currentFlyTime + elapsedTime;
        if (m_currentFlyTime >= m_currentFlyPath->GetFullTime())
        {
            Remove();
        }
    }

    void CinematicMover::LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        // RVA 0x7FA760 - NOTE: a path already set is not deleted before a saved one replaces it.
        Obj::LoadRuntimeValues(xmlFile, xmlNode);
        m3d::SafeStrAttrib(m_flyPathName, xmlNode, "FlyPathName");
        m3d::SafeFloatAttrib(m_currentFlyTime, xmlNode, "CurrentFlyTime");
        m3d::SafeIntAttrib(m_controlledObjId, xmlNode, "ControlledObjId");
        ref_ptr flyPathNode = xmlFile->CreateNode();
        xmlNode->GetFirstChild(flyPathNode, "CurrentFlyPath");
        if (!flyPathNode->IsEmpty())
        {
            m_currentFlyPath = new m3d::CameraPath();
            m_currentFlyPath->LoadFromXmlRuntime(xmlFile, flyPathNode);
        }
    }

    CinematicMoverPrototypeInfo const* CinematicMover::GetPrototypeInfo() const
    {
        // RVA 0x7FA600 - NOTE: the prototype is cast without a type check.
        return static_cast<CinematicMoverPrototypeInfo const*>(thePrototypeManager->GetPrototypeInfo(GetPrototypeId()));
    }

    CinematicMover::~CinematicMover()
    {
        // RVA 0x7FA8A0
        delete m_currentFlyPath;
        m_currentFlyPath = nullptr;
    }

    PhysicObj* CinematicMover::_GetControlledObj() const
    {
        // RVA 0x7FA630 - NOTE: not type-checked.
        return static_cast<PhysicObj*>(theObjects->GetEntityByObjId(m_controlledObjId));
    }

    m3d::Object* CinematicMover::CreateObject()
    {
        // RVA 0x7FA2F0
        SYS_ERROR("!\"Object cannot be created directly\"");
        return nullptr;
    }

    m3d::Object* CinematicMover::Clone()
    {
        // RVA 0x7FA130
        SYS_ERROR("!\"Object cannot be cloned\"");
        return nullptr;
    }
}
