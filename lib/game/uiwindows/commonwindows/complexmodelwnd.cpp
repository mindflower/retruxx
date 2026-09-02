#include "complexmodelwnd.h"

#include <vector>

#include <core/kernel.h>
#include <core/log.h>
#include <core/timer.h>
#include <i_event.h>
#include <game/m3dgame.h>
#include <scene/nodes/sgnodeanimatedmodel.h>
#include <scene/nodes/sgnodedecals.h>
#include <server/objects/base/objcontainer.h>
#include <server/objects/vehicle.h>

RT_CLASS_EXPORTS_BEGIN(ComplexModelWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(ComplexModelWnd);

m3d::Class* ComplexModelWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(SgNodeArrayWnd);
}

m3d::Class* ComplexModelWnd::GetClass() const
{
    return RT_CLASS_LOCAL(ComplexModelWnd);
}

m3d::Object* ComplexModelWnd::CreateObject()
{
    return new ComplexModelWnd;
}

m3d::Object* ComplexModelWnd::Clone()
{
    return new ComplexModelWnd(*this);
}

ComplexModelWnd::ComplexModelWnd()
{
    m_objId = -1;
    m_defaultRotationAngleX = -0.52359879f;
    m_rotationAngle.x = -0.52359879f;
    m_rotationAngle.y = 0.0f;
    m_rotationVelocity = 0.17453294f;
    m_rotationByHandVelocity = 0.017453292f;
    m_bAllowRotate = true;
    m_bAllowRotateByHandX = false;
    m_bAllowRotateByHandY = true;
    m_bInRotationByHandMode = false;
}

ComplexModelWnd::ComplexModelWnd(ComplexModelWnd const&) : ComplexModelWnd()
{
}

ComplexModelWnd::~ComplexModelWnd()
{
    ClearSgNodes();
    m_objId = -1;
    m_rotationAngle.x = m_defaultRotationAngleX;
    m_rotationAngle.y = 0.0f;
    // ~SgNodeArrayWnd runs via the compiler-chained base destructor.
}

// ---------------------------------------------------------------------------
//  Rotation configuration
// ---------------------------------------------------------------------------

void ComplexModelWnd::AllowRotate(bool bAllow)
{
    m_bAllowRotate = bAllow;
}

void ComplexModelWnd::AllowRotateByHandX(bool bAllow)
{
    m_bAllowRotateByHandX = bAllow;
}

void ComplexModelWnd::AllowRotateByHandY(bool bAllow)
{
    m_bAllowRotateByHandY = bAllow;
}

bool ComplexModelWnd::IsAllowedRotate() const
{
    return m_bAllowRotate;
}

bool ComplexModelWnd::IsAllowedRotateByHandX() const
{
    return m_bAllowRotateByHandX;
}

bool ComplexModelWnd::IsAllowedRotateByHandY() const
{
    return m_bAllowRotateByHandY;
}

void ComplexModelWnd::SetRotationVelocity(float velocity)
{
    m_rotationVelocity = velocity;
}

float ComplexModelWnd::GetRotationVelocity() const
{
    return m_rotationVelocity;
}

void ComplexModelWnd::SetRotationByHandVelocity(float velocity)
{
    m_rotationByHandVelocity = velocity;
}

float ComplexModelWnd::GetRotationByHandVelocity() const
{
    return m_rotationByHandVelocity;
}

void ComplexModelWnd::SetDefaultRotationAngleX(float angle)
{
    m_defaultRotationAngleX = angle;
}

float ComplexModelWnd::GetDefaultRotationAngleX() const
{
    return m_defaultRotationAngleX;
}

void ComplexModelWnd::SetRotationByHandMode(bool bState)
{
    m_bInRotationByHandMode = bState;
    M3D_APP->CaptureMouse(bState ? this : nullptr);
}

bool ComplexModelWnd::IsDisabled() const
{
    return (m_style & 2) != 0 || (m_style & 0x80000) != 0;
}

// ---------------------------------------------------------------------------
//  Object binding
// ---------------------------------------------------------------------------

ai::Obj* ComplexModelWnd::GetObj() const
{
    if (m_objId < 0)
    {
        return nullptr;
    }
    return ai::theObjects->GetEntityByObjId(m_objId);
}

void ComplexModelWnd::SetObjId(int objId)
{
    m_objId = objId;
    if (IsChildOf(m3d::Application::g_pApp))
    {
        SetupModel();
    }
}

void ComplexModelWnd::UpdateRotationAngle()
{
    m_rotationAngle.y += static_cast<float>(
        static_cast<double>(M3D_KERNEL->GetTimer().GetLastFrameTimeUnscaled()) * m_rotationVelocity * 0.001);
}

// ---------------------------------------------------------------------------
//  Window lifecycle
// ---------------------------------------------------------------------------

int ComplexModelWnd::CreateFromPattern(m3d::ui::Wnd* patternWnd, bool deleteSrc)
{
    if (!patternWnd || !patternWnd->IsKindOf(&m3d::ui::SgNodeArrayWnd::m_classSgNodeArrayWnd))
    {
        M3D_LOG_INFO("ComplexModelWnd::CreateFromPattern error - invalid patternWnd");
        return 0;
    }

    auto* patternSg = static_cast<m3d::ui::SgNodeArrayWnd*>(patternWnd);

    unsigned const style = patternWnd->GetStyle();
    BoundsBase<float> const rc = patternWnd->GetBounds();
    int const id = patternWnd->GetId();
    m3d::rend::TexHandle const image = patternSg->GetImage();
    m3d::rend::TexHandle const renderTex = patternSg->GetTargetTexture();

    m3d::Object* parent = patternWnd->GetParent();
    if (!parent)
    {
        return 0;
    }

    if (!CreateSgNodeArrayWnd(image, style, rc, id, renderTex))
    {
        M3D_LOG_INFO("ComplexModelWnd::CreateFromPattern error - cannot create");
        return 0;
    }

    SetPane(patternWnd->GetPaneName());
    SetPaneFlags(patternWnd->GetPaneFlags());
    parent->AddChild(this);
    if (deleteSrc)
    {
        parent->RemoveChild(patternWnd);
        patternWnd->DecRef();
    }

    std::vector<m3d::Class*> classes;
    classes.push_back(&m3d::SgAnimatedModelNode::m_classSgAnimatedModelNode);
    classes.push_back(&m3d::SgDecalsNode::m_classSgDecalsNode);
    SetClassesToRender(classes);
    return 1;
}

int ComplexModelWnd::GameDataClear(bool)
{
    ClearSgNodes();
    m_objId = -1;
    m_rotationAngle.x = m_defaultRotationAngleX;
    m_rotationAngle.y = 0.0f;
    return 1;
}

int ComplexModelWnd::GameDataUpdate(void* data, int dataType)
{
    if (dataType == 65)
    {
        if (data && IsChildOf(m3d::Application::g_pApp))
        {
            ai::Obj* obj = GetObj();
            auto const* evt = static_cast<m3d::Event const*>(data);
            if (obj && obj->IsKindOf(&ai::Vehicle::m_classVehicle) && obj->GetId() == evt->m_intEv[0])
            {
                SetupModel();
            }
        }
        return 1;
    }
    if (dataType == 89)
    {
        UpdateOnNewFrame();
    }
    return 1;
}

int ComplexModelWnd::OnBeforeAddToWndStation()
{
    SetupModel();
    if (m_bInRotationByHandMode)
    {
        m_bInRotationByHandMode = false;
        M3D_APP->CaptureMouse(nullptr);
    }
    if (m_bAllowRotateByHandX)
    {
        m_rotationAngle.x = m_defaultRotationAngleX;
    }
    return m3d::ui::Wnd::OnBeforeAddToWndStation();
}

int ComplexModelWnd::OnAfterRemoveFromWndStation()
{
    int const result = m3d::ui::Wnd::OnAfterRemoveFromWndStation();
    ClearSgNodes();
    if (m_bInRotationByHandMode)
    {
        m_bInRotationByHandMode = false;
        M3D_APP->CaptureMouse(nullptr);
    }
    return result;
}

int ComplexModelWnd::OnMouseButton0(unsigned state, PointBase<float> const& at)
{
    if (state && (m_bAllowRotateByHandX || m_bAllowRotateByHandY))
    {
        m_bInRotationByHandMode = true;
        M3D_APP->CaptureMouse(this);
    }
    else
    {
        m_bInRotationByHandMode = false;
        M3D_APP->CaptureMouse(nullptr);
    }
    return m3d::ui::Wnd::OnMouseButton0(state, at);
}

int ComplexModelWnd::OnMouseMove(PointBase<float> const& pt, PointBase<float> const& deltas)
{
    if (m_bInRotationByHandMode)
    {
        float dy = 0.0f;
        float dx = 0.0f;
        if (m_bAllowRotateByHandY)
        {
            dx = -deltas.x;
        }
        if (m_bAllowRotateByHandX)
        {
            dy = -deltas.y;
        }
        m_rotationAngle.x += dy * m_rotationByHandVelocity;
        m_rotationAngle.y += dx * m_rotationByHandVelocity;
        UpdateCamera();
    }
    return m3d::ui::Wnd::OnMouseMove(pt, deltas);
}

int ComplexModelWnd::UpdateOnNewFrame()
{
    if ((m_style & 2) != 0 || (m_style & 0x80000) != 0)
    {
        return 0;
    }
    if (m_bAllowRotate && !m_bInRotationByHandMode)
    {
        UpdateRotationAngle();
        UpdateCamera();
    }
    return 1;
}

int ComplexModelWnd::UpdateOnVehiclePartChanged()
{
    SetupModel();
    return 1;
}

// ---------------------------------------------------------------------------
//  Not yet ported: the scene-graph model assembly. SetupModel dispatches on the
//  bound object's class into SetupForVehicle / SetupForGun / SetupForCompoundGun,
//  which walk SgNode trees and gun geometry; UpdateCamera rebuilds the view
//  matrix; the aabb / node helpers support them.
// ---------------------------------------------------------------------------

void ComplexModelWnd::SetupModel()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void ComplexModelWnd::SetupForVehicle(ai::Vehicle*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void ComplexModelWnd::SetupForGun(ai::Gun*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void ComplexModelWnd::SetupForCompoundGun(ai::CompoundGun*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void ComplexModelWnd::UpdateCamera()
{
    RETRUXX_NOT_IMPLEMENTED;
}

CVector ComplexModelWnd::getGunSize(ai::Gun*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void ComplexModelWnd::AddOneNode(m3d::SgNode*, CMatrix const&, Quaternion const&, bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void ComplexModelWnd::AddOneNode(m3d::SgNode*, CMatrix const&, bool, bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void ComplexModelWnd::EmbraceGunAabb(Aabb&, ai::Gun const*) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void ComplexModelWnd::EmbraceCompoundGunAabb(Aabb&, ai::CompoundGun const*) const
{
    RETRUXX_NOT_IMPLEMENTED;
}
