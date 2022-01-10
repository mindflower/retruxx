#pragma once
#include "i_renderer_colors.h"
#include "i_renderer_handles.h"
#include "i_renderer_vertex.h"
#include <iface.h>
#include <core/stringm3d.h>
#include <math/vector.h>
#include <Windows.h>

class CVector2;
class CPlane;
class CMatrix;

namespace m3d
{
    class Kernel;
}

namespace m3d
{
    namespace rend
    {
        class TexHandle;

        enum Cull
        {
            M3DCULL_NONE = 0x0,
            M3DCULL_CW = 0x1,
            M3DCULL_CCW = 0x2,
        };

        enum BlendMode
        {
            BM_NONE = 0x0,
            BM_COLOR = 0x1,
            BM_ALPHA = 0x2,
            BM_0_0 = 0x3,
            BM_0_1 = 0x4,
            BM_0_SCOLOR = 0x5,
            BM_0_ISCOLOR = 0x6,
            BM_0_SALPHA = 0x7,
            BM_0_ISALPHA = 0x8,
            BM_0_DALPHA = 0x9,
            BM_0_IDALPHA = 0xA,
            BM_0_DCOLOR = 0xB,
            BM_0_IDCOLOR = 0xC,
            BM_0_SALPHASAT = 0xD,
            BM_1_0 = 0xE,
            BM_1_1 = 0xF,
            BM_1_SCOLOR = 0x10,
            BM_1_ISCOLOR = 0x11,
            BM_1_SALPHA = 0x12,
            BM_1_ISALPHA = 0x13,
            BM_1_DALPHA = 0x14,
            BM_1_IDALPHA = 0x15,
            BM_1_DCOLOR = 0x16,
            BM_1_IDCOLOR = 0x17,
            BM_1_SALPHASAT = 0x18,
            BM_SCOLOR_0 = 0x19,
            BM_SCOLOR_1 = 0x1A,
            BM_SCOLOR_SCOLOR = 0x1B,
            BM_SCOLOR_ISCOLOR = 0x1C,
            BM_SCOLOR_SALPHA = 0x1D,
            BM_SCOLOR_ISALPHA = 0x1E,
            BM_SCOLOR_DALPHA = 0x1F,
            BM_SCOLOR_IDALPHA = 0x20,
            BM_SCOLOR_DCOLOR = 0x21,
            BM_SCOLOR_IDCOLOR = 0x22,
            BM_SCOLOR_SALPHASAT = 0x23,
            BM_ISCOLOR_0 = 0x24,
            BM_ISCOLOR_1 = 0x25,
            BM_ISCOLOR_SCOLOR = 0x26,
            BM_ISCOLOR_ISCOLOR = 0x27,
            BM_ISCOLOR_SALPHA = 0x28,
            BM_ISCOLOR_ISALPHA = 0x29,
            BM_ISCOLOR_DALPHA = 0x2A,
            BM_ISCOLOR_IDALPHA = 0x2B,
            BM_ISCOLOR_DCOLOR = 0x2C,
            BM_ISCOLOR_IDCOLOR = 0x2D,
            BM_ISCOLOR_SALPHASAT = 0x2E,
            BM_SALPHA_0 = 0x2F,
            BM_SALPHA_1 = 0x30,
            BM_SALHPA_SCOLOR = 0x31,
            BM_SALPHA_ISCOLOR = 0x32,
            BM_SALPHA_SALPHA = 0x33,
            BM_SALPHA_ISALPHA = 0x34,
            BM_SALPHA_DALPHA = 0x35,
            BM_SALPHA_IDALPHA = 0x36,
            BM_SALPHA_DCOLOR = 0x37,
            BM_SALPHA_IDCOLOR = 0x38,
            BM_SALPHA_SALPHASAT = 0x39,
            BM_ISALPHA_0 = 0x3A,
            BM_ISALPHA_1 = 0x3B,
            BM_ISALHPA_SCOLOR = 0x3C,
            BM_ISALPHA_ISCOLOR = 0x3D,
            BM_ISALPHA_SALPHA = 0x3E,
            BM_ISALPHA_ISALPHA = 0x3F,
            BM_ISALPHA_DALPHA = 0x40,
            BM_ISALPHA_IDALPHA = 0x41,
            BM_ISALPHA_DCOLOR = 0x42,
            BM_ISALPHA_IDCOLOR = 0x43,
            BM_ISALPHA_SALPHASAT = 0x44,
            BM_DALPHA_0 = 0x45,
            BM_DALPHA_1 = 0x46,
            BM_DALHPA_SCOLOR = 0x47,
            BM_DALPHA_ISCOLOR = 0x48,
            BM_DALPHA_SALPHA = 0x49,
            BM_DALPHA_ISALPHA = 0x4A,
            BM_DALPHA_DALPHA = 0x4B,
            BM_DALPHA_IDALPHA = 0x4C,
            BM_DALPHA_DCOLOR = 0x4D,
            BM_DALPHA_IDCOLOR = 0x4E,
            BM_DALPHA_SALPHASAT = 0x4F,
            BM_IDALPHA_0 = 0x50,
            BM_IDALPHA_1 = 0x51,
            BM_IDALHPA_SCOLOR = 0x52,
            BM_IDALPHA_ISCOLOR = 0x53,
            BM_IDALPHA_SALPHA = 0x54,
            BM_IDALPHA_ISALPHA = 0x55,
            BM_IDALPHA_DALPHA = 0x56,
            BM_IDALPHA_IDALPHA = 0x57,
            BM_IDALPHA_DCOLOR = 0x58,
            BM_IDALPHA_IDCOLOR = 0x59,
            BM_IDALPHA_SALPHASAT = 0x5A,
            BM_DCOLOR_0 = 0x5B,
            BM_DCOLOR_1 = 0x5C,
            BM_DCOLOR_SCOLOR = 0x5D,
            BM_DCOLOR_ISCOLOR = 0x5E,
            BM_DCOLOR_SALPHA = 0x5F,
            BM_DCOLOR_ISALPHA = 0x60,
            BM_DCOLOR_DALPHA = 0x61,
            BM_DCOLOR_IDALPHA = 0x62,
            BM_DCOLOR_DCOLOR = 0x63,
            BM_DCOLOR_IDCOLOR = 0x64,
            BM_DCOLOR_SALPHASAT = 0x65,
            BM_IDCOLOR_0 = 0x66,
            BM_IDCOLOR_1 = 0x67,
            BM_IDCOLOR_SCOLOR = 0x68,
            BM_IDCOLOR_ISCOLOR = 0x69,
            BM_IDCOLOR_SALPHA = 0x6A,
            BM_IDCOLOR_ISALPHA = 0x6B,
            BM_IDCOLOR_DALPHA = 0x6C,
            BM_IDCOLOR_IDALPHA = 0x6D,
            BM_IDCOLOR_DCOLOR = 0x6E,
            BM_IDCOLOR_IDCOLOR = 0x6F,
            BM_IDCOLOR_SALPHASAT = 0x70,
            BM_SALPHASAT_0 = 0x71,
            BM_SALPHASAT_1 = 0x72,
            BM_SALPHASAT_SCOLOR = 0x73,
            BM_SALPHASAT_ISCOLOR = 0x74,
            BM_SALPHASAT_SALPHA = 0x75,
            BM_SALPHASAT_ISALPHA = 0x76,
            BM_SALPHASAT_DALPHA = 0x77,
            BM_SALPHASAT_IDALPHA = 0x78,
            BM_SALPHASAT_DCOLOR = 0x79,
            BM_SALPHASAT_IDCOLOR = 0x7A,
            BM_SALPHASAT_SALPHASAT = 0x7B,
            BM_COLOR_ADD = 0xF,
            BM_COLOR_MODULATE = 0x5B,
            BM_COLOR_ADDSIGNED = 0x5D,
            BM_COLOR_ADDSMOOTH = 0x11,
        };


        enum ZbState
        {
            ZB_DISABLE = 0x0,
            ZB_ENABLE = 0x1,
            ZB_NOWRITE = 0x2,
            ZB_WRITE_NOTEST = 0x3,
        };

        enum CmpFunc
        {
            M3DCMP_NEVER = 0x0,
            M3DCMP_LESS = 0x1,
            M3DCMP_EQUAL = 0x2,
            M3DCMP_LESSEQUAL = 0x3,
            M3DCMP_GREATER = 0x4,
            M3DCMP_NOTEQUAL = 0x5,
            M3DCMP_GREATEREQUAL = 0x6,
            M3DCMP_ALWAYS = 0x7,
        };

        enum ShadeMode
        {
            M3DSHADE_FLAT = 0x0,
            M3DSHADE_GOURAUD = 0x1,
        };

        enum FogMode
        {
            M3DFOG_NONE = 0x0,
            M3DFOG_EXP = 0x1,
            M3DFOG_EXP2 = 0x2,
            M3DFOG_LINEAR = 0x3,
        };

        enum FillMode
        {
            M3DFILL_POINT = 0x0,
            M3DFILL_WIREFRAME = 0x1,
            M3DFILL_SOLID = 0x2,
        };

        enum StencilOp
        {
            OP_KEEP = 0x0,
            OP_ZERO = 0x1,
            OP_REPLACE = 0x2,
            OP_INCRSAT = 0x3,
            OP_DECRSAT = 0x4,
            OP_INVERT = 0x5,
            OP_INCR = 0x6,
            OP_DECR = 0x7,
        };

        enum TexCreateFlags
        {
            TM_MIPQ_NOMIPS = 0x0,
            TM_MIPQ_LOW = 0x1,
            TM_MIPQ_HIGH = 0x2,
            TM_MIPQ_MASK = 0x3,
            TM_NO_COMPRESS = 0x4,
            TM_WANT_ALPHA = 0x8,
        };

        enum ClearFlags
        {
            M3DCLEAR_C = 0x1,
            M3DCLEAR_Z = 0x2,
            M3DCLEAR_S = 0x4,
            M3DCLEAR_ZS = 0x6,
            M3DCLEAR_CS = 0x5,
            M3DCLEAR_CZ = 0x3,
            M3DCLEAR_CZS = 0x7,
        };

        enum TgMode
        {
            TG_DISABLE = 0x0,
            TG_THRU_1 = 0x1,
            TG_THRU_2 = 0x2,
            TG_THRU_3 = 0x3,
            TG_THRU_4 = 0x4,
            TG_PROJ_1 = 0x5,
            TG_PROJ_2 = 0x6,
            TG_PROJ_3 = 0x7,
            TG_PROJ_4 = 0x8,
            TG_PROJ_PS11 = 0x9,
        };

        enum PrimType
        {
            M3DPT_POINTLIST = 0x0,
            M3DPT_LINELIST = 0x1,
            M3DPT_LINESTRIP = 0x2,
            M3DPT_TRIANGLELIST = 0x3,
            M3DPT_TRIANGLESTRIP = 0x4,
            M3DPT_TRIANGLEFAN = 0x5,
        };

        enum TexDynFormat
        {
            TM_DTF_RENDER_TARGET = 0x0,
            TM_DTF_SAME_AS_RENDER_TARGET = 0x1,
            TM_DTF_LUMINANCE8 = 0x2,
            TM_DTF_ALPHA8 = 0x3,
            TM_DTF_RGBA8888 = 0x4,
            TM_DTF_RGBA8888_VIDEOFRAME = 0x5,
            TM_DTF_SHADOW_TARGET = 0x6,
            TM_DTF_WANT_MIPS = 0x8000,
        };

        enum TcSource
        {
            TC_FROM_VERTEX = 0x0,
            TC_FROM_NORMAL_IN_CAMERA_SPACE = 0x1,
            TC_FROM_POSITION_IN_CAMERA_SPACE = 0x2,
            TC_FROM_REFLECTION_IN_CAMERA_SPACE = 0x3,
        };

        enum DeviceFeature
        {
            FEATURE_VS_1_1 = 0x0,
            FEATURE_VS_2_0 = 0x1,
            FEATURE_VS_3_0 = 0x2,
            FEATURE_PS_1_1 = 0x3,
            FEATURE_PS_1_3 = 0x4,
            FEATURE_PS_1_4 = 0x5,
            FEATURE_PS_2_0 = 0x6,
            FEATURE_PS_3_0 = 0x7,
            FEATURE_STENCIL = 0x8,
            FEATURE_2SIDED_STENCIL = 0x9,
            FEATURE_NON_POW2_RT = 0xA,
            FEATURE_MRT = 0xB,
            FEATURE_NON_POW2_CONDITIONAL = 0xC,
            FEATURE_QUERY_VCACHE = 0xD,
            FEATURE_QUERY_EVENT = 0xE,
            FEATURE_QUERY_OCCLUSION = 0xF,
            FEATURE_QUERY_TIMESTAMP = 0x10,
            FEATURE_QUERY_TIMESTAMPDISJOINT = 0x11,
            FEATURE_QUERY_TIMESTAMPFREQ = 0x12,
            FEATURE_QUERY_PIPELINETIMINGS = 0x13,
            FEATURE_QUERY_INTERFACETIMINGS = 0x14,
            FEATURE_QUERY_VERTEXTIMINGS = 0x15,
            FEATURE_QUERY_PIXELTIMINGS = 0x16,
            FEATURE_QUERY_BANDWIDTHTIMINGS = 0x17,
            FEATURE_QUERY_CACHEUTILIZATION = 0x18,
            FEATURE_NUM_FEATURES = 0x19,
        };

        enum TextureState
        {
            TS_NONE = 0x0,
            TS_TEXTURE = 0x1,
            TS_DIFFUSE = 0x2,
            TS_TFACTOR = 0x3,
            TS_MODULATE = 0x4,
            TS_MODULATE2X = 0x5,
            TS_TEX_ADDSIGNED_DIFF = 0x6,
            TS_ITEX_ADDSIGNED_DIFF = 0x7,
            TS_TEX_DP3_TFAC = 0x8,
            TS_TEX_ADD_DIFF = 0x9,
            TS_PREV = 0xA,
            TS_IPREV = 0xB,
            TS_IPREV_ADDSIGNED_DIFF = 0xC,
            TS_IPREV_ADD_DIFF = 0xD,
            TS_TEX_ADD_PREV = 0xE,
            TS_TEX_ADDSMOOTH_PREV = 0xF,
            TS_TEX_MODULATE_PREV = 0x10,
            TS_TEX_MODULATE2X_PREV = 0x11,
            TS_TEX_ADDSIGNED_PREV = 0x12,
            TS_DIFF_MODULATE_PREV = 0x13,
            TS_DIFF_ADD_PREV = 0x14,
            TS_DIFF_ADDSMOOTH_PREV = 0x15,
            TS_DIFF_MODULATE_TFAC = 0x16,
            TS_TEX_TFAC_LERP_PREV = 0x17,
            TS_PREV_MINUS_TEX = 0x18,
            TS_PREV_ADDSMOOTH_TFAC = 0x19,
            TS_TEX_MODULATE_TFAC = 0x1A,
            TS_TEX_MODULATE2X_TFAC = 0x1B,
        };

        enum TexParam
        {
            TM_TEX_FILTER = 0x0,
            TM_TEX_MIN_FILTER = 0x1,
            TM_TEX_MAG_FILTER = 0x2,
            TM_WRAP_S = 0x3,
            TM_WRAP_T = 0x4,
            TM_WRAP_R = 0x5,
            TM_WRAP_BORDER_COLOR = 0x6,
            TM_MIP_LOD_BIAS = 0x7,
            TM_MIP_LOD_MAX = 0x8,
            TM_MAX_ANISOTROPY = 0x9,
        };

        struct Viewport
        {
            int m_x0;
            int m_y0;
            int m_width;
            int m_height;
            float m_zMin;
            float m_zMax;
        };

        class Material
        {
        public:
            Material();
            void init(Colorf const&);

        private:
            Colorf m_diffuse;
            Colorf m_ambient;
            Colorf m_specular;
            Colorf m_emissive;
            float m_specularPower;
        };

        class TexHandle : public Handle<TexHandle>
        {
            
        };

        class IbHandle : public Handle<IbHandle>
        {
        };

        class VbHandle : public Handle<VbHandle>
        {
        };

        class VbPoolField
        {
        public:
            unsigned int GetOffset() const;
            VbHandle GetVbHandle() const;

        private:
            unsigned int Offset;
            unsigned int Size;
            unsigned int RealOffset;
            VertexType VertType;
            VbHandle Vb;
        };

        class IbPoolField
        {
        public:
            unsigned int GetOffset() const;
            IbHandle GetIbHandle() const;

        private:
            unsigned int Offset;
            unsigned int Size;
            unsigned int RealOffset;
            IbHandle Ib;
        };

        enum LightType
        {
            M3DLIGHT_POINT = 0x0,
            M3DLIGHT_SPOT = 0x1,
            M3DLIGHT_DIRECTIONAL = 0x2,
        };

        class LightSource
        {
        public:
            void init(LightType, CVector const&);

        private:
            LightType m_type;
            Colorf m_diffuse;
            Colorf m_specular;
            Colorf m_ambient;
            CVector m_origin;
            CVector m_direction;
            float m_range;
            float m_falloff;
            float m_attenuation0;
            float m_attenuation1;
            float m_attenuation2;
            float m_theta;
            float m_phi;
        };

        struct RenderStats
        {
            unsigned int polyCount;
            unsigned int DIPs;
            unsigned int DPs;
            unsigned int swVertexShaders;
            unsigned int swPixelShaders;
            unsigned int swFfpTpShader;
            unsigned int swRenderStates;
            unsigned int swTextures;
            unsigned int swTextureStageStates;
            unsigned int swTextureSamplerStates;
            unsigned int swMatrices;
            unsigned int swRenderTargets;
        };

        struct DeviceMemStats
        {
            unsigned int RtTexSize;
            unsigned int RtTexCount;
            unsigned int StaticTexSize;
            unsigned int StaticTexCount;
            unsigned int DynamicTexSize;
            unsigned int DynamicTexCount;
            unsigned int DynamicVBSize;
            unsigned int DynamicVBCount;
            unsigned int StaticVBSize;
            unsigned int StaticVBCount;
            unsigned int VBPoolsSize;
            unsigned int DynamicIBSize;
            unsigned int DynamicIBCount;
            unsigned int StaticIBSize;
            unsigned int StaticIBCount;
            unsigned int IBPoolsSize;
        };

        class ShaderMacro
        {
        public:
            ShaderMacro(char const*, char const*);

        private:
            CStr name;
            CStr definition;
        };

        class IRenderResource
        {
        private:
            //m3d::rend::IRenderResource_vtbl* __vftable /*VFT*/;
            int m_refCount;
        };

        class IQuery : public IRenderResource
        {
        public:
            enum Type
            {
                QUERY_VCACHE = 0x4,
                QUERY_EVENT = 0x8,
                QUERY_OCCLUSION = 0x9,
                QUERY_TIMESTAMP = 0xA,
                QUERY_TIMESTAMPDISJOINT = 0xB,
                QUERY_TIMESTAMPFREQ = 0xC,
                QUERY_PIPELINETIMINGS = 0xD,
                QUERY_INTERFACETIMINGS = 0xE,
                QUERY_VERTEXTIMINGS = 0xF,
                QUERY_BANDWIDTHTIMINGS = 0x11,
                QUERY_CACHEUTILIZATION = 0x12,
            };

            enum State
            {
                QUERY_NOT_SUPPORT = 0x0,
                QUERY_SIGNALED = 0x1,
                QUERY_ISSUED = 0x2,
                QUERY_ERROR = 0x3,
            };
        };

        class IHlslShader : public IRenderResource
        {
        public:
            enum Profile
            {
                VS_1_1 = 0x0,
                VS_2_0 = 0x1,
                VS_3_0 = 0x2,
                PS_1_1 = 0x3,
                PS_1_3 = 0x4,
                PS_1_4 = 0x5,
                PS_2_0 = 0x6,
                PS_2_a = 0x7,
                PS_3_0 = 0x8,
            };
        };

        class IAsmShader : public IRenderResource
        {
        public:
            enum Type
            {
                VERTEX_SHADER = 0x0,
                PIXEL_SHADER = 0x1,
            };
        };

        class IEffect : public IRenderResource
        {
        public:
            enum Parameter
            {
                World = 0x0,
                View = 0x1,
                Projection = 0x2,
                ModelView = 0x3,
                InvWorld = 0x4,
                ModelViewProjection = 0x5,
                ViewPos = 0x6,
                DiffMap0 = 0x7,
                CubeMap0 = 0x8,
                BumpMap0 = 0x9,
                DetailMap0 = 0xA,
                LightMap0 = 0xB,
                NormalizationCubemap = 0xC,
                Time_Linear = 0xD,
                Tree_Bend_Term = 0xE,
                LightAmbient = 0xF,
                LightDiffuse = 0x10,
                LightPlant = 0x11,
                LightSpecular = 0x12,
                FogTerm = 0x13,
                Transparency = 0x14,
                TransStartDist = 0x15,
                TransObjectWidth = 0x16,
                TmpLight0Dir = 0x17,
                User_float_param = 0x18,
                User_float_param2 = 0x19,
                User_float_param3 = 0x1A,
                User_float3_param = 0x1B,
                User_float3_param2 = 0x1C,
                User_float4_param = 0x1D,
                User_float4x4_param = 0x1E,
                NumParameters = 0x1F,
                InvalidParameter = 0x20,
            };

            class TechniqueDesc
            {
            private:
                CStr name;
                unsigned int numPasses;
                CStr briefDesc;
                CStr vertexFormatStr;
                m3d::rend::VertexType vertexFormat;
                bool tangentSpaceUsed;
                bool isDefault;
                bool isPS20;
                bool useAlpha;
            };
        };

        //IMPORTANT: fields and member order is strict!
        class IRenderer : public IBase
        {
        public:
            virtual int Create(void __fastcall(CStr const&), m3d::Kernel*);
            virtual int CreateDevice();
            virtual int SwitchDisplayModes(HWND, int, int, int);
            virtual int Reset();
            char gap20[4];
            virtual void PushBlend();
            virtual void PopBlend();
            virtual void SetBlend(m3d::rend::BlendMode, bool);
            char gap30[4];
            virtual void PushZbState();
            virtual void PopZbState();
            virtual void SetZbState(m3d::rend::ZbState, bool);
            char gap40[4];
            virtual void PushCull();
            virtual void PopCull();
            virtual void SetCull(m3d::rend::Cull, bool);
            char gap50[4];
            virtual void PushZFunc();
            virtual void PopZFunc();
            virtual void SetZFunc(m3d::rend::CmpFunc, bool);
            char gap60[4];
            virtual void PushLighting();
            virtual void PopLighting();
            virtual void SetLighting(bool, bool);
            char gap70[4];
            virtual void PushAmbient();
            virtual void PopAmbient();
            virtual void SetAmbient(unsigned int, bool);
            char gap80[4];
            virtual void PushFog();
            virtual void PopFog();
            virtual void SetFog(bool, bool);
            char gap90[4];
            virtual void PushFogColor();
            virtual void PopFogColor();
            virtual void SetFogColor(unsigned int, bool);
            char gapA0[4];
            virtual void PushFogMode();
            virtual void PopFogMode();
            virtual void SetFogMode(m3d::rend::FogMode, bool);
            char gapB0[4];
            virtual void PushFogStart();
            virtual void PopFogStart();
            virtual void SetFogStart(float, bool);
            char gapC0[4];
            virtual void PushFogEnd();
            virtual void PopFogEnd();
            virtual void SetFogEnd(float, bool);
            char gapD0[4];
            virtual void PushFillMode();
            virtual void PopFillMode();
            virtual void SetFillMode(m3d::rend::FillMode, bool);
            char gapE0[4];
            virtual void PushZBias();
            virtual void PopZBias();
            virtual void SetZBias(float, bool);
            char gapF0[4];
            virtual void PushZBiasSlopeScale();
            virtual void PopZBiasSlopeScale();
            virtual void SetZBiasSlopeScale(float, bool);
            char gap100[4];
            virtual void PushShadeMode();
            virtual void PopShadeMode();
            virtual void SetShadeMode(m3d::rend::ShadeMode, bool);
            char gap110[4];
            virtual void PushPointSpriteEnable();
            virtual void PopPointSpriteEnable();
            virtual void SetPointSpriteEnable(int, bool);
            char gap120[4];
            virtual void PushPointScaleEnable();
            virtual void PopPointScaleEnable();
            virtual void SetPointScaleEnable(int, bool);
            char gap130[4];
            virtual void PushPointSizeMin();
            virtual void PopPointSizeMin();
            virtual void SetPointSizeMin(float, bool);
            char gap140[4];
            virtual void PushPointSizeMax();
            virtual void PopPointSizeMax();
            virtual void SetPointSizeMax(float, bool);
            char gap150[4];
            virtual void PushPointSize();
            virtual void PopPointSize();
            virtual void SetPointSize(float, bool);
            char gap160[4];
            virtual void PushPointScaleA();
            virtual void PopPointScaleA();
            virtual void SetPointScaleA(float, bool);
            char gap170[4];
            virtual void PushPointScaleB();
            virtual void PopPointScaleB();
            virtual void SetPointScaleB(float, bool);
            char gap180[4];
            virtual void PushPointScaleC();
            virtual void PopPointScaleC();
            virtual void SetPointScaleC(float, bool);
            char gap190[4];
            virtual void PushTFactor();
            virtual void PopTFactor();
            virtual void SetTFactor(unsigned int, bool);
            char gap1A0[4];
            virtual void PushLocalViewer();
            virtual void PopLocalViewer();
            virtual void SetLocalViewer(bool, bool);
            char gap1B0[4];
            virtual void PushSpecularLighting();
            virtual void PopSpecularLighting();
            virtual void SetSpecularLighting(bool, bool);
            char gap1C0[4];
            virtual void PushColorWriteMask();
            virtual void PopColorWriteMask();
            virtual void SetColorWriteMask(unsigned int, bool);
            char gap1D0[4];
            virtual void PushNPatchLevel();
            virtual void PopNPatchLevel();
            virtual void SetNPatchLevel(float, bool);
            char gap1E0[4];
            virtual void PushStencilState();
            virtual void PopStencilState();
            virtual void SetStencilState(bool, bool);
            char gap1F0[4];
            virtual void PushStencilMask();
            virtual void PopStencilMask();
            virtual void SetStencilMask(unsigned int, bool);
            char gap200[4];
            virtual void PushStencilRef();
            virtual void PopStencilRef();
            virtual void SetStencilRef(unsigned int, bool);
            char gap210[4];
            virtual void PushStencilWriteMask();
            virtual void PopStencilWriteMask();
            virtual void SetStencilWriteMask(unsigned int, bool);
            char gap220[4];
            virtual void PushStencilFunc();
            virtual void PopStencilFunc();
            virtual void SetStencilFunc(m3d::rend::CmpFunc, bool);
            char gap230[4];
            virtual void PushStencilFail();
            virtual void PopStencilFail();
            virtual void SetStencilFail(m3d::rend::StencilOp, bool);
            char gap240[4];
            virtual void PushStencilZFail();
            virtual void PopStencilZFail();
            virtual void SetStencilZFail(m3d::rend::StencilOp, bool);
            char gap250[4];
            virtual void PushStencilPass();
            virtual void PopStencilPass();
            virtual void SetStencilPass(m3d::rend::StencilOp, bool);
            char gap260[4];
            virtual void PushStencil2SidedEnable();
            virtual void PopStencil2SidedEnable();
            virtual void SetStencil2SidedEnable(bool, bool);
            char gap270[4];
            virtual void PushStencilCcwFunc();
            virtual void PopStencilCcwFunc();
            virtual void SetStencilCcwFunc(m3d::rend::CmpFunc, bool);
            char gap280[4];
            virtual void PushStencilCcwFail();
            virtual void PopStencilCcwFail();
            virtual void SetStencilCcwFail(m3d::rend::StencilOp, bool);
            char gap290[4];
            virtual void PushStencilCcwZFail();
            virtual void PopStencilCcwZFail();
            virtual void SetStencilCcwZFail(m3d::rend::StencilOp, bool);
            char gap2A0[4];
            virtual void PushStencilCcwPass();
            virtual void PopStencilCcwPass();
            virtual void SetStencilCcwPass(m3d::rend::StencilOp, bool);
            char gap2B0[4];
            virtual void PushMultiSample();
            virtual void PopMultiSample();
            virtual void SetMultiSample(bool, bool);
            char gap2C0[4];
            virtual void PushMultiSampleMask();
            virtual void PopMultiSampleMask();
            virtual void SetMultiSampleMask(unsigned int, bool);
            virtual void SetViewMatrix(const CMatrix*);
            virtual const CMatrix* GetViewMatrix();
            virtual const CVector* GetViewOrigin();
            char gap2DC[4];
            virtual void MatPush();
            virtual void MatPop(bool);
            virtual void MatMul(const CMatrix*);
            virtual void MatMulR(const CMatrix*);
            virtual const CMatrix* MatGet();
            virtual const CMatrix* MatGetInv();
            virtual void MatSet(const CMatrix*);
            virtual void MatGetBasis(CVector*, CVector*, CVector*);
            virtual CVector MatGetOrgInv();
            virtual CVector MatGetOrg();
            virtual void MatSetWorld(const CMatrix*);
            virtual const CMatrix* MatGetWorld();
            virtual void MatPushWorld();
            virtual void MatPopWorld();
            virtual void MatSetProj(const CMatrix*);
            virtual const CMatrix* MatGetProj();
            virtual void MatPushProj();
            virtual void MatPopProj();
            virtual const CMatrix* GetModelViewProjMatrix();
            virtual CVector* Unproject(CVector* result, const CVector2*);
            virtual CVector* Project(CVector* result, const CVector*);
            virtual CVector* ProjectWorldAbs(CVector* result, const CVector*);
            virtual void TgEnableSetLinearSt(int, float, float, float, float, float, bool, float, float, float, float);
            virtual void TgEnableSetMatrixSt(int, const CMatrix*, bool);
            virtual void TgEnableSetMatrixStr(int, const CMatrix*, bool);
            virtual void TgSetTransformMode(int, m3d::rend::TgMode);
            virtual void TgSetTcSource(int, m3d::rend::TcSource, int);
            virtual void TgDisable(int);
            virtual void SetTextureMatrix(int, const CMatrix*);
            virtual void Set2x2BumpMatrix(int, float, float, float, float);
            virtual int RenderToTexStart(const m3d::rend::TexHandle*, bool);
            virtual void RenderToTexFinish();
            virtual void CopyRenderTargetToTexture(const m3d::rend::TexHandle*);
            virtual int CanRender();
            virtual int SetActiveState(int);
            virtual int BeginScene();
            virtual int EndScene();
            virtual int InScene();
            virtual int PresentScene();
            virtual void ClearViewport(m3d::rend::ClearFlags, unsigned int);
            virtual rend::Viewport GetViewport();
            virtual int SetViewport(const m3d::rend::Viewport*);
            virtual void RegisterResetCallback(m3d::IDeviceResetCallback*);
            virtual void UnregisterResetCallback(m3d::IDeviceResetCallback*);
            virtual void SetGamma(float, float, float);
            virtual bool IsFeatureSupported(m3d::rend::DeviceFeature);
            virtual bool IsMultiSamplingSupported(int);
            virtual void SetAlphaTest(int);
            virtual void SetStageState(int, m3d::rend::BlendMode, m3d::rend::TextureState);
            virtual float GetMaxPointSize();
            virtual float GetMaxNPatchTessellationLevel();
            virtual int GetMaxVertexShaderConst();
            virtual TexHandle* AddTexture(m3d::rend::TexHandle* result, const CStr*, unsigned int);
            virtual TexHandle* AddDynamicTexture(m3d::rend::TexHandle* result, const char*, int, int, unsigned int);
            virtual TexHandle* AddRenderTargetTexture(m3d::rend::TexHandle* result, const char*, int, int);
            virtual TexHandle* GetBufferedTargetTexture(m3d::rend::TexHandle* result, int);
            virtual TexHandle GetFullFrameFrameBufferTexture();
            virtual int ReloadTextures();
            virtual bool ReportTexturesInfo(const char*);
            virtual int SetTexture(int, const m3d::rend::TexHandle*, long double);
            virtual void SetWhiteTexture(int);
            virtual void SetBlackTexture(int);
            virtual void SetErrorTexture(int);
            virtual void DisableTextureStages(int);
            virtual int ReferenceTexture(const m3d::rend::TexHandle*);
            virtual int ReleaseTexture(m3d::rend::TexHandle&);
            virtual void SetTextureParameter(m3d::rend::TexHandle const&, m3d::rend::TexParam, unsigned int);
            virtual int GetTextureName(const m3d::rend::TexHandle*, CStr*);
            virtual int UploadTexImage(const m3d::rend::TexHandle*, unsigned int, unsigned int, unsigned __int8*, m3d::rend::TexDynFormat, int);
            virtual void* LockTexture(const m3d::rend::TexHandle*, m3d::rend::TexDynFormat, int*, int);
            virtual void UnlockTexture(const m3d::rend::TexHandle*);
            char gap3FC[4];
            virtual int DownloadTexImageRgba8888(unsigned int*, const m3d::rend::TexHandle*, int, int);
            virtual void GetDims(const m3d::rend::TexHandle*, int*, int*);
            virtual void TexCopy(const m3d::rend::TexHandle*, const m3d::rend::TexHandle*);
            virtual void RepaintAllTexturesMips();
            char gap410[4];
            virtual void DrawFullScreenQuad();
            virtual m3d::rend::IbHandle* AddIb(m3d::rend::IbHandle* result, int, bool);
            char gap41C[4];
            virtual void SetIndices(const m3d::rend::IbHandle*, int);
            virtual void* LockIb(const m3d::rend::IbHandle*, int, int, unsigned int);
            virtual void* LockIbStreaming(const m3d::rend::IbHandle*, int, int*, int*);
            virtual void UnlockIb(const m3d::rend::IbHandle*);
            virtual int ReleaseIb(m3d::rend::IbHandle*);
            virtual int ReferenceIb(const m3d::rend::IbHandle*);
            virtual m3d::rend::IbPoolField* AddIbPoolField(m3d::rend::IbPoolField* result, unsigned int);
            virtual void ReleaseIbPoolField(m3d::rend::IbPoolField*);
            virtual void* LockIbPoolField(const m3d::rend::IbPoolField*);
            virtual void UnlockIbPoolField(const m3d::rend::IbPoolField*);
            virtual bool ReportIbsInfo(const char*);
            virtual m3d::rend::VbHandle* AddVb(m3d::rend::VbHandle* result, m3d::rend::VertexType, int, const CStr*, unsigned int);
            char gap450[4];
            virtual void SetToStream0(const m3d::rend::VbHandle*);
            char gap458[4];
            virtual void SetToStream(int, const m3d::rend::VbHandle*);
            virtual void* LockVb(const m3d::rend::VbHandle*, int, int, unsigned int);
            virtual void* LockVbStreaming(const m3d::rend::VbHandle*, int, int*, int*);
            virtual void UnlockVb(const m3d::rend::VbHandle*);
            virtual int ReferenceVb(const m3d::rend::VbHandle*);
            virtual int ReleaseVb(m3d::rend::VbHandle*);
            virtual bool ReportVbsInfo(const char*);
            virtual m3d::rend::VbPoolField* AddVbPoolField(m3d::rend::VbPoolField* result, m3d::rend::VertexType, unsigned int);
            virtual void ReleaseVbPoolField(m3d::rend::VbPoolField*);
            virtual void* LockVbPoolField(const m3d::rend::VbPoolField*);
            virtual void UnlockVbPoolField(const m3d::rend::VbPoolField*);
            virtual m3d::rend::VbHandle* GetVbStreaming(m3d::rend::VbHandle* result, m3d::rend::VertexType);
            virtual int DrawIndexedPrimitive(m3d::rend::PrimType, unsigned int, unsigned int, unsigned int, unsigned int);
            virtual int DrawPrimitive(m3d::rend::PrimType, unsigned int, unsigned int);
            virtual int GetMaxLights();
            virtual void LightEnable(int, int);
            virtual void LightSet(int, const m3d::rend::LightSource*);
            virtual void MaterialSet(const m3d::rend::Material*);
            virtual void RelToAbs(float&, float&);
            virtual void AbsToRel(float&, float&);
            char gap4AC[4];
            virtual m3d::rend::TexHandle* AddTextureFromBackBuffer(m3d::rend::TexHandle* result, int, int);
            virtual void ScreenShot(const char*, int, int);
            virtual int SaveTextureToTgaFile(m3d::rend::TexHandle, const char*);
            virtual char* GetCurBppStr(int*);
            virtual char* GetLastErrorStr();
            virtual unsigned int GetMaxClipPlanes();
            virtual void SetClipPlane(int, const CPlane*);
            virtual void EnableClipPlane(int, bool);
            virtual int GetMaxAnisotropy();
            virtual void ResetStats();
            virtual void GetStats(m3d::rend::RenderStats*);
            virtual void GetDeviceMemStats(m3d::rend::DeviceMemStats*);
            virtual void ShowStats();
            virtual int OptimizeGeometryToSingleStrip(m3d::rend::VertexType, void*, int, unsigned __int16*, int, int, void**, int*, int**, unsigned __int16**, int*);
            virtual int OptimizeGeometryToTriList(m3d::rend::VertexType, void*, int, unsigned __int16*, int, int, void**, int*, int**, unsigned __int16**, int*);
            virtual void* GetInternalData();
            virtual void SetVsFloatConst(unsigned int, const float*, unsigned int);
            virtual void SetVsIntConst(unsigned int, const int*, unsigned int);
            virtual void SetVsBoolConst(unsigned int, const int*, unsigned int);
            virtual void SetPsFloatConst(unsigned int, const float*, unsigned int);
            virtual void SetPsIntConst(unsigned int, const int*, unsigned int);
            virtual void SetPsBoolConst(unsigned int, const int*, unsigned int);
            virtual m3d::rend::IAsmShader* NewAsmShader(const char*, m3d::rend::IAsmShader::Type);
            virtual m3d::rend::IHlslShader* NewHlslShader(const char*, const char*, m3d::rend::IHlslShader::Profile);
            virtual m3d::rend::IEffect* NewEffect(const char*, bool);
            virtual const char* EffectParameterToString(m3d::rend::IEffect::Parameter);
            virtual m3d::rend::IEffect::Parameter StringToEffectParameter(const char*);
            virtual bool ReloadShaders();
            virtual m3d::rend::IQuery* NewQuery(m3d::rend::IQuery::Type);
            virtual void AddChangeShaderMacro(const m3d::rend::ShaderMacro*);
            virtual void DeleteShaderMacro(const CStr*);
            virtual int DrawIndexedPrimitiveEffect(m3d::rend::PrimType, m3d::rend::IEffect*, unsigned int, unsigned int, unsigned int, unsigned int);
            virtual int DrawPrimitiveEffect(m3d::rend::PrimType, m3d::rend::IEffect*, unsigned int, unsigned int);
            virtual int DrawIndexedPrimitiveShader(m3d::rend::PrimType, unsigned int, unsigned int, unsigned int, unsigned int);
            virtual int DrawPrimitiveShader(m3d::rend::PrimType, unsigned int, unsigned int);
            virtual int SetupDXCursor(const m3d::rend::TexHandle*, int, int, int);
            virtual void MoveDXCursor(int, int);
            virtual void ShowDXCursor(bool);
            virtual int UpdateDXCursorFrame();
            virtual bool IsHardWareCursorAvailableForTexture(const m3d::rend::TexHandle*);
            virtual bool IsNV3x();
            virtual void ResetTextureStates();
            virtual void SingleLayerStencilStart();
            virtual void SingleLayerStencilContinue();
            virtual void SingleLayerStencilFinish();
        };
    }
}
