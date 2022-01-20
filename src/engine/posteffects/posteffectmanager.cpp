#include <m3dapp.h>
#include <stdexcept>
#include <core/ini.h>
#include <core/log.h>
#include <core/ref_ptr.h>
#include <posteffects/posteffectmanager.h>

char const* PostEffectManager::GetCallbackName() const
{
    return nullptr;
}

void PostEffectManager::OnBeforeDeviceReset()
{
    throw std::logic_error("Not implemented");
}

void PostEffectManager::OnAfterDeviceReset()
{
    throw std::logic_error("Not implemented");
}

PostEffectManager::~PostEffectManager()
{
    throw std::logic_error("Not implemented");
}

PostEffectManager::PostEffectManager()
{
}

bool PostEffectManager::GetParam(CStr const&, float&)
{
    throw std::logic_error("Not implemented");
}

void PostEffectManager::UnregisterEffect(PostEffect*)
{
    throw std::logic_error("Not implemented");
}

bool PostEffectManager::Initialize()
{
    m3d::Application::g_pApp->m_renderer->RegisterResetCallback(this);
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
    m_varList["BloomOrigTerm"] = m_BloomOrigTerm;
    m_BloomOrigTerm[0] = 0.0;
    m_BloomOrigTerm[1] = 0.0;
    m_BloomOrigTerm[2] = 0.0;
    m_varList["BloomBlurTerm"] = m_BloomBlurTerm;
    m_BloomBlurTerm[0] = 0.0;
    m_BloomBlurTerm[1] = 0.0;
    m_BloomBlurTerm[2] = 0.0;
    m_varList["BloomIntensity"] = m_BloomIntensity;
    m_BloomIntensity[0] = 0.0;
    m_BloomIntensity[1] = 0.0;
    m_BloomIntensity[2] = 0.0;
    m_varList["BloomWidth"] = m_BloomWidth;
    m_BloomWidth[0] = 0.0;
    m_BloomWidth[1] = 0.0;
    m_BloomWidth[2] = 0.0;
    m_varList["BloomThreshold"] = m_BloomThreshold;
    m_BloomThreshold[0] = 0.0;
    m_BloomThreshold[1] = 0.0;
    m_BloomThreshold[2] = 0.0;
    m_varList["MotionBlur"] = m_MotionBlur;
    m_MotionBlur[0] = 0.0;
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
    m_MaskColorR[0] = 0.0;
    m_MaskColorR[1] = 0.0;
    m_MaskColorR[2] = 0.0;
    m_varList["MaskColorG"] = m_MaskColorG;
    m_MaskColorG[0] = 0.0;
    m_MaskColorG[1] = 0.0;
    m_MaskColorG[2] = 0.0;
    m_varList["MaskColorB"] = m_MaskColorB;
    m_MaskColorB[0] = 0.0;
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
    m_FilmFrameVis[0] = 0.0;
    m_FilmFrameVis[1] = 0.0;
    m_FilmFrameVis[2] = 0.0;

    CStr err;
    ref_ptr xmlFile = m3d::ReadXmlFile("data\\PostEffects.xml", &err);
    if (xmlFile)
    {
        ref_ptr node = xmlFile->CreateNode(m3d::cmn::XML_NODE_EMPTY, nullptr);
        xmlFile->GetFirstChild_(node, "Effects");
        LoadFromXml(xmlFile, node);
        OnAfterDeviceReset();

        g_filmTex1 = m3d::Application::g_pApp->m_renderer->AddTexture("data\\textures\\OldFilm\\Frame.dds", 0);
        m3d::Application::g_pApp->m_renderer->SetTextureParameter(g_filmTex1, m3d::rend::TM_WRAP_S, 3);
        m3d::Application::g_pApp->m_renderer->SetTextureParameter(g_filmTex1, m3d::rend::TM_WRAP_T, 3);

        g_filmShift = m3d::Application::g_pApp->m_renderer->AddTexture("data\\textures\\OldFilm\\Shift.bmp", 0);
        m3d::Application::g_pApp->m_renderer->SetTextureParameter(g_filmShift, m3d::rend::TM_TEX_FILTER, 1);
        m3d::Application::g_pApp->m_renderer->SetTextureParameter(g_filmShift, m3d::rend::TM_WRAP_S, 3);
        m3d::Application::g_pApp->m_renderer->SetTextureParameter(g_filmShift, m3d::rend::TM_WRAP_T, 3);

        g_filmScrach1 = m3d::Application::g_pApp->m_renderer->AddTexture("data\\textures\\OldFilm\\Scrach_dsdt.shader", 0);
        m3d::Application::g_pApp->m_renderer->SetTextureParameter(g_filmScrach1, m3d::rend::TM_WRAP_S, 1);
        m3d::Application::g_pApp->m_renderer->SetTextureParameter(g_filmScrach1, m3d::rend::TM_WRAP_T, 1);

        InitShaders();
        return true;
    }
    M3D_LOG_INFO("PostEffectManager:: cannot load  data\\PostEffects.xml, err: " + err);
    return false
}

bool PostEffectManager::SetParam(CStr const&, float)
{
    throw std::logic_error("Not implemented");
}

void PostEffectManager::Render(bool)
{
    throw std::logic_error("Not implemented");
}

bool PostEffectManager::AddEffect(CStr const&, float, unsigned)
{
    throw std::logic_error("Not implemented");
}

bool PostEffectManager::KillEffect(CStr const&)
{
    throw std::logic_error("Not implemented");
}

void PostEffectManager::Destroy()
{
    throw std::logic_error("Not implemented");
}

void PostEffectManager::LoadFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
{
    throw std::logic_error("Not implemented");
}

void PostEffectManager::InitShaders()
{
    throw std::logic_error("Not implemented");
}

bool PostEffectManager::Reload()
{
    Destroy();
    return Initialize();
}
