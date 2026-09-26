#pragma once
#include <math/matrix.h>
#include <math/vector.h>
#include <renderer/i_renderer.h>
#include <skelmodel.h>

namespace m3d
{
    class Profiler;
    class ShadowVolume;

    // Stencil shadows cast by animated models under the sun. The volumes of up to 100 meshes are built from the
    // casters added between BeginScene and EndScene, counted into the stencil buffer, and the shadowed pixels are
    // then darkened by a full-screen quad.
    class ShadowManager
    {
    public:
        ShadowManager(const m3d::ShadowManager&);
        ShadowManager();
        ~ShadowManager();
        void EnableShadows(bool bEnable);
        bool ShadowsEnabled() const;
        void SetShadowColor(unsigned int color);
        unsigned int GetShadowColor() const;
        void BeginScene();
        void AddDirectionalLight(const CVector& lightDir);
        void AddShadowCaster(const CMatrix& objToWorldMat, const m3d::AnimatedModel::Mesh& caster);
        void EndScene();
        void OnChangeScreenResolution();

    private:
        /* 0x0000 */ bool m_bIsShadowsEnabled;
        /* 0x0001 */ char Padding_215[3];
        /* 0x0004 */ unsigned int m_shadowColor;
        /* 0x0008 */ bool m_bInScene;
        /* 0x0009 */ char Padding_216[3];
        /* 0x000c */ CVector m_lightDir;
        /* 0x0018 */ m3d::ShadowVolume* m_volumes[100];
        /* 0x01a8 */ CVector m_objLightDir[100];
        /* 0x0658 */ CMatrix m_objToWorldMatrices[100];
        /* 0x1f58 */ unsigned int m_numVolumes;
        /* 0x1f5c */ unsigned short* m_edgesBuf;
        /* 0x1f60 */ CVector* m_volumeVertexPool;
        /* 0x1f64 */ unsigned int m_volumeVertexPoolSize;
        /* 0x1f68 */ m3d::rend::VbHandle m_fsQuadVb;
        /* 0x1f6c */ m3d::Profiler* m_profiler1;
        /* 0x1f70 */ m3d::Profiler* m_profiler2;
    }; /* size: 0x1f74 */

    static_assert(sizeof(ShadowManager) == 0x1f74);
}
