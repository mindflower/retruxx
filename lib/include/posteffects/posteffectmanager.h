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
        struct XmlNode;
        class XmlFile;
    }
}

class PostEffect;

class PostEffectManager : public m3d::IDeviceResetCallback
{
public:
    PostEffectManager();
    virtual  ~PostEffectManager();
    bool Reload();
    void Destroy();
    bool Initialize();
    bool GetParam(const CStr& effectName, float& effParam);
    bool SetParam(const CStr& effectName, float effParam);
    bool AddEffect(const CStr& effName, float effParam, unsigned int seqenceNumber);
    bool KillEffect(const CStr& effName);
    virtual void OnBeforeDeviceReset() override;
    virtual const char* GetCallbackName() const override;
    virtual void OnAfterDeviceReset() override;
    void Render(bool isBackgroundTextureUsing);
    void UnregisterEffect(PostEffect* pEffect);

private:
    void InitShaders();
    void LoadFromXml(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode);
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

    using VarList = std::map<CStr, float*, std::less<CStr>, std::allocator<std::pair<CStr const, float*> > >;

private:
    std::map<CStr, float*, std::less<CStr>, std::allocator<std::pair<CStr const, float*> > > m_varList;

    using EffectsList = std::list<PostEffect*, std::allocator<PostEffect*> >;

private:
    std::list<PostEffect*, std::allocator<PostEffect*> > m_unregList;
    std::list<PostEffect*, std::allocator<PostEffect*> > m_effectList;

    using ModelsList = std::vector<PostEffectModel*, std::allocator<PostEffectModel*> >;

private:
    std::vector<PostEffectModel*, std::allocator<PostEffectModel*> > m_models;

    struct Sequence
    {
        std::vector<PostEffectModel*, std::allocator<PostEffectModel*> > m_list;
        bool m_loop;
        bool m_inUse;
    };

    using SequenceMap = std::map<CStr, PostEffectManager::Sequence, std::less<CStr>, std::allocator<std::pair<CStr const, PostEffectManager::Sequence> > >;

private:
    std::map<CStr, PostEffectManager::Sequence, std::less<CStr>, std::allocator<std::pair<CStr const, PostEffectManager::Sequence> > > m_sequence;
};
