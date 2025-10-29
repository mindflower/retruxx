#include "maincursorwnd.h"

RT_CLASS_EXPORTS_BEGIN(MainCursorWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(MainCursorWnd);

MainCursorWnd::AuxInfo::AuxInfo()
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool MainCursorWnd::NeedShow() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* MainCursorWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(SmartCursorWnd);
}

m3d::Object* MainCursorWnd::CreateObject()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* MainCursorWnd::GetClass() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

MainCursorWnd::~MainCursorWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* MainCursorWnd::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void MainCursorWnd::OnShowCursor(void*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void MainCursorWnd::CheckAndShow()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void MainCursorWnd::FillCapturingDrawInfo(SmartCursorWnd::AuxDrawInfo*, int, bool) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void MainCursorWnd::OnGameModeChanged(void*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void MainCursorWnd::UpdateCapturingTex()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void MainCursorWnd::OnNewFrame()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int MainCursorWnd::GameDataSetup()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int MainCursorWnd::GameDataUpdate(void*, int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

MainCursorWnd::MainCursorWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

MainCursorWnd::MainCursorWnd(MainCursorWnd const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}
