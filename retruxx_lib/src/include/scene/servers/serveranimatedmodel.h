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

    class AnimatedModelsServer : public m3d::DataServer
    {
    public:
        AnimatedModelsServer();
        virtual  ~AnimatedModelsServer() override /* 0x00 */;
        virtual int Init() override /* 0x04 */;
        virtual int Release() override /* 0x08 */;
        virtual int AddItem(const char* params, const char* id) override /* 0x0c */;
        virtual int RemoveItem(int id) override /* 0x18 */;
        virtual int GetItemProperty(int id, int prop, void* dest) override /* 0x10 */;
        virtual int SetItemProperty(int id, int prop, void* src) override /* 0x14 */;
        virtual void RenderItem(int id, void* params) override /* 0x1c */;
        virtual void UpdateItem(int id, void* param) override /* 0x30 */;
        virtual void RegisterNode(m3d::SgNode* node) override /* 0x34 */;
        virtual void UnregisterNode(m3d::SgNode* node) override /* 0x38 */;
        virtual int RenderNodeSet(m3d::SgNode** nodes, unsigned int numNodes, m3d::RenderNodeInfo rni) override /* 0x20 */;
        int RenderShadowVolumes();
        virtual int RenderShadowVolumesSet(m3d::SgNode** nodes, unsigned int numNodes) override /* 0x28 */;
        virtual void RenderTransparents(m3d::SgNode** nodes, unsigned int numNodes) override /* 0x24 */;
        virtual int GenerateImpostorsIfNeeded() override /* 0x2c */;
        int GetBoneMatrixByNameFromModelName(const char* modelname, const CStr& boneName, CMatrix& res, bool theLastOneOnly);
        CVector GetBoundSizes(const char* modelName);
        bool IsBonePresentsInModel(const char* modelname, const char* bonename);
        virtual int SaveAllLoadedEntities(const char* filename) override /* 0x4c */;
        virtual void PostLoad() override /* 0x50 */;
        virtual bool ReportServerInfo(const char* filename) override /* 0x54 */;

    protected:
        virtual void AddItemsList(retruxx::vector<m3d::DataServer::ServerItem, retruxx::allocator<m3d::DataServer::ServerItem> >& itemslist) override /* 0x58 */;

    private:
        int RenderMesh(m3d::SgAnimatedModelNode* node, m3d::AnimatedModel::Mesh& mh, m3d::rend::IEffect* Shader);
        int RenderMesh(m3d::AnimInfo* ai, m3d::AnimatedModel::Mesh& mh, m3d::rend::IEffect* Shader);
        void UpdateGlobalRenderingParams();
        /* 0x0048 */ CVector m_colorAmbient;
        /* 0x0054 */ CVector m_colorDiffuse;
        /* 0x0060 */ CVector m_colorPlant;
        /* 0x006c */ CVector m_fogTerm;
        /* 0x0078 */ CVector m_treeBendTerm;
        /* 0x0084 */ CVector m_colorSpecular;
        /* 0x0090 */ bool m_tessellate = true;
        /* 0x0091 */ bool m_alreadyCached = false;
        /* 0x0092 */ char Padding_366[2];
        /* 0x0094 */ m3d::MeshMaterialManager m_MeshMaterialManager;
        /* 0x00a4 */ m3d::ShadowManager* m_ShadowMan = nullptr;
        /* 0x00a8 */ int m_numShadowingNodes = 0;
        /* 0x00ac */ m3d::SgNode* m_shadowingNodes[100] = { 0 };
        /* 0x023c */ m3d::Profiler* m_profiler = nullptr;
        /* 0x0240 */ m3d::DbgCounter* m_countNodes = nullptr;
        /* 0x0244 */ m3d::DbgCounter* m_countMeshes = nullptr;
        /* 0x0248 */ bool m_globalFxParamAmbientNotActuated;
        /* 0x0249 */ bool m_globalFxParamDiffuseNotActuated;
        /* 0x024a */ bool m_globalFxParamPlantAmbientNotActuated;
        /* 0x024b */ bool m_globalFxParamFogNotActuated;
        /* 0x024c */ bool m_globalFxParamFrameStartTimeNotActuated;
        /* 0x024d */ bool m_globalFxParamTreeBendTermNotActuated;
        /* 0x024e */ bool m_globalFxParamSpecularNotActuated;
        /* 0x024f */ char Padding_367;
        /* 0x0250 */ m3d::rend::IHlslShader* m_impostorVs = nullptr;
        /* 0x0254 */ m3d::rend::IHlslShader* m_impostorPs = nullptr;
        void RenderModelForImpostor(m3d::AnimatedModel* mdl, float rotY, int offX, int offY);

        struct ModelStat
        {
            /* 0x0000 */ int handle;
            /* 0x0004 */ int instanceCount;
        }; /* size: 0x0008 */

        struct SortModelStatPred
        {
            bool operator()(const m3d::AnimatedModelsServer::ModelStat& ms1, const m3d::AnimatedModelsServer::ModelStat& ms2) const;
        }; /* size: 0x0001 */

    }; /* size: 0x0258 */

    static_assert(sizeof(AnimatedModelsServer) == 0x0258);
}
