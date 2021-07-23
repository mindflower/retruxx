#pragma once

class VideoOptionsWnd :  public m3d::ui::Wnd
{
public:
    static struct m3d::Class * __fastcall GetBaseClass();
    int ApplyChanges(bool);
    virtual class m3d::Object * Clone();
    static class m3d::Object * __fastcall CreateObject();
    virtual struct m3d::Class * GetClass() const ;
    virtual ~VideoOptionsWnd();
    std::allocator<enum GraphicQuality>::allocator<enum GraphicQuality>(class std::allocator<enum GraphicQuality> const &);
    std::allocator<enum GraphicQuality>::allocator<enum GraphicQuality>();
protected:
    int WaterQualityEnum2Val(enum WaterQuality) const ;
    void OnCbBlumChange(class m3d::AIParam const &);
    enum ShadowsQuality GetCurrentShadowQuality() const ;
    void UpdateAntialiasingControls(enum GraphicQuality);
    struct ShadowSettings const & GetShadowSettings(enum GraphicQuality) const ;
    class CStr GraphicQuality2Str(enum GraphicQuality) const ;
    void ApplyGamma();
    int GetWaterShaderVersionByWaterQualityVal(int) const ;
    int GetDefaultAntialiasingForGraphicQuality(enum GraphicQuality) const ;
    void InitFarDistanceControls();
    int BlumQualityEnum2Val(enum BlumQuality) const ;
    int GetCurrentBlum() const ;
    class CStr BlumQuality2Str(enum BlumQuality) const ;
    class CStr Antialiasing2Str(enum Antialiasing) const ;
    virtual int GameDataSetup();
    class CStr ShadowsQuality2Str(enum ShadowsQuality) const ;
    enum m3d::ui::MbRetCodes RunChangeWarningDlg();
    void UpdateShadowsControls(enum GraphicQuality);
    void UpdateFiltrationControls(enum GraphicQuality);
    void InitBlumControls();
    void OnCbResolutionChange(class m3d::AIParam const &);
    void SetChanged(bool);
    void InitShadowsControls();
    int GetCurrentAntialiasing() const ;
    void OnCbShadowsChange(class m3d::AIParam const &);
    void InitAntialiasingControls();
    void InitGrassControls();
    void OnBtnGammaNextClick(class m3d::AIParam const &);
    enum Filtration FiltrationVal2Enum(int) const ;
    void OnGraphicQualityDependendControlChanged();
    void ApplyFiltration();
    void UpdateGammaControls();
    void UpdateResolutionControls();
    void OnSliderFarDistanceChange(class m3d::AIParam const &);
    void OnCbFiltrationChange(class m3d::AIParam const &);
    enum Antialiasing AntialiasingVal2Enum(int) const ;
    int GetCurrentFiltration() const ;
    enum Resolution ScreenWH2Resolution(class PointBase<int> const &) const ;
    float GetDefaultGrassForGraphicQuality(enum GraphicQuality) const ;
    void OnBtnFarDistancePrevClick(class m3d::AIParam const &);
    int FiltrationEnum2Val(enum Filtration) const ;
    void OnSliderGammaChange(class m3d::AIParam const &);
    void UpdateGammaPrevNextButtonsState();
    enum GraphicQuality DetectCurrentGraphicQuality() const ;
    int GetDefaultBlumForGraphicQuality(enum GraphicQuality) const ;
    void UpdateFarDistanceControls(enum GraphicQuality);
    void InitGraphicQualityControls();
    void ApplyResolution();
    void UpdateGraphicQualityDependendControls(enum GraphicQuality);
    void OnBtnGammaPrevClick(class m3d::AIParam const &);
    void OnCbAntialiasingChange(class m3d::AIParam const &);
    void UpdateFarDistancePrevNextButtonsState();
    float GetCurrentFarDistance() const ;
    void UpdateBlumControls(enum GraphicQuality);
    void OnBtnFarDistanceNextClick(class m3d::AIParam const &);
    class PointBase<int> Resolution2ScreenWH(enum Resolution) const ;
    void ApplyWaterQuality();
    void ValidateWaterQualityVal(int &) const ;
    struct ShadowSettings GetCurrentShadowSettings() const ;
    float GetDefaultFarDistanceForGraphicQuality(enum GraphicQuality) const ;
    void UpdateGrassControls(enum GraphicQuality);
    void InitGammaControls();
    void ApplyFarDistance();
    int GetDefaultFiltrationForGraphicQuality(enum GraphicQuality) const ;
    void UpdateControls();
    void OnCbWaterQualityChange(class m3d::AIParam const &);
    int GetCurrentWaterQuality() const ;
    class CStr ScreenWH2Str(class PointBase<int> const &) const ;
    float GetCurrentGrass() const ;
    void UpdateGraphicQualityControls();
    class CStr Filtration2Str(enum Filtration) const ;
    void SetDefaultParamsForGraphicQuality(enum GraphicQuality);
    void InitWaterQualityControls();
    enum BlumQuality BlumQualityVal2Enum(int) const ;
    void InitResolutionControls();
    void ApplyAntialiasing();
    enum WaterQuality WaterQualityVal2Enum(int) const ;
    void OnBtnApplyClick(class m3d::AIParam const &);
    void ApplyBlum();
    class CStr WaterQuality2Str(enum WaterQuality) const ;
    bool IsChanged() const ;
    void ApplyGrass();
    enum GrassDistance GrassDistanceVal2Enum(float) const ;
    virtual int OnWndNotify(class m3d::ui::Wnd *,unsigned int,unsigned int,class m3d::AIParam const &);
    void InitFiltrationControls();
    bool IsWaterQualitySupported(enum WaterQuality);
    float GrassDistanceEnum2Val(enum GrassDistance) const ;
    void ApplyShadows();
    class CStr GrassDistance2Str(enum GrassDistance) const ;
    virtual int OnAfterRemoveFromWndStation();
    void InitControls();
    int AntialiasingEnum2Val(enum Antialiasing) const ;
    void OnCbGraphicQualityChange(class m3d::AIParam const &);
    int GetWaterQualityValByWaterShaderVersion(int) const ;
    virtual int OnBeforeAddToWndStation();
    void OnCbGrassChange(class m3d::AIParam const &);
    VideoOptionsWnd();
    VideoOptionsWnd(class VideoOptionsWnd const &);
    void UpdateWaterQualityControls(enum GraphicQuality);
    void ApplyGraphicQuality();
    int GetDefaultWaterQualityForGraphicQuality(enum GraphicQuality) const ;
private:
    CreateObject();
    m3d::ui::ComboBoxWnd *m_cbResolution;
    m3d::ui::SliderWnd *m_sliderGamma;
    m3d::ui::ComboBoxWnd *m_cbGraphicQuality;
    m3d::ui::SliderWnd *m_sliderFarDistance;
    m3d::ui::ComboBoxWnd *m_cbGrass;
    m3d::ui::ComboBoxWnd *m_cbShadows;
    m3d::ui::ComboBoxWnd *m_cbWaterQuality;
    m3d::ui::ComboBoxWnd *m_cbAntialiasing;
    m3d::ui::ComboBoxWnd *m_cbFiltration;
    m3d::ui::ComboBoxWnd *m_cbBlum;
    m3d::ui::ButtonWnd *m_btnGammaPrev;
    m3d::ui::ButtonWnd *m_btnGammaNext;
    m3d::ui::ButtonWnd *m_btnFarDistancePrev;
    m3d::ui::ButtonWnd *m_btnFarDistanceNext;
    VideoOptionsWnd::AuxInfo m_aif;
    bool m_bVideoOptionsChanged;
    int m_cbResolutionBlocked;
    int m_sliderGammaBlocked;
    int m_cbGraphicQualityBlocked;
    int m_sliderFarDistanceBlocked;
    int m_cbGrassBlocked;
    int m_cbShadowsBlocked;
    int m_cbBlumBlocked;
    int m_cbFiltrationBlocked;
    int m_cbAntialiasingBlocked;
    int m_cbWaterQualityBlocked;
};

namespace VideoOptionsWnd
{
    class ShadowSettings
    {
    public:
        bool operator==(struct ShadowSettings const &) const ;
        ShadowSettings(int,int,float,float);
    protected:
    private:
        int shadowTexSize;
        int detShadowTexSize;
        float shadowBlurCoeff;
        float detailRadius;
    };
}
