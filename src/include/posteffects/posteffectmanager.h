#pragma once
#include <iface.h>
#include <list>
#include <map>
#include <vector>
#include <renderer/i_renderer.h>

class PostEffectModel;

namespace m3d
{
    namespace cmn
    {
        class XmlNode;
        class XmlFile;
    }
}

class PostEffect;
class CStr;

class PostEffectManager : public m3d::IDeviceResetCallback
{
public:
    class Sequence
    {
    private:
        std::vector<PostEffectModel*> m_list;
        bool m_loop;
        bool m_inUse;
    };

public:
    virtual char const* GetCallbackName() const;
    virtual void OnBeforeDeviceReset();
    virtual void OnAfterDeviceReset();
    virtual ~PostEffectManager();

    PostEffectManager();
    bool GetParam(CStr const&, float&);
    void UnregisterEffect(PostEffect*);
    bool Initialize();
    bool SetParam(CStr const&, float);
    void Render(bool);
    bool AddEffect(CStr const&, float, unsigned int);
    bool KillEffect(CStr const&);
    bool Reload();
    void Destroy();

private:
    void LoadFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
    void InitShaders();

private:
    m3d::rend::TexHandle g_texRtCopy;
    m3d::rend::TexHandle g_tex1;
    m3d::rend::TexHandle g_tex2;
    m3d::rend::TexHandle g_filmTex1;
    m3d::rend::TexHandle g_filmScrach1;
    m3d::rend::TexHandle g_filmShift;
    m3d::rend::IHlslShader* g_DownsampleVs;
    m3d::rend::IHlslShader* g_DownsamplePs;
    m3d::rend::IHlslShader* g_BlurVs;
    m3d::rend::IHlslShader* g_BlurPs;
    m3d::rend::IHlslShader* g_FinalCompVs;
    m3d::rend::IAsmShader* g_FinalCompPsAsm;
    m3d::rend::IHlslShader* g_BlackNWhiteVs;
    m3d::rend::IAsmShader* g_BlackNWhitePs;
    m3d::rend::IHlslShader* g_FilmVs;
    m3d::rend::IHlslShader* g_FilmPs;
    m3d::rend::IAsmShader* g_FilmPsAsm;
    float m_Sepia[3];
    float m_SepiaIntensity[3];
    float m_SepiaColorR[3];
    float m_SepiaColorG[3];
    float m_SepiaColorB[3];
    float m_Bloom[3];
    float m_BloomOrigTerm[3];
    float m_BloomBlurTerm[3];
    float m_BloomIntensity[3];
    float m_BloomWidth[3];
    float m_BloomThreshold[3];
    float m_MotionBlur[3];
    float m_MotionBlurAlpha[3];
    float m_ScreenWidth[3];
    float m_ScreenHeight[3];
    float m_Time[3];
    float m_Mask[3];
    float m_MaskIntensity[3];
    float m_MaskColorR[3];
    float m_MaskColorG[3];
    float m_MaskColorB[3];
    float m_Film[3];
    float m_FilmIntensity[3];
    float m_FilmFrameVis[3];
    float m_FilmSpeed[3];
    float m_FilmShift[3];
    std::map<CStr, float*> m_varList;
    std::list<PostEffect*> m_unregList;
    std::list<PostEffect*> m_effectList;
    std::vector<PostEffectModel*> m_models;
    std::map<CStr, Sequence> m_sequence;
};
