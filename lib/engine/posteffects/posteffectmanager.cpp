#include <config.h>
#include <m3dapp.h>
#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <core/ini.h>
#include <core/log.h>
#include <core/ref_ptr.h>
#include <core/timer.h>
#include <posteffects/posteffectmanager.h>
#include <posteffects/posteffectmodel.h>

#include "posteffects/posteffect.h"

namespace
{
    // Packs a 0..1 float into one byte of an ARGB colour.
    // The shipped code converts through a 64-bit integer and then keeps the low
    // byte, which is what saturating outside 0..1 relies on.
    unsigned ToColorByte(float v)
    {
        return static_cast<unsigned>(static_cast<unsigned long long>(v * 255.0f)) & 0xFFu;
    }
}  // namespace

char const* PostEffectManager::GetCallbackName() const
{
    return nullptr;
}

void PostEffectManager::OnBeforeDeviceReset()
{
    // RVA 0x667A70 - the shipped body is empty; the dynamic textures are
    // recreated from scratch in OnAfterDeviceReset instead.
}

void PostEffectManager::OnAfterDeviceReset()
{
    m_ScreenWidth[0] = m3d::g_Kernel->GetEngineCfg().m_r_width.GetI();
    m_ScreenHeight[0] = m3d::g_Kernel->GetEngineCfg().m_r_height.GetI();
    int const divisor = m3d::g_Kernel->GetEngineCfg().m_g_postEffectBloom.GetI() == 1 ? 4 : 2;
    int const width = static_cast<int>(m_ScreenWidth[0]) / divisor;
    int const height = static_cast<int>(m_ScreenHeight[0]) / divisor;

    g_texRtCopy = M3D_RENDERER->GetFullFrameFrameBufferTexture();
    M3D_RENDERER->SetTextureParameter(g_texRtCopy, m3d::rend::TM_WRAP_S, 3);
    M3D_RENDERER->SetTextureParameter(g_texRtCopy, m3d::rend::TM_WRAP_T, 3);
    M3D_RENDERER->SetTextureParameter(g_texRtCopy, m3d::rend::TM_TEX_FILTER, 1);

    if (g_tex1.IsValid())
    {
        M3D_RENDERER->ReleaseTexture(g_tex1);
    }
    g_tex1 = M3D_RENDERER->AddDynamicTexture("$tex1", width, height, 0);
    M3D_RENDERER->SetTextureParameter(g_tex1, m3d::rend::TM_WRAP_S, 3);
    M3D_RENDERER->SetTextureParameter(g_tex1, m3d::rend::TM_WRAP_T, 3);

    if (g_tex2.IsValid())
    {
        M3D_RENDERER->ReleaseTexture(g_tex2);
    }
    g_tex2 = M3D_RENDERER->AddDynamicTexture("$tex2", width, height, 0);
    M3D_RENDERER->SetTextureParameter(g_tex2, m3d::rend::TM_WRAP_S, 3);
    M3D_RENDERER->SetTextureParameter(g_tex2, m3d::rend::TM_WRAP_T, 3);
}

PostEffectManager::~PostEffectManager()
{
    // RVA 0x414A90
    Destroy();
}

PostEffectManager::PostEffectManager()
{
    // RVA 0x4149D0 - the shader pointers are deliberately *not* cleared here;
    // only the texture handles are invalidated.
    g_texRtCopy.SetInvalid();
    g_tex1.SetInvalid();
    g_tex2.SetInvalid();
    g_filmTex1.SetInvalid();
    g_filmScrach1.SetInvalid();
    g_filmShift.SetInvalid();
}

bool PostEffectManager::GetParam(CStr const& effectName, float& effParam)
{
    // RVA 0x66BF10
    auto it = m_varList.find(effectName);
    if (it == m_varList.end())
    {
        return false;
    }
    effParam = *it->second;
    return true;
}

void PostEffectManager::UnregisterEffect(PostEffect* pEffect)
{
    // RVA 0x66C400 - the effect is only queued here; Render() does the actual
    // removal so that the effect list can be walked safely.
    m_unregList.push_back(pEffect);
}

bool PostEffectManager::Initialize()
{
    M3D_RENDERER->RegisterResetCallback(this);
    m_varList["Sepia"] = m_Sepia;
    m_Sepia[0] = 0.0;
    m_Sepia[1] = 0.0;
    m_Sepia[2] = 0.0;
    m_varList["SepiaIntensity"] = m_SepiaIntensity;
    m_SepiaIntensity[0] = 0.0;
    m_SepiaIntensity[1] = 0.0;
    m_SepiaIntensity[2] = 0.0;
    m_varList["SepiaColorR"] = m_SepiaColorR;
    m_SepiaColorR[0] = 0.0;
    m_SepiaColorR[1] = 0.0;
    m_SepiaColorR[2] = 0.0;
    m_varList["SepiaColorG"] = m_SepiaColorG;
    m_SepiaColorG[0] = 0.0;
    m_SepiaColorG[1] = 0.0;
    m_SepiaColorG[2] = 0.0;
    m_varList["SepiaColorB"] = m_SepiaColorB;
    m_SepiaColorB[0] = 0.0;
    m_SepiaColorB[1] = 0.0;
    m_SepiaColorB[2] = 0.0;
    m_varList["Bloom"] = m_Bloom;
    m_Bloom[0] = 0.0;
    m_Bloom[1] = 0.0;
    m_Bloom[2] = 0.0;
    m_varList["BloomOrigTerm"] = m_BloomOrigTerm;
    m_BloomOrigTerm[0] = 1.0;
    m_BloomOrigTerm[1] = 0.0;
    m_BloomOrigTerm[2] = 0.0;
    m_varList["BloomBlurTerm"] = m_BloomBlurTerm;
    m_BloomBlurTerm[0] = 0.0;
    m_BloomBlurTerm[1] = 0.0;
    m_BloomBlurTerm[2] = 0.0;
    m_varList["BloomIntensity"] = m_BloomIntensity;
    m_BloomIntensity[0] = 0.40000001f;
    m_BloomIntensity[1] = 0.0;
    m_BloomIntensity[2] = 0.0;
    m_varList["BloomWidth"] = m_BloomWidth;
    m_BloomWidth[0] = 3.0f;
    m_BloomWidth[1] = 0.0;
    m_BloomWidth[2] = 0.0;
    m_varList["BloomThreshold"] = m_BloomThreshold;
    m_BloomThreshold[0] = 0.80000001f;
    m_BloomThreshold[1] = 0.0;
    m_BloomThreshold[2] = 0.0;
    m_varList["MotionBlur"] = m_MotionBlur;
    m_MotionBlur[0] = 1.0f;
    m_MotionBlur[1] = 0.0;
    m_MotionBlur[2] = 0.0;
    m_varList["MotionBlurAlpha"] = m_MotionBlurAlpha;
    m_MotionBlurAlpha[0] = 0.0;
    m_MotionBlurAlpha[1] = 0.0;
    m_MotionBlurAlpha[2] = 0.0;
    m_varList["ScreenWidth"] = m_ScreenWidth;
    m_ScreenWidth[0] = 0.0;
    m_ScreenWidth[1] = 0.0;
    m_ScreenWidth[2] = 0.0;
    m_varList["ScreenHeight"] = m_ScreenHeight;
    m_ScreenHeight[0] = 0.0;
    m_ScreenHeight[1] = 0.0;
    m_ScreenHeight[2] = 0.0;
    m_varList["Time"] = m_Time;
    m_Time[0] = 0.0;
    m_Time[1] = 0.0;
    m_Time[2] = 0.0;
    m_varList["Mask"] = m_Mask;
    m_Mask[0] = 0.0;
    m_Mask[1] = 0.0;
    m_Mask[2] = 0.0;
    m_varList["MaskIntensity"] = m_MaskIntensity;
    m_MaskIntensity[0] = 0.0;
    m_MaskIntensity[1] = 0.0;
    m_MaskIntensity[2] = 0.0;
    m_varList["MaskColorR"] = m_MaskColorR;
    m_MaskColorR[0] = 1.0f;
    m_MaskColorR[1] = 0.0;
    m_MaskColorR[2] = 0.0;
    m_varList["MaskColorG"] = m_MaskColorG;
    m_MaskColorG[0] = 0.5f;
    m_MaskColorG[1] = 0.0;
    m_MaskColorG[2] = 0.0;
    m_varList["MaskColorB"] = m_MaskColorB;
    m_MaskColorB[0] = 0.80000001f;
    m_MaskColorB[1] = 0.0;
    m_MaskColorB[2] = 0.0;
    m_varList["Film"] = m_Film;
    m_Film[0] = 0.0;
    m_Film[1] = 0.0;
    m_Film[2] = 0.0;
    m_varList["FilmIntensity"] = m_FilmIntensity;
    m_FilmIntensity[0] = 0.0;
    m_FilmIntensity[1] = 0.0;
    m_FilmIntensity[2] = 0.0;
    m_varList["FilmSpeed"] = m_FilmSpeed;
    m_FilmSpeed[0] = 0.0;
    m_FilmSpeed[1] = 0.0;
    m_FilmSpeed[2] = 0.0;
    m_varList["FilmShift"] = m_FilmShift;
    m_FilmShift[0] = 0.0;
    m_FilmShift[1] = 0.0;
    m_FilmShift[2] = 0.0;
    m_varList["FilmFrameVis"] = m_FilmFrameVis;
    m_FilmFrameVis[0] = 1.0f;
    m_FilmFrameVis[1] = 0.0;
    m_FilmFrameVis[2] = 0.0;

    CStr err;
    ref_ptr xmlFile = m3d::ReadXmlFile("data\\PostEffects.xml", &err);
    if (xmlFile)
    {
        ref_ptr node = xmlFile->CreateNode(m3d::cmn::XML_NODE_EMPTY, nullptr);
        xmlFile->GetFirstChild(node, "Effects");
        LoadFromXml(xmlFile, node);
        OnAfterDeviceReset();

        g_filmTex1 = M3D_RENDERER->AddTexture("data\\textures\\OldFilm\\Frame.dds", 0);
        M3D_RENDERER->SetTextureParameter(g_filmTex1, m3d::rend::TM_WRAP_S, 3);
        M3D_RENDERER->SetTextureParameter(g_filmTex1, m3d::rend::TM_WRAP_T, 3);

        g_filmShift = M3D_RENDERER->AddTexture("data\\textures\\OldFilm\\Shift.bmp", 0);
        M3D_RENDERER->SetTextureParameter(g_filmShift, m3d::rend::TM_TEX_FILTER, 1);
        M3D_RENDERER->SetTextureParameter(g_filmShift, m3d::rend::TM_WRAP_S, 3);
        M3D_RENDERER->SetTextureParameter(g_filmShift, m3d::rend::TM_WRAP_T, 3);

        g_filmScrach1 = M3D_RENDERER->AddTexture("data\\textures\\OldFilm\\Scrach_dsdt.shader", 0);
        M3D_RENDERER->SetTextureParameter(g_filmScrach1, m3d::rend::TM_WRAP_S, 1);
        M3D_RENDERER->SetTextureParameter(g_filmScrach1, m3d::rend::TM_WRAP_T, 1);

        InitShaders();
        return true;
    }
    M3D_LOG_INFO("PostEffectManager:: cannot load  data\\PostEffects.xml, err: " + err);
    return false;
}

bool PostEffectManager::SetParam(CStr const& effectName, float effParam)
{
    // RVA 0x66BF80
    auto it = m_varList.find(effectName);
    if (it == m_varList.end())
    {
        return false;
    }
    *it->second = effParam;
    return true;
}

void PostEffectManager::Render(bool isBackgroundTextureUsing)
{
    // RVA 0x66A530
    for (auto* effect : m_effectList)
    {
        effect->DoFrame();
    }

    if (!m_unregList.empty())
    {
        for (auto* effect : m_unregList)
        {
            auto it = std::find(begin(m_effectList), end(m_effectList), effect);
            if (it != end(m_effectList))
            {
                delete *it;
                m_effectList.erase(it);
            }
        }
        m_unregList.clear();
    }

    for (int stage = 0; stage < 8; ++stage)
    {
        M3D_RENDERER->TgDisable(stage);
    }

    bool const bloomOn = m_Bloom[0] != 0.0f && m3d::g_Kernel->GetEngineCfg().m_g_postEffectBloom.GetI() != 0;
    if (bloomOn)
    {
        M3D_RENDERER->CopyRenderTargetToTexture(g_tex2);
    }

    // Half-texel offset so the full-screen blits sample texel centres.
    float const halfTexelU = 0.5f / m_ScreenWidth[0];
    float const halfTexelV = 0.5f / m_ScreenHeight[0];

    if (m_MotionBlur[0] != 0.0f)
    {
        // Blend the previous frame's copy back over the scene.
        M3D_RENDERER->SetBlend(m3d::rend::BM_ALPHA, false);
        M3D_RENDERER->SetLighting(false, false);
        M3D_RENDERER->SetTexture(0, g_tex1, -1.0);
        M3D_RENDERER->SetStageState(0, m3d::rend::BM_COLOR, m3d::rend::TS_TEXTURE);
        M3D_RENDERER->SetStageState(0, m3d::rend::BM_ALPHA, m3d::rend::TS_TFACTOR);
        M3D_RENDERER->DisableTextureStages(1);
        M3D_RENDERER->SetTFactor((ToColorByte(m_MotionBlurAlpha[0]) << 24) | 0xFFFFFF, false);
        M3D_APP->PutSprite2Abs(
            0.0f,
            m_ScreenHeight[0],
            0.0f,
            0.0f,
            m_ScreenWidth[0],
            m_ScreenHeight[0],
            m_ScreenWidth[0],
            0.0f,
            halfTexelU,
            halfTexelV + 1.0f,
            halfTexelU,
            halfTexelV,
            halfTexelU + 1.0f,
            halfTexelV + 1.0f,
            halfTexelU + 1.0f,
            halfTexelV,
            0.0f,
            0xFFFFFFFF);
    }

    M3D_RENDERER->SetCull(m3d::rend::M3DCULL_NONE, false);
    M3D_RENDERER->PushZbState(m3d::rend::ZB_DISABLE);
    M3D_RENDERER->SetLighting(false, false);
    M3D_RENDERER->SetBlend(m3d::rend::BM_NONE, false);
    M3D_RENDERER->SetAlphaTest(0);
    M3D_RENDERER->SetStageState(0, m3d::rend::BM_COLOR, m3d::rend::TS_TEXTURE);
    M3D_RENDERER->SetStageState(0, m3d::rend::BM_ALPHA, m3d::rend::TS_PREV);
    M3D_RENDERER->DisableTextureStages(1);

    // NOTE: the shipped code only fills the components it cares about of these
    // four-float constants and passes whatever the neighbouring stack slots
    // happened to hold for the rest. They start zeroed here instead.
    float c[4] = {};

    if (bloomOn)
    {
        if (!isBackgroundTextureUsing)
        {
            M3D_RENDERER->CopyRenderTargetToTexture(g_texRtCopy);
        }

        // Downsample the bright parts of the frame into g_tex1. The first three
        // constants are the luminance weights, the fourth the bloom threshold.
        M3D_RENDERER->RenderToTexStart(g_tex1, false);
        g_DownsampleVs->Apply();
        g_DownsamplePs->Apply();
        c[0] = 0.30000001f;
        c[1] = 0.58999997f;
        c[2] = 0.11f;
        c[3] = m_BloomThreshold[0];
        M3D_RENDERER->SetPsFloatConst(0, c, 1);
        M3D_RENDERER->SetTexture(0, g_tex2, -1.0);
        M3D_RENDERER->DrawFullScreenQuad();
        M3D_RENDERER->RenderToTexFinish();

        // Four ping-pong blur passes with a widening kernel.
        for (int pass = 0; pass < 4; ++pass)
        {
            m3d::rend::TexHandle const& dst = (pass & 1) == 0 ? g_tex2 : g_tex1;
            m3d::rend::TexHandle const& src = (pass & 1) == 0 ? g_tex1 : g_tex2;

            M3D_RENDERER->RenderToTexStart(dst, false);
            g_BlurVs->Apply();
            g_BlurPs->Apply();
            c[0] = m_BloomWidth[0] + 0.25f + static_cast<float>(pass);
            M3D_RENDERER->SetVsFloatConst(0, c, 1);
            c[0] = m_ScreenWidth[0];
            c[1] = m_ScreenHeight[0];
            M3D_RENDERER->SetVsFloatConst(1, c, 1);
            for (int stage = 0; stage < 4; ++stage)
            {
                M3D_RENDERER->SetTexture(stage, src, -1.0);
            }
            M3D_RENDERER->DrawFullScreenQuad();
            M3D_RENDERER->RenderToTexFinish();
        }

        // Composite the blurred copy back over the original frame.
        g_FinalCompVs->Apply();
        g_FinalCompPsAsm->Apply();
        c[0] = 0.050000001f;
        M3D_RENDERER->SetVsFloatConst(0, c, 1);
        c[0] = m_ScreenWidth[0];
        c[1] = m_ScreenHeight[0];
        M3D_RENDERER->SetVsFloatConst(1, c, 1);
        c[0] = c[1] = c[2] = c[3] = m_BloomOrigTerm[0];
        M3D_RENDERER->SetPsFloatConst(0, c, 1);
        c[0] = c[1] = c[2] = c[3] = m_BloomBlurTerm[0];
        M3D_RENDERER->SetPsFloatConst(1, c, 1);
        c[0] = c[1] = c[2] = c[3] = m_BloomIntensity[0];
        M3D_RENDERER->SetPsFloatConst(2, c, 1);
        M3D_RENDERER->SetTexture(0, g_texRtCopy, -1.0);
        M3D_RENDERER->SetTexture(1, g_tex1, -1.0);
        M3D_RENDERER->DrawFullScreenQuad();
    }

    if (m_Mask[0] != 0.0f)
    {
        // A flat coloured sheet over the whole screen.
        M3D_RENDERER->SetBlend(m3d::rend::BM_ALPHA, false);
        M3D_RENDERER->SetStageState(0, m3d::rend::BM_COLOR, m3d::rend::TS_DIFFUSE);
        M3D_RENDERER->SetStageState(0, m3d::rend::BM_ALPHA, m3d::rend::TS_DIFFUSE);
        unsigned const maskColor = (ToColorByte(m_MaskIntensity[0]) << 24) | (ToColorByte(m_MaskColorR[0]) << 16) |
            (ToColorByte(m_MaskColorG[0]) << 8) | ToColorByte(m_MaskColorB[0]);
        M3D_APP->PutSprite2Abs(
            0.0f,
            m_ScreenHeight[0],
            0.0f,
            0.0f,
            m_ScreenWidth[0],
            m_ScreenHeight[0],
            m_ScreenWidth[0],
            0.0f,
            halfTexelU,
            halfTexelV + 1.0f,
            halfTexelU,
            halfTexelV,
            halfTexelU + 1.0f,
            halfTexelV + 1.0f,
            halfTexelU + 1.0f,
            halfTexelV,
            0.0f,
            maskColor);
    }

    if (m_Sepia[0] != 0.0f && m_Film[0] == 0.0f)
    {
        // Sepia on its own; when the film effect is on it does the tinting.
        if (!isBackgroundTextureUsing)
        {
            M3D_RENDERER->CopyRenderTargetToTexture(g_texRtCopy);
        }
        M3D_RENDERER->SetTexture(0, g_texRtCopy, -1.0);
        M3D_RENDERER->SetWhiteTexture(1);
        M3D_RENDERER->DisableTextureStages(1);

        if (m_SepiaIntensity[0] < 0.0f)
        {
            m_SepiaIntensity[0] = 0.0f;
        }
        else if (m_SepiaIntensity[0] > 1.0f)
        {
            m_SepiaIntensity[0] = 1.0f;
        }

        c[0] = c[1] = c[2] = m_SepiaIntensity[0];
        c[3] = 0.0f;
        M3D_RENDERER->SetPsFloatConst(0, c, 1);
        c[0] = m_SepiaColorR[0];
        c[1] = m_SepiaColorG[0];
        c[2] = m_SepiaColorB[0];
        c[3] = 1.0f;
        M3D_RENDERER->SetPsFloatConst(2, c, 1);
        g_BlackNWhiteVs->Apply();
        g_BlackNWhitePs->Apply();
        M3D_RENDERER->DrawFullScreenQuad();
    }

    if (m_MotionBlur[0] != 0.0f)
    {
        M3D_RENDERER->CopyRenderTargetToTexture(g_tex1);
    }

    if (m_Film[0] != 0.0f)
    {
        // Letterbox bars, laid out in the virtual 1024x768 space.
        M3D_RENDERER->DisableTextureStages(0);
        float barX1 = 0.0f;
        float barY1 = 0.0f;
        float barX2 = 1024.0f;
        float barY2 = 25.0f;
        M3D_RENDERER->RelToAbs(barX1, barY1);
        M3D_RENDERER->RelToAbs(barX2, barY2);
        M3D_APP->PutSprite2Abs(barX1, barY1, 0.0f, 0.0f, barX2, barY2, 1.0f, 1.0f, 0);

        barX1 = 0.0f;
        barY1 = 743.0f;
        barX2 = 1024.0f;
        barY2 = 768.0f;
        M3D_RENDERER->RelToAbs(barX1, barY1);
        M3D_RENDERER->RelToAbs(barX2, barY2);
        M3D_APP->PutSprite2Abs(barX1, barY1, 0.0f, 0.0f, barX2, barY2, 1.0f, 1.0f, 0);

        if (!isBackgroundTextureUsing)
        {
            M3D_RENDERER->CopyRenderTargetToTexture(g_texRtCopy);
        }
        M3D_RENDERER->SetTexture(0, g_filmShift, -1.0);
        M3D_RENDERER->SetTexture(1, g_filmTex1, -1.0);
        M3D_RENDERER->SetTexture(2, g_filmScrach1, -1.0);
        M3D_RENDERER->SetTexture(3, g_texRtCopy, -1.0);
        M3D_RENDERER->Set2x2BumpMatrix(3, 1.0f, 0.0f, 0.0f, -1.0f);

        float const time = static_cast<float>(static_cast<double>(M3D_KERNEL->GetTimer().GetCurTimeUnscaled()) * 0.001);

        // Flicker: a triangle wave in [0.875, 1.0] driven by the film intensity.
        c[0] = c[1] = c[2] = c[3] = std::fabs(std::fmod(time * m_FilmIntensity[0], 1.0f) * 0.25f - 0.125f) + 0.875f;
        M3D_RENDERER->SetPsFloatConst(0, c, 1);

        // NOTE: the shift accumulates the absolute time rather than the frame
        // delta, so it grows quadratically. Preserved as shipped.
        m_FilmShift[0] = m_FilmSpeed[0] * time + m_FilmShift[0];
        if (M3D_RENDERER->IsFeatureSupported(m3d::rend::FEATURE_NON_POW2_CONDITIONAL))
        {
            c[0] = c[1] = c[2] = c[3] = m_FilmShift[0];
        }
        else
        {
            c[0] = c[1] = c[2] = c[3] = 0.0f;
        }
        M3D_RENDERER->SetVsFloatConst(0, c, 1);

        if (m_SepiaIntensity[0] < 0.0f)
        {
            m_SepiaIntensity[0] = 0.0f;
        }
        else if (m_SepiaIntensity[0] > 1.0f)
        {
            m_SepiaIntensity[0] = 1.0f;
        }

        c[0] = c[1] = c[2] = m_SepiaIntensity[0];
        c[3] = 0.0f;
        M3D_RENDERER->SetPsFloatConst(3, c, 1);
        c[0] = m_SepiaColorR[0];
        c[1] = m_SepiaColorG[0];
        c[2] = m_SepiaColorB[0];
        c[3] = 1.0f;
        M3D_RENDERER->SetPsFloatConst(2, c, 1);
        c[0] = (m_FilmFrameVis[0] + 1.0f) * 0.5f;
        c[1] = 0.0f;
        c[2] = 0.0f;
        c[3] = 0.0f;
        M3D_RENDERER->SetVsFloatConst(1, c, 1);

        g_FilmPsAsm->Apply();
        g_FilmVs->Apply();
        M3D_RENDERER->DrawFullScreenQuad();
        M3D_RENDERER->DisableTextureStages(1);
        M3D_RENDERER->DisableTextureStages(2);
        M3D_RENDERER->DisableTextureStages(3);
    }

    M3D_RENDERER->SetWhiteTexture(0);
    M3D_RENDERER->SetWhiteTexture(1);
    M3D_RENDERER->DisableTextureStages(0);
    M3D_RENDERER->PopZbState();
}

bool PostEffectManager::AddEffect(CStr const& effName, float effParam, unsigned seqenceNumber)
{
    // RVA 0x66CBC0
    auto it = m_sequence.find(effName);
    if (it == m_sequence.end())
    {
        return false;
    }

    // Restarting a sequence from its first unit kills whatever is still running
    // under the same name.
    bool kill = false;
    if (it->second.m_inUse && seqenceNumber == 0)
    {
        M3D_ASSERT(KillEffect(effName));
        kill = true;
    }
    it->second.m_inUse = true;

    if (seqenceNumber >= it->second.m_list.size())
    {
        if (!it->second.m_loop)
        {
            it->second.m_inUse = false;
            return false;
        }
        seqenceNumber = 0;
    }

    m_effectList.push_back(
        new PostEffect(it->second.m_list[seqenceNumber], effParam, this, seqenceNumber, effName, kill));
    return true;
}

bool PostEffectManager::KillEffect(CStr const& effName)
{
    // RVA 0x66C640
    auto it = std::find_if(
        begin(m_effectList),
        end(m_effectList),
        [&effName](PostEffect* effect)
        {
            return effect->FindByName(effName);
        });
    if (it == end(m_effectList))
    {
        return false;
    }

    auto seqItor = m_sequence.find(effName);
    M3D_ASSERT(seqItor != m_sequence.end());
    seqItor->second.m_inUse = false;
    m_unregList.push_back(*it);
    return true;
}

void PostEffectManager::Destroy()
{
    for (auto& effect : m_effectList)
    {
        delete effect;
    }

    for (auto& model : m_models)
    {
        delete model;
    }
    m_models.clear();
    m_sequence.clear();
    m_unregList.clear();
    m_effectList.clear();

    m_varList.clear();
    M3D_RENDERER->ReleaseTexture(this->g_tex1);
    M3D_RENDERER->ReleaseTexture(this->g_tex2);
    M3D_RENDERER->ReleaseTexture(this->g_filmTex1);
    M3D_RENDERER->ReleaseTexture(this->g_filmShift);
    M3D_RENDERER->ReleaseTexture(this->g_filmScrach1);

    if (g_DownsampleVs)
    {
        g_DownsampleVs->Release();
        g_DownsampleVs = nullptr;
    }

    if (g_DownsamplePs)
    {
        g_DownsamplePs->Release();
        this->g_DownsamplePs = 0;
    }
    if (g_BlurVs)
    {
        g_BlurVs->Release();
        this->g_BlurVs = 0;
    }
    if (g_BlurPs)
    {
        g_BlurPs->Release();
        this->g_BlurPs = 0;
    }
    if (g_FinalCompVs)
    {
        g_FinalCompVs->Release();
        this->g_FinalCompVs = 0;
    }
    if (g_FinalCompPsAsm)
    {
        g_FinalCompPsAsm->Release();
        this->g_FinalCompPsAsm = 0;
    }
    if (g_BlackNWhiteVs)
    {
        g_BlackNWhiteVs->Release();
        this->g_BlackNWhiteVs = 0;
    }
    if (g_BlackNWhitePs)
    {
        g_BlackNWhitePs->Release();
        this->g_BlackNWhitePs = 0;
    }
    if (g_FilmPsAsm)
    {
        g_FilmPsAsm->Release();
        this->g_FilmPsAsm = 0;
    }
    if (g_FilmVs)
    {
        g_FilmVs->Release();
        this->g_FilmVs = 0;
    }
    M3D_RENDERER->UnregisterResetCallback(this);
}

void PostEffectManager::LoadFromXml(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
{
    ref_ptr node = xmlFile->CreateNode(m3d::cmn::XML_NODE_EMPTY, nullptr);

    for (xmlNode->GetFirstChild(node, "Model"); !node->IsEmpty(); node->GetNextSibling(node, "Model"))
    {
        auto postEffectModel = new PostEffectModel(&m_varList);
        postEffectModel->LoadFromXml(xmlFile, node);
        m_models.push_back(postEffectModel);
    }

    CStr tmpName;
    for (xmlNode->GetFirstChild(node, "Sequence"); !node->IsEmpty(); node->GetNextSibling(node, "Sequence"))
    {
        Sequence sequence;
        m3d::SafeBoolAttrib(sequence.m_loop, node, "Loop");
        ref_ptr tmp = xmlFile->CreateNode(m3d::cmn::XML_NODE_EMPTY, nullptr);
        for (node->GetFirstChild(tmp, "Unit"); !tmp->IsEmpty(); tmp->GetNextSibling(tmp, "Unit"))
        {
            m3d::SafeStrAttrib(tmpName, tmp, "Name");
            auto it = std::find_if(
                begin(m_models),
                end(m_models),
                [&tmpName](PostEffectModel* model)
                {
                    return model->FindByName(tmpName);
                });
            M3D_ASSERT(it != end(m_models));
            sequence.m_list.push_back(*it);
        }
        sequence.m_inUse = false;
        // The sequence's own name comes from the Sequence node, not from the
        // last Unit child.
        m3d::SafeStrAttrib(tmpName, node, "Name");
        m_sequence[tmpName] = sequence;
    }
}

void PostEffectManager::InitShaders()
{
    if (m3d::g_Kernel->GetEngineCfg().m_r_allowPS20.GetB() &&
        M3D_RENDERER->IsFeatureSupported(m3d::rend::FEATURE_PS_2_0))
    {
        g_DownsampleVs = M3D_RENDERER->NewHlslShader(
            "data/shaders/post_downsample_vs20.vs", "VS_Downsample", m3d::rend::IHlslShader::VS_2_0);
        M3D_ASSERT(g_DownsampleVs);
        g_DownsamplePs = M3D_RENDERER->NewHlslShader(
            "data/shaders/post_downsample_ps20.ps", "PS_Downsample", m3d::rend::IHlslShader::PS_2_0);
        M3D_ASSERT(g_DownsamplePs);
        g_BlurVs =
            M3D_RENDERER->NewHlslShader("data/shaders/post_blur_vs20.vs", "VS_Blur", m3d::rend::IHlslShader::VS_2_0);
        M3D_ASSERT(g_BlurVs);
        g_BlurPs =
            M3D_RENDERER->NewHlslShader("data/shaders/post_blur_ps20.ps", "PS_Blur7", m3d::rend::IHlslShader::PS_2_0);
        M3D_ASSERT(g_BlurPs);
        g_FinalCompVs = M3D_RENDERER->NewHlslShader(
            "data/shaders/post_composite_vs20.vs", "VS_Quad", m3d::rend::IHlslShader::VS_2_0);
        M3D_ASSERT(g_FinalCompVs);
        g_FinalCompPsAsm =
            M3D_RENDERER->NewAsmShader("data/shaders/post_composite_ps20.asm", m3d::rend::IAsmShader::PIXEL_SHADER);
        M3D_ASSERT(g_FinalCompPsAsm);
    }
    else
    {
        g_DownsampleVs = M3D_RENDERER->NewHlslShader(
            "data/shaders/post_downsample_vs11.vs", "VS_Downsample", m3d::rend::IHlslShader::VS_1_1);
        M3D_ASSERT(g_DownsampleVs);
        g_DownsamplePs = M3D_RENDERER->NewHlslShader(
            "data/shaders/post_downsample_ps11.ps", "PS_Downsample", m3d::rend::IHlslShader::PS_1_1);
        M3D_ASSERT(g_DownsamplePs);
        g_BlurVs =
            M3D_RENDERER->NewHlslShader("data/shaders/post_blur_vs11.vs", "VS_Blur", m3d::rend::IHlslShader::VS_1_1);
        M3D_ASSERT(g_BlurVs);
        g_BlurPs =
            M3D_RENDERER->NewHlslShader("data/shaders/post_blur_ps11.ps", "PS_Blur7", m3d::rend::IHlslShader::PS_1_1);
        M3D_ASSERT(g_BlurPs);
        g_FinalCompVs = M3D_RENDERER->NewHlslShader(
            "data/shaders/post_composite_vs11.vs", "VS_Quad", m3d::rend::IHlslShader::VS_1_1);
        M3D_ASSERT(g_FinalCompVs);
        g_FinalCompPsAsm =
            M3D_RENDERER->NewAsmShader("data/shaders/post_composite_ps11.asm", m3d::rend::IAsmShader::PIXEL_SHADER);
        M3D_ASSERT(g_FinalCompPsAsm);
    }
    g_FilmVs = M3D_RENDERER->NewHlslShader("data/shaders/post_film_vs11.vs", "VS_Film", m3d::rend::IHlslShader::VS_1_1);
    M3D_ASSERT(g_FilmVs);
    if (M3D_RENDERER->IsFeatureSupported(m3d::rend::FEATURE_NON_POW2_CONDITIONAL))
    {
        g_FilmPsAsm =
            M3D_RENDERER->NewAsmShader("data/shaders/post_film_ps11.asm", m3d::rend::IAsmShader::PIXEL_SHADER);
    }
    else
    {
        g_FilmPsAsm =
            M3D_RENDERER->NewAsmShader("data/shaders/post_film_ps11sp.asm", m3d::rend::IAsmShader::PIXEL_SHADER);
    }
    M3D_ASSERT(g_FilmPsAsm);
    g_BlackNWhiteVs =
        M3D_RENDERER->NewHlslShader("data/shaders/post_bw_ps11.vs", "VS_Quad", m3d::rend::IHlslShader::VS_1_1);
    M3D_ASSERT(g_BlackNWhiteVs);
    g_BlackNWhitePs = M3D_RENDERER->NewAsmShader("data/shaders/post_bw_ps11.asm", m3d::rend::IAsmShader::PIXEL_SHADER);
    M3D_ASSERT(g_BlackNWhitePs);
}

bool PostEffectManager::Reload()
{
    Destroy();
    return Initialize();
}
