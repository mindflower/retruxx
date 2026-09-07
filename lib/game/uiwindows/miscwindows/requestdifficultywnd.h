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
        /* 0x0000 */ CStr m_cbDifficultyLevelsName;
        AuxInfo(RequestDifficultyWnd::AuxInfo const&);
        AuxInfo();
    }; /* size: 0x000c */

protected:
    virtual int GameDataSetup() override /* 0x00 */;
    virtual int OnBeforeAddToWndStation() override /* 0x00 */;
    virtual int OnBeforeRemoveFromWndStation() override /* 0x00 */;
    void FillDifficultyLevelsList();
    virtual int OnKey(unsigned short key, unsigned char scanCode, unsigned int state) override /* 0x00 */;
    void Clear();
    void ApplyDifficultyLevel();
    /* 0x0224 */ m3d::ui::ComboBoxWnd* m_cbDifficultyLevels;
    /* 0x0228 */ RequestDifficultyWnd::AuxInfo m_aif;
    RequestDifficultyWnd();
    RequestDifficultyWnd(RequestDifficultyWnd const& rhs);

public:
    virtual ~RequestDifficultyWnd() override /* 0x00 */;
    virtual m3d::Object* Clone() override /* 0x00 */;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override /* 0x00 */;
    RT_CLASS_DECLARE(RequestDifficultyWnd);
}; /* size: 0x0234 */
