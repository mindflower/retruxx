#pragma once

class ControlOptionsWnd :  public m3d::ui::Wnd
{
public:
    static struct m3d::Class * __fastcall GetBaseClass();
    int ApplyChanges(bool);
    virtual struct m3d::Class * GetClass() const ;
    static class m3d::Object * __fastcall CreateObject();
    virtual ~ControlOptionsWnd();
    virtual class m3d::Object * Clone();
protected:
    void OnCheckMouseFlipYClick(class m3d::AIParam const &);
    void OnCheckMouseFlipXClick(class m3d::AIParam const &);
    void UpdateControls();
    void UpdateMouseFlipYControls();
    void InitMouseSensitivityControls();
    void UpdateMouseFlipXControls();
    void ApplyMouseFlipY();
    void ApplyMouseFlipX();
    void InitMouseFlipXControls();
    void InitMouseFlipYControls();
    ControlOptionsWnd(class ControlOptionsWnd const &);
    ControlOptionsWnd();
    void OnBtnMouseSensitivityNextClick(class m3d::AIParam const &);
    virtual int GameDataSetup();
    void OnBtnMouseSensitivityPrevClick(class m3d::AIParam const &);
    virtual int OnWndNotify(class m3d::ui::Wnd *,unsigned int,unsigned int,class m3d::AIParam const &);
    void UpdateMouseSensitivityControls();
    virtual int OnBeforeAddToWndStation();
    void ApplyMouseSensitivity();
    void UpdateMouseSensitivityPrevNextButtonsState();
    void InitControls();
    void OnSliderMouseSensitivityChange(class m3d::AIParam const &);
private:
    m3d::ui::SliderWnd *m_sliderMouseSensitivity;
    m3d::ui::ButtonWnd *m_btnMouseSensitivityPrev;
    m3d::ui::ButtonWnd *m_btnMouseSensitivityNext;
    m3d::ui::CheckWnd *m_checkMouseFlipY;
    m3d::ui::CheckWnd *m_checkMouseFlipX;
    int m_sliderMouseSensitivityBlocked;
    ControlOptionsWnd::AuxInfo m_aif;
    ref_ptr<BindKeysWnd> m_wndKeyBindings;
};
