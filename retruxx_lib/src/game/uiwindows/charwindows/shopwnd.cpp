#include "shopwnd.h"

RT_CLASS_EXPORTS_BEGIN(ShopWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(ShopWnd);

m3d::Class* ShopWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(SaleWnd);
}

m3d::Object* ShopWnd::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* ShopWnd::CreateObject()
{
    return new ShopWnd;
}

m3d::Class* ShopWnd::GetClass() const
{
    return RT_CLASS_LOCAL(ShopWnd);
}

ShopWnd::~ShopWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void ShopWnd::OnDDItemQuickDrop(void*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ShopWnd::GameDataSetup()
{
    // TODO: implement ShopWnd::GameDataSetup
    // RETRUXX_NOT_IMPLEMENTED;
    return 1;
}

int ShopWnd::GameDataUpdate(void*, int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void ShopWnd::SetupListOfTabTypes()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void ShopWnd::OnDDDrop(void*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ShopWnd::SetUpForWorkshop(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

ShopWnd::ShopWnd()
{
    m_buildingType = ai::SHOP;
}

ShopWnd::ShopWnd(ShopWnd const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ShopWnd::OnAfterAddToWndStation()
{
    RETRUXX_NOT_IMPLEMENTED;
}

ai::BuildingType ShopWnd::GetBuildingType()
{
    RETRUXX_NOT_IMPLEMENTED;
}
