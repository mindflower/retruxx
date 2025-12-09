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
    virtual  ~PostEffectManager() /* 0x0c */;
    bool Reload();
    void Destroy();
    bool Initialize();
    bool GetParam(const CStr& effectName, float& effParam);
    bool SetParam(const CStr& effectName, float effParam);
    bool AddEffect(const CStr& effName, float effParam, unsigned int seqenceNumber);
    bool KillEffect(const CStr& effName);
    virtual void OnBeforeDeviceReset() override /* 0x04 */;
    virtual const char* GetCallbackName() const override /* 0x00 */;
    virtual void OnAfterDeviceReset() override /* 0x08 */;
    void Render(bool isBackgroundTextureUsing);
    void UnregisterEffect(PostEffect* pEffect);

private:
    void InitShaders();
    void LoadFromXml(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode);
    /* 0x0004 */ m3d::rend::TexHandle g_texRtCopy;
    /* 0x0008 */ m3d::rend::TexHandle g_tex1;
    /* 0x000c */ m3d::rend::TexHandle g_tex2;
    /* 0x0010 */ m3d::rend::TexHandle g_filmTex1;
    /* 0x0014 */ m3d::rend::TexHandle g_filmScrach1;
    /* 0x0018 */ m3d::rend::TexHandle g_filmShift;
    /* 0x001c */ m3d::rend::IHlslShader* g_DownsampleVs;
    /* 0x0020 */ m3d::rend::IHlslShader* g_DownsamplePs;
    /* 0x0024 */ m3d::rend::IHlslShader* g_BlurVs;
    /* 0x0028 */ m3d::rend::IHlslShader* g_BlurPs;
    /* 0x002c */ m3d::rend::IHlslShader* g_FinalCompVs;
    /* 0x0030 */ m3d::rend::IAsmShader* g_FinalCompPsAsm;
    /* 0x0034 */ m3d::rend::IHlslShader* g_BlackNWhiteVs;
    /* 0x0038 */ m3d::rend::IAsmShader* g_BlackNWhitePs;
    /* 0x003c */ m3d::rend::IHlslShader* g_FilmVs;
    /* 0x0040 */ m3d::rend::IHlslShader* g_FilmPs;
    /* 0x0044 */ m3d::rend::IAsmShader* g_FilmPsAsm;
    /* 0x0048 */ float m_Sepia[3];
    /* 0x0054 */ float m_SepiaIntensity[3];
    /* 0x0060 */ float m_SepiaColorR[3];
    /* 0x006c */ float m_SepiaColorG[3];
    /* 0x0078 */ float m_SepiaColorB[3];
    /* 0x0084 */ float m_Bloom[3];
    /* 0x0090 */ float m_BloomOrigTerm[3];
    /* 0x009c */ float m_BloomBlurTerm[3];
    /* 0x00a8 */ float m_BloomIntensity[3];
    /* 0x00b4 */ float m_BloomWidth[3];
    /* 0x00c0 */ float m_BloomThreshold[3];
    /* 0x00cc */ float m_MotionBlur[3];
    /* 0x00d8 */ float m_MotionBlurAlpha[3];
    /* 0x00e4 */ float m_ScreenWidth[3];
    /* 0x00f0 */ float m_ScreenHeight[3];
    /* 0x00fc */ float m_Time[3];
    /* 0x0108 */ float m_Mask[3];
    /* 0x0114 */ float m_MaskIntensity[3];
    /* 0x0120 */ float m_MaskColorR[3];
    /* 0x012c */ float m_MaskColorG[3];
    /* 0x0138 */ float m_MaskColorB[3];
    /* 0x0144 */ float m_Film[3];
    /* 0x0150 */ float m_FilmIntensity[3];
    /* 0x015c */ float m_FilmFrameVis[3];
    /* 0x0168 */ float m_FilmSpeed[3];
    /* 0x0174 */ float m_FilmShift[3];

    using VarList = std::map<CStr, float*, std::less<CStr>, std::allocator<std::pair<CStr const, float*> > >;

private:
    /* 0x0180 */ std::map<CStr, float*, std::less<CStr>, std::allocator<std::pair<CStr const, float*> > > m_varList;

    using EffectsList = std::list<PostEffect*, std::allocator<PostEffect*> >;

private:
    /* 0x018c */ std::list<PostEffect*, std::allocator<PostEffect*> > m_unregList;
    /* 0x0198 */ std::list<PostEffect*, std::allocator<PostEffect*> > m_effectList;

    using ModelsList = std::vector<PostEffectModel*, std::allocator<PostEffectModel*> >;

private:
    /* 0x01a4 */ std::vector<PostEffectModel*, std::allocator<PostEffectModel*> > m_models;

    struct Sequence
    {
        /* 0x0000 */ std::vector<PostEffectModel*, std::allocator<PostEffectModel*> > m_list;
        /* 0x0010 */ bool m_loop;
        /* 0x0011 */ bool m_inUse;
    }; /* size: 0x0014 */

    using SequenceMap = std::map<CStr, PostEffectManager::Sequence, std::less<CStr>, std::allocator<std::pair<CStr const, PostEffectManager::Sequence> > >;

private:
    /* 0x01b4 */ std::map<CStr, PostEffectManager::Sequence, std::less<CStr>, std::allocator<std::pair<CStr const, PostEffectManager::Sequence> > > m_sequence;
}; /* size: 0x01c0 */
