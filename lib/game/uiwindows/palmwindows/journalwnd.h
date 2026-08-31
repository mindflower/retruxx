#pragma once
#include "screenwnd.h"
#include <core/ref_ptr.h>

namespace m3d
{
    namespace ui
    {
        class ButtonWnd;
    }
}  // namespace m3d

class JournalWnd : public ScreenWnd
{
public:
    int AddHistory(CStr const& strTextId, m3d::AIParam const& time);
    int AddBook(CStr const& strBookNameId, CStr const& strBookTextId);
    bool BookExists(CStr const& strBookNameId) const;
    int AddPrototypeToEncyclopaedia(CStr const& prototypeName);
    int AddClanToEncyclopaedia(int clanBelong);
    void ShowAllInEncyclopaedia();

    enum Tab
    {
        TAB_HISTORY = 0,
        TAB_BOOKS = 1,
        TAB_RELATIONS = 2,
        TAB_STATS = 3,
        TAB_ENCYCLOPAEDIA = 4,
        NUM_TABS = 5,
    };

    enum
    {
        CTRL_ID_BTN_HISTORY = 0x30d40,
        CTRL_ID_BTN_BOOKS = 0x30d41,
        CTRL_ID_BTN_RELATIONS = 0x30d42,
        CTRL_ID_BTN_STATS = 0x30d43,
        CTRL_ID_BTN_ENCYCLOPAEDIA = 0x30d44,
    };

    struct AuxInfo
    {
        /* 0x0000 */ CStr m_tabButtonNames[5];
        /* 0x003c */ CStr m_tabBtnPaneNameUnselected;
        /* 0x0048 */ CStr m_tabBtnPaneNameSelected;
        AuxInfo(JournalWnd::AuxInfo const&);
        AuxInfo();
    }; /* size: 0x0054 */

protected:
    virtual int GameDataSetup() override /* 0x00 */;
    virtual int GameDataSave(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* guiNode) override /* 0x00 */;
    virtual int GameDataLoad(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* guiNode) override /* 0x00 */;
    virtual int OnWndNotify(m3d::ui::Wnd* from, unsigned int idFrom, unsigned int message, m3d::AIParam const& data)
        override /* 0x00 */;
    virtual int OnBeforeAddToWndStation() override /* 0x00 */;
    int SetCurTab(JournalWnd::Tab tabId, bool bPostTabMessage);
    int UpdateButtonsState();
    int SelectButton(JournalWnd::Tab tabId, bool bSelect);
    void PostCurrentTabMessage() const;
    JournalWnd::Tab CtrlId2TabId(int ctrlId) const;
    /* 0x0224 */ JournalWnd::AuxInfo m_aif;
    /* 0x0278 */ m3d::ui::ButtonWnd* m_tabButtons[5];
    /* 0x028c */ ref_ptr<m3d::ui::Wnd> m_tabs[5];
    /* 0x02a0 */ JournalWnd::Tab m_curTab;
    JournalWnd();
    JournalWnd(JournalWnd const& rhs);

public:
    virtual ~JournalWnd() override /* 0x00 */;
    virtual m3d::Object* Clone() override /* 0x04 */;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override /* 0x34 */;
    static m3d::Class m_classJournalWnd;
}; /* size: 0x02a4 */
