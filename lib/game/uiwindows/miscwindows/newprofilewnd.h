#pragma once
#include <ui/wnd.h>

namespace m3d
{
    namespace ui
    {
        class ButtonWnd;
        class EditWnd;
    }
}

class NewProfileWnd : public m3d::ui::ModalWnd
{
public:
    struct AuxInfo
    {
        /* 0x0000 */ CStr m_editNameName;
        /* 0x000c */ CStr m_btnCancelName;
        AuxInfo(NewProfileWnd::AuxInfo const&);
        AuxInfo();
    }; /* size: 0x0018 */

protected:
    virtual int GameDataSetup() override /* 0x00 */;
    virtual int OnWndNotify(m3d::ui::Wnd* from, unsigned int id, unsigned int msg, m3d::AIParam const& data) override /* 0x00 */;
    virtual int OnKey(unsigned short key, unsigned char scanCode, unsigned int state) override /* 0x00 */;
    virtual int OnBeforeAddToWndStation() override /* 0x00 */;
    int CreateNewProfile();
    void OnEditChange();

    /* 0x0224 */ m3d::ui::EditWnd* m_editName;
    /* 0x0228 */ m3d::ui::ButtonWnd* m_btnCancel;
    /* 0x022c */ NewProfileWnd::AuxInfo m_aif;

    NewProfileWnd();
    NewProfileWnd(NewProfileWnd const& rhs);

public:
    virtual ~NewProfileWnd() override /* 0x00 */;
    virtual m3d::Object* Clone() override /* 0x00 */;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override /* 0x00 */;
    RT_CLASS_DECLARE(NewProfileWnd);
}; /* size: 0x0244 */
