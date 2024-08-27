#include "gadgetwnd.h"

RT_CLASS_EXPORTS_BEGIN(GadgetWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(GadgetWnd);

int GadgetWnd::GetSlotId() const
{
    throw std::logic_error("Not implemented");
}

m3d::Class* GadgetWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(ItemWnd);
}

void GadgetWnd::SetSlotId(int)
{
    throw std::logic_error("Not implemented");
}

int GadgetWnd::CreateFromPattern(m3d::ui::Wnd const*, bool)
{
    throw std::logic_error("Not implemented");
}

m3d::Class* GadgetWnd::GetClass() const
{
    throw std::logic_error("Not implemented");
}

m3d::Object* GadgetWnd::CreateObject()
{
    throw std::logic_error("Not implemented");
}

BoundsBase<float> GadgetWnd::GeomToWndBounds(BoundsBase<int> const&)
{
    throw std::logic_error("Not implemented");
}

void GadgetWnd::SetResourceName(CStr const&)
{
    throw std::logic_error("Not implemented");
}

m3d::Object* GadgetWnd::Clone()
{
    throw std::logic_error("Not implemented");
}

GadgetWnd::~GadgetWnd()
{
    throw std::logic_error("Not implemented");
}

int GadgetWnd::GetResourceId() const
{
    throw std::logic_error("Not implemented");
}

CStr const& GadgetWnd::GetResourceName() const
{
    throw std::logic_error("Not implemented");
}

void GadgetWnd::UpdateItemValue()
{
    throw std::logic_error("Not implemented");
}

void GadgetWnd::DrawItemIco(m3d::ui::DrawInfo const&)
{
    throw std::logic_error("Not implemented");
}

bool GadgetWnd::IsValid() const
{
    throw std::logic_error("Not implemented");
}

bool GadgetWnd::CanContainItem() const
{
    throw std::logic_error("Not implemented");
}

void GadgetWnd::LaunchEventOnItemDeactivation() const
{
    throw std::logic_error("Not implemented");
}

int GadgetWnd::OnPaint(m3d::ui::DrawInfo const&)
{
    throw std::logic_error("Not implemented");
}

int GadgetWnd::GiveUpItem(ai::GeomRepositoryItem const&, m3d::ui::Wnd*)
{
    throw std::logic_error("Not implemented");
}

void GadgetWnd::SetupInfoWnd(ItemInfoWnd*)
{
    throw std::logic_error("Not implemented");
}

void GadgetWnd::DrawBackground(m3d::ui::DrawInfo const&)
{
    throw std::logic_error("Not implemented");
}

GadgetWnd::GadgetWnd(GadgetWnd const&)
{
    throw std::logic_error("Not implemented");
}

GadgetWnd::GadgetWnd()
{
    throw std::logic_error("Not implemented");
}

void GadgetWnd::DrawFrame(m3d::ui::DrawInfo const&)
{
    throw std::logic_error("Not implemented");
}

void GadgetWnd::DrawSelection(m3d::ui::DrawInfo const&)
{
    throw std::logic_error("Not implemented");
}

bool GadgetWnd::SetItemObjId(int)
{
    throw std::logic_error("Not implemented");
}

int GadgetWnd::GameDataUpdate(void*, int)
{
    throw std::logic_error("Not implemented");
}

bool GadgetWnd::NeedUpdateInfoWnd(ItemInfoWnd*) const
{
    throw std::logic_error("Not implemented");
}
