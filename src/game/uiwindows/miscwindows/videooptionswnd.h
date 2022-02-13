#pragma once
#include <ui/msgbox.h>
#include <ui/ui.h>

namespace m3d
{
    namespace ui
    {
        class ButtonWnd;
        class SliderWnd;
        class ComboBoxWnd;
    }
}

class VideoOptionsWnd :  public m3d::ui::Wnd
{
public:
    class ShadowSettings
    {
    public:
        bool operator==(ShadowSettings const&) const;
        ShadowSettings(int, int, float, float);

    private:
        int shadowTexSize;
        int detShadowTexSize;
        float shadowBlurCoeff;
        float detailRadius;
    };

    class AuxInfo
    {
    public:
        AuxInfo();

    public:
        CStr m_cbResolutionName = "cbResolution";
        CStr m_sliderGammaName = "sliderGamma";
        CStr m_cbGraphicQualityName = "cbGraphicQuality";
        CStr m_sliderFarDistanceName = "sliderFarDistance";
        CStr m_cbGrassName = "cbGrass";
        CStr m_cbShadowsName = "cbShadows";
        CStr m_cbWaterQualityName = "cbWaterQuality";
        CStr m_cbAntialiasingName = "cbAntialiasing";
        CStr m_cbFiltrationName = "cbFiltration";
        CStr m_cbBlumName = "cbBlum";
        CStr m_btnGammaNextName = "btnGammaPrev";
        CStr m_btnGammaPrevName = "btnGammaNext";
        CStr m_btnFarDistancePrevName = "btnFarDistancePrev";
        CStr m_btnFarDistanceNextName = "btnFarDistanceNext";
    };

    enum Antialiasing
    {
        ANTIALIASING_NONE = 0x0,
        ANTIALIASING_X2 = 0x1,
        ANTIALIASING_X4 = 0x2,
        ANTIALIASING_X8 = 0x3,
        ANTIALIASING_X16 = 0x4,
        ANTIALIASING_NUM_ANTIALIASINGS = 0x5,
        ANTIALIASING_INVALID = 0x5,
    };

    enum Filtration
    {
        FILTRATION_BILINEAR = 0x0,
        FILTRATION_TRILINEAR = 0x1,
        FILTRATION_ANISOTROP = 0x2,
        FILTRATION_NUM_FILTRATIONS = 0x3,
        FILTRATION_INVALID = 0x3,
    };

    enum WaterQuality
    {
        WATER_QUALITY_LOW = 0x0,
        WATER_QUALITY_MEDIUM = 0x1,
        WATER_QUALITY_HIGH = 0x2,
        WATER_QUALITY_NUM_WATER_QUALITIES = 0x3,
        WATER_QUALITY_INVALID = 0x3,
    };

    enum GrassDistance
    {
        GRASS_DISTANCE_NONE = 0x0,
        GRASS_QUALITY_NEAR = 0x1,
        GRASS_QUALITY_FAR = 0x2,
        GRASS_DISTANCE_NUM_GRASS_DISTANCES = 0x3,
        GRASS_DISTANCE_INVALID = 0x3,
    };

    enum GraphicQuality
    {
        GRAPHIC_QUALITY_LOW = 0x0,
        GRAPHIC_QUALITY_MEDIUM = 0x1,
        GRAPHIC_QUALITY_MAX = 0x2,
        GRAPHIC_QUALITY_CUSTOM = 0x3,
        GRAPHIC_QUALITY_NUM_GRAPHIC_QUALITIES = 0x4,
        GRAPHIC_QUALITY_INVALID = 0x4,
    };

    enum BlumQuality
    {
        BLUM_QUALITY_NONE = 0x0,
        BLUM_QUALITY_LOW = 0x1,
        BLUM_QUALITY_HIGH = 0x2,
        BLUM_QUALITY_NUM_BLUM_QUALITIES = 0x3,
        BLUM_QUALITY_INVALID = 0x3,
    };

    enum ShadowsQuality
    {
        SHADOWS_QUALITY_NONE = 0x0,
        SHADOWS_QUALITY_LOW = 0x1,
        SHADOWS_QUALITY_HIGH = 0x2,
        SHADOWS_QUALITY_NUM_SHADOWS_QUALITIES = 0x3,
        SHADOWS_QUALITY_INVALID = 0x3,
    };

    enum Resolution
    {
        RESOLUTION_800x600 = 0x0,
        RESOLUTION_1024x768 = 0x1,
        RESOLUTION_1152x864 = 0x2,
        RESOLUTION_1280x960 = 0x3,
        RESOLUTION_1600x1200 = 0x4,
        RESOLUTION_NUM_RESOLUTIONS = 0x5,
        RESOLUTION_INVALID = 0x5,
    };

public:
    static m3d::Class * GetBaseClass();
    int ApplyChanges(bool);
    virtual m3d::Object * Clone();
    static m3d::Object * CreateObject();
    virtual m3d::Class * GetClass() const ;
    virtual ~VideoOptionsWnd();

protected:
    int WaterQualityEnum2Val(WaterQuality) const ;
    void OnCbBlumChange(m3d::AIParam const &);
    ShadowsQuality GetCurrentShadowQuality() const ;
    void UpdateAntialiasingControls(GraphicQuality);
    ShadowSettings const & GetShadowSettings(GraphicQuality) const ;
    CStr GraphicQuality2Str(GraphicQuality) const ;
    void ApplyGamma();
    int GetWaterShaderVersionByWaterQualityVal(int) const ;
    int GetDefaultAntialiasingForGraphicQuality(GraphicQuality) const ;
    void InitFarDistanceControls();
    int BlumQualityEnum2Val(BlumQuality) const ;
    int GetCurrentBlum() const ;
    CStr BlumQuality2Str(BlumQuality) const ;
    CStr Antialiasing2Str(Antialiasing) const ;
    virtual int GameDataSetup();
    CStr ShadowsQuality2Str(ShadowsQuality) const ;
    m3d::ui::MbRetCodes RunChangeWarningDlg();
    void UpdateShadowsControls(GraphicQuality);
    void UpdateFiltrationControls(GraphicQuality);
    void InitBlumControls();
    void OnCbResolutionChange(m3d::AIParam const &);
    void SetChanged(bool);
    void InitShadowsControls();
    int GetCurrentAntialiasing() const ;
    void OnCbShadowsChange(m3d::AIParam const &);
    void InitAntialiasingControls();
    void InitGrassControls();
    void OnBtnGammaNextClick(m3d::AIParam const &);
    Filtration FiltrationVal2Enum(int) const ;
    void OnGraphicQualityDependendControlChanged();
    void ApplyFiltration();
    void UpdateGammaControls();
    void UpdateResolutionControls();
    void OnSliderFarDistanceChange(m3d::AIParam const &);
    void OnCbFiltrationChange(m3d::AIParam const &);
    Antialiasing AntialiasingVal2Enum(int) const ;
    int GetCurrentFiltration() const ;
    Resolution ScreenWH2Resolution(PointBase<int> const &) const ;
    float GetDefaultGrassForGraphicQuality(GraphicQuality) const ;
    void OnBtnFarDistancePrevClick(m3d::AIParam const &);
    int FiltrationEnum2Val(Filtration) const ;
    void OnSliderGammaChange(m3d::AIParam const &);
    void UpdateGammaPrevNextButtonsState();
    GraphicQuality DetectCurrentGraphicQuality() const ;
    int GetDefaultBlumForGraphicQuality(GraphicQuality) const ;
    void UpdateFarDistanceControls(GraphicQuality);
    void InitGraphicQualityControls();
    void ApplyResolution();
    void UpdateGraphicQualityDependendControls(GraphicQuality);
    void OnBtnGammaPrevClick(m3d::AIParam const &);
    void OnCbAntialiasingChange(m3d::AIParam const &);
    void UpdateFarDistancePrevNextButtonsState();
    float GetCurrentFarDistance() const ;
    void UpdateBlumControls(GraphicQuality);
    void OnBtnFarDistanceNextClick(m3d::AIParam const &);
    PointBase<int> Resolution2ScreenWH(Resolution) const ;
    void ApplyWaterQuality();
    void ValidateWaterQualityVal(int &) const ;
    ShadowSettings GetCurrentShadowSettings() const ;
    float GetDefaultFarDistanceForGraphicQuality(GraphicQuality) const ;
    void UpdateGrassControls(GraphicQuality);
    void InitGammaControls();
    void ApplyFarDistance();
    int GetDefaultFiltrationForGraphicQuality(GraphicQuality) const ;
    void UpdateControls();
    void OnCbWaterQualityChange(m3d::AIParam const &);
    int GetCurrentWaterQuality() const ;
    CStr ScreenWH2Str(PointBase<int> const &) const ;
    float GetCurrentGrass() const ;
    void UpdateGraphicQualityControls();
    CStr Filtration2Str(Filtration) const ;
    void SetDefaultParamsForGraphicQuality(GraphicQuality);
    void InitWaterQualityControls();
    BlumQuality BlumQualityVal2Enum(int) const ;
    void InitResolutionControls();
    void ApplyAntialiasing();
    WaterQuality WaterQualityVal2Enum(int) const ;
    void OnBtnApplyClick(m3d::AIParam const &);
    void ApplyBlum();
    CStr WaterQuality2Str(WaterQuality) const ;
    bool IsChanged() const ;
    void ApplyGrass();
    GrassDistance GrassDistanceVal2Enum(float) const ;
    virtual int OnWndNotify(m3d::ui::Wnd *,unsigned int,unsigned int, m3d::AIParam const &);
    void InitFiltrationControls();
    bool IsWaterQualitySupported(WaterQuality);
    float GrassDistanceEnum2Val(GrassDistance) const ;
    void ApplyShadows();
    CStr GrassDistance2Str(GrassDistance) const ;
    virtual int OnAfterRemoveFromWndStation();
    void InitControls();
    int AntialiasingEnum2Val(Antialiasing) const ;
    void OnCbGraphicQualityChange(m3d::AIParam const &);
    int GetWaterQualityValByWaterShaderVersion(int) const ;
    virtual int OnBeforeAddToWndStation();
    void OnCbGrassChange(m3d::AIParam const &);
    VideoOptionsWnd();
    VideoOptionsWnd(VideoOptionsWnd const &);
    void UpdateWaterQualityControls(GraphicQuality);
    void ApplyGraphicQuality();
    int GetDefaultWaterQualityForGraphicQuality(GraphicQuality) const ;

public:
    RT_CLASS_DECLARE(VideoOptionsWnd);

private:
    m3d::ui::ComboBoxWnd* m_cbResolution = nullptr;
    m3d::ui::SliderWnd *m_sliderGamma = nullptr;
    m3d::ui::ComboBoxWnd *m_cbGraphicQuality = nullptr;
    m3d::ui::SliderWnd *m_sliderFarDistance = nullptr;
    m3d::ui::ComboBoxWnd *m_cbGrass = nullptr;
    m3d::ui::ComboBoxWnd *m_cbShadows = nullptr;
    m3d::ui::ComboBoxWnd *m_cbWaterQuality = nullptr;
    m3d::ui::ComboBoxWnd *m_cbAntialiasing = nullptr;
    m3d::ui::ComboBoxWnd *m_cbFiltration = nullptr;
    m3d::ui::ComboBoxWnd *m_cbBlum = nullptr;
    m3d::ui::ButtonWnd *m_btnGammaPrev = nullptr;
    m3d::ui::ButtonWnd *m_btnGammaNext = nullptr;
    m3d::ui::ButtonWnd *m_btnFarDistancePrev = nullptr;
    m3d::ui::ButtonWnd *m_btnFarDistanceNext = nullptr;
    VideoOptionsWnd::AuxInfo m_aif;
    bool m_bVideoOptionsChanged = false;
    int m_cbResolutionBlocked = 0;
    int m_sliderGammaBlocked = 0;
    int m_cbGraphicQualityBlocked = 0;
    int m_sliderFarDistanceBlocked = 0;
    int m_cbGrassBlocked = 0;
    int m_cbShadowsBlocked = 0;
    int m_cbBlumBlocked = 0;
    int m_cbFiltrationBlocked = 0;
    int m_cbAntialiasingBlocked = 0;
    int m_cbWaterQualityBlocked = 0;
};
