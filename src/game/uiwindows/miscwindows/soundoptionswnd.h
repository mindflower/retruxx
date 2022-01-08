#pragma once

class SoundOptionsWnd :  public m3d::ui::Wnd
{
public:
    virtual struct m3d::Class * GetClass() const ;
    static class m3d::Object * CreateObject();
    virtual class m3d::Object * Clone();
    static struct m3d::Class * GetBaseClass();
    virtual ~SoundOptionsWnd();
protected:
    virtual int OnBeforeAddToWndStation();
    void InitControls();
    void InitEffectsVolumeControls();
    void ApplyEffectsVolume();
    virtual int OnWndNotify(class m3d::ui::Wnd *,unsigned int,unsigned int,class m3d::AIParam const &);
    void OnBtnMusicVolumeNextClick(class m3d::AIParam const &);
    void OnBtnMusicVolumePrevClick(class m3d::AIParam const &);
    void OnBtnEffectsVolumePrevClick(class m3d::AIParam const &);
    void ApplySpeakVolume();
    void UpdateControls();
    void OnBtnEffectsVolumeNextClick(class m3d::AIParam const &);
    void UpdateMusicVolumeControls();
    void UpdateMusicVolumePrevNextButtonsState();
    void OnSliderSpeakVolumeChange(class m3d::AIParam const &);
    void UpdateSpeakVolumeControls();
    void OnSliderMusicVolumeChange(class m3d::AIParam const &);
    void UpdateSpeakVolumePrevNextButtonsState();
    void ApplyMusicVolume();
    void OnBtnSpeakVolumePrevClick(class m3d::AIParam const &);
    void OnBtnSpeakVolumeNextClick(class m3d::AIParam const &);
    void UpdateEffectsVolumePrevNextButtonsState();
    void InitMusicVolumeControls();
    void OnSliderEffectsVolumeChange(class m3d::AIParam const &);
    void InitSpeakVolumeControls();
    virtual int GameDataSetup();
    SoundOptionsWnd(class SoundOptionsWnd const &);
    SoundOptionsWnd();
    void UpdateEffectsVolumeControls();
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
