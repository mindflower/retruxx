#include "iteminfownd.h"

RT_CLASS_EXPORTS_BEGIN(ItemInfoWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(ItemInfoWnd);

m3d::Object* ItemInfoWnd::CreateObject()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* ItemInfoWnd::GetClass() const
{
    throw std::logic_error("Not implemented");
}

ai::GeomRepositoryItem const& ItemInfoWnd::GetItem() const
{
    throw std::logic_error("Not implemented");
}

void ItemInfoWnd::SetUpForRepositoryItem(ai::GeomRepositoryItem const&, ref_ptr<m3d::ui::Wnd>)
{
    throw std::logic_error("Not implemented");
}

void ItemInfoWnd::SetUpForObject(int, ref_ptr<m3d::ui::Wnd>)
{
    throw std::logic_error("Not implemented");
}

ref_ptr<m3d::ui::Wnd> ItemInfoWnd::GetSrcWnd() const
{
    throw std::logic_error("Not implemented");
}

ItemInfoWnd::~ItemInfoWnd()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* ItemInfoWnd::GetBaseClass()
{
    throw std::logic_error("Not implemented");
}

ai::Obj const* ItemInfoWnd::GetObjectA() const
{
    throw std::logic_error("Not implemented");
}

m3d::Object* ItemInfoWnd::Clone()
{
    throw std::logic_error("Not implemented");
}

int ItemInfoWnd::Update(bool)
{
    throw std::logic_error("Not implemented");
}

int ItemInfoWnd::ShowInfoForCabin(bool)
{
    throw std::logic_error("Not implemented");
}

int ItemInfoWnd::ShowInfoForVehiclePart(bool)
{
    throw std::logic_error("Not implemented");
}

int ItemInfoWnd::ShowPropertyInfo(Property, bool)
{
    throw std::logic_error("Not implemented");
}

int ItemInfoWnd::ShowInfoForGoods(bool)
{
    throw std::logic_error("Not implemented");
}

int ItemInfoWnd::ShowInfoForGold(bool)
{
    throw std::logic_error("Not implemented");
}

bool ItemInfoWnd::NeedShowChargeSize() const
{
    throw std::logic_error("Not implemented");
}

int ItemInfoWnd::ShowInfoForGun(bool)
{
    throw std::logic_error("Not implemented");
}

int ItemInfoWnd::ShowInfoForBasket(bool)
{
    throw std::logic_error("Not implemented");
}

ItemInfoWnd::ItemInfoWnd()
{
    throw std::logic_error("Not implemented");
}

ItemInfoWnd::ItemInfoWnd(ItemInfoWnd const&)
{
    throw std::logic_error("Not implemented");
}

int ItemInfoWnd::ShowInfoBase(bool)
{
    throw std::logic_error("Not implemented");
}

int ItemInfoWnd::GameDataClear(bool)
{
    throw std::logic_error("Not implemented");
}

int ItemInfoWnd::GameDataUpdate(void*, int)
{
    throw std::logic_error("Not implemented");
}

bool ItemInfoWnd::IsValid() const
{
    throw std::logic_error("Not implemented");
}

int ItemInfoWnd::ShowInfoForGadget(bool)
{
    throw std::logic_error("Not implemented");
}

int ItemInfoWnd::GetResourceId() const
{
    throw std::logic_error("Not implemented");
}

bool ItemInfoWnd::NeedShowPrice() const
{
    throw std::logic_error("Not implemented");
}
