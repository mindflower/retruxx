#include "complexmodelwnd.h"

#include <map>
#include <vector>

#include <core/kernel.h>
#include <core/log.h>
#include <core/timer.h>
#include <i_event.h>
#include <game/m3dgame.h>
#include <math/matrix.h>
#include <scene/nodes/sgnode.h>
#include <scene/nodes/sgnodeanimatedmodel.h>
#include <scene/nodes/sgnodedecals.h>
#include <server/objects/base/objcontainer.h>
#include <server/objects/guns/compoundgun.h>
#include <server/objects/guns/gun.h>
#include <server/objects/physicbodies/geoms/geom.h>
#include <server/objects/physicbodies/vehiclepart.h>
#include <server/objects/vehicle.h>
#include <server/objects/wheel.h>

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
//  Scene-graph model assembly
// ---------------------------------------------------------------------------

void ComplexModelWnd::SetupModel()
{
    // RVA 0x50DFA0
    ai::Obj* obj = GetObj();
    if (!obj)
    {
        return;
    }

    ClearSgNodes();
    if (obj->IsKindOf(&ai::Vehicle::m_classVehicle))
    {
        SetupForVehicle(static_cast<ai::Vehicle*>(obj));
        UpdateCamera();
    }
    else if (obj->IsKindOf(&ai::Gun::m_classGun))
    {
        SetupForGun(static_cast<ai::Gun*>(obj));
        UpdateCamera();
    }
    else if (obj->IsKindOf(&ai::CompoundGun::m_classCompoundGun))
    {
        SetupForCompoundGun(static_cast<ai::CompoundGun*>(obj));
        UpdateCamera();
    }
}

void ComplexModelWnd::SetupForVehicle(ai::Vehicle* vehicle)
{
    // RVA 0x50FDE0
    if (!vehicle)
    {
        return;
    }

    // Straighten the vehicle out so the measurement below is axis-aligned; the
    // original rotation goes back on at the end.
    Quaternion const oldRot = vehicle->GetRotation();
    vehicle->SetRotation(IdentityQuaternion);

    m_aabb.StartEmbracing();
    for (auto it = vehicle->begin(); it != vehicle->end(); ++it)
    {
        ai::VehiclePart* vp = it->second;
        if (!vp)
        {
            continue;
        }
        if (vp->IsKindOf(&ai::CompoundGun::m_classCompoundGun))
        {
            EmbraceCompoundGunAabb(m_aabb, static_cast<ai::CompoundGun*>(vp));
        }
        else if (!vp->m_pGeoms.empty())
        {
            m_aabb.EmbraceBox(vp->m_pGeoms[0]->GetAabb());
        }
    }
    for (unsigned i = 0; i < vehicle->GetNumWheels(); ++i)
    {
        ai::Wheel* wheel = vehicle->GetWheel(i);
        ai::SimplePhysicBody* body = wheel ? wheel->GetPhysicBody() : nullptr;
        if (body && !body->m_pGeoms.empty())
        {
            m_aabb.EmbraceBox(body->m_pGeoms[0]->GetAabb());
        }
    }

    // Everything is placed relative to the centre of that box, so the model sits
    // in the middle of the window whatever its world position is.
    CVector const center(
        (m_aabb.m_box[3] + m_aabb.m_box[0]) * 0.5f,
        (m_aabb.m_box[4] + m_aabb.m_box[1]) * 0.5f,
        (m_aabb.m_box[5] + m_aabb.m_box[2]) * 0.5f);
    CMatrix invMatr;
    invMatr.rotTranslate(vehicle->GetRotation(), center);
    invMatr = invMatr.getInverse();

    for (auto it = vehicle->begin(); it != vehicle->end(); ++it)
    {
        ai::VehiclePart* vp = it->second;
        if (vp->IsKindOf(&ai::Gun::m_classGun))
        {
            auto* gun = static_cast<ai::Gun*>(vp);
            AddOneNode(gun->m_Node, invMatr, false, false);
            AddOneNode(gun->GetBarrelNode(), invMatr, false, false);
        }
        else if (vp->IsKindOf(&ai::CompoundGun::m_classCompoundGun))
        {
            auto* cgun = static_cast<ai::CompoundGun*>(vp);
            for (auto sub = cgun->begin(); sub != cgun->end(); ++sub)
            {
                auto* gun = static_cast<ai::Gun*>(sub->second.vp);
                AddOneNode(gun->m_Node, invMatr, false, false);
                AddOneNode(gun->GetBarrelNode(), invMatr, false, false);
            }
        }
        else
        {
            // Ordinary parts keep their own orientation and bring their children.
            AddOneNode(vp->m_Node, invMatr, true, true);
        }
    }

    for (unsigned i = 0; i < vehicle->GetNumWheels(); ++i)
    {
        ai::Wheel* wheel = vehicle->GetWheel(i);
        if (wheel)
        {
            // Wheels are posed at their mounting rotation rather than whatever
            // angle they happen to have been left spinning at.
            AddOneNode(wheel->GetPhysicBody()->m_Node, invMatr, vehicle->GetWheelInitialRotation(i), false);
            AddOneNode(wheel->m_suspensionNode, invMatr, true, false);
        }
    }

    vehicle->SetRotation(oldRot);
}

void ComplexModelWnd::SetupForGun(ai::Gun* gun)
{
    // RVA 0x510220
    if (!gun || !gun->m_Node)
    {
        return;
    }

    m_aabb.StartEmbracing();
    EmbraceGunAabb(m_aabb, gun);

    CMatrix invMatr;
    invMatr.identity();
    invMatr._41 = -((m_aabb.m_box[3] + m_aabb.m_box[0]) * 0.5f);
    invMatr._42 = -((m_aabb.m_box[4] + m_aabb.m_box[1]) * 0.5f);
    invMatr._43 = -((m_aabb.m_box[5] + m_aabb.m_box[2]) * 0.5f);

    AddOneNode(gun->m_Node, invMatr, false, false);
    AddOneNode(gun->GetBarrelNode(), invMatr, false, false);
}

void ComplexModelWnd::SetupForCompoundGun(ai::CompoundGun* cvp)
{
    // RVA 0x510380
    if (!cvp)
    {
        return;
    }

    // The sub-guns are scattered around wherever they sit on the vehicle, so
    // collapse them all onto the origin, measure and capture them there, then
    // put every one of them back.
    std::map<ai::Obj*, CVector> oldPositions;
    for (auto it = cvp->begin(); it != cvp->end(); ++it)
    {
        ai::VehiclePart* vp = it->second.vp;
        oldPositions[vp] = vp->GetNodeAbsolutePosition();
        vp->SetNodeAbsolutePosition(CVector(0.0f, 0.0f, 0.0f));
        vp->TransferPhysicParamsToSceneGraphNode();
    }

    m_aabb.StartEmbracing();
    EmbraceCompoundGunAabb(m_aabb, cvp);

    Quaternion rot = IdentityQuaternion;
    if (cvp->GetOwner())
    {
        rot = cvp->GetOwner()->GetRotation();
    }
    CMatrix invMatr;
    invMatr.rotTranslate(rot, CVector(0.0f, 0.0f, 0.0f));
    invMatr = invMatr.getInverse();

    for (auto it = cvp->begin(); it != cvp->end(); ++it)
    {
        auto* gun = static_cast<ai::Gun*>(it->second.vp);
        if (gun->m_Node)
        {
            AddOneNode(gun->m_Node, invMatr, false, false);
            AddOneNode(gun->GetBarrelNode(), invMatr, false, false);
        }
    }

    for (auto it = cvp->begin(); it != cvp->end(); ++it)
    {
        ai::VehiclePart* vp = it->second.vp;
        auto const found = oldPositions.find(vp);
        if (found != oldPositions.end())
        {
            vp->SetNodeAbsolutePosition(found->second);
            vp->TransferPhysicParamsToSceneGraphNode();
        }
    }
}

void ComplexModelWnd::UpdateCamera()
{
    // RVA 0x50F1C0
    if ((m_style & 2) != 0 || (m_style & 0x80000) != 0)
    {
        return;
    }
    ai::Obj* obj = GetObj();
    if (!obj)
    {
        return;
    }

    CMatrix rotX;
    rotX.rotX(m_rotationAngle.x);
    CMatrix rotY;
    rotY.rotY(m_rotationAngle.y);

    // Pull back far enough that the model's widest horizontal extent fits.
    float const sizeX = m_aabb.m_box[3] - m_aabb.m_box[0];
    float const sizeY = m_aabb.m_box[4] - m_aabb.m_box[1];
    float const sizeZ = m_aabb.m_box[5] - m_aabb.m_box[2];
    float const maxSize = (sizeZ > sizeX) ? sizeZ : sizeX;

    // NOTE: for anything that is not a vehicle or a gun the shipped code jumps
    // straight to the matrix product below with `tr` still uninitialised stack
    // memory. SetupModel only ever calls us for those types, so it never bites
    // in practice; preserved rather than papered over.
    CMatrix tr;
    if (obj->IsKindOf(&ai::Vehicle::m_classVehicle))
    {
        tr.identity();
        tr._41 = 0.0f;
        tr._42 = sizeY * 0.2f;
        tr._43 = maxSize * 1.8f;
    }
    else if (obj->IsKindOf(&ai::Gun::m_classGun) || obj->IsKindOf(&ai::CompoundGun::m_classCompoundGun))
    {
        tr.identity();
        tr._41 = 0.0f;
        tr._42 = sizeY * 0.1f;
        // Small guns get pushed away proportionally harder so they do not end up
        // filling the whole window.
        tr._43 = (2.0f / maxSize) + (maxSize * 2.0f);
    }

    CMatrix const res = rotX * rotY * tr;
    Rotation().FromMatrix(res);
    Translation() = CVector(res._41, res._42, res._43);
}

CVector ComplexModelWnd::getGunSize(ai::Gun* gun)
{
    // RVA 0x50EEF0 - the gun's own body plus its barrel, as one box.
    CVector baseMin(0.0f, 0.0f, 0.0f);
    CVector baseMax(0.0f, 0.0f, 0.0f);
    CVector barrelMin(0.0f, 0.0f, 0.0f);
    CVector barrelMax(0.0f, 0.0f, 0.0f);

    m3d::SgNode* const node = gun->m_Node;
    if (node)
    {
        baseMin =
            CVector(node->m_ownBoundingBox.m_box[0], node->m_ownBoundingBox.m_box[1], node->m_ownBoundingBox.m_box[2]);
        baseMax =
            CVector(node->m_ownBoundingBox.m_box[3], node->m_ownBoundingBox.m_box[4], node->m_ownBoundingBox.m_box[5]);
    }

    m3d::SgNode* const barrelNode = gun->GetBarrelNode();
    // NOTE: the shipped code re-tests the gun's own node here rather than
    // barrelNode, so a gun with a body but no barrel node dereferences null.
    // Preserved as shipped.
    if (node)
    {
        barrelMin = CVector(
            barrelNode->m_ownBoundingBox.m_box[0],
            barrelNode->m_ownBoundingBox.m_box[1],
            barrelNode->m_ownBoundingBox.m_box[2]);
        barrelMax = CVector(
            barrelNode->m_ownBoundingBox.m_box[3],
            barrelNode->m_ownBoundingBox.m_box[4],
            barrelNode->m_ownBoundingBox.m_box[5]);
    }

    return CVector(
        (barrelMax.x > baseMax.x ? barrelMax.x : baseMax.x) - (baseMin.x <= barrelMin.x ? baseMin.x : barrelMin.x),
        (barrelMax.y > baseMax.y ? barrelMax.y : baseMax.y) - (baseMin.y <= barrelMin.y ? baseMin.y : barrelMin.y),
        (barrelMax.z > baseMax.z ? barrelMax.z : baseMax.z) - (baseMin.z <= barrelMin.z ? baseMin.z : barrelMin.z));
}

void ComplexModelWnd::AddOneNode(m3d::SgNode* node, CMatrix const& InvMatr, Quaternion const& rot, bool withChilds)
{
    // RVA 0x50EA00 - takes the node's position from the composed matrix but
    // poses it with the caller's rotation.
    if (!node)
    {
        return;
    }
    node->UpdateXForm(false, true);
    CMatrix const fullMatr = node->m_currentXForm * InvMatr;
    CMatrix resMatr;
    resMatr.rotTranslate(rot, CVector(fullMatr._41, fullMatr._42, fullMatr._43));
    AddSgNode(node, resMatr, withChilds);
}

void ComplexModelWnd::AddOneNode(m3d::SgNode* node, CMatrix const& InvMatr, bool useRotation, bool withChilds)
{
    // RVA 0x50E050
    if (!node)
    {
        return;
    }
    node->UpdateXForm(false, true);
    CMatrix const fullMatr = node->m_currentXForm * InvMatr;
    CMatrix resMatr;
    if (useRotation)
    {
        resMatr = fullMatr;
    }
    else
    {
        // Position only - drop whatever orientation the node currently has.
        resMatr.identity();
        resMatr._41 = fullMatr._41;
        resMatr._42 = fullMatr._42;
        resMatr._43 = fullMatr._43;
    }
    AddSgNode(node, resMatr, withChilds);
}

void ComplexModelWnd::EmbraceGunAabb(Aabb& aabb, ai::Gun const* gun) const
{
    // RVA 0x5107B0
    if (!gun || gun->m_pGeoms.empty())
    {
        return;
    }
    aabb.EmbraceBox(gun->m_pGeoms[0]->GetAabb());

    m3d::SgNode* const barrelNode = gun->GetBarrelNode();
    if (!barrelNode)
    {
        return;
    }
    // The barrel's own box is node-local, so shift it out to where the barrel
    // actually sits before folding it in.
    CVector const origin = barrelNode->m_currentWorldOrigin;
    Aabb barrelBox;
    barrelBox.m_box[0] = barrelNode->m_ownBoundingBox.m_box[0] + origin.x;
    barrelBox.m_box[1] = barrelNode->m_ownBoundingBox.m_box[1] + origin.y;
    barrelBox.m_box[2] = barrelNode->m_ownBoundingBox.m_box[2] + origin.z;
    barrelBox.m_box[3] = barrelNode->m_ownBoundingBox.m_box[3] + origin.x;
    barrelBox.m_box[4] = barrelNode->m_ownBoundingBox.m_box[4] + origin.y;
    barrelBox.m_box[5] = barrelNode->m_ownBoundingBox.m_box[5] + origin.z;
    aabb.EmbraceBox(barrelBox);
}

void ComplexModelWnd::EmbraceCompoundGunAabb(Aabb& aabb, ai::CompoundGun const* cgun) const
{
    // RVA 0x5108C0
    if (!cgun)
    {
        return;
    }
    for (auto it = cgun->begin(); it != cgun->end(); ++it)
    {
        auto const* gun = static_cast<ai::Gun const*>(it->second.vp);
        if (gun->m_Node)
        {
            EmbraceGunAabb(aabb, gun);
        }
    }
}
