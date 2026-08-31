#include "journalwnd.h"

#include "core/aiparam.h"
#include "core/log.h"
#include "ui/button.h"
#include "game/uiwindows/palmwindows/historywnd.h"
#include "game/uiwindows/palmwindows/reputationwnd.h"
#include "game/uiwindows/palmwindows/bookswnd.h"
#include "game/uiwindows/palmwindows/statswnd.h"
#include "game/uiwindows/palmwindows/encyclopaediawnd.h"
#include "game/m3dgame.h"

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
    // TODO: generated code JournalWnd::GameDataSetup
    // m_gameDataFlags @ +0x114 : bit0 = "init OK", bit1 = "don't rebind"
    if ((m_gameDataFlags & 2) == 0)
    {
        int ok = 1;

        // --- 1. bind the five tab buttons by name (loop, i = 0..4) ---
        for (int i = 0; i < NUM_TABS; ++i)
        {
            m3d::Object* child = GetChildByName(m_aif.m_tabButtonNames[i]);
            if (child && child->IsKindOf(&m3d::ui::ButtonWnd::m_classButtonWnd))
            {
                m_tabButtons[i] = static_cast<m3d::ui::ButtonWnd*>(child);  // raw ptr, no addref
            }
            else
            {
                // JournalWnd.cpp:112
                M3D_LOG_ERR(
                    CStr("Get control error: control ") + m_aif.m_tabButtonNames[i] +
                    CStr(" is not found or incorrect type"));
                ok = 0;
            }
        }

        // --- 2. bind the five tab sub-windows by GUI id ---
        // ids: IW_WND_HISTORY=95, BOOKS=96, REPUTATION=106, STATS=105, ENCYCLOPAEDIA=97
        static int const kTabWndId[NUM_TABS] = {95, 96, 106, 105, 97};
        m3d::Class* const kTabClass[NUM_TABS] = {
            &HistoryWnd::m_classHistoryWnd,
            &BooksWnd::m_classBooksWnd,
            &ReputationWnd::m_classReputationWnd,  // TAB_RELATIONS
            &StatsWnd::m_classStatsWnd,
            &EncyclopaediaWnd::m_classEncyclopaediaWnd,
        };

        // NB: emitted fully unrolled, one block per tab
        for (int i = 0; i < NUM_TABS; ++i)
        {
            ref_ptr<m3d::ui::Wnd> w = M3D_APP->m_pInterfaceManager->GetWindow(kTabWndId[i]);
            if (w && w->IsKindOf(kTabClass[i]))
                m_tabs[i] = w;  // ref_ptr assign (addref/release)
            else
                ok = 0;
        }

        if (ok)
            m_gameDataFlags |= 1;
    }

    if ((m_gameDataFlags & 1) != 0)
        return 1;

    // JournalWnd.cpp:150
    M3D_LOG_ERR("JournalWnd: error - fail to init because of a bad resource");
    return 0;
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
