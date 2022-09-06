#include "counterwnd.h"

RT_CLASS_EXPORTS_BEGIN(CounterWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(CounterWnd);

CounterWnd::AuxInfo::AuxInfo()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* CounterWnd::GetClass() const
{
    throw std::logic_error("Not implemented");
}

m3d::Class* CounterWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

m3d::Object* CounterWnd::CreateObject()
{
    throw std::logic_error("Not implemented");
}

bool CounterWnd::NeedShow() const
{
    throw std::logic_error("Not implemented");
}

CounterWnd::~CounterWnd()
{
    throw std::logic_error("Not implemented");
}

m3d::Object* CounterWnd::Clone()
{
    throw std::logic_error("Not implemented");
}

void CounterWnd::ClearCounter()
{
    throw std::logic_error("Not implemented");
}

CounterWnd::CounterWnd(CounterWnd const&)
{
    throw std::logic_error("Not implemented");
}

CounterWnd::CounterWnd()
{
    throw std::logic_error("Not implemented");
}

void CounterWnd::OnStartLevel(void*)
{
    throw std::logic_error("Not implemented");
}

int CounterWnd::SetupForHuntQuest(int)
{
    throw std::logic_error("Not implemented");
}

void CounterWnd::GetMinAndSecFromTime(float, int&, int&) const
{
    throw std::logic_error("Not implemented");
}

int CounterWnd::GameDataClear(bool)
{
    throw std::logic_error("Not implemented");
}

ai::DynamicQuestHunt const* CounterWnd::GetQuestHunt() const
{
    throw std::logic_error("Not implemented");
}

int CounterWnd::GameDataSetup()
{
    throw std::logic_error("Not implemented");
}

void CounterWnd::OnDynamicQuestStateChanged(void*)
{
    throw std::logic_error("Not implemented");
}

void CounterWnd::CheckAndShow()
{
    throw std::logic_error("Not implemented");
}

float CounterWnd::GetHuntTimeLeft() const
{
    throw std::logic_error("Not implemented");
}

int CounterWnd::GameDataUpdate(void*, int)
{
    throw std::logic_error("Not implemented");
}

void CounterWnd::UpdateCounter()
{
    throw std::logic_error("Not implemented");
}

void CounterWnd::OnNewFrame()
{
    throw std::logic_error("Not implemented");
}
