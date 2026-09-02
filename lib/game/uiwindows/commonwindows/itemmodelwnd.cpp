#include "itemmodelwnd.h"

#include "m3dapp.h"
#include "core/kernel.h"
#include "core/log.h"
#include "core/timer.h"

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
    if (modelName.empty())
    {
        GameDataClear(false);
        return 0;
    }

    RETRUXX_NOT_IMPLEMENTED;

    //if ((ItemByName = m3d::DataServer::GetItemByName(m3d::Application::g_pApp->m_serverAnimatedModels, m_charPtr, 1), ItemByName == -1))
    //{
    //    this->GameDataClear(this, 0);
    //    return 0;
    //}
    //mdl = 0;
    //m3d::Application::g_pApp->m_serverAnimatedModels->GetItemProperty(m3d::Application::g_pApp->m_serverAnimatedModels, ItemByName, 16394, &mdl);
    //v7 = this;
    //if (mdl)
    //{
    //    m3d::ui::ModelWnd::SetModel(this, mdl);
    //    if (this->m_Model && this->m_Animation)
    //    {
    //        m3d::ui::ModelWnd::SetCfgNum(this, cfgNumber);
    //        this->m_SkinNum = skinNumber;
    //        return 1;
    //    }
    //    v7 = this;
    //}
    //this->GameDataClear(v7, 0);
    //return 0;
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
    // TODO: rebuild the view matrix from m_rotationAngle + the (autosize/default)
    // translation, then feed m_Rotation / m_Translation. The shipped code inlines a
    // CMatrix x-rot * y-rot * translate chain and Quaternion::FromMatrix.
    RETRUXX_NOT_IMPLEMENTED;
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
    // TODO: check and refactor this
    if (IsValid())
    {
        auto v3 = m_Model->m_box.m_box[1];
        auto v4 = m_Model->m_box.m_box[3];
        auto v5 = m_Model->m_box.m_box[0];
        auto v6 = (float)((float)(v4 - v5) * 0.5) + v5;
        auto v7 = m_Model->m_box.m_box[4];
        auto v8 = (float)((float)(v7 - v3) * 0.5) + v3;
        auto v9 = m_Model->m_box.m_box[5];

        CVector center;
        center.z = (float)((float)(v9 - m_Model->m_box.m_box[2]) * 0.5) + m_Model->m_box.m_box[2];
        auto v10 = v7 - v3;
        auto v11 = v9 - m_Model->m_box.m_box[2];
        auto v12 = v4 - v5;

        CVector itemSize;
        itemSize.x = v12;
        itemSize.y = v10;
        itemSize.z = v11;
        auto p_y = (CVector*)&itemSize.y;
        if (v10 <= v11)
            p_y = (CVector*)&itemSize.z;
        if (p_y->x <= v12)
            p_y = &itemSize;
        auto x = p_y->x;
        auto p_itemSize = &itemSize;
        if (v12 <= v11)
            p_itemSize = (CVector*)&itemSize.z;
        auto v16 = 3.0;
        if (v10 <= p_itemSize->x)
            v16 = 2.2;
        auto v17 = (float)(0.0 - center.z) + (float)((float)(v16 * x) + (float)(2.5 / x));
        translation.x = 0.0 - v6;
        translation.y = (float)(0.0 - v8) + (float)(v10 * 0.1);
        translation.z = v17;
    }
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

int ItemModelWnd::GameDataUpdate(void*, int)
{
    // TODO: implement GameDataUpdate
    //  RETRUXX_NOT_IMPLEMENTED;
    return 0;
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
