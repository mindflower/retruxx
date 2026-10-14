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
        CStr m_tabButtonNames[5];
        CStr m_tabBtnPaneNameUnselected;
        CStr m_tabBtnPaneNameSelected;
        AuxInfo(JournalWnd::AuxInfo const&);
        AuxInfo();
    };

protected:
    virtual int GameDataSetup() override;
    virtual int GameDataSave(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* guiNode) override;
    virtual int GameDataLoad(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* guiNode) override;
    virtual int OnWndNotify(m3d::ui::Wnd* from, unsigned int idFrom, unsigned int message, m3d::AIParam const& data)
        override;
    virtual int OnBeforeAddToWndStation() override;
    int SetCurTab(JournalWnd::Tab tabId, bool bPostTabMessage);
    int UpdateButtonsState();
    int SelectButton(JournalWnd::Tab tabId, bool bSelect);
    void PostCurrentTabMessage() const;
    JournalWnd::Tab CtrlId2TabId(int ctrlId) const;
    JournalWnd::AuxInfo m_aif;
    m3d::ui::ButtonWnd* m_tabButtons[5];
    ref_ptr<m3d::ui::Wnd> m_tabs[5];
    JournalWnd::Tab m_curTab;
    JournalWnd();
    JournalWnd(JournalWnd const& rhs);

public:
    virtual ~JournalWnd() override;
    virtual m3d::Object* Clone() override;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override;
    static m3d::Class m_classJournalWnd;
};
