#include "iteminfownd.h"

RT_CLASS_EXPORTS_BEGIN(ItemInfoWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(ItemInfoWnd);

m3d::Object* ItemInfoWnd::CreateObject()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* ItemInfoWnd::GetClass() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

ai::GeomRepositoryItem const& ItemInfoWnd::GetItem() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void ItemInfoWnd::SetUpForRepositoryItem(ai::GeomRepositoryItem const&, ref_ptr<m3d::ui::Wnd>)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void ItemInfoWnd::SetUpForObject(int, ref_ptr<m3d::ui::Wnd>)
{
    RETRUXX_NOT_IMPLEMENTED;
}

ref_ptr<m3d::ui::Wnd> ItemInfoWnd::GetSrcWnd() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

ItemInfoWnd::~ItemInfoWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* ItemInfoWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(InfoWnd);
}

ai::Obj const* ItemInfoWnd::GetObjectA() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* ItemInfoWnd::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ItemInfoWnd::Update(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ItemInfoWnd::ShowInfoForCabin(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ItemInfoWnd::ShowInfoForVehiclePart(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ItemInfoWnd::ShowPropertyInfo(Property, bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ItemInfoWnd::ShowInfoForGoods(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ItemInfoWnd::ShowInfoForGold(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool ItemInfoWnd::NeedShowChargeSize() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ItemInfoWnd::ShowInfoForGun(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ItemInfoWnd::ShowInfoForBasket(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

ItemInfoWnd::ItemInfoWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

ItemInfoWnd::ItemInfoWnd(ItemInfoWnd const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ItemInfoWnd::ShowInfoBase(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ItemInfoWnd::GameDataClear(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ItemInfoWnd::GameDataUpdate(void*, int)
{
    // TODO: implement GameDataUpdate
    //  RETRUXX_NOT_IMPLEMENTED;
    return 0;
}

bool ItemInfoWnd::IsValid() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ItemInfoWnd::ShowInfoForGadget(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ItemInfoWnd::GetResourceId() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool ItemInfoWnd::NeedShowPrice() const
{
    RETRUXX_NOT_IMPLEMENTED;
}
