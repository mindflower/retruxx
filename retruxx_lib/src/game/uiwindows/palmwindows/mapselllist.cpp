#include "mapselllist.h"

RT_CLASS_EXPORTS_BEGIN(MapSellItem)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(MapSellItem);

MapSellItem::AuxInfo::AuxInfo()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* MapSellItem::GetClass() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int MapSellItem::GetWarePrototypeId() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int MapSellItem::SetUp(int, int, int, PointBase<float> const&, float, bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* MapSellItem::CreateObject()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int MapSellItem::GetBuyPrice() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* MapSellItem::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int MapSellItem::GetSellPrice() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* MapSellItem::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

MapSellItem::~MapSellItem()
{
    RETRUXX_NOT_IMPLEMENTED;
}

MapSellItem::MapSellItem(MapSellItem const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

MapSellItem::MapSellItem()
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORTS_BEGIN(MapSellList)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(MapSellList);

m3d::Class* MapSellList::GetClass() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* MapSellList::CreateObject()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* MapSellList::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* MapSellList::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

int MapSellList::CreateFromPattern(m3d::ui::Wnd const*, bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

MapSellList::~MapSellList()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int MapSellList::SetUpForObject(ObjectInfo*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int MapSellList::OnBeforeAddToWndStation()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int MapSellList::CreateItemsFromSaveList()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int MapSellList::CompareItem(int, int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int MapSellList::CreateItemsFromRealObject()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int MapSellList::CreateItem(int, int, int, bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int MapSellList::MeasureItem(int, BoundsBase<float>&) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int MapSellList::DeleteItem(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

MapSellList::MapSellList(MapSellList const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

MapSellList::MapSellList()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int MapSellList::RenderItem(int, PointBase<float> const&, m3d::ui::DrawInfo const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int MapSellList::GameDataClear(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}
