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
        CStr m_wndPictureName;
        CStr m_wndDizName;
        CStr m_wndItemsListName;
        AuxInfo(const QuestItemsWnd::AuxInfo&);
        AuxInfo();
    };

protected:
    virtual int GameDataSetup() override;
    virtual int GameDataUpdate(void* data, int dataType) override;
    virtual int GameDataClear(bool beforeContinuousLevel) override;
    virtual int OnWndNotify(m3d::ui::Wnd* from, unsigned int idFrom, unsigned int message, m3d::AIParam const& data)
        override;
    virtual int OnBeforeAddToWndStation() override;
    virtual int OnAfterAddToWndStation() override;
    void FullUpdate();
    void OnQuestItemsChanged();
    void CreateItems();
    void ClearItems();
    void OnItemsListSelChanged(m3d::AIParam const& data);
    QuestItemsWnd::AuxInfo m_aif;
    ContextModelWnd* m_wndPicture;
    m3d::ui::TextBoxWnd* m_wndDiz;
    CheckList* m_wndItemsList;
    QuestItemsWnd();
    QuestItemsWnd(QuestItemsWnd const& rhs);

public:
    virtual ~QuestItemsWnd() override;
    virtual m3d::Object* Clone() override;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override;
    RT_CLASS_DECLARE(QuestItemsWnd);
};
