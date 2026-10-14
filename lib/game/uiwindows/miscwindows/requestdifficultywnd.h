#pragma once
#include <ui/wnd.h>

namespace m3d
{
    namespace ui
    {
        class ComboBoxWnd;
    }
}  // namespace m3d

class RequestDifficultyWnd : public m3d::ui::ModalWnd
{
    struct AuxInfo
    {
        CStr m_cbDifficultyLevelsName;
        AuxInfo(RequestDifficultyWnd::AuxInfo const&);
        AuxInfo();
    };

protected:
    virtual int GameDataSetup() override;
    virtual int OnBeforeAddToWndStation() override;
    virtual int OnBeforeRemoveFromWndStation() override;
    void FillDifficultyLevelsList();
    virtual int OnKey(unsigned short key, unsigned char scanCode, unsigned int state) override;
    void Clear();
    void ApplyDifficultyLevel();
    m3d::ui::ComboBoxWnd* m_cbDifficultyLevels;
    RequestDifficultyWnd::AuxInfo m_aif;
    RequestDifficultyWnd();
    RequestDifficultyWnd(RequestDifficultyWnd const& rhs);

public:
    virtual ~RequestDifficultyWnd() override;
    virtual m3d::Object* Clone() override;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override;
    RT_CLASS_DECLARE(RequestDifficultyWnd);
};
