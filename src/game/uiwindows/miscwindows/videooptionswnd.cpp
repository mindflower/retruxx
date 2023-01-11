#include "videooptionswnd.h"
#include "ui/comboboxwnd.h"
#include "ui/slider.h"
#include "ui/button.h"
#include <core/log.h>

#include "config.h"
#include "m3dapp.h"

RT_CLASS_EXPORTS_BEGIN(VideoOptionsWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(VideoOptionsWnd);

PointBase<int> VideoOptionsWnd::m_screenWH[5] = {
	{800, 600},
    {1024, 768},
    {1152, 864},
    {1280, 960},
    {1600, 1200},
};

int VideoOptionsWnd::m_waterQualities[3] = { 1,2,3 };
int VideoOptionsWnd::m_antialiasings[5] = { 0,2,4, 8, 16 };

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

CStr VideoOptionsWnd::GraphicQuality2Str(GraphicQuality graphicQuality) const
{
    if (graphicQuality >= GRAPHIC_QUALITY_NUM_GRAPHIC_QUALITIES)
    {
        return {};
    }
    static const CStr names[] = {
        "LowQuality",
        "MediumQuality",
        "MaxQuality",
        "CustomQuality",
    };
    return m3d::Application::g_pApp->GetStringByStringId0(names[graphicQuality]);
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

CStr VideoOptionsWnd::BlumQuality2Str(BlumQuality blumQuality) const
{
    if (blumQuality >= BLUM_QUALITY_NUM_BLUM_QUALITIES)
    {
        return {};
    }
    static const CStr names[] = {
        "None",
        "LowQuality",
        "HighQuality",
    };
    return m3d::Application::g_pApp->GetStringByStringId0(names[blumQuality]);
}

CStr VideoOptionsWnd::Antialiasing2Str(Antialiasing antialiasing) const
{
    if (antialiasing >= ANTIALIASING_NUM_ANTIALIASINGS)
    {
        return {};
    }
    static const CStr names[] = {
        "None",
        "Antialiasing_x2",
        "Antialiasing_x4",
        "Antialiasing_x8",
        "Antialiasing_x16",
    };
    return m3d::Application::g_pApp->GetStringByStringId0(names[antialiasing]);
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

CStr VideoOptionsWnd::ShadowsQuality2Str(ShadowsQuality shadowsQuality) const
{
    if (shadowsQuality >= SHADOWS_QUALITY_NUM_SHADOWS_QUALITIES)
    {
        return{};
    }
    static const CStr names[] = {
        "None",
        "LowQuality",
        "HighQuality",
    };
    return m3d::Application::g_pApp->GetStringByStringId0(names[shadowsQuality]);
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
    if ((m_gameDataFlags & 1) != 0)
    {
        for (int i = 0; i < 3; ++i)
        {
            auto idx = m_cbBlum->AddItem(BlumQuality2Str(static_cast<BlumQuality>(i)));
            if (idx != -1)
            {
                m_cbBlum->SetItemData(idx, i);
            }
        }
    }
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
    if ((m_gameDataFlags & 1) != 0)
    {
        for (int i = 0; i < 3; ++i)
        {
            auto idx = m_cbShadows->AddItem(ShadowsQuality2Str(static_cast<ShadowsQuality>(i)));
            if (idx != -1)
            {
                m_cbShadows->SetItemData(idx, i);
            }
        }
    }
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
    if ((m_gameDataFlags & 1) != 0)
    {
        for (int i = 0; i < 5; ++i)
        {
            if (m3d::Application::g_pApp->m_renderer->IsMultiSamplingSupported(m_antialiasings[i]))
            {
                auto idx = m_cbAntialiasing->AddItem(Antialiasing2Str(static_cast<Antialiasing>(i)));
                if (idx != -1)
                {
                    m_cbAntialiasing->SetItemData(idx, i);
                }
            }
        }
    }
}

void VideoOptionsWnd::InitGrassControls()
{
    if ((m_gameDataFlags & 1) != 0)
    {
        for (int i = 0; i < 3; ++i)
        {
            auto idx = m_cbGrass->AddItem(GrassDistance2Str(static_cast<GrassDistance>(i)));
            if (idx != -1)
            {
                m_cbGrass->SetItemData(idx, i);
            }
        }
    }
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
    //TODO: check this
    if ((m_gameDataFlags & 1) != 0)
    {
        auto const height = M3D_KERNEL->GetEngineCfg().m_r_height.GetI();
        auto const width = M3D_KERNEL->GetEngineCfg().m_r_width.GetI();
        int i = 0;
        for (; i < RESOLUTION_NUM_RESOLUTIONS; ++i)
        {
            if (m_screenWH[i].x == width && m_screenWH[i].y == height)
            {
                break;
            }
        }
        ++m_cbResolutionBlocked;
        m_cbResolution->SetCurSel(-1);
        if (i == RESOLUTION_NUM_RESOLUTIONS)
        {
            ++m_cbResolutionBlocked;
            m_cbResolution->SetCurSel(-1);
            m_cbResolution->SetText(ScreenWH2Str({width, height}));
        }
        else
        {
            auto const count = m_cbResolution->GetCount();
            if (count > 0)
            {
                for (int j = 0; j < count;++j)
                {
                    if (m_cbResolution->GetItemData(j) == i)
                    {
                        ++m_cbResolutionBlocked;
                        m_cbResolution->SetCurSel(j);
                        break;
                    }
                }
            }
        }
    }
}

void VideoOptionsWnd::OnSliderFarDistanceChange(m3d::AIParam const&)
{
    UpdateFarDistancePrevNextButtonsState();
    if (IsChildOf(m3d::Application::g_pApp) && !m_sliderFarDistanceBlocked)
    {
        m_bVideoOptionsChanged = true;
        OnGraphicQualityDependendControlChanged();
    }
    if (m_sliderFarDistanceBlocked > 0)
        m_sliderFarDistanceBlocked = m_sliderFarDistanceBlocked - 1;
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
	UpdateGammaPrevNextButtonsState();
    if (IsChildOf(m3d::Application::g_pApp) && !m_sliderGammaBlocked)
        ApplyGamma();
    if (m_sliderGammaBlocked > 0)
        m_sliderGammaBlocked = m_sliderGammaBlocked - 1;
}

void VideoOptionsWnd::UpdateGammaPrevNextButtonsState()
{
    if ((m_gameDataFlags & 1) != 0)
    {
        auto notch = m_sliderGamma->GetNotch();
        m_btnGammaPrev->EnableWindow(notch > m_sliderGamma->GetMin());
        m_btnGammaNext->EnableWindow(notch < m_sliderGamma->GetMax());
    }
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
    if ((m_gameDataFlags & 1) != 0)
    {
	    for (int i = 0; i < 4; ++i)
	    {
            auto idx = m_cbGraphicQuality->AddItem(GraphicQuality2Str(static_cast<GraphicQuality>(i)));
            if (idx != -1)
            {
                m_cbGraphicQuality->SetItemData(idx, i);
            }
	    }
    }
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
    if ((m_gameDataFlags & 1) != 0)
    {
        auto notch = m_sliderFarDistance->GetNotch();
        m_btnFarDistancePrev->EnableWindow(notch > m_sliderFarDistance->GetMin());
        m_btnFarDistanceNext->EnableWindow(notch < m_sliderFarDistance->GetMax());
    }
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

void VideoOptionsWnd::ValidateWaterQualityVal(int& waterQualityVal) const
{
    if (waterQualityVal == 1)
    {
        return;
    }
    else if (waterQualityVal == 2)
    {
	    if (m3d::Application::g_pApp->m_renderer->IsFeatureSupported(m3d::rend::FEATURE_PS_1_4))
	    {
            return;
	    }
    }
    else if (waterQualityVal == 3)
    {
        if (m3d::Application::g_pApp->m_renderer->IsFeatureSupported(m3d::rend::FEATURE_PS_2_0))
        {
            return;
        }
    }
    waterQualityVal = 1;
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
    UpdateResolutionControls();
    UpdateGammaControls();
    UpdateGraphicQualityControls();
    if ((m_gameDataFlags & 1) != 0)
    {
        ++m_sliderFarDistanceBlocked;
        m_sliderFarDistance->SetNotch(M3D_KERNEL->GetEngineCfg().m_lsViewDistanceDivider.GetF() * 100.0);
    }
    UpdateGrassControls(GRAPHIC_QUALITY_NUM_GRAPHIC_QUALITIES);
    UpdateShadowsControls(GRAPHIC_QUALITY_NUM_GRAPHIC_QUALITIES);
    UpdateWaterQualityControls(GRAPHIC_QUALITY_NUM_GRAPHIC_QUALITIES);
    UpdateAntialiasingControls(GRAPHIC_QUALITY_NUM_GRAPHIC_QUALITIES);
    UpdateFiltrationControls(GRAPHIC_QUALITY_NUM_GRAPHIC_QUALITIES);
    UpdateBlumControls(GRAPHIC_QUALITY_NUM_GRAPHIC_QUALITIES);
}

void VideoOptionsWnd::OnCbWaterQualityChange(m3d::AIParam const&)
{
    throw std::logic_error("Not implemented");
}

int VideoOptionsWnd::GetCurrentWaterQuality() const
{
    throw std::logic_error("Not implemented");
}

CStr VideoOptionsWnd::ScreenWH2Str(PointBase<int> const& wh) const
{
    return CStr(wh.x) + " x " + CStr(wh.y);
}

float VideoOptionsWnd::GetCurrentGrass() const
{
    throw std::logic_error("Not implemented");
}

void VideoOptionsWnd::UpdateGraphicQualityControls()
{
    throw std::logic_error("Not implemented");
}

CStr VideoOptionsWnd::Filtration2Str(Filtration filtration) const
{
    if (filtration >= FILTRATION_NUM_FILTRATIONS)
    {
        return{};
    }
    static const CStr names[] = {
    "BilinearFiltration",
    "TrilinearFiltration",
    "AnisotropFiltration",
    };
    return m3d::Application::g_pApp->GetStringByStringId0(names[filtration]);
}

void VideoOptionsWnd::SetDefaultParamsForGraphicQuality(GraphicQuality)
{
    throw std::logic_error("Not implemented");
}

void VideoOptionsWnd::InitWaterQualityControls()
{
    //TODO: check this!
    if ((m_gameDataFlags & 1) != 0)
    {
        for (int i = 0; i < 3; ++i)
        {
            auto qual = m_waterQualities[i];
            ValidateWaterQualityVal(qual);
            if (qual == m_waterQualities[i])
            {
                auto idx = m_cbWaterQuality->AddItem(WaterQuality2Str(static_cast<WaterQuality>(i)));
                if (idx != -1)
                {
                    m_cbWaterQuality->SetItemData(idx, i);
                }
            }
        }
    }
}

VideoOptionsWnd::BlumQuality VideoOptionsWnd::BlumQualityVal2Enum(int) const
{
    throw std::logic_error("Not implemented");
}

void VideoOptionsWnd::InitResolutionControls()
{
    //TODO: check this and refactor
    int i; // esi
    int v4; // ecx
    CStr* v5; // eax
    int v6; // edi
    PointBase<int> wh; // [esp+4h] [ebp-14h] BYREF
    char* v10; // [esp+Ch] [ebp-Ch]
    int v11; // [esp+10h] [ebp-8h]
    char v12; // [esp+14h] [ebp-4h] BYREF

    if ((m_gameDataFlags & 1) != 0)
    {
        for (i = 0; i < 5; ++i)
        {
            wh.x = m_screenWH[i].x;
            wh.y = m_screenWH[i].y;
            v6 = m_cbResolution->AddItem(ScreenWH2Str(wh));
            if (v6 != -1)
                m_cbResolution->SetItemData(v6, i);
        }
    }
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

CStr VideoOptionsWnd::WaterQuality2Str(WaterQuality waterQuality) const
{
    if (waterQuality >= WATER_QUALITY_NUM_WATER_QUALITIES)
    {
        return{};
    }
    static const CStr names[] = {
        "LowQuality",
        "MediumQuality",
        "HighQuality",
    };
    return m3d::Application::g_pApp->GetStringByStringId0(names[waterQuality]);
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

int VideoOptionsWnd::OnWndNotify(m3d::ui::Wnd* from, unsigned id, unsigned msg, m3d::AIParam const& data)
{
    if ((this->m_style & 0x100000) != 0)
    {
        ReflectChildNotifyToParent(from, id, msg, data);
    }
    switch (id)
    {
    case 0x2774u:
    {
        if (msg != 5)
            return 0;
        OnCbResolutionChange(data);
        return 1;
    }
    case 0x2775u:
    {
        if (msg != 5)
            return 0;
        VideoOptionsWnd::OnSliderGammaChange(data);
        return 1;
    }
    case 0x2776u:
    {
        if (msg != 5)
            return 0;
        VideoOptionsWnd::OnCbGraphicQualityChange(data);
        return 1;
    }
    case 0x2777u:
    {
        if (msg != 5)
            return 0;
        VideoOptionsWnd::OnSliderFarDistanceChange(data);
        return 1;
    }
    case 0x2778u:
    {
        if (msg != 5)
            return 0;
        VideoOptionsWnd::OnCbGrassChange(data);
        return 1;
    }
    case 0x2779u:
    {
        if (msg != 5)
            return 0;
        VideoOptionsWnd::OnCbShadowsChange(data);
        return 1;
    }
    case 0x277Au:
    {
        if (msg != 5)
            return 0;
        VideoOptionsWnd::OnCbWaterQualityChange(data);
        return 1;
    }
    case 0x277Cu:
    {
        if (msg != 5)
            return 0;
        VideoOptionsWnd::OnCbAntialiasingChange(data);
        return 1;
    }
    case 0x277Du:
    {
        if (msg != 5)
            return 0;
        VideoOptionsWnd::OnCbFiltrationChange(data);
        return 1;
    }
    case 0x277Eu:
    {
        if (msg != 5)
            return 0;
        VideoOptionsWnd::OnCbBlumChange(data);
        return 1;
    }
    case 0x277Fu:
    {
        if (msg != 1)
            return 0;
        VideoOptionsWnd::OnBtnApplyClick(data);
        return 1;
    }
    case 0x2781u:
    {
        if (msg != 1)
            return 0;
        VideoOptionsWnd::OnBtnGammaPrevClick(data);
        return 1;
    }
    case 0x2782u:
    {
        if (msg != 1)
            return 0;
        VideoOptionsWnd::OnBtnGammaNextClick(data);
        return 1;
    }
    case 0x2783u:
    {
        if (msg != 1)
            return 0;
        VideoOptionsWnd::OnBtnFarDistancePrevClick(data);
        return 1;
    }
    case 0x2784u:
    {
        if (msg != 1)
            return 0;
        VideoOptionsWnd::OnBtnFarDistanceNextClick(data);
        return 1;
    }
    default:
        return 0;
    }
    throw std::logic_error("Not implemented");
}

void VideoOptionsWnd::InitFiltrationControls()
{
    if ((m_gameDataFlags & 1) != 0)
    {
        for (int i = 0; i < 3; ++i)
        {
            auto idx = m_cbFiltration->AddItem(Filtration2Str(static_cast<Filtration>(i)));
            if (idx != -1)
            {
                m_cbFiltration->SetItemData(idx, i);
            }
        }
    }
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

CStr VideoOptionsWnd::GrassDistance2Str(GrassDistance grassDistance) const
{
    if (grassDistance >= GRASS_DISTANCE_NUM_GRASS_DISTANCES)
    {
        return{};
    }
    static const CStr names[] = {
        "None",
        "Near",
        "Far",
    };
    return m3d::Application::g_pApp->GetStringByStringId0(names[grassDistance]);
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
    //TODO: implement VideoOptionsWnd::OnBeforeAddToWndStation
    //UpdateControls();
    return Wnd::OnBeforeAddToWndStation();
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
