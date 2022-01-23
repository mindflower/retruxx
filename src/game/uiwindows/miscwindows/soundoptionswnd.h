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
        CStr m_sliderMusicVolumeName;
        CStr m_sliderEffectsVolumeName;
        CStr m_sliderSpeakVolumeName;
        CStr m_btnMusicVolumePrevName;
        CStr m_btnMusicVolumeNextName;
        CStr m_btnEffectsVolumePrevName;
        CStr m_btnEffectsVolumeNextName;
        CStr m_btnSpeakVolumePrevName;
        CStr m_btnSpeakVolumeNextName;
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
    m3d::ui::SliderWnd *m_sliderMusicVolume;
    m3d::ui::SliderWnd *m_sliderEffectsVolume;
    m3d::ui::SliderWnd *m_sliderSpeakVolume;
    m3d::ui::ButtonWnd *m_btnMusicVolumePrev;
    m3d::ui::ButtonWnd *m_btnMusicVolumeNext;
    m3d::ui::ButtonWnd *m_btnEffectsVolumePrev;
    m3d::ui::ButtonWnd *m_btnEffectsVolumeNext;
    m3d::ui::ButtonWnd *m_btnSpeakVolumePrev;
    m3d::ui::ButtonWnd *m_btnSpeakVolumeNext;
    int m_sliderMusicVolumeBlocked;
    int m_sliderEffectsVolumeBlocked;
    int m_sliderSpeakVolumeBlocked;
    SoundOptionsWnd::AuxInfo m_aif;
};
