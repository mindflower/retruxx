#pragma once
#include <ui/ui.h>

class CheckList;

namespace m3d
{
    namespace ui
    {
        class TextBoxWnd;
    }
}

class ContextModelWnd;

class QuestItemsWnd : public m3d::ui::Wnd
{
    struct AuxInfo
    {
        /* 0x0000 */ CStr m_wndPictureName;
        /* 0x000c */ CStr m_wndDizName;
        /* 0x0018 */ CStr m_wndItemsListName;
        AuxInfo(const QuestItemsWnd::AuxInfo&);
        AuxInfo();
    }; /* size: 0x0024 */

protected:
    virtual int GameDataSetup() override /* 0x104 */;
    virtual int GameDataUpdate(void* data, int dataType) override /* 0x10c */;
    virtual int GameDataClear(bool beforeContinuousLevel) override /* 0x108 */;
    virtual int OnWndNotify(m3d::ui::Wnd* from, unsigned int idFrom, unsigned int message, m3d::AIParam const& data)
        override /* 0xc8 */;
    virtual int OnBeforeAddToWndStation() override /* 0x68 */;
    virtual int OnAfterAddToWndStation() override /* 0x6c */;
    void FullUpdate();
    void OnQuestItemsChanged();
    void CreateItems();
    void ClearItems();
    void OnItemsListSelChanged(m3d::AIParam const& data);
    /* 0x0220 */ QuestItemsWnd::AuxInfo m_aif;
    /* 0x0244 */ ContextModelWnd* m_wndPicture;
    /* 0x0248 */ m3d::ui::TextBoxWnd* m_wndDiz;
    /* 0x024c */ CheckList* m_wndItemsList;
    QuestItemsWnd();
    QuestItemsWnd(QuestItemsWnd const& rhs);

public:
    virtual ~QuestItemsWnd() override /* 0x00 */;
    virtual m3d::Object* Clone() override /* 0x04 */;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override /* 0x34 */;
    RT_CLASS_DECLARE(QuestItemsWnd);
}; /* size: 0x0250 */
