#include "fadepanelbeforenextmap.h"

#include "config.h"
#include "core/kernel.h"

RT_CLASS_EXPORTS_BEGIN(FadePanelBeforeNextMap)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(FadePanelBeforeNextMap);

m3d::Class* FadePanelBeforeNextMap::GetClass() const
{
    return RT_CLASS_LOCAL(FadePanelBeforeNextMap);
}

FadePanelBeforeNextMap::~FadePanelBeforeNextMap()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* FadePanelBeforeNextMap::CreateObject()
{
    return new FadePanelBeforeNextMap;
}

m3d::Class* FadePanelBeforeNextMap::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

m3d::Object* FadePanelBeforeNextMap::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

FadePanelBeforeNextMap::FadePanelBeforeNextMap(FadePanelBeforeNextMap const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

FadePanelBeforeNextMap::FadePanelBeforeNextMap()
{
    m_alpha = 0.0;
    m_fadePeriod = M3D_ENGINE_CFG.m_fadingTimeBeforeNextMap.GetF();;
    m_blackColor = 0xFF000000;
}

unsigned FadePanelBeforeNextMap::CalcCurColor() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void FadePanelBeforeNextMap::OnPlayerPassToMapFading()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void FadePanelBeforeNextMap::UpdateAlpha()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void FadePanelBeforeNextMap::StartFading()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void FadePanelBeforeNextMap::OnNewFrame()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int FadePanelBeforeNextMap::OnPaint(m3d::ui::DrawInfo const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int FadePanelBeforeNextMap::GameDataUpdate(void*, int)
{
    // TODO: implement GameDataUpdate
    //  RETRUXX_NOT_IMPLEMENTED;
    return 0;
}
