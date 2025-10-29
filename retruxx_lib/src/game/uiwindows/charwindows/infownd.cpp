#include "infownd.h"

RT_CLASS_EXPORTS_BEGIN(InfoWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(InfoWnd);

InfoWnd::InfoItem::~InfoItem()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void InfoWnd::InfoItem::Draw(m3d::ui::DrawInfo const&, BoundsBase<float> const&) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void InfoWnd::InfoItem::SetFlags(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int InfoWnd::InfoItem::GetFlags() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void InfoWnd::InfoItem::SetString(CStr)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int InfoWnd::InfoItem::GetId() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void InfoWnd::InfoItem::SetIco(m3d::rend::TexHandle)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void InfoWnd::InfoItem::SetIcoSize(PointBase<float> const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

CStr const& InfoWnd::InfoItem::GetString() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

InfoWnd::InfoItem::InfoItem(InfoWnd::InfoItem const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

InfoWnd::InfoItem::InfoItem(int, CStr const&, m3d::rend::TexHandle, PointBase<float>)
{
    RETRUXX_NOT_IMPLEMENTED;
}

InfoWnd::InfoItem& InfoWnd::InfoItem::operator=(InfoWnd::InfoItem const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::rend::TexHandle InfoWnd::InfoItem::GetIco() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

PointBase<float> const& InfoWnd::InfoItem::GetIcoSize() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

PointBase<float> InfoWnd::InfoItem::GetDrawSize() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void InfoWnd::InfoItem::CalculateStringSize()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void InfoWnd::InfoItem::SetId(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

InfoWnd::InfoItem* InfoWnd::GetItemById(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* InfoWnd::CreateObject()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int InfoWnd::AddItemById(int, InfoItem const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

InfoWnd::~InfoWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* InfoWnd::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void InfoWnd::SetNumberOfColumns(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

InfoWnd::InfoWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

InfoWnd::InfoWnd(InfoWnd const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int InfoWnd::GameDataClear(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int InfoWnd::OnPaint(m3d::ui::DrawInfo const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}
