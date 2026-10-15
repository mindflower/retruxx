#pragma once
#include <ui/wnd.h>

namespace m3d
{
    namespace ui
    {
        class ButtonWnd;
        class EditWnd;
    }  // namespace ui
}  // namespace m3d

class NewProfileWnd : public m3d::ui::ModalWnd
{
public:
    struct AuxInfo
    {
        CStr m_editNameName;
        CStr m_btnCancelName;
        AuxInfo(NewProfileWnd::AuxInfo const&);
        AuxInfo();
    };

protected:
    virtual int GameDataSetup() override;
    virtual int OnWndNotify(m3d::ui::Wnd* from, unsigned int id, unsigned int msg, m3d::AIParam const& data) override;
    virtual int OnKey(unsigned short key, unsigned char scanCode, unsigned int state) override;
    virtual int OnBeforeAddToWndStation() override;
    int CreateNewProfile();
    void OnEditChange();

    m3d::ui::EditWnd* m_editName;
    m3d::ui::ButtonWnd* m_btnCancel;
    NewProfileWnd::AuxInfo m_aif;

    NewProfileWnd();
    NewProfileWnd(NewProfileWnd const& rhs);

public:
    virtual ~NewProfileWnd() override;
    virtual m3d::Object* Clone() override;
    static m3d::Object* CreateObject();
    static m3d::Class* GetBaseClass();
    virtual m3d::Class* GetClass() const override;
    RT_CLASS_DECLARE(NewProfileWnd);
};
