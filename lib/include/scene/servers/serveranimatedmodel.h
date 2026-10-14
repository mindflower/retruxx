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

    // What SgAnimatedModelNode hands to AnimatedModelsServer::SetItemProperty for PROP_DM_ACTION,
    // PROP_DM_EFFECT_ACTION and PROP_DM_MODEL_ACTION (AnimatedModelsServer::SetItemProperty::RenderInfo in the PDB).
    struct PropSrvNodeAction
    {
        m3d::SgNode* m_node;
        ActionType m_action;
    };

    // The same for PROP_DM_ARRAY_EFFECT_ACTIONS: the node's whole list of effect actions.
    struct PropSrvNodeActions
    {
        m3d::SgNode* m_node;
        retruxx::vector<ActionType, retruxx::allocator<ActionType> >* m_Actions;
    };

    class AnimatedModelsServer : public m3d::DataServer
    {
    public:
        AnimatedModelsServer();
        virtual  ~AnimatedModelsServer() override;
        virtual int Init() override;
        virtual int Release() override;
        virtual int AddItem(const char* params, const char* id) override;
        virtual int RemoveItem(int id) override;
        virtual int GetItemProperty(int id, int prop, void* dest) override;
        virtual int SetItemProperty(int id, int prop, void* src) override;
        virtual void RenderItem(int id, void* params) override;
        virtual void UpdateItem(int id, void* param) override;
        virtual void RegisterNode(m3d::SgNode* node) override;
        virtual void UnregisterNode(m3d::SgNode* node) override;
        virtual int RenderNodeSet(m3d::SgNode** nodes, unsigned int numNodes, m3d::RenderNodeInfo rni) override;
        int RenderShadowVolumes();
        virtual int RenderShadowVolumesSet(m3d::SgNode** nodes, unsigned int numNodes) override;
        virtual void RenderTransparents(m3d::SgNode** nodes, unsigned int numNodes) override;
        virtual int GenerateImpostorsIfNeeded() override;
        int GetBoneMatrixByNameFromModelName(const char* modelname, const CStr& boneName, CMatrix& res, bool theLastOneOnly);
        CVector GetBoundSizes(const char* modelName);
        bool IsBonePresentsInModel(const char* modelname, const char* bonename);
        virtual int SaveAllLoadedEntities(const char* filename) override;
        virtual void PostLoad() override;
        virtual bool ReportServerInfo(const char* filename) override;

    protected:
        virtual void AddItemsList(retruxx::vector<m3d::DataServer::ServerItem, retruxx::allocator<m3d::DataServer::ServerItem> >& itemslist) override;

    private:
        int RenderMesh(m3d::SgAnimatedModelNode* node, m3d::AnimatedModel::Mesh& mh, m3d::rend::IEffect* Shader);
        int RenderMesh(m3d::AnimInfo* ai, m3d::AnimatedModel::Mesh& mh, m3d::rend::IEffect* Shader);
        void UpdateGlobalRenderingParams();
        CVector m_colorAmbient;
        CVector m_colorDiffuse;
        CVector m_colorPlant;
        CVector m_fogTerm;
        CVector m_treeBendTerm;
        CVector m_colorSpecular;
        bool m_tessellate = true;
        bool m_alreadyCached = false;
        m3d::MeshMaterialManager m_MeshMaterialManager;
        m3d::ShadowManager* m_ShadowMan = nullptr;
        int m_numShadowingNodes = 0;
        m3d::SgNode* m_shadowingNodes[100] = { 0 };
        m3d::Profiler* m_profiler = nullptr;
        m3d::DbgCounter* m_countNodes = nullptr;
        m3d::DbgCounter* m_countMeshes = nullptr;
        bool m_globalFxParamAmbientNotActuated;
        bool m_globalFxParamDiffuseNotActuated;
        bool m_globalFxParamPlantAmbientNotActuated;
        bool m_globalFxParamFogNotActuated;
        bool m_globalFxParamFrameStartTimeNotActuated;
        bool m_globalFxParamTreeBendTermNotActuated;
        bool m_globalFxParamSpecularNotActuated;
        m3d::rend::IHlslShader* m_impostorVs = nullptr;
        m3d::rend::IHlslShader* m_impostorPs = nullptr;
        void RenderModelForImpostor(m3d::AnimatedModel* mdl, float rotY, int offX, int offY);

        struct ModelStat
        {
            int handle;
            int instanceCount;
        };

        struct SortModelStatPred
        {
            bool operator()(const m3d::AnimatedModelsServer::ModelStat& ms1, const m3d::AnimatedModelsServer::ModelStat& ms2) const;
        };

    };

    struct PropSrvSoundForAction
    {
        int m_action;
        char* m_soundStr;
        int m_looped;
    };
}

struct DynamicModel
{
    static const unsigned short MAX_INSTANCES_PER_BUFFER;
    DynamicModel(const DynamicModel&);
    DynamicModel();
    ~DynamicModel();
    CStr m_soundIds[32];
    int m_soundsLooped[32];
    m3d::AnimatedModel* m_mdl[5];
    unsigned char m_numLods;
    unsigned char m_curLod;
    bool m_useImpostors;
    m3d::rend::TexHandle m_impostorTex;
    m3d::rend::VbHandle m_impostorVb;
    m3d::rend::IbHandle m_impostorIb;
    unsigned short m_numVerts;
    unsigned short m_numTris;
    unsigned short m_numIndices;
    float m_impostorDisplacement;
    void _createImpostorShit();
    void _releaseImpostorShit();

    struct auxEffectDesc
    {
        CStr m_lpName;
        CStr m_effectName;
        int m_lpId;
        int m_effectId;
        bool m_restartOnAnimChange;
        bool m_immediateRemove;
    };

    using tActionEffectsDesc = retruxx::vector<DynamicModel::auxEffectDesc, retruxx::allocator<DynamicModel::auxEffectDesc> >;

    struct auxActionEffectsDesc
    {
        retruxx::vector<DynamicModel::auxEffectDesc, retruxx::allocator<DynamicModel::auxEffectDesc> > lpEffects;
        int startAttackFrame;
        int endAttackFrame;
        int skinNum;
        int cfgNum;
        auxActionEffectsDesc(const DynamicModel::auxActionEffectsDesc&);
        auxActionEffectsDesc();
    };

    DynamicModel::auxActionEffectsDesc m_effects[32];
    int GetRandomSound(m3d::PropSrvSoundForAction*);
};

struct ModelEffectList
{
    struct tEffect
    {
        DynamicModel::auxEffectDesc* m_desc;
        m3d::SgNode* m_effectNode;
        bool IsValid() const;
    };

    using tActionEffects = retruxx::vector<ModelEffectList::tEffect, retruxx::allocator<ModelEffectList::tEffect> >;

    retruxx::vector<ModelEffectList::tEffect, retruxx::allocator<ModelEffectList::tEffect> > m_curEffectList;
    ModelEffectList(const ModelEffectList&);
    ModelEffectList(DynamicModel* meta);
    void adjustModelEffects(m3d::SgNode* realModel, retruxx::vector<ModelEffectList::tEffect, retruxx::allocator<ModelEffectList::tEffect> >& newEffectList);
    void adjustModelEffects(m3d::SgNode* realModel, const retruxx::vector<enum ActionType, retruxx::allocator<enum ActionType> >& newActions);
    void adjustModelEffects(m3d::SgNode* realModel, ActionType newAction);
    DynamicModel* m_dynModel;
    
    struct SortPred
    {
        bool operator()(const ModelEffectList::tEffect& a, const ModelEffectList::tEffect& b);
    };
};
