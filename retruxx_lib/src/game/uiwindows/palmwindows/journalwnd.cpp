#include "journalwnd.h"

RT_CLASS_EXPORT_METHOD_DEFINE(JournalWnd, AddHistory)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(JournalWnd, AddBook)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(JournalWnd, BookExists)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(JournalWnd, AddPrototypeToEncyclopaedia)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(JournalWnd, AddClanToEncyclopaedia)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(JournalWnd, ShowAllInEncyclopaedia)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORTS_BEGIN(JournalWnd)
    RT_CLASS_EXPORT(JournalWnd, m3d::METHOD, AddHistory, "", "", "")
    RT_CLASS_EXPORT(JournalWnd, m3d::METHOD, AddBook, "", "", "")
    RT_CLASS_EXPORT(JournalWnd, m3d::METHOD, BookExists, "", "", "")
    RT_CLASS_EXPORT(JournalWnd, m3d::METHOD, AddPrototypeToEncyclopaedia, "", "", "")
    RT_CLASS_EXPORT(JournalWnd, m3d::METHOD, AddClanToEncyclopaedia, "", "", "")
    RT_CLASS_EXPORT(JournalWnd, m3d::METHOD, ShowAllInEncyclopaedia, "", "", "")
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(JournalWnd);

JournalWnd::AuxInfo::AuxInfo()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void JournalWnd::ShowAllInEncyclopaedia()
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool JournalWnd::BookExists(CStr const&) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* JournalWnd::CreateObject()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* JournalWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(ScreenWnd);
}

m3d::Object* JournalWnd::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int JournalWnd::AddHistory(CStr const&, m3d::AIParam const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* JournalWnd::GetClass() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int JournalWnd::AddClanToEncyclopaedia(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

JournalWnd::~JournalWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int JournalWnd::AddPrototypeToEncyclopaedia(CStr const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int JournalWnd::AddBook(CStr const&, CStr const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void JournalWnd::PostCurrentTabMessage() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int JournalWnd::UpdateButtonsState()
{
    RETRUXX_NOT_IMPLEMENTED;
}

JournalWnd::Tab JournalWnd::CtrlId2TabId(int) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int JournalWnd::OnWndNotify(m3d::ui::Wnd*, unsigned, unsigned, m3d::AIParam const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

JournalWnd::JournalWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

JournalWnd::JournalWnd(JournalWnd const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int JournalWnd::GameDataSetup()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int JournalWnd::GameDataSave(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int JournalWnd::OnBeforeAddToWndStation()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int JournalWnd::GameDataLoad(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int JournalWnd::SetCurTab(Tab, bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int JournalWnd::SelectButton(Tab, bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}
