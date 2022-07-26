#include "itemmodelwnd.h"

#include "core/log.h"

RT_CLASS_DEFINE(ItemModelWnd);

bool ItemModelWnd::IsAllowedRotateByHandX() const
{
    throw std::logic_error("Not implemented");
}

bool ItemModelWnd::IsAllowedRotateByHandY() const
{
    throw std::logic_error("Not implemented");
}

m3d::Object* ItemModelWnd::Clone()
{
    throw std::logic_error("Not implemented");
}

void ItemModelWnd::AllowRotate(bool)
{
    throw std::logic_error("Not implemented");
}

m3d::Class* ItemModelWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(ModelWnd);
}

float ItemModelWnd::GetRotationVelocity() const
{
    throw std::logic_error("Not implemented");
}

ItemModelWnd::~ItemModelWnd()
{
    throw std::logic_error("Not implemented");
}

bool ItemModelWnd::IsAutosized() const
{
    throw std::logic_error("Not implemented");
}

void ItemModelWnd::SetRotationVelocity(float)
{
    throw std::logic_error("Not implemented");
}

m3d::Object* ItemModelWnd::CreateObject()
{
    return new ItemModelWnd;
}

int ItemModelWnd::SetModelByName(CStr const&, unsigned, unsigned)
{
    throw std::logic_error("Not implemented");
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
    throw std::logic_error("Not implemented");
}

float ItemModelWnd::GetRotationByHandVelocity() const
{
    throw std::logic_error("Not implemented");
}

void ItemModelWnd::AllowRotateByHandX(bool)
{
    throw std::logic_error("Not implemented");
}

CVector const& ItemModelWnd::GetDefaultTranslation() const
{
    throw std::logic_error("Not implemented");
}

void ItemModelWnd::SetAutosized(bool)
{
    throw std::logic_error("Not implemented");
}

void ItemModelWnd::AllowRotateByHandY(bool)
{
    throw std::logic_error("Not implemented");
}

m3d::Class* ItemModelWnd::GetClass() const
{
    return RT_CLASS_LOCAL(ItemModelWnd);
}

void ItemModelWnd::SetDefaultRotationAngleX(float)
{
    throw std::logic_error("Not implemented");
}

void ItemModelWnd::SetDefaultTranslation(CVector const&)
{
    throw std::logic_error("Not implemented");
}

bool ItemModelWnd::IsAllowedRotate() const
{
    throw std::logic_error("Not implemented");
}

float ItemModelWnd::GetDefaultRotationAngleX() const
{
    throw std::logic_error("Not implemented");
}

int ItemModelWnd::OnAfterRemoveFromWndStation()
{
    throw std::logic_error("Not implemented");
}

void ItemModelWnd::UpdateCamera()
{
    throw std::logic_error("Not implemented");
}

bool ItemModelWnd::IsDisabled() const
{
    throw std::logic_error("Not implemented");
}

ItemModelWnd::ItemModelWnd(ItemModelWnd const&)
{
    throw std::logic_error("Not implemented");
}

ItemModelWnd::ItemModelWnd()
{
}

int ItemModelWnd::OnMouseButton0(unsigned, PointBase<float> const&)
{
    throw std::logic_error("Not implemented");
}

bool ItemModelWnd::IsValid() const
{
    throw std::logic_error("Not implemented");
}

void ItemModelWnd::SetRotationByHandMode(bool)
{
    throw std::logic_error("Not implemented");
}

void ItemModelWnd::CalcAutosizeTranslation(CVector&) const
{
    throw std::logic_error("Not implemented");
}

int ItemModelWnd::OnBeforeAddToWndStation()
{
    throw std::logic_error("Not implemented");
}

int ItemModelWnd::GameDataUpdate(void*, int)
{
    throw std::logic_error("Not implemented");
}

int ItemModelWnd::GameDataClear(bool)
{
    throw std::logic_error("Not implemented");
}

int ItemModelWnd::OnMouseMove(PointBase<float> const&, PointBase<float> const&)
{
    throw std::logic_error("Not implemented");
}

int ItemModelWnd::OnNewFrame()
{
    throw std::logic_error("Not implemented");
}

void ItemModelWnd::UpdateRotationAngle()
{
    throw std::logic_error("Not implemented");
}
