#pragma once
#include "lswnd.h"

namespace m3d
{
    namespace ui
    {
        class EditWnd;
    }
}  // namespace m3d

class SaveWnd : public LSWnd
{
public:
    struct SAuxInfo
    {
        CStr m_btnSaveName;
        CStr m_btnDeleteName;
        CStr m_editSaveName;
        SAuxInfo(SaveWnd::SAuxInfo const&);
        SAuxInfo();
    };

protected:
    virtual int GameDataSetup() override;
    virtual int GameDataUpdate(void* data, int dataType) override;
    virtual int OnWndNotify(m3d::ui::Wnd* from, unsigned int id, unsigned int msg, m3d::AIParam const& data) override;
    virtual void OnSaveSelectionChange() override;
    void OnSave();
    void OnDelete();
    void OnEditChange();
    virtual int SetScreenshotForSave(int saveIdx) override;
    virtual void OnEnter() override;

    m3d::ui::ButtonWnd* m_btnSave;
    m3d::ui::ButtonWnd* m_btnDelete;
    m3d::ui::EditWnd* m_editSaveName;
    SaveWnd::SAuxInfo m_saif;
    bool m_bNeedScreenshot;

    SaveWnd();
    SaveWnd(SaveWnd const& rhs);

public:
    virtual ~SaveWnd() override;
    virtual m3d::Object* Clone() override;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override;
    RT_CLASS_DECLARE(SaveWnd);
};
