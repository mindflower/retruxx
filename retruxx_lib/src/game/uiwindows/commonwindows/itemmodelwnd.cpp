#include "itemmodelwnd.h"

#include "m3dapp.h"
#include "core/log.h"

RT_CLASS_EXPORTS_BEGIN(ItemModelWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(ItemModelWnd);

bool ItemModelWnd::IsAllowedRotateByHandX() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool ItemModelWnd::IsAllowedRotateByHandY() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* ItemModelWnd::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void ItemModelWnd::AllowRotate(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* ItemModelWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(ModelWnd);
}

float ItemModelWnd::GetRotationVelocity() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

ItemModelWnd::~ItemModelWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool ItemModelWnd::IsAutosized() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void ItemModelWnd::SetRotationVelocity(float)
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* ItemModelWnd::CreateObject()
{
    return new ItemModelWnd;
}

int ItemModelWnd::SetModelByName(const CStr& modelName, unsigned skinNumber, unsigned cfgNumber)
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
            patternModelWnd->GetTargetTexture()
        ))
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

void ItemModelWnd::SetRotationByHandVelocity(float)
{
    RETRUXX_NOT_IMPLEMENTED;
}

float ItemModelWnd::GetRotationByHandVelocity() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void ItemModelWnd::AllowRotateByHandX(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

CVector const& ItemModelWnd::GetDefaultTranslation() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void ItemModelWnd::SetAutosized(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void ItemModelWnd::AllowRotateByHandY(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* ItemModelWnd::GetClass() const
{
    return RT_CLASS_LOCAL(ItemModelWnd);
}

void ItemModelWnd::SetDefaultRotationAngleX(float)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void ItemModelWnd::SetDefaultTranslation(CVector const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool ItemModelWnd::IsAllowedRotate() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

float ItemModelWnd::GetDefaultRotationAngleX() const
{
    RETRUXX_NOT_IMPLEMENTED;
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
    RETRUXX_NOT_IMPLEMENTED;
}

bool ItemModelWnd::IsDisabled() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

ItemModelWnd::ItemModelWnd(ItemModelWnd const&)
{
    RETRUXX_NOT_IMPLEMENTED;
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

int ItemModelWnd::OnMouseButton0(unsigned, PointBase<float> const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool ItemModelWnd::IsValid() const
{
    return m_Model && m_Animation;
}

void ItemModelWnd::SetRotationByHandMode(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
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
    RETRUXX_NOT_IMPLEMENTED;
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

int ItemModelWnd::OnMouseMove(PointBase<float> const&, PointBase<float> const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ItemModelWnd::OnNewFrame()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void ItemModelWnd::UpdateRotationAngle()
{
    RETRUXX_NOT_IMPLEMENTED;
}
