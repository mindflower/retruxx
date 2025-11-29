#include "savewnd.h"

RT_CLASS_EXPORTS_BEGIN(SaveWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(SaveWnd);

SaveWnd::SAuxInfo::SAuxInfo()
{
    m_btnSaveName = "btnSave";
    m_btnDeleteName = "btnDelete";
    m_editSaveName = "editSaveName";
}

m3d::Class* SaveWnd::GetClass() const
{
    return RT_CLASS_LOCAL(SaveWnd);
}

m3d::Object* SaveWnd::CreateObject()
{
    return new SaveWnd;
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
    // TODO: implement SaveWnd::GameDataSetup
    // RETRUXX_NOT_IMPLEMENTED;
    return 1;
}

int SaveWnd::GameDataUpdate(void*, int)
{
    // TODO: implement GameDataUpdate
    //  RETRUXX_NOT_IMPLEMENTED;
    return 0;
}

SaveWnd::SaveWnd(SaveWnd const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

SaveWnd::SaveWnd()
{
    m_btnSave = 0;
    m_btnDelete = 0;
    m_editSaveName = 0;
    m_bNeedScreenshot = 0;
}
