#include "maingameinterfacewnd.h"
#include "radarwnd.h"
#include "igrokamochatwnd.h"
#include "damageinfownd.h"
#include "weaponinfolist.h"
#include "fadingmsg.h"
#include "counterwnd.h"
#include "targetinfownd.h"
#include "maincursorwnd.h"
#include "speedometerwnd.h"
#include "bossindicatorwnd.h"

RT_CLASS_EXPORT_METHOD_DEFINE(MainGameInterfaceWnd, SetupForBoss)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORTS_BEGIN(MainGameInterfaceWnd)
	RT_CLASS_EXPORT(MainGameInterfaceWnd, m3d::METHOD, SetupForBoss, "", "", "")
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(MainGameInterfaceWnd);

MainGameInterfaceWnd::AuxInfo::AuxInfo()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* MainGameInterfaceWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

int MainGameInterfaceWnd::GetBossId() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void MainGameInterfaceWnd::CheckAndShowTargetInfoWnd(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* MainGameInterfaceWnd::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

ai::Obj const* MainGameInterfaceWnd::GetBoss() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void MainGameInterfaceWnd::CheckAndShowCounterWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void MainGameInterfaceWnd::CheckAndShowMainCursorWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int MainGameInterfaceWnd::SetupForBoss(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

MainGameInterfaceWnd::~MainGameInterfaceWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

MainGameInterfaceWnd::MainGameInterfaceWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

MainGameInterfaceWnd::MainGameInterfaceWnd(MainGameInterfaceWnd const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void MainGameInterfaceWnd::CheckAndShowNearbyTownIco()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int MainGameInterfaceWnd::GameDataSetup()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int MainGameInterfaceWnd::OnBeforeAddToWndStation()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void MainGameInterfaceWnd::CheckAndShowNearbyChestsIco()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void MainGameInterfaceWnd::OnBossModeChanged()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int MainGameInterfaceWnd::OnAfterRemoveFromWndStation()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int MainGameInterfaceWnd::GameDataSave(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void MainGameInterfaceWnd::OnTownRuined(void*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void MainGameInterfaceWnd::UpdateInfoContour()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void MainGameInterfaceWnd::HideContour(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void MainGameInterfaceWnd::ShowContour(int, unsigned, float)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void MainGameInterfaceWnd::UpdateContours()
{
    RETRUXX_NOT_IMPLEMENTED;
}

unsigned MainGameInterfaceWnd::GetColorForInfoContour(int) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void MainGameInterfaceWnd::ShowCapturedContour(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void MainGameInterfaceWnd::OnNearbyChests(void*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void MainGameInterfaceWnd::ClearCapturedContour()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void MainGameInterfaceWnd::OnApproachTown(void*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int MainGameInterfaceWnd::GetCapturedObjId() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void MainGameInterfaceWnd::ClearContours()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void MainGameInterfaceWnd::CheckAndShowBossIndicator()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int MainGameInterfaceWnd::GameDataLoad(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int MainGameInterfaceWnd::GameDataUpdate(void*, int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int MainGameInterfaceWnd::OnBeforeRemoveFromWndStation()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void MainGameInterfaceWnd::UpdateBossMode()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int MainGameInterfaceWnd::GameDataClear(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void MainGameInterfaceWnd::UpdateCapturedContour()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int MainGameInterfaceWnd::GetInfoObjId() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void MainGameInterfaceWnd::ShowInfoContour(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void MainGameInterfaceWnd::ClearInfoContour()
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool MainGameInterfaceWnd::IsInBossMode() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void MainGameInterfaceWnd::OnNewFrame()
{
    RETRUXX_NOT_IMPLEMENTED;
}
