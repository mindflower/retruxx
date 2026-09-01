#include "journalwnd.h"

#include "core/aiparam.h"
#include "core/ini.h"
#include "core/log.h"
#include "core/ref_ptr.h"
#include "ui/button.h"
#include <server/server.h>
#include <server/objects/base/prototypemanager.h>
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
    auto* journal = (JournalWnd*)context->asObject(0, "JournalWnd");
    auto bookNameId = context->asString(1);
    auto bookTextId = context->asString(2);
    context->pushInt(journal->AddBook(bookNameId, bookTextId));
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(JournalWnd, BookExists)
{
    auto* journal = (JournalWnd*)context->asObject(0, "JournalWnd");
    auto bookNameId = context->asString(1);
    context->pushInt(journal->BookExists(bookNameId) ? 1 : 0);
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(JournalWnd, AddPrototypeToEncyclopaedia)
{
    auto* journal = (JournalWnd*)context->asObject(0, "JournalWnd");
    auto prototypeName = context->asString(1);
    context->pushInt(journal->AddPrototypeToEncyclopaedia(prototypeName));
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(JournalWnd, AddClanToEncyclopaedia)
{
    auto* journal = (JournalWnd*)context->asObject(0, "JournalWnd");
    int clanBelong = context->asInt(1);
    context->pushInt(journal->AddClanToEncyclopaedia(clanBelong));
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(JournalWnd, ShowAllInEncyclopaedia)
{
    auto* journal = (JournalWnd*)context->asObject(0, "JournalWnd");
    journal->ShowAllInEncyclopaedia();
    return 1;
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
    if ((m_gameDataFlags & 1) == 0)
    {
        M3D_LOG_INFO("JournalWnd::ShowAllInEncyclopaedia error - window was not inited");
        return;
    }
    if (auto* ency = RT_DYNCAST(m_tabs[TAB_ENCYCLOPAEDIA].get(), EncyclopaediaWnd))
    {
        ency->ShowAll();
    }
}

bool JournalWnd::BookExists(CStr const& strBookNameId) const
{
    if ((m_gameDataFlags & 1) == 0)
    {
        M3D_LOG_INFO("JournalWnd::AddBook error - window was not inited");
        return false;
    }
    auto const* booksWnd = RT_DYNCAST(m_tabs[TAB_BOOKS].get(), BooksWnd const);
    return booksWnd && booksWnd->BookExists(strBookNameId);
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
    return new JournalWnd;
}

int JournalWnd::AddHistory(CStr const& strTextId, m3d::AIParam const& time)
{
    if ((m_gameDataFlags & 1) == 0)
    {
        M3D_LOG_INFO("JournalWnd::AddHistory error - window was not inited");
        return 0;
    }
    auto* historyWnd = RT_DYNCAST(m_tabs[TAB_HISTORY].get(), HistoryWnd);
    if (!historyWnd)
    {
        return 0;
    }
    return historyWnd->AddRecord(strTextId, time);
}

m3d::Class* JournalWnd::GetClass() const
{
    return RT_CLASS_LOCAL(JournalWnd);
}

int JournalWnd::AddClanToEncyclopaedia(int clanBelong)
{
    if ((m_gameDataFlags & 1) == 0)
    {
        M3D_LOG_INFO("JournalWnd::AddClanToEncyclopaedia error - window was not inited");
        return 0;
    }
    auto* ency = RT_DYNCAST(m_tabs[TAB_ENCYCLOPAEDIA].get(), EncyclopaediaWnd);
    if (!ency)
    {
        return 0;
    }
    if (ency->AddClan(clanBelong))
    {
        return 1;
    }
    M3D_LOG_INFO("JournalWnd::AddClanToEncyclopaedia - fail to add belong " + CStr(clanBelong));
    return 0;
}

JournalWnd::~JournalWnd() = default;

int JournalWnd::AddPrototypeToEncyclopaedia(CStr const& prototypeName)
{
    if ((m_gameDataFlags & 1) == 0)
    {
        M3D_LOG_INFO("JournalWnd::AddPrototypeToEncyclopaedia error - window was not inited");
        return 0;
    }
    auto* ency = RT_DYNCAST(m_tabs[TAB_ENCYCLOPAEDIA].get(), EncyclopaediaWnd);
    if (!ency)
    {
        return 0;
    }
    if (ency->AddPrototype(ai::thePrototypeManager->GetPrototypeId(prototypeName)))
    {
        return 1;
    }
    M3D_LOG_INFO("JournalWnd::AddPrototypeToEncyclopaedia - fail to add prototype " + prototypeName);
    return 0;
}

int JournalWnd::AddBook(CStr const& strBookNameId, CStr const& strBookTextId)
{
    if ((m_gameDataFlags & 1) == 0)
    {
        M3D_LOG_INFO("JournalWnd::AddBook error - window was not inited");
        return 0;
    }
    auto* booksWnd = RT_DYNCAST(m_tabs[TAB_BOOKS].get(), BooksWnd);
    if (!booksWnd)
    {
        return 0;
    }
    return booksWnd->AddBook(strBookNameId, strBookTextId, true);
}

void JournalWnd::PostCurrentTabMessage() const
{
    switch (m_curTab)
    {
    case TAB_HISTORY:
        ai::pServer->PostPlayerEvent(ai::GE_TUTORIAL_HISTORY);
        break;
    case TAB_BOOKS:
        ai::pServer->PostPlayerEvent(ai::GE_TUTORIAL_BOOKS);
        break;
    case TAB_RELATIONS:
        ai::pServer->PostPlayerEvent(ai::GE_TUTORIAL_RELATIONS);
        break;
    case TAB_STATS:
        ai::pServer->PostPlayerEvent(ai::GE_TUTORIAL_STATS);
        break;
    case TAB_ENCYCLOPAEDIA:
        ai::pServer->PostPlayerEvent(ai::GE_TUTORIAL_ENCYCLOPAEDIA);
        break;
    default:
        break;
    }
}

int JournalWnd::UpdateButtonsState()
{
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }

    int result = 1;
    for (int i = 0; i < NUM_TABS; ++i)
    {
        if (!m_tabButtons[i])
        {
            result = 0;
            continue;
        }
        m_tabButtons[i]->SetPane(i == m_curTab ? m_aif.m_tabBtnPaneNameSelected : m_aif.m_tabBtnPaneNameUnselected);
    }
    return result;
}

JournalWnd::Tab JournalWnd::CtrlId2TabId(int ctrlId) const
{
    switch (ctrlId)
    {
    case CTRL_ID_BTN_HISTORY:
        return TAB_HISTORY;
    case CTRL_ID_BTN_BOOKS:
        return TAB_BOOKS;
    case CTRL_ID_BTN_RELATIONS:
        return TAB_RELATIONS;
    case CTRL_ID_BTN_STATS:
        return TAB_STATS;
    case CTRL_ID_BTN_ENCYCLOPAEDIA:
        return TAB_ENCYCLOPAEDIA;
    default:
        return NUM_TABS;
    }
}

int JournalWnd::OnWndNotify(m3d::ui::Wnd* from, unsigned idFrom, unsigned message, m3d::AIParam const& data)
{
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }
    if (ChildPanel::OnWndNotify(from, idFrom, message, data))
    {
        return 1;
    }
    if (idFrom >= CTRL_ID_BTN_HISTORY && idFrom <= CTRL_ID_BTN_ENCYCLOPAEDIA && message == 1)
    {
        SetCurTab(CtrlId2TabId(idFrom), true);
        return 1;
    }
    return 0;
}

JournalWnd::JournalWnd()
{
    m_curTab = TAB_HISTORY;
}

JournalWnd::JournalWnd(JournalWnd const&) : JournalWnd()
{
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

int JournalWnd::GameDataSave(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* guiNode)
{
    if ((m_gameDataFlags & 1) == 0)
    {
        M3D_LOG_INFO("JournalWnd::GameDataSave error - journal has been not properly inited");
        return 0;
    }
    if (!xmlFile || !guiNode)
    {
        M3D_LOG_INFO("JournalWnd::GameDataSave error - invalid params");
        return 0;
    }

    ref_ptr journalNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, "Journal");
    guiNode->AddChild(journalNode);
    journalNode->SetAttribute("CurTab", CStr(static_cast<int>(m_curTab)).c_str());
    return 1;
}

int JournalWnd::OnBeforeAddToWndStation()
{
    SetCurTab(m_curTab, true);
    return m3d::ui::Wnd::OnBeforeAddToWndStation();
}

int JournalWnd::GameDataLoad(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* guiNode)
{
    if ((m_gameDataFlags & 1) == 0)
    {
        M3D_LOG_INFO("JournalWnd::GameDataLoad error - journal has been not properly inited");
        return 0;
    }

    GameDataClear(false);

    if (!xmlFile || !guiNode)
    {
        M3D_LOG_INFO("JournalWnd::GameDataLoad error - invalid params");
        return 0;
    }

    ref_ptr journalNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_EMPTY, nullptr);
    guiNode->GetFirstChild(journalNode, "Journal");
    if (journalNode->IsEmpty())
    {
        M3D_LOG_INFO("JournalWnd::GameDataLoad error - cannot find journal node");
        return 0;
    }

    int curTab = NUM_TABS;
    m3d::SafeIntAttrib(curTab, journalNode, "CurTab");
    if (static_cast<unsigned int>(curTab) > TAB_ENCYCLOPAEDIA)
    {
        M3D_LOG_INFO("JournalWnd::GameDataLoad error - invalid current tab");
        return 0;
    }

    m_curTab = static_cast<Tab>(curTab);
    return 1;
}

int JournalWnd::SetCurTab(Tab tabId, bool bPostTabMessage)
{
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }

    Tab const oldTab = m_curTab;
    m_curTab = tabId;

    int res = 1;
    for (int i = 0; i < NUM_TABS; ++i)
    {
        auto* tabWnd = m_tabs[i].get();
        if (!tabWnd)
        {
            res = 0;
            continue;
        }

        bool const isChild = tabWnd->IsChildOf(this);
        if (tabId == i)
        {
            if (!isChild)
            {
                AddChild(tabWnd);
                MoveChildToFirstPosition(tabWnd);
            }
        }
        else if (isChild)
        {
            RemoveChild(tabWnd);
        }
    }

    int const result = UpdateButtonsState() & res;
    if (oldTab != m_curTab && bPostTabMessage)
    {
        PostCurrentTabMessage();
    }
    return result;
}

int JournalWnd::SelectButton(Tab tabId, bool bSelect)
{
    if (tabId == NUM_TABS || !m_tabButtons[tabId])
    {
        return 0;
    }
    m_tabButtons[tabId]->SetPane(bSelect ? m_aif.m_tabBtnPaneNameSelected : m_aif.m_tabBtnPaneNameUnselected);
    return 1;
}
