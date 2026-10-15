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
        ShadowManager(m3d::ShadowManager const&);
        ShadowManager();
        ~ShadowManager();
        void EnableShadows(bool bEnable);
        bool ShadowsEnabled() const;
        void SetShadowColor(unsigned int color);
        unsigned int GetShadowColor() const;
        void BeginScene();
        void AddDirectionalLight(CVector const& lightDir);
        void AddShadowCaster(CMatrix const& objToWorldMat, m3d::AnimatedModel::Mesh const& caster);
        void EndScene();
        void OnChangeScreenResolution();

    private:
        bool m_bIsShadowsEnabled;
        unsigned int m_shadowColor;
        bool m_bInScene;
        CVector m_lightDir;
        m3d::ShadowVolume* m_volumes[100];
        CVector m_objLightDir[100];
        CMatrix m_objToWorldMatrices[100];
        unsigned int m_numVolumes;
        unsigned short* m_edgesBuf;
        CVector* m_volumeVertexPool;
        unsigned int m_volumeVertexPoolSize;
        m3d::rend::VbHandle m_fsQuadVb;
        m3d::Profiler* m_profiler1;
        m3d::Profiler* m_profiler2;
    };
}  // namespace m3d
