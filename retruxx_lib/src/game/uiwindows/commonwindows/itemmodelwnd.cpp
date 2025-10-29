#include "itemmodelwnd.h"
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

int ItemModelWnd::SetModelByName(CStr const&, unsigned, unsigned)
{
    RETRUXX_NOT_IMPLEMENTED;
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
    RETRUXX_NOT_IMPLEMENTED;
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
}

int ItemModelWnd::OnMouseButton0(unsigned, PointBase<float> const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool ItemModelWnd::IsValid() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void ItemModelWnd::SetRotationByHandMode(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void ItemModelWnd::CalcAutosizeTranslation(CVector&) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ItemModelWnd::OnBeforeAddToWndStation()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ItemModelWnd::GameDataUpdate(void*, int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ItemModelWnd::GameDataClear(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
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
