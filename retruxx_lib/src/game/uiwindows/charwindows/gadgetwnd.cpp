#include "gadgetwnd.h"

RT_CLASS_EXPORTS_BEGIN(GadgetWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(GadgetWnd);

int GadgetWnd::GetSlotId() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* GadgetWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(ItemWnd);
}

void GadgetWnd::SetSlotId(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int GadgetWnd::CreateFromPattern(m3d::ui::Wnd const*, bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* GadgetWnd::GetClass() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* GadgetWnd::CreateObject()
{
    RETRUXX_NOT_IMPLEMENTED;
}

BoundsBase<float> GadgetWnd::GeomToWndBounds(BoundsBase<int> const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void GadgetWnd::SetResourceName(CStr const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* GadgetWnd::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

GadgetWnd::~GadgetWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int GadgetWnd::GetResourceId() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

CStr const& GadgetWnd::GetResourceName() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void GadgetWnd::UpdateItemValue()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void GadgetWnd::DrawItemIco(m3d::ui::DrawInfo const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool GadgetWnd::IsValid() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool GadgetWnd::CanContainItem() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void GadgetWnd::LaunchEventOnItemDeactivation() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int GadgetWnd::OnPaint(m3d::ui::DrawInfo const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int GadgetWnd::GiveUpItem(ai::GeomRepositoryItem const&, m3d::ui::Wnd*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void GadgetWnd::SetupInfoWnd(ItemInfoWnd*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void GadgetWnd::DrawBackground(m3d::ui::DrawInfo const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

GadgetWnd::GadgetWnd(GadgetWnd const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

GadgetWnd::GadgetWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void GadgetWnd::DrawFrame(m3d::ui::DrawInfo const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void GadgetWnd::DrawSelection(m3d::ui::DrawInfo const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool GadgetWnd::SetItemObjId(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int GadgetWnd::GameDataUpdate(void*, int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool GadgetWnd::NeedUpdateInfoWnd(ItemInfoWnd*) const
{
    RETRUXX_NOT_IMPLEMENTED;
}
