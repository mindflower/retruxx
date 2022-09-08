#include "mapselllist.h"

RT_CLASS_EXPORTS_BEGIN(MapSellItem)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(MapSellItem);

MapSellItem::AuxInfo::AuxInfo()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* MapSellItem::GetClass() const
{
    throw std::logic_error("Not implemented");
}

int MapSellItem::GetWarePrototypeId() const
{
    throw std::logic_error("Not implemented");
}

int MapSellItem::SetUp(int, int, int, PointBase<float> const&, float, bool)
{
    throw std::logic_error("Not implemented");
}

m3d::Object* MapSellItem::CreateObject()
{
    throw std::logic_error("Not implemented");
}

int MapSellItem::GetBuyPrice() const
{
    throw std::logic_error("Not implemented");
}

m3d::Object* MapSellItem::Clone()
{
    throw std::logic_error("Not implemented");
}

int MapSellItem::GetSellPrice() const
{
    throw std::logic_error("Not implemented");
}

m3d::Class* MapSellItem::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

MapSellItem::~MapSellItem()
{
    throw std::logic_error("Not implemented");
}

MapSellItem::MapSellItem(MapSellItem const&)
{
    throw std::logic_error("Not implemented");
}

MapSellItem::MapSellItem()
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORTS_BEGIN(MapSellList)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(MapSellList);

m3d::Class* MapSellList::GetClass() const
{
    throw std::logic_error("Not implemented");
}

m3d::Object* MapSellList::CreateObject()
{
    throw std::logic_error("Not implemented");
}

m3d::Object* MapSellList::Clone()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* MapSellList::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

int MapSellList::CreateFromPattern(m3d::ui::Wnd const*, bool)
{
    throw std::logic_error("Not implemented");
}

MapSellList::~MapSellList()
{
    throw std::logic_error("Not implemented");
}

int MapSellList::SetUpForObject(ObjectInfo*)
{
    throw std::logic_error("Not implemented");
}

int MapSellList::OnBeforeAddToWndStation()
{
    throw std::logic_error("Not implemented");
}

int MapSellList::CreateItemsFromSaveList()
{
    throw std::logic_error("Not implemented");
}

int MapSellList::CompareItem(int, int)
{
    throw std::logic_error("Not implemented");
}

int MapSellList::CreateItemsFromRealObject()
{
    throw std::logic_error("Not implemented");
}

int MapSellList::CreateItem(int, int, int, bool)
{
    throw std::logic_error("Not implemented");
}

int MapSellList::MeasureItem(int, BoundsBase<float>&) const
{
    throw std::logic_error("Not implemented");
}

int MapSellList::DeleteItem(int)
{
    throw std::logic_error("Not implemented");
}

MapSellList::MapSellList(MapSellList const&)
{
    throw std::logic_error("Not implemented");
}

MapSellList::MapSellList()
{
    throw std::logic_error("Not implemented");
}

int MapSellList::RenderItem(int, PointBase<float> const&, m3d::ui::DrawInfo const&)
{
    throw std::logic_error("Not implemented");
}

int MapSellList::GameDataClear(bool)
{
    throw std::logic_error("Not implemented");
}
