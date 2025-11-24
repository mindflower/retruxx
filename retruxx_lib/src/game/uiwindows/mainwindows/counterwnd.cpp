#include "counterwnd.h"

RT_CLASS_EXPORTS_BEGIN(CounterWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(CounterWnd);

CounterWnd::AuxInfo::AuxInfo()
{
    m_wndCounterName = "wndCounter";
    m_strHuntTime = "HuntTime";
}

m3d::Class* CounterWnd::GetClass() const
{
    return RT_CLASS_LOCAL(CounterWnd);
}

m3d::Class* CounterWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

m3d::Object* CounterWnd::CreateObject()
{
    return new CounterWnd;
}

bool CounterWnd::NeedShow() const
{
    return m_counterType != COUNTERTYPE_NUM_COUNTER_TYPES;
}

CounterWnd::~CounterWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* CounterWnd::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void CounterWnd::ClearCounter()
{
    RETRUXX_NOT_IMPLEMENTED;
}

CounterWnd::CounterWnd(CounterWnd const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

CounterWnd::CounterWnd()
{
    m_wndCounter = 0;
    m_counterType = COUNTERTYPE_NUM_COUNTER_TYPES;
    m_dQuestId = -1;
}

void CounterWnd::OnStartLevel(void*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int CounterWnd::SetupForHuntQuest(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void CounterWnd::GetMinAndSecFromTime(float, int&, int&) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int CounterWnd::GameDataClear(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

ai::DynamicQuestHunt const* CounterWnd::GetQuestHunt() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int CounterWnd::GameDataSetup()
{
    // TODO: implement CounterWnd::GameDataSetup
    // RETRUXX_NOT_IMPLEMENTED;
    return 1;
}

void CounterWnd::OnDynamicQuestStateChanged(void*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void CounterWnd::CheckAndShow()
{
    RETRUXX_NOT_IMPLEMENTED;
}

float CounterWnd::GetHuntTimeLeft() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int CounterWnd::GameDataUpdate(void*, int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void CounterWnd::UpdateCounter()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void CounterWnd::OnNewFrame()
{
    RETRUXX_NOT_IMPLEMENTED;
}
