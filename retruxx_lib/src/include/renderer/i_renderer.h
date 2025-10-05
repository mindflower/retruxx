#pragma once
#include "i_renderer_colors.h"
#include "i_renderer_handles.h"
#include "i_renderer_vertex.h"
#include <iface.h>
#include <core/stringm3d.h>
#include <math/vector.h>
#include <Windows.h>

#include "i_renderer_query.h"

class CVector2;
class CPlane;
class CMatrix;

namespace m3d
{
    class Kernel;
}

struct nFloat4
{
    float x;
    float y;
    float z;
    float w;
};

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

        struct Material
        {
            m3d::rend::Colorf m_diffuse;
            m3d::rend::Colorf m_ambient;
            m3d::rend::Colorf m_specular;
            m3d::rend::Colorf m_emissive;
            /* 0x0040 */ float m_specularPower;

            void init(const m3d::rend::Colorf& diff);
        }; /* size: 0x0044 */

        class TexHandle : public Handle<TexHandle>
        {
            
        };

        class IbHandle : public Handle<IbHandle>
        {
        };

        class VbHandle : public Handle<VbHandle>
        {
        };

        struct VbPoolField
        {
            unsigned int GetOffset() const;
            unsigned int GetSize() const;
            m3d::rend::VertexType GetVertexType() const;
            m3d::rend::VbHandle GetVbHandle() const;
            /* 0x0000 */ unsigned int Offset;
            /* 0x0004 */ unsigned int Size;
            /* 0x0008 */ unsigned int RealOffset;
            /* 0x000c */ m3d::rend::VertexType VertType;
            m3d::rend::VbHandle Vb;
        }; /* size: 0x0014 */

        struct IbPoolField
        {
            unsigned int GetOffset() const;
            unsigned int GetSize() const;
            m3d::rend::IbHandle GetIbHandle() const;
            /* 0x0000 */ unsigned int Offset;
            /* 0x0004 */ unsigned int Size;
            /* 0x0008 */ unsigned int RealOffset;
            m3d::rend::IbHandle Ib;
        }; /* size: 0x0010 */

        enum LightType
        {
            M3DLIGHT_POINT = 0x0,
            M3DLIGHT_SPOT = 0x1,
            M3DLIGHT_DIRECTIONAL = 0x2,
        };

        struct LightSource
        {
            /* 0x0000 */ m3d::rend::LightType m_type;
            m3d::rend::Colorf m_diffuse;
            m3d::rend::Colorf m_specular;
            m3d::rend::Colorf m_ambient;
            CVector m_origin;
            CVector m_direction;
            /* 0x004c */ float m_range;
            /* 0x0050 */ float m_falloff;
            /* 0x0054 */ float m_attenuation0;
            /* 0x0058 */ float m_attenuation1;
            /* 0x005c */ float m_attenuation2;
            /* 0x0060 */ float m_theta;
            /* 0x0064 */ float m_phi;
            void init(m3d::rend::LightType type, const CVector& pos);
        }; /* size: 0x0068 */

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

        struct ShaderMacro
        {
            CStr name;
            CStr definition;
            ShaderMacro(const m3d::rend::ShaderMacro&);
            ShaderMacro(const char* n, const char* d);
            ShaderMacro();
        }; /* size: 0x0018 */

        struct IRenderResource
        {
            /* 0x0000 */;
            IRenderResource(const m3d::rend::IRenderResource&);
            IRenderResource();
            virtual int AddRef() /* 0x00 */;
            virtual int Release() /* 0x04 */;
            virtual int GetRefCount() /* 0x08 */;
            virtual bool IsValid() const = 0 /* 0x0c */;
            /* 0x0004 */ int m_refCount;
            virtual  ~IRenderResource() /* 0x10 */;
        }; /* size: 0x0008 */

        class IQuery : public m3d::rend::IRenderResource
        {
        public:
            enum Type
            {
                QUERY_VCACHE = 4,
                QUERY_EVENT = 8,
                QUERY_OCCLUSION = 9,
                QUERY_TIMESTAMP = 10,
                QUERY_TIMESTAMPDISJOINT = 11,
                QUERY_TIMESTAMPFREQ = 12,
                QUERY_PIPELINETIMINGS = 13,
                QUERY_INTERFACETIMINGS = 14,
                QUERY_VERTEXTIMINGS = 15,
                QUERY_BANDWIDTHTIMINGS = 17,
                QUERY_CACHEUTILIZATION = 18,
            };

            enum State
            {
                QUERY_NOT_SUPPORT = 0,
                QUERY_SIGNALED = 1,
                QUERY_ISSUED = 2,
                QUERY_ERROR = 3,
            };

        public:
            virtual m3d::rend::IQuery::Type GetType() const = 0 /* 0x14 */;
            virtual void Begin() = 0 /* 0x18 */;
            virtual void End() = 0 /* 0x1c */;
            virtual m3d::rend::IQuery::State GetState() = 0 /* 0x20 */;
            virtual const m3d::rend::QueryReturnValue& GetData() = 0 /* 0x24 */;

        protected:
            virtual  ~IQuery() override /* 0x00 */;
        }; /* size: 0x0008 */

        struct IHlslShader : public m3d::rend::IRenderResource
        {
            enum Profile
            {
                VS_1_1 = 0,
                VS_2_0 = 1,
                VS_3_0 = 2,
                PS_1_1 = 3,
                PS_1_3 = 4,
                PS_1_4 = 5,
                PS_2_0 = 6,
                PS_2_a = 7,
                PS_3_0 = 8,
            };

            using ParameterHandle = unsigned int;

            static const unsigned int INVALID_PARAM;
            virtual unsigned int GetNumberOfParams() const = 0 /* 0x14 */;
            virtual unsigned int GetParamHandleByName(const char*) = 0 /* 0x18 */;
            virtual void SetInt(unsigned int, int) = 0 /* 0x1c */;
            virtual void SetFloat(unsigned int, float) = 0 /* 0x20 */;
            virtual void SetVector4(unsigned int, const CVector4&) = 0 /* 0x24 */;
            virtual void SetVector3(unsigned int, const CVector&) = 0 /* 0x28 */;
            virtual void SetFloat4(unsigned int, const nFloat4&) = 0 /* 0x2c */;
            virtual void SetMatrix(unsigned int, const CMatrix&) = 0 /* 0x30 */;
            virtual void SetIntArray(unsigned int, const int*, int) = 0 /* 0x34 */;
            virtual void SetFloatArray(unsigned int, const float*, int) = 0 /* 0x38 */;
            virtual void SetFloat4Array(unsigned int, const nFloat4*, int) = 0 /* 0x3c */;
            virtual void SetVector4Array(unsigned int, const CVector4*, int) = 0 /* 0x40 */;
            virtual void SetMatrixArray(unsigned int, const CMatrix*, int) = 0 /* 0x44 */;
            virtual void SetMatrixPointerArray(unsigned int, const CMatrix**, int) = 0 /* 0x48 */;
            virtual void Apply() = 0 /* 0x4c */;
            virtual  ~IHlslShader() override /* 0x00 */;
        }; /* size: 0x0008 */

        struct IAsmShader : public m3d::rend::IRenderResource
        {
            enum Type
            {
                VERTEX_SHADER = 0,
                PIXEL_SHADER = 1,
            };

            virtual void Apply() = 0 /* 0x14 */;
            virtual  ~IAsmShader() override /* 0x00 */;
        }; /* size: 0x0008 */

        struct IEffect : public m3d::rend::IRenderResource
        {
            enum Parameter
            {
                World = 0,
                View = 1,
                Projection = 2,
                ModelView = 3,
                InvWorld = 4,
                ModelViewProjection = 5,
                ViewPos = 6,
                DiffMap0 = 7,
                CubeMap0 = 8,
                BumpMap0 = 9,
                DetailMap0 = 10,
                LightMap0 = 11,
                NormalizationCubemap = 12,
                Time_Linear = 13,
                Tree_Bend_Term = 14,
                LightAmbient = 15,
                LightDiffuse = 16,
                LightPlant = 17,
                LightSpecular = 18,
                FogTerm = 19,
                Transparency = 20,
                TransStartDist = 21,
                TransObjectWidth = 22,
                TmpLight0Dir = 23,
                User_float_param = 24,
                User_float_param2 = 25,
                User_float_param3 = 26,
                User_float3_param = 27,
                User_float3_param2 = 28,
                User_float4_param = 29,
                User_float4x4_param = 30,
                NumParameters = 31,
                InvalidParameter = 32,
            };

            struct TechniqueDesc
            {
                CStr name;
                /* 0x000c */ unsigned int numPasses;
                CStr briefDesc;
                CStr vertexFormatStr;
                /* 0x0028 */ m3d::rend::VertexType vertexFormat;
                /* 0x002c */ bool tangentSpaceUsed;
                /* 0x002d */ bool isDefault;
                /* 0x002e */ bool isPS20;
                /* 0x002f */ bool useAlpha;
            }; /* size: 0x0030 */

            virtual unsigned int GetNumTechniques() const = 0 /* 0x14 */;
            virtual const m3d::rend::IEffect::TechniqueDesc& GetTechniqueDesc(unsigned int) const = 0 /* 0x18 */;
            virtual void SetCurTechnique(unsigned int) = 0 /* 0x1c */;
            virtual unsigned int GetCurTechnique() const = 0 /* 0x20 */;
            virtual void SetDefaultTechnique(bool) = 0 /* 0x24 */;
            virtual bool IsParameterUsed(m3d::rend::IEffect::Parameter) = 0 /* 0x28 */;
            virtual void SetInt(m3d::rend::IEffect::Parameter, int) = 0 /* 0x2c */;
            virtual void SetFloat(m3d::rend::IEffect::Parameter, float) = 0 /* 0x30 */;
            virtual void SetVector4(m3d::rend::IEffect::Parameter, const CVector4&) = 0 /* 0x34 */;
            virtual void SetVector3(m3d::rend::IEffect::Parameter, const CVector&) = 0 /* 0x38 */;
            virtual void SetFloat4(m3d::rend::IEffect::Parameter, const nFloat4&) = 0 /* 0x3c */;
            virtual void SetMatrix(m3d::rend::IEffect::Parameter, const CMatrix&) = 0 /* 0x40 */;
            virtual void SetTexture(m3d::rend::IEffect::Parameter, m3d::rend::TexHandle*) = 0 /* 0x44 */;
            virtual void SetIntArray(m3d::rend::IEffect::Parameter, const int*, int) = 0 /* 0x48 */;
            virtual void SetFloatArray(m3d::rend::IEffect::Parameter, const float*, int) = 0 /* 0x4c */;
            virtual void SetFloat4Array(m3d::rend::IEffect::Parameter, const nFloat4*, int) = 0 /* 0x50 */;
            virtual void SetVector4Array(m3d::rend::IEffect::Parameter, const CVector4*, int) = 0 /* 0x54 */;
            virtual void SetMatrixArray(m3d::rend::IEffect::Parameter, const CMatrix*, int) = 0 /* 0x58 */;
            virtual void SetMatrixPointerArray(m3d::rend::IEffect::Parameter, const CMatrix**, int) = 0 /* 0x5c */;
            virtual  ~IEffect() override /* 0x00 */;
        }; /* size: 0x0008 */

        //IMPORTANT: fields and member order is strict!
        struct IRenderer : public IBase
        {
            virtual int Create(void __fastcall(CStr const&), m3d::Kernel*) = 0;
            virtual int CreateDevice() = 0;
            virtual int SwitchDisplayModes(HWND, int, int, int) = 0;
            virtual int Reset() = 0;
            virtual void PushBlend() = 0;
            virtual void PushBlend(m3d::rend::BlendMode) = 0;
            virtual void PopBlend() = 0;
            virtual void SetBlend(m3d::rend::BlendMode, bool) = 0;
            virtual void PushZbState() = 0;
            virtual void PushZbState(m3d::rend::ZbState) = 0;
            virtual void PopZbState() = 0;
            virtual void SetZbState(m3d::rend::ZbState, bool) = 0;
            virtual void PushCull() = 0;
            virtual void PushCull(m3d::rend::Cull) = 0;
            virtual void PopCull() = 0;
            virtual void SetCull(m3d::rend::Cull, bool) = 0;
            virtual void PushZFunc() = 0;
            virtual void PushZFunc(m3d::rend::CmpFunc) = 0;
            virtual void PopZFunc() = 0;
            virtual void SetZFunc(m3d::rend::CmpFunc, bool) = 0;
            virtual void PushLighting() = 0;
            virtual void PushLighting(bool) = 0;
            virtual void PopLighting() = 0;
            virtual void SetLighting(bool, bool) = 0;
            virtual void PushAmbient(unsigned int) = 0;
            virtual void PushAmbient() = 0;
            virtual void PopAmbient() = 0;
            virtual void SetAmbient(unsigned int, bool) = 0;
            virtual void PushFog() = 0;
            virtual void PushFog(bool) = 0;
            virtual void PopFog() = 0;
            virtual void SetFog(bool, bool) = 0;
            virtual void PushFogColor(unsigned int) = 0;
            virtual void PushFogColor() = 0;
            virtual void PopFogColor() = 0;
            virtual void SetFogColor(unsigned int, bool) = 0;
            virtual void PushFogMode(m3d::rend::FogMode) = 0;
            virtual void PushFogMode() = 0;
            virtual void PopFogMode() = 0;
            virtual void SetFogMode(m3d::rend::FogMode, bool) = 0;
            virtual void PushFogStart(float) = 0;
            virtual void PushFogStart() = 0;
            virtual void PopFogStart() = 0;
            virtual void SetFogStart(float, bool) = 0;
            virtual void PushFogEnd(float) = 0;
            virtual void PushFogEnd() = 0;
            virtual void PopFogEnd() = 0;
            virtual void SetFogEnd(float, bool) = 0;
            virtual void PushFillMode() = 0;
            virtual void PushFillMode(m3d::rend::FillMode) = 0;
            virtual void PopFillMode() = 0;
            virtual void SetFillMode(m3d::rend::FillMode, bool) = 0;
            virtual void PushZBias(float) = 0;
            virtual void PushZBias() = 0;
            virtual void PopZBias() = 0;
            virtual void SetZBias(float, bool) = 0;
            virtual void PushZBiasSlopeScale(float) = 0;
            virtual void PushZBiasSlopeScale() = 0;
            virtual void PopZBiasSlopeScale() = 0;
            virtual void SetZBiasSlopeScale(float, bool) = 0;
            virtual void PushShadeMode(m3d::rend::ShadeMode) = 0;
            virtual void PushShadeMode() = 0;
            virtual void PopShadeMode() = 0;
            virtual void SetShadeMode(m3d::rend::ShadeMode, bool) = 0;
            virtual void PushPointSpriteEnable(int) = 0;
            virtual void PushPointSpriteEnable() = 0;
            virtual void PopPointSpriteEnable() = 0;
            virtual void SetPointSpriteEnable(int, bool) = 0;
            virtual void PushPointScaleEnable(int) = 0;
            virtual void PushPointScaleEnable() = 0;
            virtual void PopPointScaleEnable() = 0;
            virtual void SetPointScaleEnable(int, bool) = 0;
            virtual void PushPointSizeMin(float) = 0;
            virtual void PushPointSizeMin() = 0;
            virtual void PopPointSizeMin() = 0;
            virtual void SetPointSizeMin(float, bool) = 0;
            virtual void PushPointSizeMax(float) = 0;
            virtual void PushPointSizeMax() = 0;
            virtual void PopPointSizeMax() = 0;
            virtual void SetPointSizeMax(float, bool) = 0;
            virtual void PushPointSize(float) = 0;
            virtual void PushPointSize() = 0;
            virtual void PopPointSize() = 0;
            virtual void SetPointSize(float, bool) = 0;
            virtual void PushPointScaleA(float) = 0;
            virtual void PushPointScaleA() = 0;
            virtual void PopPointScaleA() = 0;
            virtual void SetPointScaleA(float, bool) = 0;
            virtual void PushPointScaleB(float) = 0;
            virtual void PushPointScaleB() = 0;
            virtual void PopPointScaleB() = 0;
            virtual void SetPointScaleB(float, bool) = 0;
            virtual void PushPointScaleC(float) = 0;
            virtual void PushPointScaleC() = 0;
            virtual void PopPointScaleC() = 0;
            virtual void SetPointScaleC(float, bool) = 0;
            virtual void PushTFactor(unsigned int) = 0;
            virtual void PushTFactor() = 0;
            virtual void PopTFactor() = 0;
            virtual void SetTFactor(unsigned int, bool) = 0;
            virtual void PushLocalViewer(bool) = 0;
            virtual void PushLocalViewer() = 0;
            virtual void PopLocalViewer() = 0;
            virtual void SetLocalViewer(bool, bool) = 0;
            virtual void PushSpecularLighting(bool) = 0;
            virtual void PushSpecularLighting() = 0;
            virtual void PopSpecularLighting() = 0;
            virtual void SetSpecularLighting(bool, bool) = 0;
            virtual void PushColorWriteMask(unsigned int) = 0;
            virtual void PushColorWriteMask() = 0;
            virtual void PopColorWriteMask() = 0;
            virtual void SetColorWriteMask(unsigned int, bool) = 0;
            virtual void PushNPatchLevel(float) = 0;
            virtual void PushNPatchLevel() = 0;
            virtual void PopNPatchLevel() = 0;
            virtual void SetNPatchLevel(float, bool) = 0;
            virtual void PushStencilState(bool) = 0;
            virtual void PushStencilState() = 0;
            virtual void PopStencilState() = 0;
            virtual void SetStencilState(bool, bool) = 0;
            virtual void PushStencilMask(unsigned int) = 0;
            virtual void PushStencilMask() = 0;
            virtual void PopStencilMask() = 0;
            virtual void SetStencilMask(unsigned int, bool) = 0;
            virtual void PushStencilRef(unsigned int) = 0;
            virtual void PushStencilRef() = 0;
            virtual void PopStencilRef() = 0;
            virtual void SetStencilRef(unsigned int, bool) = 0;
            virtual void PushStencilWriteMask(unsigned int) = 0;
            virtual void PushStencilWriteMask() = 0;
            virtual void PopStencilWriteMask() = 0;
            virtual void SetStencilWriteMask(unsigned int, bool) = 0;
            virtual void PushStencilFunc(m3d::rend::CmpFunc) = 0;
            virtual void PushStencilFunc() = 0;
            virtual void PopStencilFunc() = 0;
            virtual void SetStencilFunc(m3d::rend::CmpFunc, bool) = 0;
            virtual void PushStencilFail(m3d::rend::StencilOp) = 0;
            virtual void PushStencilFail() = 0;
            virtual void PopStencilFail() = 0;
            virtual void SetStencilFail(m3d::rend::StencilOp, bool) = 0;
            virtual void PushStencilZFail(m3d::rend::StencilOp) = 0;
            virtual void PushStencilZFail() = 0;
            virtual void PopStencilZFail() = 0;
            virtual void SetStencilZFail(m3d::rend::StencilOp, bool) = 0;
            virtual void PushStencilPass(m3d::rend::StencilOp) = 0;
            virtual void PushStencilPass() = 0;
            virtual void PopStencilPass() = 0;
            virtual void SetStencilPass(m3d::rend::StencilOp, bool) = 0;
            virtual void PushStencil2SidedEnable(bool) = 0;
            virtual void PushStencil2SidedEnable() = 0;
            virtual void PopStencil2SidedEnable() = 0;
            virtual void SetStencil2SidedEnable(bool, bool) = 0;
            virtual void PushStencilCcwFunc(m3d::rend::CmpFunc) = 0;
            virtual void PushStencilCcwFunc() = 0;
            virtual void PopStencilCcwFunc() = 0;
            virtual void SetStencilCcwFunc(m3d::rend::CmpFunc, bool) = 0;
            virtual void PushStencilCcwFail(m3d::rend::StencilOp) = 0;
            virtual void PushStencilCcwFail() = 0;
            virtual void PopStencilCcwFail() = 0;
            virtual void SetStencilCcwFail(m3d::rend::StencilOp, bool) = 0;
            virtual void PushStencilCcwZFail(m3d::rend::StencilOp) = 0;
            virtual void PushStencilCcwZFail() = 0;
            virtual void PopStencilCcwZFail() = 0;
            virtual void SetStencilCcwZFail(m3d::rend::StencilOp, bool) = 0;
            virtual void PushStencilCcwPass(m3d::rend::StencilOp) = 0;
            virtual void PushStencilCcwPass() = 0;
            virtual void PopStencilCcwPass() = 0;
            virtual void SetStencilCcwPass(m3d::rend::StencilOp, bool) = 0;
            virtual void PushMultiSample() = 0;
            virtual void PushMultiSample(bool) = 0;
            virtual void PopMultiSample() = 0;
            virtual void SetMultiSample(bool, bool) = 0;
            virtual void PushMultiSampleMask(unsigned int) = 0;
            virtual void PushMultiSampleMask() = 0;
            virtual void PopMultiSampleMask() = 0;
            virtual void SetMultiSampleMask(unsigned int, bool) = 0;
            virtual void SetViewMatrix(const CMatrix&) = 0;
            virtual const CMatrix& GetViewMatrix() = 0;
            virtual const CVector& GetViewOrigin() = 0;
            virtual void MatPush() = 0;
            virtual void MatPush(const CMatrix&) = 0;
            virtual void MatPop(bool) = 0;
            virtual void MatMul(const CMatrix*) = 0;
            virtual void MatMulR(const CMatrix*) = 0;
            virtual const CMatrix& MatGet() = 0;
            virtual const CMatrix& MatGetInv() = 0;
            virtual void MatSet(const CMatrix&) = 0;
            virtual void MatGetBasis(CVector&, CVector&, CVector&) const = 0;
            virtual CVector MatGetOrgInv() = 0;
            virtual CVector MatGetOrg() = 0;
            virtual void MatSetWorld(const CMatrix&) = 0;
            virtual const CMatrix* MatGetWorld() = 0;
            virtual void MatPushWorld() = 0;
            virtual void MatPopWorld() = 0;
            virtual void MatSetProj(const CMatrix&) = 0;
            virtual const CMatrix& MatGetProj() = 0;
            virtual void MatPushProj() = 0;
            virtual void MatPopProj() = 0;
            virtual const CMatrix* GetModelViewProjMatrix() = 0;
            virtual CVector* Unproject(CVector* result, const CVector2*) = 0;
            virtual CVector* Project(CVector* result, const CVector*) = 0;
            virtual CVector* ProjectWorldAbs(CVector* result, const CVector*) = 0;
            virtual void TgEnableSetLinearSt(int, float, float, float, float, float, bool, float, float, float, float) = 0;
            virtual void TgEnableSetMatrixSt(int, const CMatrix*, bool) = 0;
            virtual void TgEnableSetMatrixStr(int, const CMatrix*, bool) = 0;
            virtual void TgSetTransformMode(int, m3d::rend::TgMode) = 0;
            virtual void TgSetTcSource(int, m3d::rend::TcSource, int) = 0;
            virtual void TgDisable(int) = 0;
            virtual void SetTextureMatrix(int, const CMatrix*) = 0;
            virtual void Set2x2BumpMatrix(int, float, float, float, float) = 0;
            virtual int RenderToTexStart(const m3d::rend::TexHandle&, bool) = 0;
            virtual void RenderToTexFinish() = 0;
            virtual void CopyRenderTargetToTexture(const m3d::rend::TexHandle*) = 0;
            virtual int CanRender() = 0;
            virtual int SetActiveState(int) = 0;
            virtual int BeginScene() = 0;
            virtual int EndScene() = 0;
            virtual int InScene() = 0;
            virtual int PresentScene() = 0;
            virtual void ClearViewport(m3d::rend::ClearFlags, unsigned int) = 0;
            virtual rend::Viewport GetViewport() = 0;
            virtual int SetViewport(const m3d::rend::Viewport&) = 0;
            virtual void RegisterResetCallback(m3d::IDeviceResetCallback*) = 0;
            virtual void UnregisterResetCallback(m3d::IDeviceResetCallback*) = 0;
            virtual void SetGamma(float, float, float) = 0;
            virtual bool IsFeatureSupported(m3d::rend::DeviceFeature) = 0;
            virtual bool IsMultiSamplingSupported(int) = 0;
            virtual void SetAlphaTest(int) = 0;
            virtual void SetStageState(int, m3d::rend::BlendMode, m3d::rend::TextureState) = 0;
            virtual float GetMaxPointSize() = 0;
            virtual float GetMaxNPatchTessellationLevel() = 0;
            virtual int GetMaxVertexShaderConst() = 0;
            virtual TexHandle AddTexture(CStr const&, unsigned int) = 0;
            virtual TexHandle AddDynamicTexture(const char*, int, int, unsigned int) = 0;
            virtual TexHandle AddRenderTargetTexture(const char*, int, int) = 0;
            virtual TexHandle GetBufferedTargetTexture(int) = 0;
            virtual TexHandle GetFullFrameFrameBufferTexture() = 0;
            virtual int ReloadTextures() = 0;
            virtual bool ReportTexturesInfo(const char*) = 0;
            virtual int SetTexture(int, const m3d::rend::TexHandle&, long double) = 0;
            virtual void SetWhiteTexture(int) = 0;
            virtual void SetBlackTexture(int) = 0;
            virtual void SetErrorTexture(int) = 0;
            virtual void DisableTextureStages(int) = 0;
            virtual int ReferenceTexture(m3d::rend::TexHandle const&) = 0;
            virtual int ReleaseTexture(m3d::rend::TexHandle&) = 0;
            virtual void SetTextureParameter(m3d::rend::TexHandle const&, m3d::rend::TexParam, unsigned int) = 0;
            virtual int GetTextureName(const m3d::rend::TexHandle*, CStr*) = 0;
            virtual int UploadTexImage(const m3d::rend::TexHandle&, unsigned int, unsigned int, unsigned __int8*, m3d::rend::TexDynFormat, int) = 0;
            virtual void* LockTexture(const m3d::rend::TexHandle&, m3d::rend::TexDynFormat, int&, int) = 0;
            virtual void UnlockTexture(const m3d::rend::TexHandle&) = 0;
            virtual int DownloadTexImageRgba8888(unsigned int*, const m3d::rend::TexHandle*, int, int) = 0;
            virtual int DownloadTexImageRgba8888(unsigned int*, const m3d::rend::TexHandle&) = 0;
            virtual void GetDims(m3d::rend::TexHandle const&, int&, int&) = 0;
            virtual void TexCopy(const m3d::rend::TexHandle&, const m3d::rend::TexHandle&) = 0;
            virtual void RepaintAllTexturesMips() = 0;
            virtual void DrawFullScreenQuad(m3d::rend::IEffect*) = 0;
            virtual void DrawFullScreenQuad() = 0;
            virtual m3d::rend::IbHandle AddIb(int, bool) = 0;
            virtual void SetIndices(const m3d::rend::IbHandle&, int) = 0;
            virtual void SetIndices(const m3d::rend::IbPoolField&, int) = 0;
            virtual void* LockIb(const m3d::rend::IbHandle&, int, int, unsigned int) = 0;
            virtual void* LockIbStreaming(const m3d::rend::IbHandle*, int, int*, int*) = 0;
            virtual void UnlockIb(const m3d::rend::IbHandle&) = 0;
            virtual int ReleaseIb(m3d::rend::IbHandle&) = 0;
            virtual int ReferenceIb(const m3d::rend::IbHandle*) = 0;
            virtual m3d::rend::IbPoolField AddIbPoolField(unsigned int) = 0;
            virtual void ReleaseIbPoolField(m3d::rend::IbPoolField*) = 0;
            virtual void* LockIbPoolField(m3d::rend::IbPoolField const&) = 0;
            virtual void UnlockIbPoolField(m3d::rend::IbPoolField const&) = 0;
            virtual bool ReportIbsInfo(const char*) = 0;
            virtual m3d::rend::VbHandle AddVb(m3d::rend::VertexType, int, CStr const&, unsigned int) = 0;
            virtual void SetToStream0(const m3d::rend::VbHandle&) = 0;
            virtual void SetToStream0(const m3d::rend::VbPoolField&) = 0;
            virtual void SetToStream(int, const m3d::rend::VbHandle*) = 0;
            virtual void SetToStream(int, const m3d::rend::VbPoolField*) = 0;
            virtual void* LockVb(const m3d::rend::VbHandle&, int, int, unsigned int) = 0;
            virtual void* LockVbStreaming(const m3d::rend::VbHandle&, int, int&, int*) = 0;
            virtual void UnlockVb(const m3d::rend::VbHandle&) = 0;
            virtual int ReferenceVb(const m3d::rend::VbHandle*) = 0;
            virtual int ReleaseVb(m3d::rend::VbHandle&) = 0;
            virtual bool ReportVbsInfo(const char*) = 0;
            virtual m3d::rend::VbPoolField AddVbPoolField(m3d::rend::VertexType, unsigned int) = 0;
            virtual void ReleaseVbPoolField(m3d::rend::VbPoolField*) = 0;
            virtual void* LockVbPoolField(const m3d::rend::VbPoolField&) = 0;
            virtual void UnlockVbPoolField(const m3d::rend::VbPoolField&) = 0;
            virtual m3d::rend::VbHandle GetVbStreaming(m3d::rend::VertexType) = 0;
            virtual int DrawIndexedPrimitive(m3d::rend::PrimType, unsigned int, unsigned int, unsigned int, unsigned int) = 0;
            virtual int DrawPrimitive(m3d::rend::PrimType, unsigned int, unsigned int) = 0;
            virtual int GetMaxLights() = 0;
            virtual void LightEnable(int, int) = 0;
            virtual void LightSet(int, const m3d::rend::LightSource&) = 0;
            virtual void MaterialSet(const m3d::rend::Material&) = 0;
            virtual void RelToAbs(float&, float&) = 0;
            virtual void AbsToRel(float&, float&) = 0;
            virtual unsigned int AddTextureFromBackBuffer(m3d::rend::TexHandle result) = 0;
            virtual m3d::rend::TexHandle AddTextureFromBackBuffer(int, int) = 0;
            virtual void ScreenShot(const char*, int, int) = 0;
            virtual int SaveTextureToTgaFile(m3d::rend::TexHandle, const char*) = 0;
            virtual char* GetCurBppStr(int*) = 0;
            virtual char* GetLastErrorStr() = 0;
            virtual unsigned int GetMaxClipPlanes() = 0;
            virtual void SetClipPlane(int, const CPlane*) = 0;
            virtual void EnableClipPlane(int, bool) = 0;
            virtual int GetMaxAnisotropy() = 0;
            virtual void ResetStats() = 0;
            virtual void GetStats(m3d::rend::RenderStats*) = 0;
            virtual void GetDeviceMemStats(m3d::rend::DeviceMemStats*) = 0;
            virtual void ShowStats() = 0;
            virtual int OptimizeGeometryToSingleStrip(m3d::rend::VertexType, void*, int, unsigned __int16*, int, int, void**, int*, int**, unsigned __int16**, int*) = 0;
            virtual int OptimizeGeometryToTriList(m3d::rend::VertexType, void*, int, unsigned __int16*, int, int, void**, int*, int**, unsigned __int16**, int*) = 0;
            virtual void* GetInternalData() = 0;
            virtual void SetVsFloatConst(unsigned int, const float*, unsigned int) = 0;
            virtual void SetVsIntConst(unsigned int, const int*, unsigned int) = 0;
            virtual void SetVsBoolConst(unsigned int, const int*, unsigned int) = 0;
            virtual void SetPsFloatConst(unsigned int, const float*, unsigned int) = 0;
            virtual void SetPsIntConst(unsigned int, const int*, unsigned int) = 0;
            virtual void SetPsBoolConst(unsigned int, const int*, unsigned int) = 0;
            virtual m3d::rend::IAsmShader* NewAsmShader(const char*, m3d::rend::IAsmShader::Type) = 0;
            virtual m3d::rend::IHlslShader* NewHlslShader(const char*, const char*, m3d::rend::IHlslShader::Profile) = 0;
            virtual m3d::rend::IEffect* NewEffect(const char*, bool) = 0;
            virtual const char* EffectParameterToString(m3d::rend::IEffect::Parameter) = 0;
            virtual m3d::rend::IEffect::Parameter StringToEffectParameter(const char*) = 0;
            virtual bool ReloadShaders() = 0;
            virtual m3d::rend::IQuery* NewQuery(m3d::rend::IQuery::Type) = 0;
            virtual void AddChangeShaderMacro(const m3d::rend::ShaderMacro*) = 0;
            virtual void DeleteShaderMacro(const CStr*) = 0;
            virtual int DrawIndexedPrimitiveEffect(m3d::rend::PrimType, m3d::rend::IEffect*, unsigned int, unsigned int, unsigned int, unsigned int) = 0;
            virtual int DrawPrimitiveEffect(m3d::rend::PrimType, m3d::rend::IEffect*, unsigned int, unsigned int) = 0;
            virtual int DrawIndexedPrimitiveShader(m3d::rend::PrimType, unsigned int, unsigned int, unsigned int, unsigned int) = 0;
            virtual int DrawPrimitiveShader(m3d::rend::PrimType, unsigned int, unsigned int) = 0;
            virtual int SetupDXCursor(m3d::rend::TexHandle const&, int, int, int) = 0;
            virtual void MoveDXCursor(int, int) = 0;
            virtual void ShowDXCursor(bool) = 0;
            virtual int UpdateDXCursorFrame() = 0;
            virtual bool IsHardWareCursorAvailableForTexture(const m3d::rend::TexHandle*) = 0;
            virtual bool IsNV3x() = 0;
            virtual void ResetTextureStates() = 0;
            virtual void SingleLayerStencilStart() = 0;
            virtual void SingleLayerStencilContinue() = 0;
            virtual void SingleLayerStencilFinish() = 0;
            IRenderer(IRenderer const&) {}
            IRenderer() {}
        };
    }
}
