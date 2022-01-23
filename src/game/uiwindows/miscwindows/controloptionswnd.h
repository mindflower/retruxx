#pragma once
#include <core/ref_ptr.h>
#include <ui/ui.h>

class BindKeysWnd;

namespace m3d
{
    namespace ui
    {
        class ButtonWnd;
        class CheckWnd;
        class SliderWnd;
    }
}

class ControlOptionsWnd :  public m3d::ui::Wnd
{
public:
    class AuxInfo
    {
    public:
        AuxInfo();

    private:
        CStr m_sliderMouseSensitivityName;
        CStr m_btnMouseSensitivityPrevName;
        CStr m_btnMouseSensitivityNextName;
        CStr m_checkMouseFlipYName;
        CStr m_checkMouseFlipXName;
    };

public:
    static m3d::Class * GetBaseClass();
    int ApplyChanges(bool);
    virtual m3d::Class * GetClass() const ;
    static m3d::Object * CreateObject();
    virtual ~ControlOptionsWnd();
    virtual m3d::Object * Clone();

protected:
    void OnCheckMouseFlipYClick(m3d::AIParam const &);
    void OnCheckMouseFlipXClick(m3d::AIParam const &);
    void UpdateControls();
    void UpdateMouseFlipYControls();
    void InitMouseSensitivityControls();
    void UpdateMouseFlipXControls();
    void ApplyMouseFlipY();
    void ApplyMouseFlipX();
    void InitMouseFlipXControls();
    void InitMouseFlipYControls();
    ControlOptionsWnd(ControlOptionsWnd const &);
    ControlOptionsWnd();
    void OnBtnMouseSensitivityNextClick(m3d::AIParam const &);
    virtual int GameDataSetup();
    void OnBtnMouseSensitivityPrevClick(m3d::AIParam const &);
    virtual int OnWndNotify(m3d::ui::Wnd *,unsigned int,unsigned int, m3d::AIParam const &);
    void UpdateMouseSensitivityControls();
    virtual int OnBeforeAddToWndStation();
    void ApplyMouseSensitivity();
    void UpdateMouseSensitivityPrevNextButtonsState();
    void InitControls();
    void OnSliderMouseSensitivityChange(m3d::AIParam const &);

public:
    RT_CLASS_DECLARE(ControlOptionsWnd);

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
