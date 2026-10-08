#pragma once
#include <ui/wnd.h>

namespace m3d
{
    namespace ui
    {
        class ButtonWnd;
        class StringsListBoxWnd;
    }  // namespace ui
}  // namespace m3d

class ChangeProfileWnd : public m3d::ui::ModalWnd
{
public:
    struct AuxInfo
    {
        /* 0x0000 */ CStr m_wndProfileListName;
        /* 0x000c */ CStr m_btnChoseName;
        /* 0x0018 */ CStr m_btnDeleteName;
        /* 0x0024 */ CStr m_lblCurProfileName;
        /* 0x0030 */ CStr m_strIdCurProfile;
        /* 0x003c */ unsigned int m_curProfileNameColor;
        AuxInfo(ChangeProfileWnd::AuxInfo const&);
        AuxInfo();
    }; /* size: 0x0040 */

protected:
    virtual int GameDataSetup() override /* 0x00 */;
    virtual int GameDataUpdate(void* data, int dataType) override /* 0x00 */;
    virtual int OnWndNotify(m3d::ui::Wnd* from, unsigned int id, unsigned int msg, m3d::AIParam const& data) override
        /* 0x00 */;
    virtual int OnBeforeAddToWndStation() override /* 0x00 */;
    virtual int OnKey(unsigned short key, unsigned char scanCode, unsigned int state) override /* 0x00 */;
    int ChoseProfile();
    int ChoseProfileAndExit();
    int DeleteProfile();
    int CreateProfile();
    void OnChangeProfileSelection();
    CStr GetSelectedProfileName() const;
    void FullUpdate();
    void OnProfilesListChanged();
    void OnCurProfileChanged();
    void Clear();

    /* 0x0224 */ m3d::ui::StringsListBoxWnd* m_wndProfileList;
    /* 0x0228 */ m3d::ui::ButtonWnd* m_btnChose;
    /* 0x022c */ m3d::ui::ButtonWnd* m_btnDelete;
    /* 0x0230 */ m3d::ui::Wnd* m_lblCurProfile;
    /* 0x0234 */ ChangeProfileWnd::AuxInfo m_aif;

    ChangeProfileWnd();
    ChangeProfileWnd(ChangeProfileWnd const& rhs);

public:
    virtual ~ChangeProfileWnd() override /* 0x00 */;
    virtual m3d::Object* Clone() override /* 0x00 */;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override /* 0x00 */;
    RT_CLASS_DECLARE(ChangeProfileWnd);
}; /* size: 0x0274 */
