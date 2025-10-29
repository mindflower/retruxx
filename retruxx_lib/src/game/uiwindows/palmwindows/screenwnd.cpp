#include "screenwnd.h"

RT_CLASS_EXPORTS_BEGIN(ScreenWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(ScreenWnd);

m3d::Class* ScreenWnd::GetClass() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* ScreenWnd::CreateObject()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* ScreenWnd::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* ScreenWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(ChildPanel);
}

ScreenWnd::~ScreenWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

ScreenWnd::ScreenWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

ScreenWnd::ScreenWnd(ScreenWnd const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ScreenWnd::GameDataSetup()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ScreenWnd::AddChild(m3d::Object*)
{
    RETRUXX_NOT_IMPLEMENTED;
}
