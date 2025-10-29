#include "savewnd.h"

RT_CLASS_EXPORTS_BEGIN(SaveWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(SaveWnd);

SaveWnd::SAuxInfo::SAuxInfo()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* SaveWnd::GetClass() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* SaveWnd::CreateObject()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* SaveWnd::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* SaveWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(LSWnd);
}

SaveWnd::~SaveWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int SaveWnd::OnWndNotify(m3d::ui::Wnd*, unsigned, unsigned, m3d::AIParam const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void SaveWnd::OnSaveSelectionChange()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void SaveWnd::OnEnter()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void SaveWnd::OnEditChange()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int SaveWnd::SetScreenshotForSave(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void SaveWnd::OnSave()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void SaveWnd::OnDelete()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int SaveWnd::GameDataSetup()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int SaveWnd::GameDataUpdate(void*, int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

SaveWnd::SaveWnd(SaveWnd const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

SaveWnd::SaveWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}
