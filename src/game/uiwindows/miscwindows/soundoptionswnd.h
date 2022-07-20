#pragma once
#include <ui/ui.h>

namespace m3d
{
    namespace ui
    {
        class ButtonWnd;
        class SliderWnd;
    }
}

class SoundOptionsWnd :  public m3d::ui::Wnd
{
public:
    class AuxInfo
    {
    public:
        AuxInfo();

    private:
        CStr m_sliderMusicVolumeName = "sliderMusicVolume";
        CStr m_sliderEffectsVolumeName = "sliderEffectsVolume";
        CStr m_sliderSpeakVolumeName = "sliderSpeakVolume";
        CStr m_btnMusicVolumePrevName = "btnMusicVolumePrev";
        CStr m_btnMusicVolumeNextName = "btnMusicVolumeNext";
        CStr m_btnEffectsVolumePrevName = "btnEffectsVolumePrev";
        CStr m_btnEffectsVolumeNextName = "btnEffectsVolumeNext";
        CStr m_btnSpeakVolumePrevName = "btnSpeakVolumePrev";
        CStr m_btnSpeakVolumeNextName = "btnSpeakVolumeNext";
    };

public:
    virtual m3d::Class * GetClass() const ;
    static m3d::Object * CreateObject();
    virtual m3d::Object * Clone();
    static m3d::Class * GetBaseClass();
    virtual ~SoundOptionsWnd();

protected:
    virtual int OnBeforeAddToWndStation();
    void InitControls();
    void InitEffectsVolumeControls();
    void ApplyEffectsVolume();
    virtual int OnWndNotify(m3d::ui::Wnd *,unsigned int,unsigned int, m3d::AIParam const &);
    void OnBtnMusicVolumeNextClick(m3d::AIParam const &);
    void OnBtnMusicVolumePrevClick(m3d::AIParam const &);
    void OnBtnEffectsVolumePrevClick(m3d::AIParam const &);
    void ApplySpeakVolume();
    void UpdateControls();
    void OnBtnEffectsVolumeNextClick(m3d::AIParam const &);
    void UpdateMusicVolumeControls();
    void UpdateMusicVolumePrevNextButtonsState();
    void OnSliderSpeakVolumeChange(m3d::AIParam const &);
    void UpdateSpeakVolumeControls();
    void OnSliderMusicVolumeChange(m3d::AIParam const &);
    void UpdateSpeakVolumePrevNextButtonsState();
    void ApplyMusicVolume();
    void OnBtnSpeakVolumePrevClick(m3d::AIParam const &);
    void OnBtnSpeakVolumeNextClick(m3d::AIParam const &);
    void UpdateEffectsVolumePrevNextButtonsState();
    void InitMusicVolumeControls();
    void OnSliderEffectsVolumeChange(m3d::AIParam const &);
    void InitSpeakVolumeControls();
    virtual int GameDataSetup();
    SoundOptionsWnd(SoundOptionsWnd const &);
    SoundOptionsWnd();
    void UpdateEffectsVolumeControls();

public:
    RT_CLASS_DECLARE(SoundOptionsWnd);

private:
    m3d::ui::SliderWnd *m_sliderMusicVolume = nullptr;
    m3d::ui::SliderWnd *m_sliderEffectsVolume = nullptr;
    m3d::ui::SliderWnd *m_sliderSpeakVolume = nullptr;
    m3d::ui::ButtonWnd *m_btnMusicVolumePrev = nullptr;
    m3d::ui::ButtonWnd *m_btnMusicVolumeNext = nullptr;
    m3d::ui::ButtonWnd *m_btnEffectsVolumePrev = nullptr;
    m3d::ui::ButtonWnd *m_btnEffectsVolumeNext = nullptr;
    m3d::ui::ButtonWnd *m_btnSpeakVolumePrev = nullptr;
    m3d::ui::ButtonWnd *m_btnSpeakVolumeNext = nullptr;
    int m_sliderMusicVolumeBlocked = 0;
    int m_sliderEffectsVolumeBlocked = 0;
    int m_sliderSpeakVolumeBlocked = 0;
    SoundOptionsWnd::AuxInfo m_aif;
};
