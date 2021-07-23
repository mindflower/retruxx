#pragma once
#include "i_renderer_colors.h"
#include "i_renderer_handles.h"
#include "i_renderer_vertex.h"
#include <iface.h>
#include <core/stringm3d.h>
#include <math/vector.h>

namespace m3d
{
    namespace rend
    {
        class IRenderer : public IBase
        {
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

            class IEffect::TechniqueDesc
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
    }
}
