#include "savewnd.h"

RT_CLASS_EXPORTS_BEGIN(SaveWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(SaveWnd);

SaveWnd::SAuxInfo::SAuxInfo()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* SaveWnd::GetClass() const
{
    throw std::logic_error("Not implemented");
}

m3d::Object* SaveWnd::CreateObject()
{
    throw std::logic_error("Not implemented");
}

m3d::Object* SaveWnd::Clone()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* SaveWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(LSWnd);
}

SaveWnd::~SaveWnd()
{
    throw std::logic_error("Not implemented");
}

int SaveWnd::OnWndNotify(m3d::ui::Wnd*, unsigned, unsigned, m3d::AIParam const&)
{
    throw std::logic_error("Not implemented");
}

void SaveWnd::OnSaveSelectionChange()
{
    throw std::logic_error("Not implemented");
}

void SaveWnd::OnEnter()
{
    throw std::logic_error("Not implemented");
}

void SaveWnd::OnEditChange()
{
    throw std::logic_error("Not implemented");
}

int SaveWnd::SetScreenshotForSave(int)
{
    throw std::logic_error("Not implemented");
}

void SaveWnd::OnSave()
{
    throw std::logic_error("Not implemented");
}

void SaveWnd::OnDelete()
{
    throw std::logic_error("Not implemented");
}

int SaveWnd::GameDataSetup()
{
    throw std::logic_error("Not implemented");
}

int SaveWnd::GameDataUpdate(void*, int)
{
    throw std::logic_error("Not implemented");
}

SaveWnd::SaveWnd(SaveWnd const&)
{
    throw std::logic_error("Not implemented");
}

SaveWnd::SaveWnd()
{
    throw std::logic_error("Not implemented");
}
