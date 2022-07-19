#include "videooptionswnd.h"
#include "ui/comboboxwnd.h"
#include "ui/slider.h"
#include "ui/button.h"
#include <core/log.h>

RT_CLASS_DEFINE(VideoOptionsWnd);

bool VideoOptionsWnd::ShadowSettings::operator==(ShadowSettings const&) const
{
    throw std::logic_error("Not implemented");
}

VideoOptionsWnd::ShadowSettings::ShadowSettings(int, int, float, float)
{
    throw std::logic_error("Not implemented");
}

VideoOptionsWnd::AuxInfo::AuxInfo()
{
}

m3d::Class* VideoOptionsWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

int VideoOptionsWnd::ApplyChanges(bool)
{
    throw std::logic_error("Not implemented");
}

m3d::Object* VideoOptionsWnd::Clone()
{
    throw std::logic_error("Not implemented");
}

m3d::Object* VideoOptionsWnd::CreateObject()
{
    return new VideoOptionsWnd;
}

m3d::Class* VideoOptionsWnd::GetClass() const
{
    return RT_CLASS_LOCAL(VideoOptionsWnd);
}

VideoOptionsWnd::~VideoOptionsWnd()
{
    throw std::logic_error("Not implemented");
}

int VideoOptionsWnd::WaterQualityEnum2Val(WaterQuality) const
{
    throw std::logic_error("Not implemented");
}

void VideoOptionsWnd::OnCbBlumChange(m3d::AIParam const&)
{
    throw std::logic_error("Not implemented");
}

VideoOptionsWnd::ShadowsQuality VideoOptionsWnd::GetCurrentShadowQuality() const
{
    throw std::logic_error("Not implemented");
}

void VideoOptionsWnd::UpdateAntialiasingControls(GraphicQuality)
{
    throw std::logic_error("Not implemented");
}

VideoOptionsWnd::ShadowSettings const& VideoOptionsWnd::GetShadowSettings(GraphicQuality) const
{
    throw std::logic_error("Not implemented");
}

CStr VideoOptionsWnd::GraphicQuality2Str(GraphicQuality) const
{
    throw std::logic_error("Not implemented");
}

void VideoOptionsWnd::ApplyGamma()
{
    throw std::logic_error("Not implemented");
}

int VideoOptionsWnd::GetWaterShaderVersionByWaterQualityVal(int) const
{
    throw std::logic_error("Not implemented");
}

int VideoOptionsWnd::GetDefaultAntialiasingForGraphicQuality(GraphicQuality) const
{
    throw std::logic_error("Not implemented");
}

void VideoOptionsWnd::InitFarDistanceControls()
{
    throw std::logic_error("Not implemented");
}

int VideoOptionsWnd::BlumQualityEnum2Val(BlumQuality) const
{
    throw std::logic_error("Not implemented");
}

int VideoOptionsWnd::GetCurrentBlum() const
{
    throw std::logic_error("Not implemented");
}

CStr VideoOptionsWnd::BlumQuality2Str(BlumQuality) const
{
    throw std::logic_error("Not implemented");
}

CStr VideoOptionsWnd::Antialiasing2Str(Antialiasing) const
{
    throw std::logic_error("Not implemented");
}

int VideoOptionsWnd::GameDataSetup()
{
    using namespace m3d::ui;
    bool res = true;
    if ((m_gameDataFlags & 2) == 0)
    {
        auto resolution = GetChildByName(m_aif.m_cbResolutionName);
        if (resolution && resolution->IsKindOf(RT_CLASS_LOCAL(ComboBoxWnd)))
        {
            m_cbResolution = dynamic_cast<ComboBoxWnd*>(resolution);
        }
        else
        {
            res = false;
            M3D_LOG_INFO("Get control error: control " + m_aif.m_cbResolutionName + " is not found or incorrect type");
        }

        auto gamma = GetChildByName(m_aif.m_sliderGammaName);
        if (gamma && gamma->IsKindOf(RT_CLASS_LOCAL(SliderWnd)))
        {
            m_sliderGamma = dynamic_cast<SliderWnd*>(gamma);
        }
        else
        {
            res = false;
            M3D_LOG_INFO("Get control error: control " + m_aif.m_sliderGammaName + " is not found or incorrect type");
        }

        auto graphic = GetChildByName(m_aif.m_cbGraphicQualityName);
        if (graphic && graphic->IsKindOf(RT_CLASS_LOCAL(ComboBoxWnd)))
        {
            m_cbGraphicQuality = dynamic_cast<ComboBoxWnd*>(graphic);
        }
        else
        {
            res = false;
            M3D_LOG_INFO("Get control error: control " + m_aif.m_cbGraphicQualityName + " is not found or incorrect type");
        }

        auto distance = GetChildByName(m_aif.m_sliderFarDistanceName);
        if (distance && distance->IsKindOf(RT_CLASS_LOCAL(SliderWnd)))
        {
            m_sliderFarDistance = dynamic_cast<SliderWnd*>(distance);
        }
        else
        {
            res = false;
            M3D_LOG_INFO("Get control error: control " + m_aif.m_sliderFarDistanceName + " is not found or incorrect type");
        }

        auto grass = GetChildByName(m_aif.m_cbGrassName);
        if (grass && grass->IsKindOf(RT_CLASS_LOCAL(ComboBoxWnd)))
        {
            m_cbGrass = dynamic_cast<ComboBoxWnd*>(grass);
        }
        else
        {
            res = false;
            M3D_LOG_INFO("Get control error: control " + m_aif.m_cbGrassName + " is not found or incorrect type");
        }

        auto shadows = GetChildByName(m_aif.m_cbShadowsName);
        if (shadows && shadows->IsKindOf(RT_CLASS_LOCAL(ComboBoxWnd)))
        {
            m_cbShadows = dynamic_cast<ComboBoxWnd*>(shadows);
        }
        else
        {
            res = false;
            M3D_LOG_INFO("Get control error: control " + m_aif.m_cbShadowsName + " is not found or incorrect type");
        }

        auto water = GetChildByName(m_aif.m_cbWaterQualityName);
        if (water && water->IsKindOf(RT_CLASS_LOCAL(ComboBoxWnd)))
        {
            m_cbWaterQuality = dynamic_cast<ComboBoxWnd*>(water);
        }
        else
        {
            res = false;
            M3D_LOG_INFO("Get control error: control " + m_aif.m_cbWaterQualityName + " is not found or incorrect type");
        }

        auto antialiasing = GetChildByName(m_aif.m_cbAntialiasingName);
        if (antialiasing && antialiasing->IsKindOf(RT_CLASS_LOCAL(ComboBoxWnd)))
        {
            m_cbAntialiasing = dynamic_cast<ComboBoxWnd*>(antialiasing);
        }
        else
        {
            res = false;
            M3D_LOG_INFO("Get control error: control " + m_aif.m_cbAntialiasingName + " is not found or incorrect type");
        }

        auto filtration = GetChildByName(m_aif.m_cbFiltrationName);
        if (filtration && filtration->IsKindOf(RT_CLASS_LOCAL(ComboBoxWnd)))
        {
            m_cbFiltration = dynamic_cast<ComboBoxWnd*>(filtration);
        }
        else
        {
            res = false;
            M3D_LOG_INFO("Get control error: control " + m_aif.m_cbFiltrationName + " is not found or incorrect type");
        }

        auto blum = GetChildByName(m_aif.m_cbBlumName);
        if (blum && blum->IsKindOf(RT_CLASS_LOCAL(ComboBoxWnd)))
        {
            m_cbBlum = dynamic_cast<ComboBoxWnd*>(blum);
        }
        else
        {
            res = false;
            M3D_LOG_INFO("Get control error: control " + m_aif.m_cbBlumName + " is not found or incorrect type");
        }

        auto gammaPrev = GetChildByName(m_aif.m_btnGammaPrevName);
        if (gammaPrev && gammaPrev->IsKindOf(RT_CLASS_LOCAL(ButtonWnd)))
        {
            m_btnGammaPrev = dynamic_cast<ButtonWnd*>(gammaPrev);
        }
        else
        {
            res = false;
            M3D_LOG_INFO("Get control error: control " + m_aif.m_btnGammaPrevName + " is not found or incorrect type");
        }

        auto gammaNext = GetChildByName(m_aif.m_btnGammaNextName);
        if (gammaNext && gammaNext->IsKindOf(RT_CLASS_LOCAL(ButtonWnd)))
        {
            m_btnGammaNext = dynamic_cast<ButtonWnd*>(gammaNext);
        }
        else
        {
            res = false;
            M3D_LOG_INFO("Get control error: control " + m_aif.m_btnGammaNextName + " is not found or incorrect type");
        }

        auto distancePrev = GetChildByName(m_aif.m_btnFarDistancePrevName);
        if (distancePrev && distancePrev->IsKindOf(RT_CLASS_LOCAL(ButtonWnd)))
        {
            m_btnFarDistancePrev = dynamic_cast<ButtonWnd*>(distancePrev);
        }
        else
        {
            res = false;
            M3D_LOG_INFO("Get control error: control " + m_aif.m_btnFarDistancePrevName + " is not found or incorrect type");
        }

        auto distanceNext = GetChildByName(m_aif.m_btnFarDistanceNextName);
        if (distanceNext && distanceNext->IsKindOf(RT_CLASS_LOCAL(ButtonWnd)))
        {
            m_btnFarDistanceNext = dynamic_cast<ButtonWnd*>(distanceNext);
            if (res)
            {
                m_gameDataFlags |= 1u;
                InitControls();
            }
        }
        else
        {
            res = false;
            M3D_LOG_INFO("Get control error: control " + m_aif.m_btnFarDistanceNextName + " is not found or incorrect type");
        }
    }
    if ((m_gameDataFlags & 1) != 0)
    {
        return 1;
    }

    M3D_LOG_INFO("VideoOptionsWnd: error - fail to init because of a bad resource");
    return 0;
}

CStr VideoOptionsWnd::ShadowsQuality2Str(ShadowsQuality) const
{
    throw std::logic_error("Not implemented");
}

m3d::ui::MbRetCodes VideoOptionsWnd::RunChangeWarningDlg()
{
    throw std::logic_error("Not implemented");
}

void VideoOptionsWnd::UpdateShadowsControls(GraphicQuality)
{
    throw std::logic_error("Not implemented");
}

void VideoOptionsWnd::UpdateFiltrationControls(GraphicQuality)
{
    throw std::logic_error("Not implemented");
}

void VideoOptionsWnd::InitBlumControls()
{
    throw std::logic_error("Not implemented");
}

void VideoOptionsWnd::OnCbResolutionChange(m3d::AIParam const&)
{
    throw std::logic_error("Not implemented");
}

void VideoOptionsWnd::SetChanged(bool)
{
    throw std::logic_error("Not implemented");
}

void VideoOptionsWnd::InitShadowsControls()
{
    throw std::logic_error("Not implemented");
}

int VideoOptionsWnd::GetCurrentAntialiasing() const
{
    throw std::logic_error("Not implemented");
}

void VideoOptionsWnd::OnCbShadowsChange(m3d::AIParam const&)
{
    throw std::logic_error("Not implemented");
}

void VideoOptionsWnd::InitAntialiasingControls()
{
    throw std::logic_error("Not implemented");
}

void VideoOptionsWnd::InitGrassControls()
{
    throw std::logic_error("Not implemented");
}

void VideoOptionsWnd::OnBtnGammaNextClick(m3d::AIParam const&)
{
    throw std::logic_error("Not implemented");
}

VideoOptionsWnd::Filtration VideoOptionsWnd::FiltrationVal2Enum(int) const
{
    throw std::logic_error("Not implemented");
}

void VideoOptionsWnd::OnGraphicQualityDependendControlChanged()
{
    throw std::logic_error("Not implemented");
}

void VideoOptionsWnd::ApplyFiltration()
{
    throw std::logic_error("Not implemented");
}

void VideoOptionsWnd::UpdateGammaControls()
{
    throw std::logic_error("Not implemented");
}

void VideoOptionsWnd::UpdateResolutionControls()
{
    throw std::logic_error("Not implemented");
}

void VideoOptionsWnd::OnSliderFarDistanceChange(m3d::AIParam const&)
{
    throw std::logic_error("Not implemented");
}

void VideoOptionsWnd::OnCbFiltrationChange(m3d::AIParam const&)
{
    throw std::logic_error("Not implemented");
}

VideoOptionsWnd::Antialiasing VideoOptionsWnd::AntialiasingVal2Enum(int) const
{
    throw std::logic_error("Not implemented");
}

int VideoOptionsWnd::GetCurrentFiltration() const
{
    throw std::logic_error("Not implemented");
}

VideoOptionsWnd::Resolution VideoOptionsWnd::ScreenWH2Resolution(PointBase<int> const&) const
{
    throw std::logic_error("Not implemented");
}

float VideoOptionsWnd::GetDefaultGrassForGraphicQuality(GraphicQuality) const
{
    throw std::logic_error("Not implemented");
}

void VideoOptionsWnd::OnBtnFarDistancePrevClick(m3d::AIParam const&)
{
    throw std::logic_error("Not implemented");
}

int VideoOptionsWnd::FiltrationEnum2Val(Filtration) const
{
    throw std::logic_error("Not implemented");
}

void VideoOptionsWnd::OnSliderGammaChange(m3d::AIParam const&)
{
    throw std::logic_error("Not implemented");
}

void VideoOptionsWnd::UpdateGammaPrevNextButtonsState()
{
    throw std::logic_error("Not implemented");
}

VideoOptionsWnd::GraphicQuality VideoOptionsWnd::DetectCurrentGraphicQuality() const
{
    throw std::logic_error("Not implemented");
}

int VideoOptionsWnd::GetDefaultBlumForGraphicQuality(GraphicQuality) const
{
    throw std::logic_error("Not implemented");
}

void VideoOptionsWnd::UpdateFarDistanceControls(GraphicQuality)
{
    throw std::logic_error("Not implemented");
}

void VideoOptionsWnd::InitGraphicQualityControls()
{
    throw std::logic_error("Not implemented");
}

void VideoOptionsWnd::ApplyResolution()
{
    throw std::logic_error("Not implemented");
}

void VideoOptionsWnd::UpdateGraphicQualityDependendControls(GraphicQuality)
{
    throw std::logic_error("Not implemented");
}

void VideoOptionsWnd::OnBtnGammaPrevClick(m3d::AIParam const&)
{
    throw std::logic_error("Not implemented");
}

void VideoOptionsWnd::OnCbAntialiasingChange(m3d::AIParam const&)
{
    throw std::logic_error("Not implemented");
}

void VideoOptionsWnd::UpdateFarDistancePrevNextButtonsState()
{
    throw std::logic_error("Not implemented");
}

float VideoOptionsWnd::GetCurrentFarDistance() const
{
    throw std::logic_error("Not implemented");
}

void VideoOptionsWnd::UpdateBlumControls(GraphicQuality)
{
    throw std::logic_error("Not implemented");
}

void VideoOptionsWnd::OnBtnFarDistanceNextClick(m3d::AIParam const&)
{
    throw std::logic_error("Not implemented");
}

PointBase<int> VideoOptionsWnd::Resolution2ScreenWH(Resolution) const
{
    throw std::logic_error("Not implemented");
}

void VideoOptionsWnd::ApplyWaterQuality()
{
    throw std::logic_error("Not implemented");
}

void VideoOptionsWnd::ValidateWaterQualityVal(int&) const
{
    throw std::logic_error("Not implemented");
}

VideoOptionsWnd::ShadowSettings VideoOptionsWnd::GetCurrentShadowSettings() const
{
    throw std::logic_error("Not implemented");
}

float VideoOptionsWnd::GetDefaultFarDistanceForGraphicQuality(GraphicQuality) const
{
    throw std::logic_error("Not implemented");
}

void VideoOptionsWnd::UpdateGrassControls(GraphicQuality)
{
    throw std::logic_error("Not implemented");
}

void VideoOptionsWnd::InitGammaControls()
{
    throw std::logic_error("Not implemented");
}

void VideoOptionsWnd::ApplyFarDistance()
{
    throw std::logic_error("Not implemented");
}

int VideoOptionsWnd::GetDefaultFiltrationForGraphicQuality(GraphicQuality) const
{
    throw std::logic_error("Not implemented");
}

void VideoOptionsWnd::UpdateControls()
{
    throw std::logic_error("Not implemented");
}

void VideoOptionsWnd::OnCbWaterQualityChange(m3d::AIParam const&)
{
    throw std::logic_error("Not implemented");
}

int VideoOptionsWnd::GetCurrentWaterQuality() const
{
    throw std::logic_error("Not implemented");
}

CStr VideoOptionsWnd::ScreenWH2Str(PointBase<int> const&) const
{
    throw std::logic_error("Not implemented");
}

float VideoOptionsWnd::GetCurrentGrass() const
{
    throw std::logic_error("Not implemented");
}

void VideoOptionsWnd::UpdateGraphicQualityControls()
{
    throw std::logic_error("Not implemented");
}

CStr VideoOptionsWnd::Filtration2Str(Filtration) const
{
    throw std::logic_error("Not implemented");
}

void VideoOptionsWnd::SetDefaultParamsForGraphicQuality(GraphicQuality)
{
    throw std::logic_error("Not implemented");
}

void VideoOptionsWnd::InitWaterQualityControls()
{
    throw std::logic_error("Not implemented");
}

VideoOptionsWnd::BlumQuality VideoOptionsWnd::BlumQualityVal2Enum(int) const
{
    throw std::logic_error("Not implemented");
}

void VideoOptionsWnd::InitResolutionControls()
{
    throw std::logic_error("Not implemented");
}

void VideoOptionsWnd::ApplyAntialiasing()
{
    throw std::logic_error("Not implemented");
}

VideoOptionsWnd::WaterQuality VideoOptionsWnd::WaterQualityVal2Enum(int) const
{
    throw std::logic_error("Not implemented");
}

void VideoOptionsWnd::OnBtnApplyClick(m3d::AIParam const&)
{
    throw std::logic_error("Not implemented");
}

void VideoOptionsWnd::ApplyBlum()
{
    throw std::logic_error("Not implemented");
}

CStr VideoOptionsWnd::WaterQuality2Str(WaterQuality) const
{
    throw std::logic_error("Not implemented");
}

bool VideoOptionsWnd::IsChanged() const
{
    throw std::logic_error("Not implemented");
}

void VideoOptionsWnd::ApplyGrass()
{
    throw std::logic_error("Not implemented");
}

VideoOptionsWnd::GrassDistance VideoOptionsWnd::GrassDistanceVal2Enum(float) const
{
    throw std::logic_error("Not implemented");
}

int VideoOptionsWnd::OnWndNotify(m3d::ui::Wnd*, unsigned, unsigned, m3d::AIParam const&)
{
    throw std::logic_error("Not implemented");
}

void VideoOptionsWnd::InitFiltrationControls()
{
    throw std::logic_error("Not implemented");
}

bool VideoOptionsWnd::IsWaterQualitySupported(WaterQuality)
{
    throw std::logic_error("Not implemented");
}

float VideoOptionsWnd::GrassDistanceEnum2Val(GrassDistance) const
{
    throw std::logic_error("Not implemented");
}

void VideoOptionsWnd::ApplyShadows()
{
    throw std::logic_error("Not implemented");
}

CStr VideoOptionsWnd::GrassDistance2Str(GrassDistance) const
{
    throw std::logic_error("Not implemented");
}

int VideoOptionsWnd::OnAfterRemoveFromWndStation()
{
    throw std::logic_error("Not implemented");
}

void VideoOptionsWnd::InitControls()
{
    InitResolutionControls();
    if ((m_gameDataFlags & 1) != 0)
    {
        m_sliderGamma->SetMinMax(0, 100);
        UpdateGammaPrevNextButtonsState();
    }
    InitGraphicQualityControls();
    if ((m_gameDataFlags & 1) != 0)
    {
        m_sliderFarDistance->SetMinMax(0, 100);
        UpdateFarDistancePrevNextButtonsState();
    }
    InitGrassControls();
    InitShadowsControls();
    InitWaterQualityControls();
    InitAntialiasingControls();
    InitFiltrationControls();
    InitBlumControls();
}

int VideoOptionsWnd::AntialiasingEnum2Val(Antialiasing) const
{
    throw std::logic_error("Not implemented");
}

void VideoOptionsWnd::OnCbGraphicQualityChange(m3d::AIParam const&)
{
    throw std::logic_error("Not implemented");
}

int VideoOptionsWnd::GetWaterQualityValByWaterShaderVersion(int) const
{
    throw std::logic_error("Not implemented");
}

int VideoOptionsWnd::OnBeforeAddToWndStation()
{
    throw std::logic_error("Not implemented");
}

void VideoOptionsWnd::OnCbGrassChange(m3d::AIParam const&)
{
    throw std::logic_error("Not implemented");
}

VideoOptionsWnd::VideoOptionsWnd()
{
}

VideoOptionsWnd::VideoOptionsWnd(VideoOptionsWnd const&)
{
    throw std::logic_error("Not implemented");
}

void VideoOptionsWnd::UpdateWaterQualityControls(GraphicQuality)
{
    throw std::logic_error("Not implemented");
}

void VideoOptionsWnd::ApplyGraphicQuality()
{
    throw std::logic_error("Not implemented");
}

int VideoOptionsWnd::GetDefaultWaterQualityForGraphicQuality(GraphicQuality) const
{
    throw std::logic_error("Not implemented");
}
