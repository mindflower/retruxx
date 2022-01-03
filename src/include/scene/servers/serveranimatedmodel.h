#pragma once
#include "DataServer.h"
#include <skelmodel.h>
#include <engine/scene/servers/meshmaterialmanager.h>

namespace m3d
{
    class DbgCounter;
    class Profiler;
    class ShadowManager;

    namespace rend
    {
        class IEffect;
    }

    class AnimInfo;
    class SgAnimatedModelNode;

    class AnimatedModelsServer : public DataServer
    {
    public:
        virtual void PostLoad();
        virtual int AddItem(char const*, char const*);
        virtual int RemoveItem(int);
        bool IsBonePresentsInModel(char const*, char const*);
        virtual void RenderItem(int, void*);
        virtual void UnregisterNode(SgNode*);
        virtual bool ReportServerInfo(char const*);
        virtual int Init();
        struct CVector GetBoundSizes(char const*);
        int GetBoneMatrixByNameFromModelName(char const*, CStr const&, CMatrix&, bool);
        virtual int SaveAllLoadedEntities(char const*);
        virtual int Release();
        AnimatedModelsServer();
        virtual int SetItemProperty(int, int, void*);
        virtual int RenderNodeSet(SgNode**, unsigned int, RenderNodeInfo);
        virtual void RenderTransparents(SgNode**, unsigned int);
        virtual int RenderShadowVolumesSet(SgNode**, unsigned int);
        virtual ~AnimatedModelsServer();
        virtual int GenerateImpostorsIfNeeded();
        virtual int GetItemProperty(int, int, void*);
        virtual void RegisterNode(SgNode*);
        virtual void UpdateItem(int, void*);

    protected:
        virtual void AddItemsList(std::vector<ServerItem>&);

    private:
        int RenderMesh(SgAnimatedModelNode*, AnimatedModel::Mesh&, rend::IEffect*);
        int RenderMesh(AnimInfo*, AnimatedModel::Mesh&, rend::IEffect*);
        void RenderModelForImpostor(AnimatedModel*, float, int, int);
        void UpdateGlobalRenderingParams();

    private:
        CVector m_colorAmbient;
        CVector m_colorDiffuse;
        CVector m_colorPlant;
        CVector m_fogTerm;
        CVector m_treeBendTerm;
        CVector m_colorSpecular;
        bool m_tessellate;
        bool m_alreadyCached;
        m3d::MeshMaterialManager m_MeshMaterialManager;
        m3d::ShadowManager* m_ShadowMan;
        int m_numShadowingNodes;
        m3d::SgNode* m_shadowingNodes[100];
        m3d::Profiler* m_profiler;
        m3d::DbgCounter* m_countNodes;
        m3d::DbgCounter* m_countMeshes;
        bool m_globalFxParamAmbientNotActuated;
        bool m_globalFxParamDiffuseNotActuated;
        bool m_globalFxParamPlantAmbientNotActuated;
        bool m_globalFxParamFogNotActuated;
        bool m_globalFxParamFrameStartTimeNotActuated;
        bool m_globalFxParamTreeBendTermNotActuated;
        bool m_globalFxParamSpecularNotActuated;
        m3d::rend::IHlslShader* m_impostorVs;
        m3d::rend::IHlslShader* m_impostorPs;
    };
}
