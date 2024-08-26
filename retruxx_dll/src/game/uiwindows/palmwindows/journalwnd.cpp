#include "journalwnd.h"

RT_CLASS_EXPORT_METHOD_DEFINE(JournalWnd, AddHistory)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(JournalWnd, AddBook)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(JournalWnd, BookExists)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(JournalWnd, AddPrototypeToEncyclopaedia)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(JournalWnd, AddClanToEncyclopaedia)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(JournalWnd, ShowAllInEncyclopaedia)
{
    throw std::logic_error("Not implemented");
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
    throw std::logic_error("Not implemented");
}

void JournalWnd::ShowAllInEncyclopaedia()
{
    throw std::logic_error("Not implemented");
}

bool JournalWnd::BookExists(CStr const&) const
{
    throw std::logic_error("Not implemented");
}

m3d::Object* JournalWnd::CreateObject()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* JournalWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(ScreenWnd);
}

m3d::Object* JournalWnd::Clone()
{
    throw std::logic_error("Not implemented");
}

int JournalWnd::AddHistory(CStr const&, m3d::AIParam const&)
{
    throw std::logic_error("Not implemented");
}

m3d::Class* JournalWnd::GetClass() const
{
    throw std::logic_error("Not implemented");
}

int JournalWnd::AddClanToEncyclopaedia(int)
{
    throw std::logic_error("Not implemented");
}

JournalWnd::~JournalWnd()
{
    throw std::logic_error("Not implemented");
}

int JournalWnd::AddPrototypeToEncyclopaedia(CStr const&)
{
    throw std::logic_error("Not implemented");
}

int JournalWnd::AddBook(CStr const&, CStr const&)
{
    throw std::logic_error("Not implemented");
}

void JournalWnd::PostCurrentTabMessage() const
{
    throw std::logic_error("Not implemented");
}

int JournalWnd::UpdateButtonsState()
{
    throw std::logic_error("Not implemented");
}

JournalWnd::Tab JournalWnd::CtrlId2TabId(int) const
{
    throw std::logic_error("Not implemented");
}

int JournalWnd::OnWndNotify(m3d::ui::Wnd*, unsigned, unsigned, m3d::AIParam const&)
{
    throw std::logic_error("Not implemented");
}

JournalWnd::JournalWnd()
{
    throw std::logic_error("Not implemented");
}

JournalWnd::JournalWnd(JournalWnd const&)
{
    throw std::logic_error("Not implemented");
}

int JournalWnd::GameDataSetup()
{
    throw std::logic_error("Not implemented");
}

int JournalWnd::GameDataSave(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*)
{
    throw std::logic_error("Not implemented");
}

int JournalWnd::OnBeforeAddToWndStation()
{
    throw std::logic_error("Not implemented");
}

int JournalWnd::GameDataLoad(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*)
{
    throw std::logic_error("Not implemented");
}

int JournalWnd::SetCurTab(Tab, bool)
{
    throw std::logic_error("Not implemented");
}

int JournalWnd::SelectButton(Tab, bool)
{
    throw std::logic_error("Not implemented");
}
