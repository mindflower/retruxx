#include <config.h>
#include <m3dapp.h>
#include <stdexcept>
#include <core/ini.h>
#include <core/log.h>
#include <core/ref_ptr.h>
#include <posteffects/posteffectmanager.h>
#include <posteffects/posteffectmodel.h>

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
    m_ScreenWidth[0] = m3d::g_Kernel->GetEngineCfg().m_r_width.GetI();
    m_ScreenHeight[0] = m3d::g_Kernel->GetEngineCfg().m_r_height.GetI();
    auto width = m_ScreenWidth[0];
    auto height = m_ScreenHeight[0];
    if (m3d::g_Kernel->GetEngineCfg().m_g_postEffectBloom.GetI() == 1)
    {
        width /= 4;
        height /= 4;
    }
    else
    {
        width /= 2;
        height /= 2;
    }

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

void PostEffectManager::LoadFromXml(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
{
    ref_ptr node = xmlFile->CreateNode(m3d::cmn::XML_NODE_EMPTY, nullptr);
    
    for (xmlNode->GetFirstChild(node, "Model"); !node->IsEmpty(); node->GetNextSibling(node, "Model"))
    {
        auto postEffectModel = new PostEffectModel(&m_varList);
        postEffectModel->LoadFromXml(xmlFile, node);
        m_models.push_back(postEffectModel);
    }

    for (xmlNode->GetFirstChild(node, "Sequence"); !node->IsEmpty(); node->GetNextSibling(node, "Sequence"))
    {
        Sequence sequence;
        m3d::SafeBoolAttrib(sequence.m_loop, node, "Loop");
        ref_ptr tmp = xmlFile->CreateNode(m3d::cmn::XML_NODE_EMPTY, nullptr);
        for (node->GetFirstChild(tmp, "Unit"); !tmp->IsEmpty(); node->GetNextSibling(tmp, "Unit"))
        {
            CStr tmpName;
            m3d::SafeStrAttrib(tmpName, tmp, "Name");
            auto itor = std::find_if(cbegin(m_models), cend(m_models), [&tmpName](auto const* model)
            {
                return model->m_name == tmpName;
            });
            M3D_ASSERT(itor != cend(m_models));
            sequence.m_list.push_back(*itor);
        }
        sequence.m_inUse = false;
        CStr tmpName;
        m3d::SafeStrAttrib(tmpName, tmp, "Name");
        m_sequence[tmpName] = sequence;
    }
}

void PostEffectManager::InitShaders()
{
    if (m3d::g_Kernel->GetEngineCfg().m_r_allowPS20.GetB() &&
        M3D_RENDERER->IsFeatureSupported(m3d::rend::FEATURE_PS_2_0))
    {
        g_DownsampleVs = M3D_RENDERER->NewHlslShader("data/shaders/post_downsample_vs20.vs", "VS_Downsample", m3d::rend::IHlslShader::VS_2_0);
        M3D_ASSERT(g_DownsampleVs);
        g_DownsamplePs = M3D_RENDERER->NewHlslShader("data/shaders/post_downsample_ps20.ps", "PS_Downsample", m3d::rend::IHlslShader::PS_2_0);
        M3D_ASSERT(g_DownsamplePs);
        g_BlurVs = M3D_RENDERER->NewHlslShader("data/shaders/post_blur_vs20.vs", "VS_Blur", m3d::rend::IHlslShader::VS_2_0);
        M3D_ASSERT(g_BlurVs);
        g_BlurPs = M3D_RENDERER->NewHlslShader("data/shaders/post_blur_ps20.ps", "PS_Blur7", m3d::rend::IHlslShader::PS_2_0);
        M3D_ASSERT(g_BlurPs);
        g_FinalCompVs = M3D_RENDERER->NewHlslShader("data/shaders/post_composite_vs20.vs", "VS_Quad", m3d::rend::IHlslShader::VS_2_0);
        M3D_ASSERT(g_FinalCompVs);
        g_FinalCompPsAsm = M3D_RENDERER->NewAsmShader("data/shaders/post_composite_ps20.asm", m3d::rend::IAsmShader::PIXEL_SHADER);
        M3D_ASSERT(g_FinalCompPsAsm);
    }
    else
    {
        g_DownsampleVs = M3D_RENDERER->NewHlslShader("data/shaders/post_downsample_vs11.vs", "VS_Downsample", m3d::rend::IHlslShader::VS_1_1);
        M3D_ASSERT(g_DownsampleVs);
        g_DownsamplePs = M3D_RENDERER->NewHlslShader("data/shaders/post_downsample_ps11.ps", "PS_Downsample", m3d::rend::IHlslShader::PS_1_1);
        M3D_ASSERT(g_DownsamplePs);
        g_BlurVs = M3D_RENDERER->NewHlslShader("data/shaders/post_blur_vs11.vs", "VS_Blur", m3d::rend::IHlslShader::VS_1_1);
        M3D_ASSERT(g_BlurVs);
        g_BlurPs = M3D_RENDERER->NewHlslShader("data/shaders/post_blur_ps11.ps", "PS_Blur7", m3d::rend::IHlslShader::PS_1_1);
        M3D_ASSERT(g_BlurPs);
        g_FinalCompVs = M3D_RENDERER->NewHlslShader("data/shaders/post_composite_vs11.vs", "VS_Quad", m3d::rend::IHlslShader::VS_1_1);
        M3D_ASSERT(g_FinalCompVs);
        g_FinalCompPsAsm = M3D_RENDERER->NewAsmShader("data/shaders/post_composite_ps11.asm", m3d::rend::IAsmShader::PIXEL_SHADER);
        M3D_ASSERT(g_FinalCompPsAsm);
    }
    g_FilmVs = M3D_RENDERER->NewHlslShader("data/shaders/post_film_vs11.vs", "VS_Film", m3d::rend::IHlslShader::VS_1_1);
    M3D_ASSERT(g_FilmVs);
    if (M3D_RENDERER->IsFeatureSupported(m3d::rend::FEATURE_NON_POW2_CONDITIONAL))
    {
        g_FilmPsAsm = M3D_RENDERER->NewAsmShader("data/shaders/post_film_ps11.asm", m3d::rend::IAsmShader::PIXEL_SHADER);
    }
    else
    {
        g_FilmPsAsm = M3D_RENDERER->NewAsmShader("data/shaders/post_film_ps11sp.asm", m3d::rend::IAsmShader::PIXEL_SHADER);
    }
    M3D_ASSERT(g_FilmPsAsm);
    g_BlackNWhiteVs = M3D_RENDERER->NewHlslShader("data/shaders/post_bw_ps11.vs", "VS_Quad", m3d::rend::IHlslShader::VS_1_1);
    M3D_ASSERT(g_BlackNWhiteVs);
    g_BlackNWhitePs = M3D_RENDERER->NewAsmShader("data/shaders/post_bw_ps11.asm", m3d::rend::IAsmShader::PIXEL_SHADER);
    M3D_ASSERT(g_BlackNWhitePs);
}

bool PostEffectManager::Reload()
{
    Destroy();
    return Initialize();
}
