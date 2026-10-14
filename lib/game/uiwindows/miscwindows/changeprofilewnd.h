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
        CStr m_wndProfileListName;
        CStr m_btnChoseName;
        CStr m_btnDeleteName;
        CStr m_lblCurProfileName;
        CStr m_strIdCurProfile;
        unsigned int m_curProfileNameColor;
        AuxInfo(ChangeProfileWnd::AuxInfo const&);
        AuxInfo();
    };

protected:
    virtual int GameDataSetup() override;
    virtual int GameDataUpdate(void* data, int dataType) override;
    virtual int OnWndNotify(m3d::ui::Wnd* from, unsigned int id, unsigned int msg, m3d::AIParam const& data) override
       ;
    virtual int OnBeforeAddToWndStation() override;
    virtual int OnKey(unsigned short key, unsigned char scanCode, unsigned int state) override;
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

    m3d::ui::StringsListBoxWnd* m_wndProfileList;
    m3d::ui::ButtonWnd* m_btnChose;
    m3d::ui::ButtonWnd* m_btnDelete;
    m3d::ui::Wnd* m_lblCurProfile;
    ChangeProfileWnd::AuxInfo m_aif;

    ChangeProfileWnd();
    ChangeProfileWnd(ChangeProfileWnd const& rhs);

public:
    virtual ~ChangeProfileWnd() override;
    virtual m3d::Object* Clone() override;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override;
    RT_CLASS_DECLARE(ChangeProfileWnd);
};
