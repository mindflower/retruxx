#include "infownd.h"

RT_CLASS_EXPORTS_BEGIN(InfoWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(InfoWnd);

InfoWnd::InfoItem::~InfoItem()
{
    throw std::logic_error("Not implemented");
}

void InfoWnd::InfoItem::Draw(m3d::ui::DrawInfo const&, BoundsBase<float> const&) const
{
    throw std::logic_error("Not implemented");
}

void InfoWnd::InfoItem::SetFlags(int)
{
    throw std::logic_error("Not implemented");
}

int InfoWnd::InfoItem::GetFlags() const
{
    throw std::logic_error("Not implemented");
}

void InfoWnd::InfoItem::SetString(CStr)
{
    throw std::logic_error("Not implemented");
}

int InfoWnd::InfoItem::GetId() const
{
    throw std::logic_error("Not implemented");
}

void InfoWnd::InfoItem::SetIco(m3d::rend::TexHandle)
{
    throw std::logic_error("Not implemented");
}

void InfoWnd::InfoItem::SetIcoSize(PointBase<float> const&)
{
    throw std::logic_error("Not implemented");
}

CStr const& InfoWnd::InfoItem::GetString() const
{
    throw std::logic_error("Not implemented");
}

InfoWnd::InfoItem::InfoItem(InfoWnd::InfoItem const&)
{
    throw std::logic_error("Not implemented");
}

InfoWnd::InfoItem::InfoItem(int, CStr const&, m3d::rend::TexHandle, PointBase<float>)
{
    throw std::logic_error("Not implemented");
}

InfoWnd::InfoItem& InfoWnd::InfoItem::operator=(InfoWnd::InfoItem const&)
{
    throw std::logic_error("Not implemented");
}

m3d::rend::TexHandle InfoWnd::InfoItem::GetIco() const
{
    throw std::logic_error("Not implemented");
}

PointBase<float> const& InfoWnd::InfoItem::GetIcoSize() const
{
    throw std::logic_error("Not implemented");
}

PointBase<float> InfoWnd::InfoItem::GetDrawSize() const
{
    throw std::logic_error("Not implemented");
}

void InfoWnd::InfoItem::CalculateStringSize()
{
    throw std::logic_error("Not implemented");
}

void InfoWnd::InfoItem::SetId(int)
{
    throw std::logic_error("Not implemented");
}

InfoWnd::InfoItem* InfoWnd::GetItemById(int)
{
    throw std::logic_error("Not implemented");
}

m3d::Object* InfoWnd::CreateObject()
{
    throw std::logic_error("Not implemented");
}

int InfoWnd::AddItemById(int, InfoItem const&)
{
    throw std::logic_error("Not implemented");
}

InfoWnd::~InfoWnd()
{
    throw std::logic_error("Not implemented");
}

m3d::Object* InfoWnd::Clone()
{
    throw std::logic_error("Not implemented");
}

void InfoWnd::SetNumberOfColumns(int)
{
    throw std::logic_error("Not implemented");
}

InfoWnd::InfoWnd()
{
    throw std::logic_error("Not implemented");
}

InfoWnd::InfoWnd(InfoWnd const&)
{
    throw std::logic_error("Not implemented");
}

int InfoWnd::GameDataClear(bool)
{
    throw std::logic_error("Not implemented");
}

int InfoWnd::OnPaint(m3d::ui::DrawInfo const&)
{
    throw std::logic_error("Not implemented");
}
