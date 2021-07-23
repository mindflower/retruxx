#pragma once

namespace AnimatedModelsServer
{
    namespace m3d
    {
        class SortModelStatPred
        {
        public:
            bool operator()(ModelStat const &,ModelStat const &) const ;
        protected:
        private:
        };
    }
}

namespace m3d
{
    class AnimatedModelsServer :  public DataServer
    {
    public:
        virtual void PostLoad();
        virtual int AddItem(char const *,char const *);
        virtual int RemoveItem(int);
        bool IsBonePresentsInModel(char const *,char const *);
        virtual void RenderItem(int,void *);
        virtual void UnregisterNode(SgNode *);
        virtual bool ReportServerInfo(char const *);
        virtual int Init();
        struct CVector GetBoundSizes(char const *);
        int GetBoneMatrixByNameFromModelName(char const *,class CStr const &,struct CMatrix &,bool);
        virtual int SaveAllLoadedEntities(char const *);
        virtual int Release();
        AnimatedModelsServer();
        virtual int SetItemProperty(int,int,void *);
        virtual int RenderNodeSet(SgNode * *,unsigned int,RenderNodeInfo);
        virtual void RenderTransparents(SgNode * *,unsigned int);
        virtual int RenderShadowVolumesSet(SgNode * *,unsigned int);
        virtual ~AnimatedModelsServer();
        virtual int GenerateImpostorsIfNeeded();
        virtual int GetItemProperty(int,int,void *);
        virtual void RegisterNode(SgNode *);
        virtual void UpdateItem(int,void *);
    protected:
        virtual void AddItemsList(class std::vector<DataServer::ServerItem,class std::allocator<DataServer::ServerItem> > &);
    private:
        std::swap<ModelStat>(ModelStat &,ModelStat &);
        std::_Allocate<ModelStat>(uint,ModelStat *);
        int RenderMesh(SgAnimatedModelNode *,AnimatedModel::Mesh &,rend::IEffect *);
        int RenderMesh(AnimInfo *,AnimatedModel::Mesh &,rend::IEffect *);
        void RenderModelForImpostor(AnimatedModel *,float,int,int);
        std::_Ptr_cat<ModelStat *,ModelStat *>(ModelStat * &,ModelStat * &);
        std::allocator<ModelStat>::allocator<ModelStat>();
        std::allocator<ModelStat>::allocator<ModelStat>();
        std::_Destroy<ModelStat>(ModelStat *);
        std::_Construct<ModelStat,ModelStat>(ModelStat *,ModelStat const &);
        void UpdateGlobalRenderingParams();
        CVector m_colorAmbient;
        CVector m_colorDiffuse;
        CVector m_colorPlant;
        CVector m_fogTerm;
        CVector m_treeBendTerm;
        CVector m_colorSpecular;
        bool m_tessellate;
        bool m_alreadyCached;
        MeshMaterialManager m_MeshMaterialManager;
        ShadowManager *m_ShadowMan;
        int m_numShadowingNodes;
        SgNode *m_shadowingNodes[100];
        Profiler *m_profiler;
        DbgCounter *m_countNodes;
        DbgCounter *m_countMeshes;
        bool m_globalFxParamAmbientNotActuated;
        bool m_globalFxParamDiffuseNotActuated;
        bool m_globalFxParamPlantAmbientNotActuated;
        bool m_globalFxParamFogNotActuated;
        bool m_globalFxParamFrameStartTimeNotActuated;
        bool m_globalFxParamTreeBendTermNotActuated;
        bool m_globalFxParamSpecularNotActuated;
        rend::IHlslShader *m_impostorVs;
        rend::IHlslShader *m_impostorPs;
    };
}
