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
    }  // namespace ui
}  // namespace m3d

class VideoOptionsWnd : public m3d::ui::Wnd
{
    // OptionsWnd owns the tab and asks it to commit or discard its pending
    // changes when the options dialog closes.
    friend class OptionsWnd;

public:
    int ApplyChanges(bool bForce);

    struct AuxInfo
    {
        CStr m_cbResolutionName;
        CStr m_sliderGammaName;
        CStr m_cbGraphicQualityName;
        CStr m_sliderFarDistanceName;
        CStr m_cbGrassName;
        CStr m_cbShadowsName;
        CStr m_cbWaterQualityName;
        CStr m_cbAntialiasingName;
        CStr m_cbFiltrationName;
        CStr m_cbBlumName;
        CStr m_btnGammaNextName;
        CStr m_btnGammaPrevName;
        CStr m_btnFarDistancePrevName;
        CStr m_btnFarDistanceNextName;
        AuxInfo(VideoOptionsWnd::AuxInfo const&);
        AuxInfo();
    };

    enum Resolution
    {
        RESOLUTION_800x600 = 0,
        RESOLUTION_1024x768 = 1,
        RESOLUTION_1152x864 = 2,
        RESOLUTION_1280x960 = 3,
        RESOLUTION_1600x1200 = 4,
        RESOLUTION_NUM_RESOLUTIONS = 5,
        RESOLUTION_INVALID = 5,
    };

    enum GraphicQuality
    {
        GRAPHIC_QUALITY_LOW = 0,
        GRAPHIC_QUALITY_MEDIUM = 1,
        GRAPHIC_QUALITY_MAX = 2,
        GRAPHIC_QUALITY_CUSTOM = 3,
        GRAPHIC_QUALITY_NUM_GRAPHIC_QUALITIES = 4,
        GRAPHIC_QUALITY_INVALID = 4,
    };

    enum GrassDistance
    {
        GRASS_DISTANCE_NONE = 0,
        GRASS_QUALITY_NEAR = 1,
        GRASS_QUALITY_FAR = 2,
        GRASS_DISTANCE_NUM_GRASS_DISTANCES = 3,
        GRASS_DISTANCE_INVALID = 3,
    };

    enum ShadowsQuality
    {
        SHADOWS_QUALITY_NONE = 0,
        SHADOWS_QUALITY_LOW = 1,
        SHADOWS_QUALITY_HIGH = 2,
        SHADOWS_QUALITY_NUM_SHADOWS_QUALITIES = 3,
        SHADOWS_QUALITY_INVALID = 3,
    };

    enum BlumQuality
    {
        BLUM_QUALITY_NONE = 0,
        BLUM_QUALITY_LOW = 1,
        BLUM_QUALITY_HIGH = 2,
        BLUM_QUALITY_NUM_BLUM_QUALITIES = 3,
        BLUM_QUALITY_INVALID = 3,
    };

    enum Filtration
    {
        FILTRATION_BILINEAR = 0,
        FILTRATION_TRILINEAR = 1,
        FILTRATION_ANISOTROP = 2,
        FILTRATION_NUM_FILTRATIONS = 3,
        FILTRATION_INVALID = 3,
    };

    enum Antialiasing
    {
        ANTIALIASING_NONE = 0,
        ANTIALIASING_X2 = 1,
        ANTIALIASING_X4 = 2,
        ANTIALIASING_X8 = 3,
        ANTIALIASING_X16 = 4,
        ANTIALIASING_NUM_ANTIALIASINGS = 5,
        ANTIALIASING_INVALID = 5,
    };

    enum WaterQuality
    {
        WATER_QUALITY_LOW = 0,
        WATER_QUALITY_MEDIUM = 1,
        WATER_QUALITY_HIGH = 2,
        WATER_QUALITY_NUM_WATER_QUALITIES = 3,
        WATER_QUALITY_INVALID = 3,
    };

    struct ShadowSettings
    {
        int shadowTexSize;
        int detShadowTexSize;
        float shadowBlurCoeff;
        float detailRadius;
        ShadowSettings(int sts, int dsts, float sbc, float dr);
        bool operator==(VideoOptionsWnd::ShadowSettings const& shs) const;
    };

protected:
    virtual int GameDataSetup() override;
    virtual int OnWndNotify(m3d::ui::Wnd* from, unsigned int id, unsigned int msg, m3d::AIParam const& data) override;
    virtual int OnBeforeAddToWndStation() override;
    virtual int OnAfterRemoveFromWndStation() override;
    void InitControls();
    void InitResolutionControls();
    void InitGammaControls();
    void InitGraphicQualityControls();
    void InitFarDistanceControls();
    void InitGrassControls();
    void InitShadowsControls();
    void InitWaterQualityControls();
    void InitAntialiasingControls();
    void InitFiltrationControls();
    void InitBlumControls();
    void UpdateControls();
    void UpdateResolutionControls();
    void UpdateGammaControls();
    void UpdateGraphicQualityControls();
    void UpdateGraphicQualityDependendControls(VideoOptionsWnd::GraphicQuality graphicQuality);
    void UpdateFarDistanceControls(VideoOptionsWnd::GraphicQuality graphicQuality);
    void UpdateGrassControls(VideoOptionsWnd::GraphicQuality graphicQuality);
    void UpdateShadowsControls(VideoOptionsWnd::GraphicQuality graphicQuality);
    void UpdateWaterQualityControls(VideoOptionsWnd::GraphicQuality graphicQuality);
    void UpdateAntialiasingControls(VideoOptionsWnd::GraphicQuality graphicQuality);
    void UpdateFiltrationControls(VideoOptionsWnd::GraphicQuality graphicQuality);
    void UpdateBlumControls(VideoOptionsWnd::GraphicQuality graphicQuality);
    void OnCbResolutionChange(m3d::AIParam const& data);
    void OnSliderGammaChange(m3d::AIParam const& data);
    void OnBtnGammaPrevClick(m3d::AIParam const& data);
    void OnBtnGammaNextClick(m3d::AIParam const& data);
    void OnCbGraphicQualityChange(m3d::AIParam const& data);
    void OnSliderFarDistanceChange(m3d::AIParam const& data);
    void OnBtnFarDistancePrevClick(m3d::AIParam const& data);
    void OnBtnFarDistanceNextClick(m3d::AIParam const& data);
    void OnCbGrassChange(m3d::AIParam const& data);
    void OnCbShadowsChange(m3d::AIParam const& data);
    void OnCbWaterQualityChange(m3d::AIParam const& data);
    void OnCbAntialiasingChange(m3d::AIParam const& data);
    void OnCbFiltrationChange(m3d::AIParam const& data);
    void OnCbBlumChange(m3d::AIParam const& data);
    void OnBtnApplyClick(m3d::AIParam const& data);
    void SetChanged(bool bChanged);
    bool IsChanged() const;
    m3d::ui::MbRetCodes RunChangeWarningDlg();
    void ApplyResolution();
    void ApplyGamma();
    void ApplyGraphicQuality();
    void ApplyFarDistance();
    void ApplyGrass();
    void ApplyShadows();
    void ApplyWaterQuality();
    void ApplyAntialiasing();
    void ApplyFiltration();
    void ApplyBlum();
    float GetDefaultFarDistanceForGraphicQuality(VideoOptionsWnd::GraphicQuality graphicQuality) const;
    float GetDefaultGrassForGraphicQuality(VideoOptionsWnd::GraphicQuality graphicQuality) const;
    VideoOptionsWnd::ShadowSettings const& GetShadowSettings(VideoOptionsWnd::GraphicQuality graphicQuality) const;
    int GetDefaultWaterQualityForGraphicQuality(VideoOptionsWnd::GraphicQuality graphicQuality) const;
    int GetDefaultAntialiasingForGraphicQuality(VideoOptionsWnd::GraphicQuality graphicQuality) const;
    int GetDefaultFiltrationForGraphicQuality(VideoOptionsWnd::GraphicQuality graphicQuality) const;
    int GetDefaultBlumForGraphicQuality(VideoOptionsWnd::GraphicQuality graphicQuality) const;
    void SetDefaultParamsForGraphicQuality(VideoOptionsWnd::GraphicQuality graphicQuality);
    float GetCurrentFarDistance() const;
    float GetCurrentGrass() const;
    int GetCurrentWaterQuality() const;
    int GetCurrentAntialiasing() const;
    int GetCurrentFiltration() const;
    int GetCurrentBlum() const;
    CStr ScreenWH2Str(PointBase<int> const& wh) const;
    PointBase<int> Resolution2ScreenWH(VideoOptionsWnd::Resolution resolution) const;
    VideoOptionsWnd::Resolution ScreenWH2Resolution(PointBase<int> const& wh) const;
    void UpdateGammaPrevNextButtonsState();
    void UpdateFarDistancePrevNextButtonsState();
    VideoOptionsWnd::GraphicQuality DetectCurrentGraphicQuality() const;
    CStr GraphicQuality2Str(VideoOptionsWnd::GraphicQuality graphicQuality) const;
    void OnGraphicQualityDependendControlChanged();
    CStr GrassDistance2Str(VideoOptionsWnd::GrassDistance grassDistance) const;
    VideoOptionsWnd::GrassDistance GrassDistanceVal2Enum(float grassDistanceVal) const;
    float GrassDistanceEnum2Val(VideoOptionsWnd::GrassDistance grassDistance) const;
    CStr ShadowsQuality2Str(VideoOptionsWnd::ShadowsQuality shadowsQuality) const;
    VideoOptionsWnd::ShadowsQuality GetCurrentShadowQuality() const;
    VideoOptionsWnd::ShadowSettings GetCurrentShadowSettings() const;
    CStr BlumQuality2Str(VideoOptionsWnd::BlumQuality blumQuality) const;
    VideoOptionsWnd::BlumQuality BlumQualityVal2Enum(int blumQualityVal) const;
    int BlumQualityEnum2Val(VideoOptionsWnd::BlumQuality blumQuality) const;
    VideoOptionsWnd::Filtration FiltrationVal2Enum(int filtrationVal) const;
    int FiltrationEnum2Val(VideoOptionsWnd::Filtration filtration) const;
    CStr Filtration2Str(VideoOptionsWnd::Filtration filtration) const;
    VideoOptionsWnd::Antialiasing AntialiasingVal2Enum(int antialiasingVal) const;
    int AntialiasingEnum2Val(VideoOptionsWnd::Antialiasing antialiasing) const;
    CStr Antialiasing2Str(VideoOptionsWnd::Antialiasing antialiasing) const;
    VideoOptionsWnd::WaterQuality WaterQualityVal2Enum(int waterQualityVal) const;
    int WaterQualityEnum2Val(VideoOptionsWnd::WaterQuality waterQuality) const;
    CStr WaterQuality2Str(VideoOptionsWnd::WaterQuality waterQuality) const;
    bool IsWaterQualitySupported(VideoOptionsWnd::WaterQuality waterQuality);
    void ValidateWaterQualityVal(int& waterQualityVal) const;
    int GetWaterShaderVersionByWaterQualityVal(int waterQualityVal) const;
    int GetWaterQualityValByWaterShaderVersion(int waterShaderVersion) const;
    m3d::ui::ComboBoxWnd* m_cbResolution = nullptr;
    m3d::ui::SliderWnd* m_sliderGamma = nullptr;
    m3d::ui::ComboBoxWnd* m_cbGraphicQuality = nullptr;
    m3d::ui::SliderWnd* m_sliderFarDistance = nullptr;
    m3d::ui::ComboBoxWnd* m_cbGrass = nullptr;
    m3d::ui::ComboBoxWnd* m_cbShadows = nullptr;
    m3d::ui::ComboBoxWnd* m_cbWaterQuality = nullptr;
    m3d::ui::ComboBoxWnd* m_cbAntialiasing = nullptr;
    m3d::ui::ComboBoxWnd* m_cbFiltration = nullptr;
    m3d::ui::ComboBoxWnd* m_cbBlum = nullptr;
    m3d::ui::ButtonWnd* m_btnGammaPrev = nullptr;
    m3d::ui::ButtonWnd* m_btnGammaNext = nullptr;
    m3d::ui::ButtonWnd* m_btnFarDistancePrev = nullptr;
    m3d::ui::ButtonWnd* m_btnFarDistanceNext = nullptr;
    VideoOptionsWnd::AuxInfo m_aif;
    bool m_bVideoOptionsChanged = false;
    static inline PointBase<int> m_screenWH[5] = {
        {800, 600},
        {1024, 768},
        {1152, 864},
        {1280, 960},
        {1600, 1200},
    };
    static inline float m_grassDistances[3] = {0.0f, 100.0f, 200.0f};
    static inline ShadowSettings m_shadowSettings[3] = {
        {0, 0, 0.0f, 0.0f},
        {0x100, 0x200, 15.0f, 35.0f},
        {0x200, 0x400, 8.0f, 35.0f},
    };
    static inline int m_filtrations[3] = {4, 5, 3};
    static inline int m_antialiasings[5] = {0, 2, 4, 8, 16};
    static inline int m_waterQualities[3] = {1, 2, 3};
    static inline int m_blumQualities[3] = {0, 1, 2};
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
    VideoOptionsWnd();
    VideoOptionsWnd(VideoOptionsWnd const& rhs);

public:
    virtual ~VideoOptionsWnd() override;
    virtual m3d::Object* Clone() override;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override;
    RT_CLASS_DECLARE(VideoOptionsWnd);
};
