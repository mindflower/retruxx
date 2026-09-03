#include "itemmodelwnd.h"

#include <cmath>

#include "m3dapp.h"
#include "core/kernel.h"
#include "core/log.h"
#include "core/timer.h"
#include "scene/servers/dataserver.h"

RT_CLASS_EXPORTS_BEGIN(ItemModelWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(ItemModelWnd);

bool ItemModelWnd::IsAllowedRotateByHandX() const
{
    return m_bAllowRotateByHandX;
}

bool ItemModelWnd::IsAllowedRotateByHandY() const
{
    return m_bAllowRotateByHandY;
}

m3d::Object* ItemModelWnd::Clone()
{
    return new ItemModelWnd;
}

void ItemModelWnd::AllowRotate(bool bAllow)
{
    m_bAllowRotate = bAllow;
}

m3d::Class* ItemModelWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(ModelWnd);
}

float ItemModelWnd::GetRotationVelocity() const
{
    return m_rotationVelocity;
}

ItemModelWnd::~ItemModelWnd()
{
    // ~ModelWnd runs via the compiler-chained base destructor.
}

bool ItemModelWnd::IsAutosized() const
{
    return m_bAutosized;
}

void ItemModelWnd::SetRotationVelocity(float velocity)
{
    m_rotationVelocity = velocity;
}

m3d::Object* ItemModelWnd::CreateObject()
{
    return new ItemModelWnd;
}

int ItemModelWnd::SetModelByName(CStr const& modelName, unsigned skinNumber, unsigned cfgNumber)
{
    // RVA 0x514E10
    auto& server = M3D_APP->GetAnimatedModelsServer();
    int const itemId = modelName.empty() ? -1 : server.GetItemByName(modelName.c_str(), true);
    if (itemId == -1)
    {
        GameDataClear(false);
        return 0;
    }

    m3d::AnimatedModel* mdl = nullptr;
    server.GetItemProperty(itemId, m3d::PROP_INTERNAL_GETMODEL, &mdl);
    if (mdl)
    {
        ModelWnd::SetModel(mdl);
        if (m_Model && m_Animation)
        {
            ModelWnd::SetCfgNum(cfgNumber);
            m_SkinNum = skinNumber;
            return 1;
        }
    }
    GameDataClear(false);
    return 0;
}

int ItemModelWnd::CreateFromPattern(m3d::ui::Wnd* patterWnd, bool deleteSrc)
{
    if (patterWnd && patterWnd->IsKindOf(RT_CLASS_LOCAL(ModelWnd)))
    {
        auto patternModelWnd = dynamic_cast<ModelWnd*>(patterWnd);
        if (CreateModelWnd(
                patternModelWnd->GetImage(),
                patterWnd->GetStyle(),
                patterWnd->GetBounds(),
                patterWnd->GetId(),
                patternModelWnd->GetTargetTexture()))
        {
            if (auto parent = patternModelWnd->GetParent())
            {
                parent->AddChild(this);
                parent->MoveChildToFirstPosition(this);
                SetStyle(patternModelWnd->GetStyle());
                SetText(patternModelWnd->GetText());
                SetId(patternModelWnd->GetId());
                SetName(patternModelWnd->GetName());
                SetBounds(patternModelWnd->GetBounds(), true);
                SetDefaultFont(patternModelWnd->GetDefaultFont());
                SetWrapMode(patternModelWnd->GetWrapMode());
                SetFormatMode(patternModelWnd->GetFormatMode());
                SetColor(patternModelWnd->GetColor());
                SetTextColor(patternModelWnd->GetTextColor());
                SetTextColorDisabled(patternModelWnd->GetTextColorDisabled());
                SetClientEdges(patternModelWnd->GetClientEdges());
                SetPane(patternModelWnd->GetPaneName());
                SetPaneFlags(patternModelWnd->GetPaneFlags());
                SetScrollPane(patternModelWnd->GetScrollPaneName());
                SetBackground(patternModelWnd->GetBackground());

                CStr tooltip;
                patternModelWnd->GetProperty(0x4000, &tooltip);
                SetProperty(0x4000, &tooltip);

                SetOnShowAnimation(patternModelWnd->GetOnShowAnimation());
                SetOnHideAnimation(patternModelWnd->GetOnHideAnimation());
                SetImage(patternModelWnd->GetImage());
                Translation() = patternModelWnd->Translation();
                Rotation() = patternModelWnd->Rotation();
                Scale() = patternModelWnd->Scale();
                m_defaultTranslation = Translation();
                if (deleteSrc)
                {
                    delete patterWnd;
                    patterWnd = nullptr;
                }
                m_gameDataFlags |= 1u;
                return 1;
            }
            else
            {
                M3D_LOG_INFO("ItemModelWnd: fail to init - bad pattern window");
                return 0;
            }
        }
        else
        {
            M3D_LOG_INFO("ItemModelWnd::CreateFromPattern error - cannot create widow");
            return 0;
        }
    }
    M3D_LOG_INFO("ItemModelWnd: fail to init - bad pattern window");
    return 0;
}

void ItemModelWnd::SetRotationByHandVelocity(float velocity)
{
    m_rotationByHandVelocity = velocity;
}

float ItemModelWnd::GetRotationByHandVelocity() const
{
    return m_rotationByHandVelocity;
}

void ItemModelWnd::AllowRotateByHandX(bool bAllow)
{
    m_bAllowRotateByHandX = bAllow;
}

CVector const& ItemModelWnd::GetDefaultTranslation() const
{
    return m_defaultTranslation;
}

void ItemModelWnd::SetAutosized(bool bAutosized)
{
    m_bAutosized = bAutosized;
    if (!IsChildOf(m3d::Application::g_pApp))
    {
        return;
    }
    if (m_bAutosized)
    {
        CalcAutosizeTranslation(m_Translation);
    }
    else
    {
        m_Translation = m_defaultTranslation;
    }
}

void ItemModelWnd::AllowRotateByHandY(bool bAllow)
{
    m_bAllowRotateByHandY = bAllow;
}

m3d::Class* ItemModelWnd::GetClass() const
{
    return RT_CLASS_LOCAL(ItemModelWnd);
}

void ItemModelWnd::SetDefaultRotationAngleX(float angle)
{
    m_defaultRotationAngleX = angle;
}

void ItemModelWnd::SetDefaultTranslation(CVector const& translation)
{
    m_defaultTranslation = translation;
    if (!IsChildOf(m3d::Application::g_pApp))
    {
        return;
    }
    if (m_bAutosized)
    {
        CalcAutosizeTranslation(m_Translation);
    }
    else
    {
        m_Translation = m_defaultTranslation;
    }
}

bool ItemModelWnd::IsAllowedRotate() const
{
    return m_bAllowRotate;
}

float ItemModelWnd::GetDefaultRotationAngleX() const
{
    return m_defaultRotationAngleX;
}

int ItemModelWnd::OnAfterRemoveFromWndStation()
{
    auto res = Wnd::OnAfterRemoveFromWndStation();
    if (m_bInRotationByHandMode)
    {
        m_bInRotationByHandMode = false;
        M3D_APP->CaptureMouse(nullptr);
    }
    return res;
}

void ItemModelWnd::UpdateCamera()
{
    // RVA 0x514F60
    if (!IsValid())
    {
        return;
    }
    if ((m_style & 2) != 0 || (m_style & 0x80000) != 0)
    {
        return;
    }

    CVector translation;
    if (m_bAutosized)
    {
        CalcAutosizeTranslation(translation);
    }
    else
    {
        translation = m_defaultTranslation;
    }

    // Rotation about the X axis by m_rotationAngle.x.
    CMatrix rotX;
    rotX.identity();
    float const cx = std::cos(m_rotationAngle.x);
    float const sx = std::sin(m_rotationAngle.x);
    rotX._22 = cx;
    rotX._23 = sx;
    rotX._32 = -sx;
    rotX._33 = cx;

    // Rotation about the Y axis by m_rotationAngle.y.
    CMatrix rotY;
    rotY.identity();
    float const cy = std::cos(m_rotationAngle.y);
    float const sy = std::sin(m_rotationAngle.y);
    rotY._11 = cy;
    rotY._13 = -sy;
    rotY._31 = sy;
    rotY._33 = cy;

    // The shipped code composes res = (rotX * rotY) * translate(translation) and
    // then splits res back into m_Rotation / m_Translation. rotX * rotY is a pure
    // rotation, so the trailing translate leaves the 3x3 (hence the quaternion)
    // untouched and simply copies translation into the 4th row.
    CMatrix const rot = rotX * rotY;
    m_Rotation.FromMatrix(rot);
    m_Translation = translation;
}

bool ItemModelWnd::IsDisabled() const
{
    return (m_style & 2) != 0 || (m_style & 0x80000) != 0;
}

ItemModelWnd::ItemModelWnd(ItemModelWnd const&) : ItemModelWnd()
{
}

ItemModelWnd::ItemModelWnd()
{
    m_defaultRotationAngleX = -0.52359879;
    m_rotationAngle.x = -0.52359879;
    m_rotationAngle.y = 0.0;
    m_rotationVelocity = 0.17453294;
    m_bAllowRotate = 1;
    m_bAllowRotateByHandY = 1;
    m_bAutosized = 1;
    m_rotationByHandVelocity = 0.17453294;
    m_bAllowRotateByHandX = 0;
    m_bInRotationByHandMode = 0;
    m_defaultTranslation = ZeroVector;
}

int ItemModelWnd::OnMouseButton0(unsigned state, PointBase<float> const& at)
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
    return Wnd::OnMouseButton0(state, at);
}

bool ItemModelWnd::IsValid() const
{
    return m_Model && m_Animation;
}

void ItemModelWnd::SetRotationByHandMode(bool bState)
{
    m_bInRotationByHandMode = bState;
    M3D_APP->CaptureMouse(bState ? this : nullptr);
}

void ItemModelWnd::CalcAutosizeTranslation(CVector& translation) const
{
    // RVA 0x5157E0: frame the model in front of the camera from its bounding box.
    if (!IsValid())
    {
        return;
    }

    // m_box.m_box layout: [minX, minY, minZ, maxX, maxY, maxZ].
    float const* const box = m_Model->m_box.m_box;
    float const centerX = (box[3] - box[0]) * 0.5f + box[0];
    float const centerY = (box[4] - box[1]) * 0.5f + box[1];
    float const centerZ = (box[5] - box[2]) * 0.5f + box[2];
    float const sizeX = box[3] - box[0];
    float const sizeY = box[4] - box[1];
    float const sizeZ = box[5] - box[2];

    // Largest of the three box dimensions.
    float maxDim = (sizeY <= sizeZ) ? sizeZ : sizeY;
    if (maxDim <= sizeX)
    {
        maxDim = sizeX;
    }

    // Pull the model further back (3.0) unless it is no taller than it is wide or
    // deep, in which case a tighter 2.2 is enough.
    float const depthScale = (sizeY <= ((sizeX <= sizeZ) ? sizeZ : sizeX)) ? 2.2f : 3.0f;

    translation.x = -centerX;
    translation.y = -centerY + sizeY * 0.1f;
    translation.z = -centerZ + (depthScale * maxDim + 2.5f / maxDim);
}

int ItemModelWnd::OnBeforeAddToWndStation()
{
    if (m_bInRotationByHandMode)
    {
        m_bInRotationByHandMode = false;
        M3D_APP->CaptureMouse(nullptr);
    }
    if (m_bAllowRotateByHandX)
    {
        m_rotationAngle.x = m_defaultRotationAngleX;
    }
    if (m_bAutosized)
    {
        CalcAutosizeTranslation(m_Translation);
    }
    else
    {
        m_Translation.x = m_defaultTranslation.x;
        m_Translation.y = m_defaultTranslation.y;
        m_Translation.z = m_defaultTranslation.z;
    }
    return Wnd::OnBeforeAddToWndStation();
}

int ItemModelWnd::GameDataUpdate(void* data, int dataType)
{
    // RVA 0x514EE0
    if (dataType == 89)
    {
        OnNewFrame();
    }
    return 1;
}

int ItemModelWnd::GameDataClear(bool beforeContinuousLevel)
{
    delete m_Animation;
    m_Animation = nullptr;
    m_Model = nullptr;
    m_cfg.m_num = 0;
    m_SkinNum = 0;
    return 1;
}

int ItemModelWnd::OnMouseMove(PointBase<float> const& pt, PointBase<float> const& deltas)
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
    return Wnd::OnMouseMove(pt, deltas);
}

int ItemModelWnd::OnNewFrame()
{
    if (!IsValid())
    {
        return 0;
    }
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

void ItemModelWnd::UpdateRotationAngle()
{
    m_rotationAngle.y += static_cast<float>(
        static_cast<double>(M3D_KERNEL->GetTimer().GetLastFrameTimeUnscaled()) * m_rotationVelocity * 0.001);
}
