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
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORTS_BEGIN(MainGameInterfaceWnd)
	RT_CLASS_EXPORT(MainGameInterfaceWnd, m3d::METHOD, SetupForBoss, "", "", "")
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(MainGameInterfaceWnd);

MainGameInterfaceWnd::AuxInfo::AuxInfo()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* MainGameInterfaceWnd::GetBaseClass()
{
    throw std::logic_error("Not implemented");
}

int MainGameInterfaceWnd::GetBossId() const
{
    throw std::logic_error("Not implemented");
}

void MainGameInterfaceWnd::CheckAndShowTargetInfoWnd(bool)
{
    throw std::logic_error("Not implemented");
}

m3d::Object* MainGameInterfaceWnd::Clone()
{
    throw std::logic_error("Not implemented");
}

ai::Obj const* MainGameInterfaceWnd::GetBoss() const
{
    throw std::logic_error("Not implemented");
}

void MainGameInterfaceWnd::CheckAndShowCounterWnd()
{
    throw std::logic_error("Not implemented");
}

void MainGameInterfaceWnd::CheckAndShowMainCursorWnd()
{
    throw std::logic_error("Not implemented");
}

int MainGameInterfaceWnd::SetupForBoss(int)
{
    throw std::logic_error("Not implemented");
}

MainGameInterfaceWnd::~MainGameInterfaceWnd()
{
    throw std::logic_error("Not implemented");
}

MainGameInterfaceWnd::MainGameInterfaceWnd()
{
    throw std::logic_error("Not implemented");
}

MainGameInterfaceWnd::MainGameInterfaceWnd(MainGameInterfaceWnd const&)
{
    throw std::logic_error("Not implemented");
}

void MainGameInterfaceWnd::CheckAndShowNearbyTownIco()
{
    throw std::logic_error("Not implemented");
}

int MainGameInterfaceWnd::GameDataSetup()
{
    throw std::logic_error("Not implemented");
}

int MainGameInterfaceWnd::OnBeforeAddToWndStation()
{
    throw std::logic_error("Not implemented");
}

void MainGameInterfaceWnd::CheckAndShowNearbyChestsIco()
{
    throw std::logic_error("Not implemented");
}

void MainGameInterfaceWnd::OnBossModeChanged()
{
    throw std::logic_error("Not implemented");
}

int MainGameInterfaceWnd::OnAfterRemoveFromWndStation()
{
    throw std::logic_error("Not implemented");
}

int MainGameInterfaceWnd::GameDataSave(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*)
{
    throw std::logic_error("Not implemented");
}

void MainGameInterfaceWnd::OnTownRuined(void*)
{
    throw std::logic_error("Not implemented");
}

void MainGameInterfaceWnd::UpdateInfoContour()
{
    throw std::logic_error("Not implemented");
}

void MainGameInterfaceWnd::HideContour(int)
{
    throw std::logic_error("Not implemented");
}

void MainGameInterfaceWnd::ShowContour(int, unsigned, float)
{
    throw std::logic_error("Not implemented");
}

void MainGameInterfaceWnd::UpdateContours()
{
    throw std::logic_error("Not implemented");
}

unsigned MainGameInterfaceWnd::GetColorForInfoContour(int) const
{
    throw std::logic_error("Not implemented");
}

void MainGameInterfaceWnd::ShowCapturedContour(int)
{
    throw std::logic_error("Not implemented");
}

void MainGameInterfaceWnd::OnNearbyChests(void*)
{
    throw std::logic_error("Not implemented");
}

void MainGameInterfaceWnd::ClearCapturedContour()
{
    throw std::logic_error("Not implemented");
}

void MainGameInterfaceWnd::OnApproachTown(void*)
{
    throw std::logic_error("Not implemented");
}

int MainGameInterfaceWnd::GetCapturedObjId() const
{
    throw std::logic_error("Not implemented");
}

void MainGameInterfaceWnd::ClearContours()
{
    throw std::logic_error("Not implemented");
}

void MainGameInterfaceWnd::CheckAndShowBossIndicator()
{
    throw std::logic_error("Not implemented");
}

int MainGameInterfaceWnd::GameDataLoad(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*)
{
    throw std::logic_error("Not implemented");
}

int MainGameInterfaceWnd::GameDataUpdate(void*, int)
{
    throw std::logic_error("Not implemented");
}

int MainGameInterfaceWnd::OnBeforeRemoveFromWndStation()
{
    throw std::logic_error("Not implemented");
}

void MainGameInterfaceWnd::UpdateBossMode()
{
    throw std::logic_error("Not implemented");
}

int MainGameInterfaceWnd::GameDataClear(bool)
{
    throw std::logic_error("Not implemented");
}

void MainGameInterfaceWnd::UpdateCapturedContour()
{
    throw std::logic_error("Not implemented");
}

int MainGameInterfaceWnd::GetInfoObjId() const
{
    throw std::logic_error("Not implemented");
}

void MainGameInterfaceWnd::ShowInfoContour(int)
{
    throw std::logic_error("Not implemented");
}

void MainGameInterfaceWnd::ClearInfoContour()
{
    throw std::logic_error("Not implemented");
}

bool MainGameInterfaceWnd::IsInBossMode() const
{
    throw std::logic_error("Not implemented");
}

void MainGameInterfaceWnd::OnNewFrame()
{
    throw std::logic_error("Not implemented");
}
