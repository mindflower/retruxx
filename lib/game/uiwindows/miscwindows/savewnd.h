#pragma once
#include "lswnd.h"

namespace m3d
{
    namespace ui
    {
        class EditWnd;
    }
}

class SaveWnd : public LSWnd
{
public:
    struct SAuxInfo
    {
        /* 0x0000 */ CStr m_btnSaveName;
        /* 0x000c */ CStr m_btnDeleteName;
        /* 0x0018 */ CStr m_editSaveName;
        SAuxInfo(SaveWnd::SAuxInfo const&);
        SAuxInfo();
    }; /* size: 0x0024 */

protected:
    virtual int GameDataSetup() override /* 0x00 */;
    virtual int GameDataUpdate(void* data, int dataType) override /* 0x00 */;
    virtual int OnWndNotify(m3d::ui::Wnd* from, unsigned int id, unsigned int msg, m3d::AIParam const& data) override /* 0x00 */;
    virtual void OnSaveSelectionChange() override /* 0x130 */;
    void OnSave();
    void OnDelete();
    void OnEditChange();
    virtual int SetScreenshotForSave(int saveIdx) override /* 0x140 */;
    virtual void OnEnter() override /* 0x144 */;

    /* 0x02a0 */ m3d::ui::ButtonWnd* m_btnSave;
    /* 0x02a4 */ m3d::ui::ButtonWnd* m_btnDelete;
    /* 0x02a8 */ m3d::ui::EditWnd* m_editSaveName;
    /* 0x02ac */ SaveWnd::SAuxInfo m_saif;
    /* 0x02d0 */ bool m_bNeedScreenshot;

    SaveWnd();
    SaveWnd(SaveWnd const& rhs);

public:
    virtual ~SaveWnd() override /* 0x00 */;
    virtual m3d::Object* Clone() override /* 0x00 */;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override /* 0x00 */;
    RT_CLASS_DECLARE(SaveWnd);
}; /* size: 0x02d4 */
