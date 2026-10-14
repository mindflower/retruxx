#pragma once
#include <ui/wnd.h>

namespace m3d
{
    namespace ui
    {
        class ButtonWnd;
        class SliderWnd;
    }
}  // namespace m3d

class DevideDlg : public m3d::ui::ModalWnd
{
public:
    void SetAmount(int amount);
    int GetDragAmount();
    int GetOriginalAmount();

    struct AuxInfo
    {
        CStr m_lblOriginalAmountName;
        CStr m_lblDragAmountName;
        CStr m_sliderName;
        CStr m_btnPlusName;
        CStr m_btnMinusName;
        AuxInfo(DevideDlg::AuxInfo const&);
        AuxInfo();
    };

protected:
    DevideDlg();
    DevideDlg(DevideDlg const& rhs);

public:
    virtual ~DevideDlg() override;
    virtual m3d::Object* Clone() override;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override;
    RT_CLASS_DECLARE(DevideDlg);
    virtual int GameDataSetup() override;
    virtual int OnWndNotify(m3d::ui::Wnd* from, unsigned int idFrom, unsigned int message, m3d::AIParam const& data)
        override;
    void UpdateLabels();
    int m_maxAmount;
    m3d::ui::Wnd* m_lblOriginalAmount;
    m3d::ui::Wnd* m_lblDragAmount;
    m3d::ui::SliderWnd* m_slider;
    m3d::ui::ButtonWnd* m_btnPlus;
    m3d::ui::ButtonWnd* m_btnMinus;
    DevideDlg::AuxInfo m_aif;
    bool m_isInited;
};
