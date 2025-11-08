#include "journalwnd.h"

#include "core/aiparam.h"

RT_CLASS_EXPORT_METHOD_DEFINE(JournalWnd, AddHistory)
{
    auto* journal = (JournalWnd*)context->asObject(0, "JournalWnd");
    auto history = context->asString(1);
    auto param = context->asAIParam(2);
    auto res = journal->AddHistory(history, param);
    context->pushInt(res);
    return 1;
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
    m_tabButtonNames[0] = "btnHistory";
    m_tabButtonNames[1] = "btnBooks";
    m_tabButtonNames[2] = "btnRelations";
    m_tabButtonNames[3] = "btnStats";
    m_tabButtonNames[4] = "btnEncyclopaedia";
    m_tabBtnPaneNameUnselected = "PaneBtnGray1";
    m_tabBtnPaneNameSelected = "PaneBtnGray1Selected";
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
    return new JournalWnd;
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
    // TODO: implement JournalWnd::AddHistory
    // RETRUXX_NOT_IMPLEMENTED;
    return 1;
}

m3d::Class* JournalWnd::GetClass() const
{
    return RT_CLASS_LOCAL(JournalWnd);
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
    m_curTab = TAB_HISTORY;
}

JournalWnd::JournalWnd(JournalWnd const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int JournalWnd::GameDataSetup()
{
    // TODO: implement JournalWnd::GameDataSetup
    // RETRUXX_NOT_IMPLEMENTED;
    return 1;
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
