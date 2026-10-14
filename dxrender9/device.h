#pragma once
// The Direct3D 9 implementation of m3d::rend::IRenderer.
//
// Ported from the original renderer's dxrender9/device.h, whose
// layout and code are recorded in that build's PDB. Field offsets in the comments are the original's
// offsets (sizeof(CDevice) == 0xf1ec there); they document the original and are not relied upon.
//
// The exported vtable is Hard Truck Apocalypse's IRenderer (lib/include/renderer/i_renderer.h),
// which declares its methods under the original names; methods that exist only in the original interface
// are plain member functions here.
#include <d3d9.h>
#include <d3dx9.h>

#include <algorithm>
#include <functional>
#include <list>
#include <map>
#include <utility>
#include <vector>

#include <core/stringm3d.h>
#include <iface.h>
#include <math/matrix.h>
#include <math/plane.h>
#include <math/vector.h>
#include <math/vector2.h>
#include <renderer/i_renderer.h>

#include "i_renderer_orig.h"
#include "shaders/effects/effect_statemanager.h"

using namespace m3d::rend;

class EffectImpl;
class HlslShaderImpl;
class AsmShaderImpl;

// Free functions of device.cpp (the D3D format / error strings) and screenshot.cpp (the TGA
// writers) that other compilands call.
CStr getD3dFmtStr(D3DFORMAT fmt);
CStr getD3dErrorStr(HRESULT hr);
HRESULT SaveSurfaceToTGAFile(IDirect3DDevice9* pD3DDevice, char const* szFileName, IDirect3DSurface9* pSurface,
                             int width, int height);
HRESULT SaveSurfaceToGrayscaleTGAFile(IDirect3DDevice9* pD3DDevice, char const* szFileName,
                                      IDirect3DSurface9* pSurface);

// The build stamp the resource reports (vb_man_stats, ib_man_stats, texture_man_stats) write. In
// the original binary each report carries its own compile time, so the original assembled it from
// __DATE__ and __TIME__. The text follows the stamp of Hard Truck Apocalypse's own dxrender9.dll
// without its version number.
#define DXRENDER9_BUILD_STRING \
    "ExMachina - release version release build (" __DATE__ " " __TIME__ ")"

enum TexType
{
    TT_2D_FROM_FILE = 0,
    TT_2D_DYNAMIC = 1,
    TT_2D_RENDER_TARGET = 2,
    TT_CUBE_FROM_FILE = 3,
    TT_CUBE_DYNAMIC = 4,
    TT_CUBE_RENDER_TARGET = 5,
    TT_3D_FROM_FILE = 6,
    TT_3D_DYNAMIC = 7,
    TT_NUM_TYPES = 8,
};

// The renderer's view of an engine handle: the index into the matching CDevice table. The engine
// side (Handle<T>) keeps the index protected; the driver reads and writes it through this.
template<class T>
class InternalHandle : public T
{
public:
    InternalHandle() {}

    int Id() const
    {
        return this->m_handle;
    }

    void SetId(int id)
    {
        this->m_handle = id;
    }
};

inline int TexId(TexHandle const& h)
{
    return static_cast<InternalHandle<TexHandle> const&>(h).Id();
}

inline int VbId(VbHandle const& h)
{
    return static_cast<InternalHandle<VbHandle> const&>(h).Id();
}

inline int IbId(IbHandle const& h)
{
    return static_cast<InternalHandle<IbHandle> const&>(h).Id();
}

inline int MeshId(MeshHandle const& h)
{
    return static_cast<InternalHandle<MeshHandle> const&>(h).Id();
}

// A sub-allocation of a pooled vertex or index buffer (vb_man.cpp / ib_man.cpp).
struct PoolFieldInfo
{
    unsigned int Size;
    unsigned int Offset;

    // orig 0x634480 device.h:160
    bool operator==(PoolFieldInfo const& A) const
    {
        return Offset == A.Offset && Size == A.Size;
    }
};

class CDevice : public IRenderer
{
public:
    class CVertexBuffer
    {
    public:
        IDirect3DVertexBuffer9* m_vb;
        D3DVERTEXBUFFER_DESC m_desc;
        IDirect3DVertexDeclaration9* m_vertexDecl;
        int m_vertSz;
        int m_curPos;
        int m_refs;
        int m_locked;
        int m_lockedAtPresent;
        CStr m_vbName;

        CVertexBuffer();
    };

    class CIndexBuffer
    {
    public:
        IDirect3DIndexBuffer9* m_ib;
        D3DINDEXBUFFER_DESC m_desc;
        int m_curPos;
        int m_refs;
        int m_locked;

        CIndexBuffer();
    };

    // One Direct3D texture object. Several CTexture entries (texture ids) may share a map, and an
    // animated texture owns several maps.
    class CTexMap
    {
    public:
        unsigned int m_usage;
        D3DPOOL m_pool;
        D3DFORMAT m_fmt;
        unsigned int m_flags;
        union
        {
            D3DSURFACE_DESC m_desc2d;
            D3DVOLUME_DESC m_desc3d;
        };
        union
        {
            IDirect3DBaseTexture9* m_pTex;
            IDirect3DTexture9* m_pTex2d;
            IDirect3DCubeTexture9* m_pTexCube;
            IDirect3DVolumeTexture9* m_pTex3d;
        };
        TexType m_type;
        CStr m_fileName;
        unsigned int m_lastFileSize;
        FILETIME m_lastFileDate;
        int m_refs;

        CTexMap();
        ~CTexMap();
        void freeTex();
        int addRef();
        int release();
        bool Is2D();
        bool IsCube();
        bool Is3D();
    };

    // A texture id as seen by the engine: sampler state plus the maps it is made of.
    class CTexture
    {
    public:
        D3DTEXTUREADDRESS m_address[3];
        D3DTEXTUREFILTERTYPE m_magFilter;
        D3DTEXTUREFILTERTYPE m_minFilter;
        D3DTEXTUREFILTERTYPE m_mipFilter;
        unsigned int m_borderColor;
        int m_maxAnisotropy;
        float m_lodBias;
        int m_lodMax;
        int m_refs;
        int m_fps;
        unsigned int m_looped;
        double m_timeStamp;  // the PDB says float, the constructor stores 8 bytes
        CStr m_fileName;
        std::vector<CTexMap*> m_maps;

        CTexture();
        CTexture(CTexture const& that);
        ~CTexture();
        int addRef();
        int release();
    };

    class CMesh
    {
    public:
        ID3DXMesh* m_mesh;
        ID3DXPMesh* m_pmesh;
        int m_refs;
        void* m_verts;
        int m_numVerts;
        unsigned short* m_indices;
        int m_numTris;
        VertexType m_vertexType;

        CMesh();
    };

    // Key of the effect and HLSL shader caches: file name plus compile parameters.
    struct ShaderIdData
    {
        CStr filename;
        std::vector<CompileParam> compileParams;

        ShaderIdData(CStr const& fname, std::vector<CompileParam> const& cp);
        bool operator<(ShaderIdData const& a) const;
    };

    struct MacroData
    {
        ShaderMacro macro;
        bool userDefined;

        MacroData();
    };

    struct DXCursorInfo
    {
        TexHandle m_texId;
        int m_xHotSpot;
        int m_yHotSpot;
        int m_frame;

        DXCursorInfo();
        void SetUp(TexHandle const& texId, int xHotSpot, int yHotSpot, int frame);
    };

    int m_refCount;
    IBase* m_parent;
    int m_presents;

    // vb_man.cpp
    std::vector<CVertexBuffer> m_vbs;
    std::vector<VertexType> m_VbPoolTypes;
    std::vector<std::vector<VbHandle>> m_VbPoolBuffers;
    std::vector<std::list<PoolFieldInfo>> m_VbPoolFields;
    unsigned int const m_VbPoolSize;

    // ib_man.cpp
    std::vector<CIndexBuffer> m_ibs;
    std::vector<IbHandle> m_IbPoolBuffers;
    std::list<PoolFieldInfo> m_IbPoolFields;
    unsigned int const m_IbPoolSize;

    // texture_man.cpp
    std::vector<CTexMap*> m_texMaps;
    std::vector<CTexture> m_textures;
    TexHandle m_texFrameBufer;
    std::map<int, TexHandle> m_texBufferedRT;

    // mesh.cpp
    std::vector<CMesh> m_meshes;

    D3DPRESENT_PARAMETERS m_d3dpp;
    IDirect3D9* m_pD3D;
    IDirect3DDevice9* m_pd3dDevice;
    D3DCAPS9 m_d3dCaps;
    D3DSURFACE_DESC m_d3dsdBackBuffer;
    D3DDISPLAYMODE m_desktopMode;
    D3DFORMAT m_surfaceFormat;
    D3DFORMAT m_texFormat[2][2];  // [useAlpha][compressed]
    D3DFORMAT m_texFormatRt;
    D3DFORMAT m_texFormatShadow;
    D3DFORMAT m_texFormatDepth;
    D3DFORMAT m_depthStencilFormat;
    D3DFORMAT m_depthStencilFormatRt;

    // rendertarget.cpp: the render-to-texture state ("rts")
    IDirect3DSurface9* m_rtsPtr;
    IDirect3DSurface9* m_rtsSaveZs;
    IDirect3DSurface9* m_rtsSaveColor;
    IDirect3DSurface9* m_rtsNewZs;
    std::map<unsigned int, IDirect3DSurface9*> m_rtsZSurfaces;
    int m_rtsWantZ;
    Viewport m_rtsSaveViewport;

    bool m_haveStencil;
    bool m_haveTwoSidedStencil;
    D3DGAMMARAMP m_gamma;

    // matrices.cpp
    CMatrix m_matViewStack[64];
    CMatrix m_matInvView;
    int m_matViewStackTop;
    bool m_matViewIsNotActuated;
    bool m_matInvViewIsNotActuated;
    bool m_matWorldIsNotActuated;
    CMatrix m_matProjStack[64];
    int m_matProjStackTop;
    CMatrix m_matWorldStack[64];
    int m_matWorldStackTop;

    Viewport m_curViewport;
    D3DVIEWPORT9 m_curViewportD3D;
    std::vector<m3d::IDeviceResetCallback*> m_resetCallbacks;

    // The Direct3D state cache (rstCaches / the set* wrappers)
    unsigned int m_curFVF;
    IDirect3DVertexDeclaration9* m_curVertexDecl;
    IDirect3DVertexBuffer9* m_curVb[4];
    unsigned int m_curStride[4];
    IDirect3DIndexBuffer9* m_curIb;
    unsigned int m_curIbBaseIdx;
    IDirect3DBaseTexture9* m_curTexStages[8];
    unsigned int m_curTexStagesStates[8][33];
    unsigned int m_curTexSamplerStates[8][14];
    unsigned int m_curStreamFreq[8];
    unsigned int m_curRenderState[210];
    IDirect3DIndexBuffer9* m_latchedIb;
    unsigned int m_latchedIbBaseIdx;
    CMatrix m_curXForms[512];
    bool m_latchedCheck;
    IDirect3DVertexShader9* m_curVertexShader;
    IDirect3DPixelShader9* m_curPixelShader;

    IDirect3DCubeTexture9* m_NormalizingCubemap;
    IDirect3DTexture9* m_SpecularPowerLookup;

    // Vertex declarations (InitVertexDeclarations)
    IDirect3DVertexDeclaration9* m_vdXYZCT1;
    IDirect3DVertexDeclaration9* m_vdXYZT1;
    IDirect3DVertexDeclaration9* m_vdXYZNT1;
    IDirect3DVertexDeclaration9* m_vdXYZNT2;
    IDirect3DVertexDeclaration9* m_vdXYZNT3;
    IDirect3DVertexDeclaration9* m_vdXYZN;
    IDirect3DVertexDeclaration9* m_vdXYZ;
    IDirect3DVertexDeclaration9* m_vdXYZW;
    IDirect3DVertexDeclaration9* m_vdXYZC;
    IDirect3DVertexDeclaration9* m_vdXYZNC;
    IDirect3DVertexDeclaration9* m_vdXYZWCT1;
    IDirect3DVertexDeclaration9* m_vdXYZWC;
    IDirect3DVertexDeclaration9* m_vdXYZNCT1;
    IDirect3DVertexDeclaration9* m_vdXYZNCT2;
    IDirect3DVertexDeclaration9* m_vdXYZCT2;
    IDirect3DVertexDeclaration9* m_vdXYZW4NCT1;
    IDirect3DVertexDeclaration9* m_vdXYZW4TNCT1;
    IDirect3DVertexDeclaration9* m_vdXYZNT1T;
    IDirect3DVertexDeclaration9* m_vdXYZNCT1T;
    IDirect3DVertexDeclaration9* m_vdXYZCT1_UVW;
    IDirect3DVertexDeclaration9* m_vdXYZCT2_UVW;
    IDirect3DVertexDeclaration9* m_vdXYZNCT1_UV2_S1;
    IDirect3DVertexDeclaration9* m_vdWaterTest;
    IDirect3DVertexDeclaration9* m_vdGrassTest;
    IDirect3DVertexDeclaration9* m_vdImpostorTest;
    IDirect3DVertexDeclaration9* m_vdYNI;
    IDirect3DVertexDeclaration9* m_vdXYZT1I;
    IDirect3DVertexDeclaration9* m_vdInstanceId;
    std::map<std::pair<IDirect3DVertexDeclaration9*, IDirect3DVertexDeclaration9*>, IDirect3DVertexDeclaration9*>
        m_combinedVD;

    int m_maxLights;
    int m_inScene;
    RenderStats m_stats;
    DeviceMemStats m_devMemStats;
    bool m_isDevMemStatsValid;
    HRESULT m_lastResult;
    int m_isActive;

    // The render state stacks (device.cpp). Each has 64 entries and a top index.
    int m_stackAlphaTest[64];
    unsigned int m_stackTopAlphaTest;
    BlendMode m_stackBlend[64];
    unsigned int m_stackTopBlend;
    ZbState m_stackZbState[64];
    unsigned int m_stackTopZbState;
    Cull m_stackCull[64];
    unsigned int m_stackTopCull;
    CmpFunc m_stackZFunc[64];
    unsigned int m_stackTopZFunc;
    bool m_stackLighting[64];
    unsigned int m_stackTopLighting;
    unsigned int m_stackAmbient[64];
    unsigned int m_stackTopAmbient;
    bool m_stackFog[64];
    unsigned int m_stackTopFog;
    unsigned int m_stackFogColor[64];
    unsigned int m_stackTopFogColor;
    FogMode m_stackFogMode[64];
    unsigned int m_stackTopFogMode;
    float m_stackFogStart[64];
    unsigned int m_stackTopFogStart;
    float m_stackFogEnd[64];
    unsigned int m_stackTopFogEnd;
    FillMode m_stackFillMode[64];
    unsigned int m_stackTopFillMode;
    float m_stackZBias[64];
    unsigned int m_stackTopZBias;
    float m_stackZBiasSlopeScale[64];
    unsigned int m_stackTopZBiasSlopeScale;
    ShadeMode m_stackShadeMode[64];
    unsigned int m_stackTopShadeMode;
    int m_stackPointSpriteEnable[64];
    unsigned int m_stackTopPointSpriteEnable;
    int m_stackPointScaleEnable[64];
    unsigned int m_stackTopPointScaleEnable;
    float m_stackPointSizeMin[64];
    unsigned int m_stackTopPointSizeMin;
    float m_stackPointSizeMax[64];
    unsigned int m_stackTopPointSizeMax;
    float m_stackPointSize[64];
    unsigned int m_stackTopPointSize;
    float m_stackPointScaleA[64];
    unsigned int m_stackTopPointScaleA;
    float m_stackPointScaleB[64];
    unsigned int m_stackTopPointScaleB;
    float m_stackPointScaleC[64];
    unsigned int m_stackTopPointScaleC;
    unsigned int m_stackTFactor[64];
    unsigned int m_stackTopTFactor;
    bool m_stackLocalViewer[64];
    unsigned int m_stackTopLocalViewer;
    bool m_stackSpecularLighting[64];
    unsigned int m_stackTopSpecularLighting;
    unsigned int m_stackColorWriteMask[64];
    unsigned int m_stackTopColorWriteMask;
    bool m_stackDithering[64];
    unsigned int m_stackTopDithering;
    float m_stackNPatchLevel[64];
    unsigned int m_stackTopNPatchLevel;
    bool m_stackStencilState[64];
    unsigned int m_stackTopStencilState;
    unsigned int m_stackStencilMask[64];
    unsigned int m_stackTopStencilMask;
    unsigned int m_stackStencilRef[64];
    unsigned int m_stackTopStencilRef;
    unsigned int m_stackStencilWriteMask[64];
    unsigned int m_stackTopStencilWriteMask;
    CmpFunc m_stackStencilFunc[64];
    unsigned int m_stackTopStencilFunc;
    StencilOp m_stackStencilFail[64];
    unsigned int m_stackTopStencilFail;
    StencilOp m_stackStencilZFail[64];
    unsigned int m_stackTopStencilZFail;
    StencilOp m_stackStencilPass[64];
    unsigned int m_stackTopStencilPass;
    bool m_stackStencil2SidedEnable[64];
    unsigned int m_stackTopStencil2SidedEnable;
    CmpFunc m_stackStencilCcwFunc[64];
    unsigned int m_stackTopStencilCcwFunc;
    StencilOp m_stackStencilCcwFail[64];
    unsigned int m_stackTopStencilCcwFail;
    StencilOp m_stackStencilCcwZFail[64];
    unsigned int m_stackTopStencilCcwZFail;
    StencilOp m_stackStencilCcwPass[64];
    unsigned int m_stackTopStencilCcwPass;
    bool m_stackMultiSample[64];
    unsigned int m_stackTopMultiSample;
    unsigned int m_stackMultiSampleMask[64];
    unsigned int m_stackTopMultiSampleMask;

    bool m_updateModelViewProj;
    bool m_updateModelViewProjWithWorld;
    bool m_updateModelMatrix;

    // fsrt.cpp: the full-screen quad
    VbHandle m_fsQuadVb;
    IbHandle m_fsQuadIb;

    // The "GlobalStreaming" vertex buffers (InitVertexDeclarations / GetVbStreaming)
    VbHandle m_vbXyz;
    VbHandle m_vbXyzc;
    VbHandle m_vbXyznc;
    VbHandle m_vbXyzct1;
    VbHandle m_vbXyzwct1;
    VbHandle m_vbXyznt1;
    VbHandle m_vbXyznt2;
    VbHandle m_vbXyznt3;
    VbHandle m_vbXyznct2;
    VbHandle m_vbXyznct1;
    VbHandle m_vbXyznt1t;
    VbHandle m_vbXyznct1t;

    // lights.cpp
    D3DLIGHT9 m_lights[8];
    int m_lightsEnabled[8];
    Colorf m_ambientLight;

    // clipplanes.cpp
    int m_userClipPlaneEnabled;
    bool m_userClipPlanesUpdated[6];
    D3DXPLANE m_userClipPlanesWorld[6];
    D3DXPLANE m_userClipPlanesProj[6];
    bool m_fastClipEnabled;
    D3DXPLANE m_fastClipPlane;
    CMatrix m_fastClipPlaneProjMatrix;

    // shaders/shaders.cpp
    StateManager m_stateManager;
    ID3DXEffectPool* m_globalFxPool;
    std::map<ShaderIdData, EffectImpl*> m_effects;
    std::map<ShaderIdData, HlslShaderImpl*> m_HlslShaders;
    std::map<CStr, AsmShaderImpl*> m_AsmShaders;
    std::vector<MacroData> m_shadersMacros;
    std::vector<D3DXMACRO> m_d3dxMacros;

    // query.cpp
    std::list<IQuery*> m_queries;

    bool m_featureSupported[FEATURE_NUM_FEATURES_15];

    // fsrt.cpp: the full-screen render target
    IDirect3DTexture9* m_fsRt;
    IDirect3DSurface9* m_fsRtSurf;
    IDirect3DSurface9* m_fsRtZBuffer;
    unsigned int m_fsRtWidth;
    unsigned int m_fsRtHeight;
    IDirect3DVertexBuffer9* m_fsRtVb;
    IDirect3DIndexBuffer9* m_fsRtIb;
    IHlslShader* m_fsRtVs;
    IHlslShader* m_fsRtPs;

    bool m_isNV30;

    // matrices.cpp: the camera
    CMatrix m_viewMatrix;
    CMatrix m_viewMatrixInv;
    CVector m_viewOrigin;
    bool m_viewMatrixWasSetThisFrame;

    ID3DXFont* m_pSysFont;
    DXCursorInfo m_currentDXCursorInfo;
    int m_activeStencilTarget;
    int m_stencilLevel[2];
    bool m_reloadAllTextures;
    unsigned int m_renderThreadId;
    bool m_bThreadSafeGuardEnabled;

    CDevice();
    virtual ~CDevice();

    // IBase
    int DecRef() override;
    int IncRef() override;
    void* QueryIface(const char* ifaceName) override;

    // ---------------------------------------------------------------- device.cpp
    int Create(void(__fastcall* logFunc)(CStr const&), m3d::Kernel* kernel) override;
    int CreateDevice() override;
    int SwitchDisplayModes(HWND wnd, int w, int h, int fullscreen) override;
    int Reset() override;
    int SwitchDisplayModes0(HWND wnd, int w, int h, int fullscreen);
    void internalReset();
    void rstStacks();
    void rstCaches();
    void logCaps();
    bool defineInstancingSupport();
    int SetupAllFeaturesSupport(CStr const& DevCompatibleFileName);

    void InitVertexDeclarations();
    void DoneVertexDeclarations();
    void CreateCombinedVertexDeclaration(IDirect3DVertexDeclaration9* a, IDirect3DVertexDeclaration9* b);
    void GetVertexInfo(VertexType tvert, unsigned int& fvf, int& sizeofvert, IDirect3DVertexDeclaration9*& decl);

    void ActuateStates(bool bForFFP);
    void ActuateMatrices();

    int FindDepthFormat(D3DFORMAT TargetFormat, D3DFORMAT* pDepthStencilFormat, int bpp);
    int FindDepthFormat(D3DFORMAT& depthFormat, D3DFORMAT TargetFormat, D3DFORMAT const* fmtDepthArray,
                        int const* fmtDepthArrayBits, int numDepthFmts, int bpp);
    int FindTexFormat(D3DFORMAT& texFormat, D3DFORMAT TargetFormat, D3DFORMAT const* fmtTextureArray, int const* bits,
                      int numTextureFmts, int bpp, int usage);
    int FindSurfaceFormat(D3DFORMAT* pSurfaceFormat, int bpp);
    int FindTextureFormat(D3DFORMAT TargetFormat, int bpp);
    int FindMultisampleType(D3DFORMAT TargetFormat, D3DFORMAT depthStencilFormat, int multiSamplesNum,
                            D3DPRESENT_PARAMETERS& d3dpp);
    bool IsMultiSamplingSupported(D3DFORMAT format, D3DMULTISAMPLE_TYPE multiSampleType, DWORD* qualityLevels);
    bool IsMultiSamplingSupported(int numSamples) override;

    D3DFORMAT GetTexFormat(bool useAlpha, bool compressed);
    D3DFORMAT GetTexFormatRt();
    D3DFORMAT GetTexFormatShadow();
    D3DFORMAT GetTexFormatDepth();

    // The cached Direct3D state setters. Each skips the call when the state is unchanged and counts
    // the switch in m_stats.
    HRESULT setRenderState(D3DRENDERSTATETYPE state, unsigned int val);
    unsigned int getRenderState(D3DRENDERSTATETYPE state);
    HRESULT setTextureStageState(unsigned int stage, D3DTEXTURESTAGESTATETYPE type, unsigned int value);
    HRESULT setTextureSamplerState(unsigned int stage, D3DSAMPLERSTATETYPE type, unsigned int value);
    HRESULT setStreamSourceFreq(unsigned int stage, unsigned int freq);
    HRESULT setFVF(unsigned int fvf);
    HRESULT setVertexDeclaration(IDirect3DVertexDeclaration9* vd);
    HRESULT setStreamSource(int stream, IDirect3DVertexBuffer9* vb, unsigned int stride);
    void setIndices(IDirect3DIndexBuffer9* ib, unsigned int baseIdx);
    HRESULT setVertexShader(IDirect3DVertexShader9* shader);
    HRESULT setPixelShader(IDirect3DPixelShader9* shader);
    HRESULT setTexture(int stage, IDirect3DBaseTexture9* tex);

    bool IsIbValid(IbHandle const& ib) const;
    bool IsMeshValid(MeshHandle const& ib) const;
    bool IsVbValid(VbHandle const& ib) const;
    bool IsTexValid(TexHandle const& ib) const;

    IDirect3DDevice9* GetDevice();
    HRESULT GetLastResult() const;
    IDirect3DCubeTexture9* GetNormalCubemap();
    D3DCAPS9 GetCaps() const;

    int CanRender() override;
    int SetActiveState(int state) override;
    int BeginScene() override;
    int EndScene() override;
    int InScene() override;
    int PresentScene() override;
    void ClearViewport(ClearFlags flags, unsigned int bkClr) override;
    Viewport GetViewport() override;
    int SetViewport(Viewport const& port) override;
    void RegisterResetCallback(m3d::IDeviceResetCallback* callback) override;
    void UnregisterResetCallback(m3d::IDeviceResetCallback* callback) override;
    void SetGamma(float gamma, float brightness, float contrast) override;
    bool IsFeatureSupported(DeviceFeature f) override;
    float GetMaxPointSize() override;
    float GetMaxNPatchTessellationLevel() override;
    int GetMaxVertexShaderConst() override;
    int GetMaxAnisotropy() override;
    char* GetCurBppStr(int* bpp) override;
    char* GetLastErrorStr() override;
    void ResetStats() override;
    void GetStats(RenderStats& stats) override;
    void GetDeviceMemStats(DeviceMemStats& stats) override;
    void ShowStats() override;
    void* GetInternalData() override;
    bool IsNV3x() override;
    void RelToAbs(float& x, float& y) override;
    void AbsToRel(float& x, float& y) override;

    void SetStageState(int stage, BlendMode mode, TextureState state) override;
    void SetStreamFrequency(int stage, StreamDataType streamType, unsigned int frequency);

    // Texture coordinate generation
    void TgEnableSetLinearSt(int stage, float sx, float sz, float tx, float tz, float roty, bool camSpace, float u0,
                             float v0, float u1, float v1) override;
    void TgEnableSetMatrixSt(int stage, CMatrix const* m, bool camSpace) override;
    void TgEnableSetMatrixStr(int stage, CMatrix const* m, bool camSpace) override;
    void TgEnableSetMatrixStrReflection(int stage, CMatrix const& m, bool camSpace);
    void TgSetTransformMode(int stage, TgMode mode) override;
    void TgSetTcSource(int stage, TcSource mode, int index) override;
    void TgDisable(int stage) override;
    void SetTextureMatrix(int stage, CMatrix const* mat) override;
    void Set2x2BumpMatrix(int stage, float m00, float m01, float m10, float m11) override;

    // The hardware cursor
    int SetupDXCursor(TexHandle const& texId, int xHotSpot, int yHotSpot, int frame) override;
    int SetupDXCursorForce(TexHandle const& texId, int xHotSpot, int yHotSpot, int frame);
    void MoveDXCursor(int x, int y) override;
    void ShowDXCursor(bool needShow) override;
    int UpdateDXCursorFrame() override;
    bool IsHardWareCursorAvailableForTexture(TexHandle const* texId) override;

    // Single layer stencil
    void SingleLayerStencilStart() override;
    void SingleLayerStencilContinue() override;
    void SingleLayerStencilFinish() override;
    void rstStencilLevels();

    void SetRenderThreadId(unsigned int renderThreadId);
    void EnableThreadSafeQuard(bool enable);

    // The render state stacks.
    void PushAlphaTest(int c);
    void PushAlphaTest();
    void PopAlphaTest();
    void SetAlphaTest(int c, bool force);
    void SetAlphaTest(int c) override;
    void PushBlend(BlendMode c) override;
    void PushBlend() override;
    void PopBlend() override;
    void SetBlend(BlendMode c, bool force) override;
    void PushZbState(ZbState c) override;
    void PushZbState() override;
    void PopZbState() override;
    void SetZbState(ZbState c, bool force) override;
    void PushCull(Cull c) override;
    void PushCull() override;
    void PopCull() override;
    void SetCull(Cull c, bool force) override;
    void PushZFunc(CmpFunc c) override;
    void PushZFunc() override;
    void PopZFunc() override;
    void SetZFunc(CmpFunc c, bool force) override;
    void PushLighting(bool c) override;
    void PushLighting() override;
    void PopLighting() override;
    void SetLighting(bool c, bool force) override;
    void PushAmbient(unsigned int c) override;
    void PushAmbient() override;
    void PopAmbient() override;
    void SetAmbient(unsigned int c, bool force) override;
    void PushFog(bool c) override;
    void PushFog() override;
    void PopFog() override;
    void SetFog(bool c, bool force) override;
    void PushFogColor(unsigned int c) override;
    void PushFogColor() override;
    void PopFogColor() override;
    void SetFogColor(unsigned int c, bool force) override;
    void PushFogMode(FogMode c) override;
    void PushFogMode() override;
    void PopFogMode() override;
    void SetFogMode(FogMode c, bool force) override;
    void PushFogStart(float c) override;
    void PushFogStart() override;
    void PopFogStart() override;
    void SetFogStart(float c, bool force) override;
    void PushFogEnd(float c) override;
    void PushFogEnd() override;
    void PopFogEnd() override;
    void SetFogEnd(float c, bool force) override;
    void PushFillMode(FillMode c) override;
    void PushFillMode() override;
    void PopFillMode() override;
    void SetFillMode(FillMode c, bool force) override;
    void PushZBias(float c) override;
    void PushZBias() override;
    void PopZBias() override;
    void SetZBias(float c, bool force) override;
    void PushZBiasSlopeScale(float c) override;
    void PushZBiasSlopeScale() override;
    void PopZBiasSlopeScale() override;
    void SetZBiasSlopeScale(float c, bool force) override;
    void PushShadeMode(ShadeMode c) override;
    void PushShadeMode() override;
    void PopShadeMode() override;
    void SetShadeMode(ShadeMode c, bool force) override;
    void PushPointSpriteEnable(int c) override;
    void PushPointSpriteEnable() override;
    void PopPointSpriteEnable() override;
    void SetPointSpriteEnable(int c, bool force) override;
    void PushPointScaleEnable(int c) override;
    void PushPointScaleEnable() override;
    void PopPointScaleEnable() override;
    void SetPointScaleEnable(int c, bool force) override;
    void PushPointSizeMin(float c) override;
    void PushPointSizeMin() override;
    void PopPointSizeMin() override;
    void SetPointSizeMin(float c, bool force) override;
    void PushPointSizeMax(float c) override;
    void PushPointSizeMax() override;
    void PopPointSizeMax() override;
    void SetPointSizeMax(float c, bool force) override;
    void PushPointSize(float c) override;
    void PushPointSize() override;
    void PopPointSize() override;
    void SetPointSize(float c, bool force) override;
    void PushPointScaleA(float c) override;
    void PushPointScaleA() override;
    void PopPointScaleA() override;
    void SetPointScaleA(float c, bool force) override;
    void PushPointScaleB(float c) override;
    void PushPointScaleB() override;
    void PopPointScaleB() override;
    void SetPointScaleB(float c, bool force) override;
    void PushPointScaleC(float c) override;
    void PushPointScaleC() override;
    void PopPointScaleC() override;
    void SetPointScaleC(float c, bool force) override;
    void PushTFactor(unsigned int c) override;
    void PushTFactor() override;
    void PopTFactor() override;
    void SetTFactor(unsigned int c, bool force) override;
    void PushLocalViewer(bool c) override;
    void PushLocalViewer() override;
    void PopLocalViewer() override;
    void SetLocalViewer(bool c, bool force) override;
    void PushSpecularLighting(bool c) override;
    void PushSpecularLighting() override;
    void PopSpecularLighting() override;
    void SetSpecularLighting(bool c, bool force) override;
    void PushColorWriteMask(unsigned int c) override;
    void PushColorWriteMask() override;
    void PopColorWriteMask() override;
    void SetColorWriteMask(unsigned int c, bool force) override;
    void PushDithering(bool c);
    void PushDithering();
    void PopDithering();
    void SetDithering(bool c, bool force);
    void PushNPatchLevel(float c) override;
    void PushNPatchLevel() override;
    void PopNPatchLevel() override;
    void SetNPatchLevel(float c, bool force) override;
    void PushStencilState(bool c) override;
    void PushStencilState() override;
    void PopStencilState() override;
    void SetStencilState(bool c, bool force) override;
    void PushStencilMask(unsigned int c) override;
    void PushStencilMask() override;
    void PopStencilMask() override;
    void SetStencilMask(unsigned int c, bool force) override;
    void PushStencilRef(unsigned int c) override;
    void PushStencilRef() override;
    void PopStencilRef() override;
    void SetStencilRef(unsigned int c, bool force) override;
    void PushStencilWriteMask(unsigned int c) override;
    void PushStencilWriteMask() override;
    void PopStencilWriteMask() override;
    void SetStencilWriteMask(unsigned int c, bool force) override;
    void PushStencilFunc(CmpFunc c) override;
    void PushStencilFunc() override;
    void PopStencilFunc() override;
    void SetStencilFunc(CmpFunc c, bool force) override;
    void PushStencilFail(StencilOp c) override;
    void PushStencilFail() override;
    void PopStencilFail() override;
    void SetStencilFail(StencilOp c, bool force) override;
    void PushStencilZFail(StencilOp c) override;
    void PushStencilZFail() override;
    void PopStencilZFail() override;
    void SetStencilZFail(StencilOp c, bool force) override;
    void PushStencilPass(StencilOp c) override;
    void PushStencilPass() override;
    void PopStencilPass() override;
    void SetStencilPass(StencilOp c, bool force) override;
    void PushStencil2SidedEnable(bool c) override;
    void PushStencil2SidedEnable() override;
    void PopStencil2SidedEnable() override;
    void SetStencil2SidedEnable(bool c, bool force) override;
    void PushStencilCcwFunc(CmpFunc c) override;
    void PushStencilCcwFunc() override;
    void PopStencilCcwFunc() override;
    void SetStencilCcwFunc(CmpFunc c, bool force) override;
    void PushStencilCcwFail(StencilOp c) override;
    void PushStencilCcwFail() override;
    void PopStencilCcwFail() override;
    void SetStencilCcwFail(StencilOp c, bool force) override;
    void PushStencilCcwZFail(StencilOp c) override;
    void PushStencilCcwZFail() override;
    void PopStencilCcwZFail() override;
    void SetStencilCcwZFail(StencilOp c, bool force) override;
    void PushStencilCcwPass(StencilOp c) override;
    void PushStencilCcwPass() override;
    void PopStencilCcwPass() override;
    void SetStencilCcwPass(StencilOp c, bool force) override;
    void PushMultiSample(bool c) override;
    void PushMultiSample() override;
    void PopMultiSample() override;
    void SetMultiSample(bool c, bool force) override;
    void PushMultiSampleMask(unsigned int c) override;
    void PushMultiSampleMask() override;
    void PopMultiSampleMask() override;
    void SetMultiSampleMask(unsigned int c, bool force) override;

    // ---------------------------------------------------------------- matrices.cpp
    void InitMatrices();
    HRESULT SetXFormMatrix(int num, CMatrix const& mat);
    void ActuateProjectionMatrix();
    void SetViewMatrix(CMatrix const& viewMatrix) override;
    CMatrix const& GetViewMatrix() override;
    CMatrix const& GetInvViewMatrix();
    CVector const& GetViewOrigin() override;
    void MatPush(CMatrix const& mat) override;
    void MatPush() override;
    void MatPop(bool dontset) override;
    void MatMul(CMatrix const& mat) override;
    void MatMulR(CMatrix const* mat) override;
    CMatrix const& MatGet() override;
    CMatrix const& MatGetInv() override;
    void MatSet(CMatrix const& mat) override;
    void MatGetBasis(CVector& right, CVector& up, CVector& fwd) const override;
    CVector MatGetOrgInv() override;
    CVector MatGetOrg() override;
    void MatSetWorld(CMatrix const& mat) override;
    CMatrix const& MatGetWorld() override;
    void MatPushWorld() override;
    void MatPopWorld() override;
    void MatSetProj(CMatrix const& mat) override;
    CMatrix const& MatGetProj() override;
    void MatPushProj() override;
    void MatPopProj() override;
    CMatrix const& GetModelMatrix();
    CMatrix const* GetModelViewProjMatrix() override;
    CVector Unproject(CVector2 const& scr) override;
    CVector Project(CVector const& world) override;
    CVector* ProjectWorldAbs(CVector* result, CVector const* world) override;

    // ---------------------------------------------------------------- lights.cpp
    int GetMaxLights() override;
    void LightEnable(int numLight, int state) override;
    void LightSet(int numLight, LightSource const& l) override;
    HRESULT LightSet(int numLight, D3DLIGHT9 const& l);
    void MaterialSet(Material const& mat) override;

    // ---------------------------------------------------------------- clipplanes.cpp
    unsigned int GetMaxClipPlanes() override;
    void SetClipPlane(int index, CPlane const* plane) override;
    void EnableClipPlane(int index, bool bEnable) override;
    void ActuateClipPlanes(bool bForFFP);
    void ForceRecalcClipPlanes();
    void EnableFastClipPlane(bool bEnable);
    void SetFastClipPlane(CPlane const& plane);

    // ---------------------------------------------------------------- screenshot.cpp
    unsigned int AddTextureFromBackBuffer(TexHandle srcTex) override;
    TexHandle AddTextureFromBackBuffer(int width, int height) override;
    void ScreenShot(const char* fileName, int width, int height) override;
    int SaveTextureToTgaFile(TexHandle tex, const char* fileName) override;
    int SaveTextureToFile(TexHandle tex, const char* fileName, ImageFileFormats format);

    // ---------------------------------------------------------------- query.cpp
    IQuery* NewQuery(IQuery::Type type) override;
    void OnQueryDestructor(IQuery* query);
    void ReleaseQueries();
    void rstQueryPrepareFor();
    void rstQueryRestoreAfter();

    // ---------------------------------------------------------------- mesh.cpp
    MeshHandle AddMesh(VertexType type, void* verts, int numVerts, unsigned short* tris, int numTris, SubmeshInfo* subs,
                       int numSubs, int* remap);
    int ReleaseMesh(MeshHandle& id);
    void RenderMesh(MeshHandle const& id, float lod);
    void StartRenderMeshes();
    void FinishRenderMeshes();
    int OptimizeGeometryToSingleStrip(VertexType vt, void* vertsIn, int numVertsIn, unsigned short* indicesIn,
                                      int numTrisIn, int zeroBaseIndex, void** vertsOut, int* numVertsOut, int** remap,
                                      unsigned short** singleStripOut, int* numSingleStripOutIndices) override;
    int OptimizeGeometryToTriList(VertexType vt, void* vertsIn, int numVertsIn, unsigned short* indicesIn, int numTrisIn,
                                  int zeroBaseIndex, void** vertsOut, int* numVertsOut, int** remap,
                                  unsigned short** trisIndices, int* numTrisIndices) override;

    // ---------------------------------------------------------------- fsrt.cpp
    void CreateFsRt();
    void ReleaseFsRt();
    void DrawFsRt();
    void initFullScreenQuad();
    void doneFullScreenQuadStuff();
    void DrawFullScreenQuad(bool useShader);
    void DrawFullScreenQuad(IEffect* shader) override;
    void DrawFullScreenQuad() override;

    // ---------------------------------------------------------------- rendertarget.cpp
    int RenderToTexStart(TexHandle const& destTex, bool wantZBuffer, TexHandle const& depthStencil);
    int RenderToTexStart(TexHandle const& destTex, bool wantZBuffer) override;
    void RenderToTexFinish() override;
    void CopyRenderTargetToTexture(TexHandle const& destTex) override;
    TexHandle AddRenderTargetTexture(const char* texName, int sx, int sy) override;
    TexHandle GetBufferedTargetTexture(int sz) override;
    TexHandle GetFullFrameFrameBufferTexture() override;

    // ---------------------------------------------------------------- drawcalls.cpp
    int DrawIndexedPrimitive(PrimType Type, unsigned int MinIndex, unsigned int NumVertices, unsigned int StartIndex,
                             unsigned int PrimitiveCount) override;
    int DrawPrimitive(PrimType PrimitiveType, unsigned int StartVertex, unsigned int PrimitiveCount) override;
    int DrawIndexedPrimitiveShader(PrimType Type, unsigned int MinIndex, unsigned int NumVertices,
                                   unsigned int StartIndex, unsigned int PrimitiveCount) override;
    int DrawPrimitiveShader(PrimType PrimitiveType, unsigned int StartVertex, unsigned int PrimitiveCount) override;
    int DrawIndexedPrimitiveEffect(PrimType Type, IEffect* effect, unsigned int MinIndex, unsigned int NumVertices,
                                   unsigned int StartIndex, unsigned int PrimitiveCount) override;
    int DrawPrimitiveEffect(PrimType PrimitiveType, IEffect* effect, unsigned int StartVertex,
                            unsigned int PrimitiveCount) override;

    // ---------------------------------------------------------------- texture_man.cpp
    TexHandle AddTexture(CStr const& filename, unsigned int flags) override;
    TexHandle AddDynamicTexture(const char* texName, int sx, int sy, unsigned int format) override;
    int ReloadTextures() override;
    int SetTexture(int stage, TexHandle const& id, long double tsc) override;  // original: float tsc
    void SetWhiteTexture(int stage) override;
    void SetBlackTexture(int stage) override;
    void SetErrorTexture(int stage) override;
    void DisableTextureStages(int stageToStartFrom) override;
    void SetTexAnimStart(TexHandle const& texId);
    int ReferenceTexture(TexHandle const& id) override;
    int ReleaseTexture(TexHandle& id) override;
    void SetTextureParameter(TexHandle const& id, TexParam param, unsigned int value) override;
    int GetTextureName(TexHandle const& id, CStr& name) override;
    int UploadTexImage(TexHandle const& id, unsigned int sx, unsigned int sy, unsigned char* bits,
                       TexDynFormat incomingFormat, int mipLevel) override;
    void* LockTexture(TexHandle const& id, TexDynFormat incomingFormat, int& pitch, int mipLevel) override;
    void UnlockTexture(TexHandle const& id) override;
    int DownloadTexImageRgba8888(unsigned int* dstBits, TexHandle const& tex) override;
    int DownloadTexImageRgba8888(unsigned int* dstBits, TexHandle const* tex, int sxx, int syy) override;
    void GetDims(TexHandle const& tex, int& sx, int& sy) override;
    void TexCopy(TexHandle const& dest, TexHandle const& src, bool withMips);
    void TexCopy(TexHandle const& dest, TexHandle const& src) override;
    void CreateMips(TexHandle const& tex);
    void RepaintAllTexturesMips() override;
    void ResetTextureStates() override;
    int newTexture();
    CTexMap* addTexMap(CStr const& filename, unsigned int flags);
    TexHandle readShader(CStr const& name, unsigned int flags);
    int loadTexMaps(CTexture* tex, CStr const& str, unsigned int flags);
    bool IsDynamic(CTexture* tex);
    int GetTextureCurrentFrame(TexHandle const& texId, float tsc);
    int GetTextureCurrentFrame(CTexture* tex, float tsc);
    void rstTexPrepareFor();
    bool rstTexRestoreAfter();

    // ---------------------------------------------------------------- texture_man_stats.cpp
    void UpdateTexMemStats();
    bool ReportTexturesInfo(const char* fileName) override;

    // ---------------------------------------------------------------- vb_man.cpp
    VbHandle AddVb(VertexType tvert, int vertcount, CStr const& vbName, unsigned int flags) override;
    VbHandle AddVbStreaming(VertexType tv, int sz, unsigned int flags);
    void SetToStream0(VbPoolField const& PoolField) override;
    void SetToStream0(VbHandle const& id) override;
    void SetToStream(int Stream, VbPoolField const& PoolField) override;
    void SetToStream(int stream, VbHandle const& id) override;
    void SetToStreams01(VbHandle const& id0, VbHandle const& id1);
    void* LockVb(VbHandle const& id, int sz, int ofs, unsigned int flags) override;
    void* LockVbStreaming(VbHandle const& id, int sz, int& ofs, int* discarded) override;
    void UnlockVb(VbHandle const& id) override;
    int ReferenceVb(VbHandle const* id) override;
    int ReleaseVb(VbHandle& id) override;
    VbPoolField AddVbPoolField(VertexType Type, unsigned int Size) override;
    void ReleaseVbPoolField(VbPoolField& PoolField) override;
    void* LockVbPoolField(VbPoolField const& PoolField) override;
    void UnlockVbPoolField(VbPoolField const& PoolField) override;
    VbHandle GetVbStreaming(VertexType VertType) override;
    void rstVbPrepareFor();
    void rstVbRestoreAfter();

    // ---------------------------------------------------------------- vb_man_stats.cpp
    void UpdateVBMemStats();
    bool ReportVbsInfo(const char* fileName) override;

    // ---------------------------------------------------------------- ib_man.cpp
    IbHandle AddIb(int indexcount, bool dyn) override;
    IbHandle AddIbStreaming(int sz);
    void SetIndices(IbPoolField const& PoolField, int VertOffset) override;
    void SetIndices(IbHandle const& id, int ofs) override;
    void* LockIb(IbHandle const& id, int sz, int ofs, unsigned int flags) override;
    void* LockIbStreaming(IbHandle const* id, int sz, int* ofs, int* discarded) override;
    void UnlockIb(IbHandle const& id) override;
    int ReleaseIb(IbHandle& id) override;
    int ReferenceIb(IbHandle const* id) override;
    IbPoolField AddIbPoolField(unsigned int Size) override;
    void ReleaseIbPoolField(IbPoolField& PoolField) override;
    void* LockIbPoolField(IbPoolField const& PoolField) override;
    void UnlockIbPoolField(IbPoolField const& PoolField) override;
    void rstIbPrepareFor();
    void rstIbRestoreAfter();

    // ---------------------------------------------------------------- ib_man_stats.cpp
    void UpdateIBMemStats();
    bool ReportIbsInfo(const char* fileName) override;

    // ---------------------------------------------------------------- shaders/shaders.cpp
    void SetVsFloatConst(unsigned int RegisterIndex, const float* pConstantData, unsigned int RegisterCount) override;
    void SetVsIntConst(unsigned int RegisterIndex, const int* pConstantData, unsigned int RegisterCount) override;
    void SetVsBoolConst(unsigned int RegisterIndex, const int* pConstantData, unsigned int RegisterCount) override;
    void SetPsFloatConst(unsigned int RegisterIndex, const float* pConstantData, unsigned int RegisterCount) override;
    void SetPsIntConst(unsigned int RegisterIndex, const int* pConstantData, unsigned int RegisterCount) override;
    void SetPsBoolConst(unsigned int RegisterIndex, const int* pConstantData, unsigned int RegisterCount) override;
    IAsmShader* NewAsmShader(const char* fileName, IAsmShader::Type type) override;
    IHlslShader* NewHlslShader(const char* fileName, const char* entryFunc, IHlslShader::Profile profile,
                               std::vector<CompileParam> const& compileParams);
    IHlslShader* NewHlslShader(const char* fileName, const char* entryFunc, IHlslShader::Profile profile) override;
    IEffect* NewEffect(const char* fileName, bool bApplyGlobalParams, std::vector<CompileParam> const& compileParams);
    IEffect* NewEffect(const char* fileName, bool bApplyGlobalParams) override;
    bool ReloadShaders() override;
    void AddChangeShaderMacro(ShaderMacro const* macro) override;
    void DeleteShaderMacro(CStr const* macroName) override;
    void OnEffectDestructor(EffectImpl* effect);
    void OnHlslShaderDestructor(HlslShaderImpl* shader);
    void OnAsmShaderDestructor(AsmShaderImpl* shader);
    void rstShadersPrepareFor();
    void rstShadersRestoreAfter();
    void ReleaseAsmShaders();
    void ReleaseHlslShaders();
    void ReleaseEffects();
    void createD3DXMacros();
    void addMacro(CStr const& name, CStr const& definition, bool userDefined);
    void loadShadersMacros();
    void saveShadersMacros();

    // ---------------------------------------------------------------- shaders/effects/effect_strings.cpp
    const char* EffectParameterToString(IEffect::Parameter p) override;
    IEffect::Parameter StringToEffectParameter(const char* str) override;
    const char* CompileParamToString(CompileParam p);
    CompileParam StringToCompileParam(const char* str);
};

// ------------------------------------------------------------------------------------------------
// The members defined inline in the original device.h.

// orig 0x636cc0 device.h:176
inline CDevice::CVertexBuffer::CVertexBuffer()
{
    m_vb = 0;
    m_refs = 0;
    m_locked = 0;
    m_lockedAtPresent = -1;
    m_vertexDecl = 0;
}

// orig 0x643400 device.h:209
inline CDevice::CIndexBuffer::CIndexBuffer()
{
    m_ib = 0;
    m_refs = 0;
    m_locked = 0;
}

// orig 0x641cc0 device.h:235
inline CDevice::CTexMap::CTexMap()
{
    m_pTex = 0;
    m_refs = 0;
}

inline CDevice::CTexMap::~CTexMap() {}

// orig 0x60cbd0 device.h:242
inline void CDevice::CTexMap::freeTex()
{
    if (m_pTex)
    {
        m_pTex->Release();
        m_pTex = 0;
    }
}

// orig 0x63d5c0 device.h:279
inline int CDevice::CTexMap::addRef()
{
    return ++m_refs;
}

// orig 0x63d5d0 device.h:280
inline int CDevice::CTexMap::release()
{
    return --m_refs;
}

// orig 0x642090 device.h:295
inline CDevice::CTexture::CTexture()
{
    m_refs = 0;
    m_looped = 0;
    m_timeStamp = 0.0;
}

inline CDevice::CTexture::CTexture(CTexture const& that) = default;

inline CDevice::CTexture::~CTexture() {}

// orig 0x641e80 device.h:303 - a reference on the texture is a reference on each of its maps.
inline int CDevice::CTexture::addRef()
{
    std::for_each(m_maps.begin(), m_maps.end(), std::mem_fn(&CTexMap::addRef));
    return ++m_refs;
}

// orig 0x641eb0 device.h:311
inline int CDevice::CTexture::release()
{
    std::for_each(m_maps.begin(), m_maps.end(), std::mem_fn(&CTexMap::release));
    return --m_refs;
}

// orig 0x64d410 device.h:346
inline CDevice::CMesh::CMesh()
{
    m_mesh = 0;
    m_pmesh = 0;
    m_refs = 0;
}

// orig 0x648660 device.h:1126
inline CDevice::ShaderIdData::ShaderIdData(CStr const& fname, std::vector<CompileParam> const& cp) :
    filename(fname),
    compileParams(cp)
{
}

// orig 0x63a540 device.h:1150
inline CDevice::MacroData::MacroData() :
    userDefined(false)
{
}

// orig 0x621d70 device.h:1253
inline CDevice::DXCursorInfo::DXCursorInfo()
{
    m_texId.SetInvalid();
    m_xHotSpot = 0;
    m_yHotSpot = 0;
    m_frame = 0;
}

// orig 0x620dc0 device.h:1262
inline void CDevice::DXCursorInfo::SetUp(TexHandle const& texId, int xHotSpot, int yHotSpot, int frame)
{
    m_texId = texId;
    m_xHotSpot = xHotSpot;
    m_yHotSpot = yHotSpot;
    m_frame = frame;
}

// orig 0x63d5e0 device.h:546
inline D3DFORMAT CDevice::GetTexFormatRt()
{
    return m_texFormatRt;
}

// orig 0x63d5f0 device.h:547
inline D3DFORMAT CDevice::GetTexFormatShadow()
{
    return m_texFormatShadow;
}

// orig 0x63d600 device.h:548
inline D3DFORMAT CDevice::GetTexFormatDepth()
{
    return m_texFormatDepth;
}

// orig 0x644470 device.h:572
inline bool CDevice::IsIbValid(IbHandle const& ib) const
{
    return IbId(ib) >= 0 && IbId(ib) < (int)m_ibs.size() && m_ibs[IbId(ib)].m_ib != 0;
}

// orig 0x64eea0 device.h:577
inline bool CDevice::IsMeshValid(MeshHandle const& ib) const
{
    return MeshId(ib) >= 0 && MeshId(ib) < (int)m_meshes.size() && m_meshes[MeshId(ib)].m_mesh != 0;
}

// orig 0x637950 device.h:583
inline bool CDevice::IsVbValid(VbHandle const& ib) const
{
    return VbId(ib) >= 0 && VbId(ib) < (int)m_vbs.size() && m_vbs[VbId(ib)].m_vb != 0;
}

// orig 0x621d00 device.h:588
inline bool CDevice::IsTexValid(TexHandle const& ib) const
{
    return TexId(ib) >= 0 && TexId(ib) < (int)m_textures.size() && !m_textures[TexId(ib)].m_maps.empty();
}

// orig 0x60cbf0 device.h:597
inline IDirect3DDevice9* CDevice::GetDevice()
{
    return m_pd3dDevice;
}

// orig 0x650840 device.h:600
inline HRESULT CDevice::GetLastResult() const
{
    return m_lastResult;
}

// orig 0x6452a0 device.h:602
inline IDirect3DCubeTexture9* CDevice::GetNormalCubemap()
{
    return m_NormalizingCubemap;
}

inline D3DCAPS9 CDevice::GetCaps() const
{
    return m_d3dCaps;
}

// orig 0x625340 device.h:680
inline CMatrix const& CDevice::MatGet()
{
    return m_matViewStack[m_matViewStackTop];
}

// orig 0x625450 device.h:690
inline void CDevice::MatGetBasis(CVector& right, CVector& up, CVector& fwd) const
{
    m_matViewStack[m_matViewStackTop].GetBasis(right, up, fwd);
    right.normalizeInplace();
    up.normalizeInplace();
    fwd.normalizeInplace();
}

// orig 0x625360 device.h:706
inline CMatrix const& CDevice::MatGetWorld()
{
    return m_matWorldStack[m_matWorldStackTop];
}

// orig 0x625380 device.h:717
inline CMatrix const& CDevice::MatGetProj()
{
    return m_matProjStack[m_matProjStackTop];
}

// orig 0x6253a0 device.h:792
inline float CDevice::GetMaxPointSize()
{
    return GetCaps().MaxPointSize;
}

// orig 0x6253d0 device.h:797
inline float CDevice::GetMaxNPatchTessellationLevel()
{
    return GetCaps().MaxNpatchTessellationLevel;
}

// orig 0x625400 device.h:802
inline int CDevice::GetMaxVertexShaderConst()
{
    return GetCaps().MaxVertexShaderConst;
}

// orig 0x625430 device.h:937
inline int CDevice::GetMaxLights()
{
    return m_maxLights;
}

// orig 0x625440 device.h:1237
inline bool CDevice::IsNV3x()
{
    return m_isNV30;
}
